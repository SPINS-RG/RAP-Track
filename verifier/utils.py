from architectures import *
from os.path import exists
import pickle
from elftools.elf.elffile import ELFFile
from keystone import *

def read_file(file, arch_type):
    '''
    This function receive the .s file name and read its lines.
    Return : 
        List with the lines of the assembly as strings
    '''
    #assert file.endswith('.s')
    if not(exists(file)) :
        raise NameError(f'File {file} not found !!')
    with open(file,'r') as f :
        lines = f.readlines()
    # Get rid of empty lines
    lines = [x.replace('\n','') for x in lines if x != '\n']

    # ARM: Get rid of "nop" and ".word" lines
    if arch_type == 'armv8-m33':
        lines = [x for x in lines if ("nop" not in x) and (".word" not in x)]

    return lines

def set_arch(arch):
    if arch == 'elf32-msp430':
        return MSP430() 
    elif arch == 'armv8-m33':
        return ARMv8M33() 
    else: 
        return None

def dump(obj, filename):
    filename = open(filename, 'wb')
    pickle.dump(obj, filename)
    filename.close()

def load(filename):
    f = open(filename,'rb')
    obj = pickle.load(f)
    f.close()
    return obj

def clean_comment(arch, comment):
    """
    This function attempts to extract a memory address from a given comment.
    """
    if arch.type == 'elf32-msp430':
        if comment is None:
            return comment
        comment = comment.split(' ')
        for c in comment:
            if '0x' in c:
                return c.strip('ghijklmnopqrstuvwyz!@#$%^&*(),<>/?.')
    # 
    elif arch.type == 'armv8-m33':
        if comment is None:
            return comment
        # comment = comment.split(' ')
        print("comment: "+str(comment))

def set_cfg_head(cfg, start_addr, end_addr=None):
    try: 
        cfg.head = cfg.nodes[start_addr]
    except KeyError as err:
        print(bcolors.RED + f'[!] Error: Start address ({start_addr}) to verify from is not a valid node' + bcolors.END)
        exit(1)
    return cfg

def conditional_print(str, file=None, flag=True):
    if flag:
        print(str, file=file)

def update_instruction(arch, elf_file_path, instr_addr, new_instruction_bytes):
    # text_start_addr = 0x80401f8
    # empty_start_addr = 0x8060000
    mtbdr_start_addr = 0x300000 #int(arch.text_base, 16) #0xe000
    tr_region_start_addr = int(arch.trampoline_region,16)
    empty_start_addr = int(arch.patch_base, 16)

    # print(f"Regions:\n\tMTBDR: {hex(mtbdr_start_addr)}\n\tMTBTPM : {hex(tr_region_start_addr)}\n\tMTBAR: {hex(empty_start_addr)}")

    # Open the ELF file for reading and writing
    with open(elf_file_path, 'rb+') as f:
        elf = ELFFile(f)
        
        if mtbdr_start_addr <= instr_addr < tr_region_start_addr:
            section_name = 'ER_MTBDR'
            section_start_addr = mtbdr_start_addr
        elif tr_region_start_addr <= instr_addr < empty_start_addr:
            section_name = 'ER_MTBTMP'
            section_start_addr = tr_region_start_addr
        else:
            section_name = 'ER_MTBAR'
            section_start_addr = empty_start_addr

        # Find the .text section
        text_section = None
        for section in elf.iter_sections():
            if section.name == section_name:
                text_section = section
                break
        
        if text_section is None:
            print(f"Error: {section_name} section not found")
            return
        
        # print(f"Writing to {section_name}")
        # Calculate the offset of the instruction within the .text section
        instr_offset = instr_addr - section_start_addr
        # print(f"text_section: {text_section}")
        # print(f"instruction_offset: {instruction_offset}")
        # Seek to the offset of the instruction within the ELF file
        f.seek(text_section['sh_offset'] + instr_offset)
        # print(f"text_section['sh_offset']: {text_section['sh_offset']}")
        # Write the new instruction bytes to the ELF file
        f.write(new_instruction_bytes)
        
        # print("Instruction updated successfully")

def instr_to_binary_ARM(instr, arch):
    
    instr_addr = instr.addr
    instruction = instr.reconstruct()

    print(f"trying '{instruction}'")
    debug = False
    if instr.instr in arch.all_br_insts and instr.instr not in arch.return_instrs and instr.instr not in arch.indr_calls:

        if ' ' in instr.arg:
            instr.arg = instr.arg.split(' ')[0]
        debug = True

        target = int(instr.arg,16)
        cur_addr = int(instr.addr,16)
        offset = target-cur_addr # shows as -4 in python, but is correct in the ELF
        instruction = instr.instr+f" #{offset}"
        # print(instruction)

    # Initialize Keystone Engine for ARM Cortex-M architecture
    ks = Ks(KS_ARCH_ARM, KS_MODE_THUMB)

    # Assemble the instruction
    try:
        encoding, _ = ks.asm(instruction)
        print(f"ENCODING : {encoding}")
    except KsError as e:
        encoding = []
        # print(f"Error")

    hex_instr = ''.join(['{:02x}'.format(byte) for byte in encoding])
    bin_instr = bytes.fromhex(hex_instr)
    return bin_instr, hex_instr, debug

def add_instruction_ARM(asm, cfg, patch, pg, mode_1_args=None):
    if pg.mode >= 1:
        loop_exit, idx = mode_1_args

        if patch.type == 0:
            asm.addr = hex(pg.base)
    
        if (asm.instr in cfg.arch.unconditional_br_instrs or asm.instr in cfg.arch.conditional_br_instrs) and idx != len(patch.instr)-1:
            if asm.arg == 'loop_exit':
                asm.arg = loop_exit
            elif pg.mode == 2:
                for i in range(0, len(patch.instr)):
                    instr = patch.instr[i]
                    if instr.prev_addr is not None:
                        if instr.prev_addr[2:] in asm.arg:
                            # print(f"in instr {asm.reconstruct()}: replacing {instr.prev_addr[2:]} with {instr.addr[2:]}")
                            asm.arg = asm.arg.replace(instr.prev_addr[2:], instr.addr[2:])
                            
        bin_instr, hex_instr, debug = instr_to_binary_ARM(asm, cfg.arch)
        # print(f"pg.mode = {pg.mode}\tasm = {asm.addr} {asm.reconstruct()}")
        patch.instr[idx] = asm
        patch.bin[idx] = bin_instr
        patch.hex[idx] = hex_instr

        if patch.type == 0:
            # print(f"incrementing in pg.mode = {pg.mode}")
            pg.base += int(len(hex_instr)/2)
    else: 
        patch.instr.append(asm)
        patch.bin.append(b'')
        patch.hex.append('')

    return patch
