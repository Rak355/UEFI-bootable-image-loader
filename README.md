#UEFI Bootable image loader
This is a simple UEFI bootable program which prints an image on the display screen on boot by using the GOP protocol provided by the UEFI firmware.

##Note
1. The image must be .bmp image.
2. The image must be mirror-inverted to print it upright and correctly. A sample of the mirror-inverted image is the "image.bmp" file. It is also the default image file.
3. The resolution of the image must be 2560x1440 as the default graphics mode has the same resolution. This could be changed if a different graphics mode is used.

#Dependencies:
1. Linux environment or subsystem of linux for other platforms.
2. make.
3. Qemu (virtual machine).
4. UEFI firmware for qemu.[Provided in the OVMF folder]
5. gdisk.
6. edk2.

#How to build:
##Getting edk2
1. Download the EDK II source and build tools.
At the console>git clone https://github.com/tianocore/edk2.git
or Download the latest .zip UDK2017 Download release (or Latest UDK release).

##Building the efi application.
1. Move to the base directory -->/src/edk2/
and run the edksetup by running following command on the console.
At the console>$. edksetup.sh BaseTools
2. Clone the source of this repository/project to another folder.
3. Open the file /src/edk2/DuetPkg/DuetPkgX64.dsc and add our application to the the [Components] section and before the [BuildOptions] section.Just add the full path of "main.inf" file betweem [Components] section and the [BuildOptions] section.
4. To build the UEFI application:
At the command promt>$ build -a X64 -p DuetPkg/DuetPkgX64.dsc
It will take a while to build.
5. To create the disk image
At the console>$ make set_disk
  - on the first entry, enter :n, to create a new partition.
  - on the second entry, enter :1, for one partition.
  - on the third entry, enter :2048, this is also the default sector size.
  - on the fourth entry, enter :1048542, for the last setor.
  - on the fifth entry, enter :ef00, this implies a efi partition.
  - on the sixth entry, enter :w, this marks the changes to be written on the disk.
  - on the seventh entry, enter :Y, to perform the write operation.
  The disk will be created.

6. To prepare the disk
At the console>$ make prep_disk
This will create /efi/boot and /img directories and also copy the .efi application and the image file to the disk.
7. To run the application on the qemu VM.
At the console>$ make run-qemu.
