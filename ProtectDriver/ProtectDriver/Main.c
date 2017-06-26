/*
Driver to protect notepad a basic editor from termination
and code injection into its executable address space.
*/

# include <Defs.h>

//#pragma warning(disable:4100) //required if UNREFERENCED_PARAMETER is not used.




//-------------------- Driver Entry info ----------------------------------


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	//we register our callback routines.
	NTSTATUS status = RegisterCallbackFunction();
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Driver Entry Called!\n");
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Processing....................!\n");


	//NT_SUCCESS is a macro that evaluets to true.
	if (!NT_SUCCESS(status))//check the return code.
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "RegisterCallbackFunction function FAILD with Error: 0x%X \n", status);
	}

	//The entry point for the driver's Unload routine, 
	DriverObject->DriverUnload = UnloadRoutine; //has to be initialized early into DriverEntry().

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Driver Loaded\n");

	return STATUS_SUCCESS;

}
// 
// PRE OPERATION
//
OB_PREOP_CALLBACK_STATUS ObjectPreCallback(
	IN  PVOID RegistrationContext,
	IN  POB_PRE_OPERATION_INFORMATION OperationInformation
)
{

	UNREFERENCED_PARAMETER(RegistrationContext); //Avoid warning C4100;

	LPSTR ProcssName;
	

    //get process by name.
	ProcssName = GetProcessNameFromPid(PsGetProcessId((PEPROCESS)OperationInformation->Object));

	if (!_stricmp(ProcssName, "notepad.exe"))
	{   
		//A new handle to a process or thread will be opened.
		//Remove access bits from open access mask.
		//we will be blocking creation of process or threads,termination,R/W.

		if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			//we use Parameters->CreateHandleInformation for create-specific information.
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE)
			{
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_OPERATION) == PROCESS_VM_OPERATION)
			{
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_OPERATION;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & ~PROCESS_VM_READ) == PROCESS_VM_READ)
			{
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_READ;
			}
			if ((OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_WRITE) == PROCESS_VM_WRITE)
			{
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_WRITE;
			}
		}
	}
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "ObjectPreCallback ----> Process Name [%s] \n", ProcssName);
	return OB_PREOP_SUCCESS;
}

NTSTATUS UnRegisterCallbackFunction()
{
	//unregister callbacks if and only if they are active.
	if (NULL != CallBack_Handles)
	{
		ObUnRegisterCallbacks(CallBack_Handles);
		CallBack_Handles = NULL;
	}
	return STATUS_SUCCESS;
}

//Unload the driver.
VOID UnloadRoutine(IN PDRIVER_OBJECT DriverObject)
{

	UnRegisterCallbackFunction();
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Driver successfully Unloaded!\n");
}

//Definition for POST callback function
VOID ObjectPostCallback(IN  PVOID RegistrationContext, IN  POB_POST_OPERATION_INFORMATION OperationInformation)
{
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Post callback called!. \n");
}


//Register callback routines.
NTSTATUS RegisterCallbackFunction()
{
	NTSTATUS ntStatus = STATUS_SUCCESS;//default return value for ObRegisterCallback()

	UNICODE_STRING Altitude; //represents load order.

	USHORT filterVersion = ObGetFilterVersion();//The version of object callback registration that is requested

	USHORT registrationCount = 1; //The number of entries in the OperationRegistration array

	OB_OPERATION_REGISTRATION RegisterOperation;

	OB_CALLBACK_REGISTRATION RegisterCallBack;

	REG_CONTEXT RegistrationContext; //driver defined.

	/*
	OB_OPERATION_REGISTRATION opRegistrations[1] = { { 0 } };
	opRegistrations[0].ObjectType    = PsProcessType;
	opRegistrations[0].Operations    = OB_OPERATION_HANDLE_CREATE;
	opRegistrations[0].PreOperation  = ObjectPreCallback;
	opRegistrations[0].PostOperation = ObjectPostCallback;
	*/

	//shortcut to the above example. Intercept every access to object handles of processes (i.e OpenProcess)
	memset(&RegisterOperation, 0, sizeof(OB_OPERATION_REGISTRATION)); //fill memory of RegOp to 0.

	memset(&RegisterCallBack, 0, sizeof(OB_CALLBACK_REGISTRATION));  //fill memory of RegcallB to 0.

	memset(&RegistrationContext, 0, sizeof(REG_CONTEXT)); //fill memory of RegContex to 0.

	RegistrationContext.ulIndex = 1;
	RegistrationContext.Version = 120;

	if (filterVersion == OB_FLT_REGISTRATION_VERSION) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Filter Version is correct.\n");

		RegisterOperation.ObjectType = PsProcessType;

		//A new process, thread, or desktop handle was or will be opened.
		RegisterOperation.Operations = OB_OPERATION_HANDLE_CREATE;
		//Our pre callback.
		RegisterOperation.PreOperation = ObjectPreCallback;
		//Our post callback.
		RegisterOperation.PostOperation = ObjectPostCallback;

		RegisterCallBack.Version = OB_FLT_REGISTRATION_VERSION;

		RegisterCallBack.OperationRegistrationCount = registrationCount;
		//load order
		RtlInitUnicodeString(&Altitude, L"XXXXXXX");//return val will have counted string of Unicode characters
		RegisterCallBack.Altitude = Altitude;
		RegisterCallBack.RegistrationContext = &RegistrationContext;
		RegisterCallBack.OperationRegistration = &RegisterOperation;
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Register Callback Function Entry.\n");


		ntStatus = ObRegisterCallbacks(&RegisterCallBack, &CallBack_Handles);//a handle will be returned and assigned to CallBack_Handles
		                                                                    //to be used in the callbacks. ELSE NULL.
		switch (ntStatus)
		{
		case STATUS_SUCCESS:
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Register Callback Function Successful.\n");
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "The specified callback routines are registered with the system.\n");
			break;
		case STATUS_FLT_INSTANCE_ALTITUDE_COLLISION:
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "The calling driver or another driver has already registered callback routines  \
                                                                     for the altitude that CallBackRegistration->Altitude specifies\n");
			break;
		case STATUS_INVALID_PARAMETER:
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Invalid Parameter.\n");
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "This might be due to an invalid value for CallBackRegistration->Version specified or\n");
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "if registration is attempted for object types that do not support callback routines.\n");
			break;
		case STATUS_ACCESS_DENIED:
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "The callback routines do not reside in a signed kernel binary image.\n");
			break;
		case STATUS_INSUFFICIENT_RESOURCES:
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "attempt Allocate Memory Failed.\n");
			break;
		default:
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Register Callback Function Failed with 0x%08x\n", ntStatus);
		}
	}
	else {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Filter Version is not supported.\n");
	
	}
	return ntStatus;
}





LPSTR GetProcessNameFromPid(HANDLE pid)
{
	PEPROCESS Process;
	if (PsLookupProcessByProcessId(pid, &Process) == STATUS_INVALID_PARAMETER)
	{
		return "Error PID required.";
	}
	return (LPSTR)PsGetProcessImageFileName(Process);

}