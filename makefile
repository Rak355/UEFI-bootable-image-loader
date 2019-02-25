
set_disk:
	qemu-img create disk 512M
	gdisk disk

prep_disk:
	mkfs.vfat -F32 disk
	sudo mount disk /mnt
	sudo mkdir /mnt/efi/
	sudo mkdir /mnt/efi/boot
	sudo cp /home/rakesh/Desktop/UEFI_bootable_image_loader/main/DEBUG/ImageLoader.efi /mnt/efi/boot/BOOTx64.EFI
	sudo mkdir /mnt/img
	sudo mkdir /mnt/rak
	sudo cp /home/rakesh/Desktop/UEFI_bootable_image_loader/image.bmp /mnt/img/image.bmp
	sudo umount /mnt


run-qemu: prep_disk
	sudo qemu-system-x86_64 -cpu qemu64 \
  -drive if=pflash,format=raw,unit=0,file=./OVMF/OVMF_CODE.fd,readonly=on \
  -drive if=pflash,format=raw,unit=1,file=./OVMF/OVMF_VARS.fd \
  -net none -cdrom disk -d guest_errors -monitor stdio -snapshot -net none -enable-kvm
