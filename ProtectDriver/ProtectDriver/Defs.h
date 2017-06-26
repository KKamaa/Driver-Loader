#pragma once
#include <ntddk.h>
#pragma warning(disable:4100) //Avoid compilation error C4100

//-----------------------------------------------
//  Defines
//-----------------------------------------------

//Process Security and Access Rights
#define PROCESS_CREATE_THREAD  (0x0002) //create a thread.
#define PROCESS_CREATE_PROCESS (0x0080) //create a process.
#define PROCESS_TERMINATE      (0x0001) //terminate a process.
#define PROCESS_VM_WRITE       (0x0020) //write to a process memory.
#define PROCESS_VM_READ        (0x0010) //read from a process memory.
#define PROCESS_VM_OPERATION   (0x0008) 
#define PROCESS_SUSPEND_RESUME (0x0800) //suspend a process.


#define NT_DEVICE_NAME      L"\\Device\\ProtectDriver"
#define DOS_DEVICE_NAME     L"\\DosDevices\\ProtectDriver"

//event for user-mode processes to monitor
#define BSProcMon_Event     L"\\BaseNamedObjects\\BSProcMonProcessEvent" 

#define MAXIMUM_FILENAME_LENGTH 256
//-----------------------------------------------
// callback 
//-----------------------------------------------

PVOID CallBack_Handles = NULL;

typedef struct _OB_REG_CONTEXT {
	__in USHORT Version;
	__in UNICODE_STRING Altitude;
	__in USHORT ulIndex;
	OB_OPERATION_REGISTRATION *OperationRegistration;
} REG_CONTEXT, *PREG_CONTEXT;


/*--------------------------------------------------------
The EPROCESS structure is an opaque structure that serves 
as the process object for a process. When you get PEPROCESS 
then use PsGetProcessImageFileName to get the image name.
--------------------------------------------------------*/
extern UCHAR *PsGetProcessImageFileName(IN PEPROCESS Process);

//Get process by ID in kernel mode.
extern   NTSTATUS PsLookupProcessByProcessId(
	HANDLE ProcessId,
	PEPROCESS *Process
);

typedef PCHAR(*GET_PROCESS_IMAGE_NAME) (PEPROCESS Process);
GET_PROCESS_IMAGE_NAME gGetProcessImageFileName;

LPSTR GetProcessNameFromPid(HANDLE pid);



/*------------------------------------------------
**Forward Declarations
**-----------------------------------------------*/


//Entry for all Drivers acts like int main() in C/C++
NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
);



//func for unregistering the driver.
VOID UnloadDriver(
	IN PDRIVER_OBJECT DriverObject
);

/*
This function accepts an input structure pointer that defines
what object handles you want to monitor and which actions on
them and gives you back a RegistrationHandle.
*/

//This routine is called by the operating system 
//when a process or thread handle operation occurs.(i.e "Before")
OB_PREOP_CALLBACK_STATUS ObjectPreCallback(
	IN  PVOID RegistrationContext,
	IN  POB_PRE_OPERATION_INFORMATION OperationInformation
);

//This routine is called by the operating system after a 
//process or thread handle operation occurs.
VOID ObjectPostCallback(
	IN  PVOID RegistrationContext,
	IN  POB_POST_OPERATION_INFORMATION OperationInformation
);


//Registers a list of callback routines for 
//thread, process, and desktop handle operations.
//returns a handle to be used by callback routines.
NTSTATUS RegisterCallbackFunction();

//Unregister a set of callback routines 
//that were registered with the ObRegisterCallbacks routine.
NTSTATUS UnRegisterCallbackFunction();

//removes the driver
VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject);