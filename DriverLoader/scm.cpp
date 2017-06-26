# include "scm.h"
static SC_HANDLE scManager;

bool SCM::init(void) {
    scManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (scManager == NULL) {
        return false;
    }

    return true;
}

SC_HANDLE SCM::Open(QString service) {
    if (service == NULL || scManager == NULL || service.trimmed().isEmpty() ||
            service.trimmed().length() > 256) return NULL;

    return OpenServiceA(scManager, service.toStdString().c_str(), SERVICE_ALL_ACCESS);
}

void SCM::uninit(void) {
    if (scManager == NULL) return;

    CloseServiceHandle(scManager);
}

unsigned long SCM::Register(QString driver, QString serviceName, QString displayName,
                                 QString startTypeStr, QString error) {
    if (driver == NULL || serviceName == NULL || scManager == NULL ||
            displayName == NULL || startTypeStr == NULL || error == NULL ||
            driver.trimmed().isEmpty() || serviceName.trimmed().isEmpty() ||
            displayName.trimmed().isEmpty() || startTypeStr.trimmed().isEmpty() ||
            error.trimmed().isEmpty() || serviceName.trimmed().length() > 256 ||
            displayName.trimmed().length() > 256) return 1;

    SC_HANDLE scService;
    unsigned long startType = SERVICE_DEMAND_START;
    unsigned long errorControl = SERVICE_ERROR_NORMAL;

    //"Automatic" "Boot" "Demand" "Disabled" "System"
    if (startTypeStr.trimmed().compare("Automatic", Qt::CaseSensitive) == 0) {
        startType = SERVICE_AUTO_START;
    } else if (startTypeStr.trimmed().compare("Boot", Qt::CaseSensitive) == 0) {
        startType = SERVICE_BOOT_START;
    } else if (startTypeStr.trimmed().compare("Demand", Qt::CaseSensitive) == 0) {
        startType = SERVICE_DEMAND_START;
    } else if (startTypeStr.trimmed().compare("Disabled", Qt::CaseSensitive) == 0) {
        startType = SERVICE_DISABLED;
    } else if (startTypeStr.trimmed().compare("System", Qt::CaseSensitive) == 0) {
        startType = SERVICE_SYSTEM_START;
    }

    //"Critical" "Ignore" "Normal" "Severe"
    if (error.trimmed().compare("Critical", Qt::CaseSensitive) == 0) {
        errorControl = SERVICE_ERROR_CRITICAL;
    } else if (error.trimmed().compare("Ignore", Qt::CaseSensitive) == 0) {
        errorControl = SERVICE_ERROR_IGNORE;
    } else if (error.trimmed().compare("Normal", Qt::CaseSensitive) == 0) {
        errorControl = SERVICE_ERROR_NORMAL;
    } else if (error.trimmed().compare("Severe", Qt::CaseSensitive) == 0) {
        errorControl = SERVICE_ERROR_SEVERE;
    }

    if ((scService = CreateServiceA(scManager, serviceName.trimmed().toStdString().c_str(),
                                    displayName.trimmed().toStdString().c_str(),
                                    SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
                                    startType, errorControl,
                                    driver.trimmed().toStdString().c_str(),
                                    NULL, NULL, NULL, NULL, NULL)) == NULL) {
        return GetLastError();
    }

    CloseServiceHandle(scService);

    return 0;
}

unsigned long SCM::Unregister(QString service) {
    if (service == NULL || scManager == NULL || service.trimmed().isEmpty() ||
            service.trimmed().length() > 256) return 1;

    SC_HANDLE srvHandle;
    unsigned long error = 0;

    if ((srvHandle = Open(service)) == NULL) {
        return GetLastError();
    }

    if ((error = Stop(srvHandle)) != 0) {
        if (error != ERROR_SERVICE_NOT_ACTIVE) {
            CloseServiceHandle(srvHandle);
            return error;
        }
    }

    if (DeleteService(srvHandle) == 0) {
        CloseServiceHandle(srvHandle);
        return GetLastError();
    }

    CloseServiceHandle(srvHandle);

    return 0;
}

unsigned long SCM::Start(SC_HANDLE service) {
    if (service == NULL || scManager == NULL) return 1;

    if (StartService(service, 0, NULL) == 0) {
        return GetLastError();
    }

    return 0;
}

unsigned long SCM::Start(QString service) {
    if (service == NULL || scManager == NULL || service.trimmed().isEmpty() ||
            service.trimmed().length() > 256) return 1;

    unsigned long error = 0;

    SC_HANDLE srvHandle;

    if ((srvHandle = Open(service)) == NULL) {
        return GetLastError();
    }

    if ((error = Start(srvHandle)) != 0) {
        CloseServiceHandle(srvHandle);
        return error;
    }

    CloseServiceHandle(srvHandle);

    return 0;
}

unsigned long SCM::Stop(SC_HANDLE service) {
    if (service == NULL || scManager == NULL) return 1;

    SERVICE_STATUS SCMtatus;

    if (ControlService(service, SERVICE_CONTROL_STOP, &SCMtatus) == 0) {
        return GetLastError();
    }

    return 0;
}

unsigned long SCM::Stop(QString service) {
    if (service == NULL || scManager == NULL || service.trimmed().isEmpty() ||
            service.trimmed().length() > 256) return 1;

    SC_HANDLE srvHandle;
    unsigned long error = 0;

    if ((srvHandle = Open(service)) == NULL) {
        return GetLastError();
    }

    if ((error = Stop(srvHandle)) != 0) {
        CloseServiceHandle(srvHandle);
        return error;
    }

    CloseServiceHandle(srvHandle);
    return 0;
}
