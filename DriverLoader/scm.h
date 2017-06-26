#ifndef SCM_H
#define SCM_H
#include "defs.h"

namespace SCM {

//initialize services
//NB needs to be an admin to get it initialized.
    bool init(void);

    //un-initialize services
    void uninit(void);

    //register a new service
    unsigned long Register(QString driver, QString serviceName,
                    QString displayName, QString startType, QString error);

    //unregister a service
    unsigned long Unregister(QString service);

    //start a service
    unsigned long Start(SC_HANDLE service);

    //start a service
    unsigned long Start(QString service);

    //stop a service
    unsigned long Stop(SC_HANDLE service);

    //stop a service
    unsigned long Stop(QString service);

    //open handle to service
    SC_HANDLE Open(QString service);
}

#endif // SCM_H
