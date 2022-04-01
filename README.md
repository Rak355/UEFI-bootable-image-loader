# UEFI Bootable image loader
This is a simple UEFI-bootable program which prints an image of specific size on screen using the GOP protocol which is provided by the UEFI firmware. This project was done as a hobby.

## Note
1. The image must be `.bmp` image.
2. The image must be <em>mirror-inverted</em> to print it upright and correctly. A sample of the mirror-inverted image is the `image.bmp` (provided in the source). It is also the default image file that is used by the program.
3. The resolution of the image must be <em>2560x1440</em> as the default graphics mode has the same resolution. This could be changed if a different graphics mode is used.

# Tools:
1. Unix environment.
2. make.
3. Qemu (virtual machine).
4. UEFI firmware for qemu.[Provided in the OVMF folder]
5. gdisk.
6. edk2.

# How to build:
## Getting edk2
1. Download the EDK II source and build tools.<br>
`$ git clone https://github.com/tianocore/edk2.git`, or<br>
2. Download the latest .zip UDK2017 Download release (or Latest UDK release).

## Install dependencies:
### For Debian based Linux
`$ sudo apt-get install build-essential uuid-dev iasl git gcc-5 nasm python3-distutils`

## Building the edk2 tree and efi application.
1. Follow instructions at https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II
and https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions
2. Open the file `edk2/DuetPkg/DuetPkgX64.dsc` and add our application to the the `[Components]` section and before the `[BuildOptions]` section. Just add the absolute path of `main.inf` file betweem `[Components]` section and the `[BuildOptions]` section.
3. To build the UEFI application:<br>
`$ build -a X64 -p DuetPkg/DuetPkgX64.dsc`
It will take a while to build.
4. To create the disk image<br>
`$ make set_disk`
  - on the first entry, enter :n, to create a new partition.
  - on the second entry, enter :1, for one partition.
  - on the third entry, enter :set default sector size.
  - on the fourth entry, enter :set the default last setor.
  - on the fifth entry, enter :ef00, this implies an efi partition.
  - on the sixth entry, enter :w, this marks the changes to be written on the disk.
  - on the seventh entry, enter :Y, to perform the final write operation.
  The disk will be created.

5. To prepare the disk<br>
`$ make prep_disk`
This will create `/efi/boot` and `/img` directories and also copy the `.efi` application and the image file to the disk.
6. To run the application on the qemu VM.<br>
`$ make run-qemu`.

These are some snapshots of the program.
![snapshot](https://i.imgur.com/AZzRCHQ.png)
![snapshot](https://i.imgur.com/6qNdm8E.png)
