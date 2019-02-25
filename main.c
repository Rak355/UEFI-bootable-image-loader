#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>

extern void raku();
//***********GUID
static EFI_GUID efi_simple_file_system_protocol_guid = {0x0964e5b22,0x6459,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}};
static EFI_GUID efi_graphics_output_protocol_guid = {0x9042a9de,0x23dc,0x4a38,{0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}};
//***********HANDLES
EFI_HANDLE *Handle_Buffer = NULL;
//***********PROTOCOLS
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *efi_simple_file_system_protocol;
EFI_GRAPHICS_OUTPUT_PROTOCOL *efi_graphics_output_protocol;
EFI_FILE_PROTOCOL *efi_file_protocol;
EFI_FILE_PROTOCOL *new_file_protocol;
//***********DATA
EFI_STATUS Status;
CHAR16 filename[] = {'i','m','g','\\','i','m','a','g','e','.','b','m','p'}; //default image file.
UINT32 graphics_mode;
UINT32 H_res;
UINT32 V_res;
UINTN FrameBufferSize;
//***********MEM ADDRESSES
VOID *Allocated_pool = (VOID*)0xf0000;
EFI_PHYSICAL_ADDRESS FrameBufferBase;
//***********

EFI_STATUS
EFIAPI

UefiMain (IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable){
  Status = SystemTable->BootServices->AllocatePool(//Allocating memory.
    EfiLoaderData,
    40000000, //This would be enough memory!!
    (VOID**)&Allocated_pool
  );
  Print(L"Pool Allocated,%llx::Base addr,%llx\n",Status,Allocated_pool);

  UINTN Handle_count;
  Status = SystemTable->BootServices->LocateHandleBuffer(
    ByProtocol,
    &efi_simple_file_system_protocol_guid,
    NULL,
    &Handle_count,
    &Handle_Buffer
  );
  Print(L"handle for efi_simple_file_system_protocol located,%llx::Handle count,%llx\n",Status,Handle_count);

  for(int i=0;i<Handle_count;i++){
    Status = SystemTable->BootServices->OpenProtocol(
      Handle_Buffer[i],
      &efi_simple_file_system_protocol_guid,
      (VOID**)&efi_simple_file_system_protocol,
      ImageHandle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    );
    Print(L"Opened efi_simple_file_system_protocol,%llx\n",Status);
  }
  //Interface to simple_file_system_protocol is available
  Status = efi_simple_file_system_protocol->OpenVolume(
    efi_simple_file_system_protocol,
    &efi_file_protocol
  );
  Print(L"file system volume opened,%llx\n",Status);
  //Volume opened and interface to root is available
  Print(L"efi_file_protocol version,%llx\n",efi_file_protocol->Revision);
  Status = efi_file_protocol->Open(
    efi_file_protocol,
    &new_file_protocol,
    filename,
    EFI_FILE_MODE_READ,
    EFI_FILE_READ_ONLY
  );
  UINTN BufferSize = 40000000;
  Print(L"file opened,%llx\n",Status);
  //File is now open
  Status = efi_file_protocol->Read(
    new_file_protocol,
    &BufferSize,
    Allocated_pool
  );
  Print(L"file read,%llx::Address,%llx\n",Status,Allocated_pool);
  //File Base Received
  //*******Graphics Output
  Status = SystemTable->BootServices->LocateHandleBuffer(
    ByProtocol,
    &efi_graphics_output_protocol_guid,
    NULL,
    &Handle_count,
    &Handle_Buffer
  );
  Print(L"handle for efi_graphics_output_protocol located,%llx::Handle count,%llx\n",Status,Handle_count);
  for(int i=0;i<Handle_count;i++){
    Status = SystemTable->BootServices->OpenProtocol(
      Handle_Buffer[i],
      &efi_graphics_output_protocol_guid,
      (VOID**)&efi_graphics_output_protocol,
      ImageHandle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    );
    Print(L"Opened efi_graphics_output_protocol,%llx\n",Status);
  }
  //Graphics Protocol opened
  Print(L"Max graphics modes,%llx",efi_graphics_output_protocol->Mode->MaxMode);
  UINTN sizeofinfo;
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  for(int i=0;i<efi_graphics_output_protocol->Mode->MaxMode;i++){
    efi_graphics_output_protocol->QueryMode(
      efi_graphics_output_protocol,
      i,
      &sizeofinfo,
      &Info
    );
    graphics_mode = i;
  }
  H_res = Info->HorizontalResolution;
  V_res = Info->VerticalResolution;
  Print(L"::Last mode nos,%llx",graphics_mode);
  Print(L"::H-Res,%llx::V-res,%llx\n",H_res,V_res);
  FrameBufferBase = efi_graphics_output_protocol->Mode->FrameBufferBase;
  FrameBufferSize = efi_graphics_output_protocol->Mode->FrameBufferSize;

  Status = efi_graphics_output_protocol->SetMode(
    efi_graphics_output_protocol,
    graphics_mode-1
  );
//  Print(L"Graphics mode set,%llx::",Status);
//  Print(L"H-res,%llx::V-res,%llx::FrameBufferBase,%llx::FrameBufferSize,%llx\n",H_res,V_res,FrameBufferBase,FrameBufferSize);
  char *file_mem = (char*)(Allocated_pool + 54);
  char *vid_mem = (char*)FrameBufferBase;
  for(int i=0;i<H_res*V_res;i++){
    *vid_mem++ = (char)*file_mem++;
    *vid_mem++ = (char)*file_mem++;
    *vid_mem++ = (char)*file_mem++;
    *vid_mem++ = (char)0x00;
  }

  while(1){//forever loop.

  }
  return EFI_SUCCESS;
}
