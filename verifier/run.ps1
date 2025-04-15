# Define paths and other variables
$path = "../IOTKit_CM33_S_NS/IOTKit_CM33_ns/Objects/"
Write-Output $path
$input_elf = "${path}IOTKit_CM33_ns.axf"
Write-Output $input_elf
$input_file = "${path}IOTKit_CM33_ns.list"
Write-Output $input_file
$OBJDUMP = "arm-none-eabi-objdump"
$arch_type = "armv8-m33"
$stack = "_estack"  # Define the stack symbol if needed

# Run objdump and save disassembly to input file
Write-Output "arm-none-eabi-objdump -d $input_elf > $input_file"
Start-Process -FilePath $OBJDUMP -ArgumentList "-d", $input_elf -RedirectStandardOutput $input_file -NoNewWindow -Wait

# Remove temporary files if they exist
Remove-Item -Path "patched.elf", "patched.lst" -ErrorAction SilentlyContinue

# Output path and input files
Write-Output "Path: $path"
Write-Output "Input file: $input_file"
Write-Output "Input ELF: $input_elf"

# Initialize the stack pointer
if ($stack) {
    Start-Process -FilePath $OBJDUMP -ArgumentList "-dt", $input_elf -NoNewWindow -Wait | Select-String $stack | ForEach-Object { $_.Line.Split(" ")[0] } | Set-Content .\objs\.sp
}

# Get initial values for .data and .bss sections
Start-Process -FilePath $OBJDUMP -ArgumentList "-t", $input_elf -NoNewWindow -Wait | Select-String "O .data" | Sort-Object { $_.Line.Split(" ")[0] } | ForEach-Object { $_.Line.Split(" ")[0, 4] -join " " } | Set-Content .\objs\.data.objs
Start-Process -FilePath $OBJDUMP -ArgumentList "-t", $input_elf -NoNewWindow -Wait | Select-String "O .bss" | Sort-Object { $_.Line.Split(" ")[0] } | ForEach-Object { $_.Line.Split(" ")[0, 4] -join " " } | Set-Content .\objs\.bss.objs
Start-Process -FilePath $OBJDUMP -ArgumentList "-s", "-j", ".data", $input_elf -NoNewWindow -Wait | Select-Object -Skip 4 | ForEach-Object { ($_ -split '\s+')[1,2,3,4] -join "" } | Set-Content .\objs\.data
Start-Process -FilePath $OBJDUMP -ArgumentList "-s", "-j", ".rodata", $input_elf -NoNewWindow -Wait | Select-Object -Skip 4 | ForEach-Object { ($_ -split '\s+')[0] } | Set-Content .\objs\.rodata.start
Start-Process -FilePath $OBJDUMP -ArgumentList "-s", "-j", ".rodata", $input_elf -NoNewWindow -Wait | Select-Object -Skip 4 | ForEach-Object { ($_ -split '\s+')[1,2,3,4] -join "" } | Set-Content .\objs\.rodata

# Create log file if it doesn't exist
New-Item -Path "./logs/timingdata.log" -ItemType File -Force | Out-Null

# Build app CFG
Write-Output "Building app CFG..."
Copy-Item $input_elf -Destination "patched.elf"
Write-Output "python generate_cfg.py --asmfile $input_file --arch $arch_type --cfgfile ./objs/cfg.bin"
python generate_cfg.py --asmfile $input_file --arch $arch_type --cfgfile ./objs/cfg.bin
Write-Output "Done"

