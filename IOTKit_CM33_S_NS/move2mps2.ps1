# Define the drive letter and expected drive name
$driveLetter = "H:"
$expectedVolumeName = "V2M_MPS2"

# Define the files to copy and their new names

# $sourceFile1 = "IOTKit_CM33_s\Objects\IOTKit_CM33_s.axf" # Update this path
# $sourceFile2 = "IOTKit_CM33_ns\Objects\IOTKit_CM33_ns.axf" # Update this path


$sourceFile1 = "out\IOTKit_CM33_s\V2MMPS2\IOTKit_CM33_s.axf" # Update this path
$sourceFile2 = "out\IOTKit_CM33_ns\V2MMPS2\IOTKit_CM33_ns.axf" # Update this path
# $sourceFile2 = "out\IOTKit_CM33_ns\V2MMPS2\instrumented.axf" # Update this path
# 
# $sourceFile1 = "out\IOTKit_CM33_s\V2MMPS2\IOTKit_CM33_s.axf" # Update this path



# $sourceFile2 = "IOTKit_CM33_ns\Objects\instrumented.axf" # Update this path
$destinationFile1 = "$driveLetter\SOFTWARE\s.axf"
$destinationFile2 = "$driveLetter\SOFTWARE\ns.axf"

# Check if the drive is mounted and has the expected volume name
if (Test-Path $driveLetter) {
    $volumeInfo = Get-Volume -DriveLetter H

    if ($volumeInfo.FileSystemLabel -eq $expectedVolumeName) {
        Write-Output "Drive $driveLetter is mounted and named $expectedVolumeName."

        # Check if the SOFTWARE directory exists
        $softwareDir = "$driveLetter\SOFTWARE"
        if (Test-Path $softwareDir) {
            Write-Output "Directory $softwareDir exists. Proceeding with file copy."

            # Copy and rename files
            Copy-Item -Path $sourceFile1 -Destination $destinationFile1 -Force
            Copy-Item -Path $sourceFile2 -Destination $destinationFile2 -Force
            Write-Output "Files copied and renamed successfully."
        }
        else {
            Write-Output "Directory $softwareDir does not exist. Aborting file copy."
        }
    }
    else {
        Write-Output "Drive $driveLetter does not have the expected name $expectedVolumeName."
    }
}
else {
    Write-Output "Drive $driveLetter is not mounted."
}
