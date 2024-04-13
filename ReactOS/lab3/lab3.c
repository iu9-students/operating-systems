#include <ntddk.h>
#include <ntifs.h>
#include <ndk/exfuncs.h>
#include <ndk/ketypes.h>
#include <ntstrsafe.h>
#ifndef NDEBUG 
#define NDEBUG 
#endif 
#include <debug.h>
 
NTSTATUS
NTAPI
DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) {
    ULONG buffsz = 0;
    NTSTATUS status = ZwQuerySystemInformation(SystemProcessInformation, NULL, 0, &buffsz);
    UNICODE_STRING imagename;

    if (status != STATUS_INFO_LENGTH_MISMATCH) {
        return status;
    }
    
    PVOID process;
    while (0 == 0) {
        process = ExAllocatePoolWithTag(PagedPool, buffsz, 'Proc');
        if (!process) {
            return STATUS_MEMORY_NOT_ALLOCATED;
        }

        ULONG buffSize = 0;
        status = ZwQuerySystemInformation(SystemProcessInformation, process, buffsz, &buffSize);

        if (buffSize != buffsz) {
            ExFreePool(process);
            buffsz = buffSize;
            continue;
        }

        if (NT_ERROR(status)) {
            ExFreePool(process);
            return status;
        }

        break;
    }

    PSYSTEM_PROCESS_INFORMATION curtpr = (PSYSTEM_PROCESS_INFORMATION)process;
    while (curtpr) {
        RtlInitUnicodeString(&imagename, curtpr->ImageName.Buffer);

        DPRINT1("Name: %wZ, PID: %d, Parent PID: %d\n", 
                    &imagename, 
                    (ULONG)curtpr->UniqueProcessId,
                    (ULONG)curtpr->InheritedFromUniqueProcessId);

        if (!curtpr->NextEntryOffset) {
            break;
        }
        
        curtpr = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)curtpr + curtpr->NextEntryOffset);
    }

    ExFreePool(process);

    return STATUS_SUCCESS; 
} 