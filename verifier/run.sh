
ns_path="../IOTKit_CM33_S_NS/out/IOTKit_CM33_ns/V2MMPS2/"
s_path="../IOTKit_CM33_S_NS/out/IOTKit_CM33_s/V2MMPS2/"
echo ${ns_path}
input_elf=${ns_path}"IOTKit_CM33_ns.axf"
echo ${input_elf}
input_file=${ns_path}"IOTKit_CM33_ns.list"
echo ${input_file}

# OS_TYPE=""
# if [[ "$(uname)" == "Linux" ]]; then
# 	OBJDUMP=arm-none-eabi-objdump
# else
# 	OBJDUMP=arm-none-eabi-objdump.exe
# fi

# # check if is a linux system
# if [[ "$(uname)" == "Linux" ]]; then
# 	echo "Linux"
# 	OBJDUMP=arm-none-eabi-objdump
# else
# 	echo "Windows"
# 	OBJDUMP=arm-none-eabi-objdump.exe
# fi

OBJDUMP=arm-none-eabi-objdump
# test if arm-none-eabi-objdump is installed
if ! command -v $OBJDUMP &> /dev/null
then
	OBJDUMP=arm-none-eabi-objdump.exe
fi

echo "OBJDUMP: " $OBJDUMP

arch_type="armv8-m33"


grep "word" ${input_file} > .words.tmp
awk {'print $1, $2'} .words.tmp > .words.tmp2
sed 's/:/,/g' .words.tmp2 > ./objs/.words


# echo "${OBJDUMP} -d" ${input_elf} ">" ${input_file}
rm ${input_file}
$OBJDUMP -d ${input_elf} > ${input_file}
$OBJDUMP -d ${s_path}"IOTKit_CM33_s.axf" > ${s_path}"IOTKit_CM33_s.list"

## get the sg labels and addrs
$OBJDUMP -d --section=ER_CMSE_VENEER -d ${s_path}"IOTKit_CM33_s.axf" > sg.lst
sed -n -i '/</p' sg.lst
sed -i '/__acle_se_/d' sg.lst

cp ${input_elf} instrumented.axf

# exit
# rm patched.elf
# rm patched.lst

echo python generate_cfg.py --asmfile ${input_file} --arch ${arch_type} --cfgfile ./objs/cfg.bin
python3 generate_cfg.py --asmfile ${input_file} --arch ${arch_type} --cfgfile ./objs/cfg.bin
echo "Done"

cp instrumented.axf ${ns_path}instrumented.axf
cp instrumented.lst ${ns_path}instrumented.lst
