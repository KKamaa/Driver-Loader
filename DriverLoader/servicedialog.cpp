#include "servicedialog.h"
#include "ui_servicedialog.h"

ServiceDialog::ServiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceDialog)
{
    ui->setupUi(this);

    QStringList data=GetServices();
    strServModel=new QStringListModel(this);
    strServModel->setStringList(data);
    ui->ServicesListView->setModel(strServModel);
    ui->ServicesListView->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents);
}

ServiceDialog::~ServiceDialog()
{
    delete ui;
}
SC_HANDLE ServiceDialog::Init()
{
    QMessageBox msgBox;
    SC_HANDLE scManager;
    scManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (scManager == NULL) {
        msgBox.setText("Handle to SCM failed");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return NULL;
    }
    else{
return scManager;
    };
}

QStringList ServiceDialog::GetServices(void)
{

    DWORD SizeNeeded = 0, returned = 0, resume = 0, Counter = 0;
    BYTE *pBuffer = NULL;
    LPENUM_SERVICE_STATUS services;
    QStringList servdata;
    QString servType,servState;
    SC_HANDLE sc_handle;
    QMessageBox msgBox;


    sc_handle=ServiceDialog::Init();
    if(sc_handle==NULL)
        return QStringList();
    else{

    if ((EnumServicesStatus(sc_handle, SERVICE_WIN32 | SERVICE_DRIVER, SERVICE_STATE_ALL, NULL, 0, &SizeNeeded, &returned, &resume) == 0) &&
                (GetLastError() == ERROR_MORE_DATA) &&
                (pBuffer = (BYTE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, SizeNeeded)) &&
                (EnumServicesStatus(sc_handle, SERVICE_WIN32 | SERVICE_DRIVER, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUS)pBuffer, SizeNeeded, &SizeNeeded, &returned, &resume) != 0)
                )
    {
        services = (LPENUM_SERVICE_STATUS)pBuffer;


        servdata.append(QString("%1,\t\t\t%2,\t\t\t%3,\t\t\t%4").arg("Service Name").arg("Display Name").
                arg("Service Type").arg("Status"));
        servdata.append("----------------------------------------------------------------------------------------------------------------------------------------------------------");

        int count=0;
        do{
            ++count;
            //NB SERVICE_INTERACTIVE_PROCESS can be defined to combine
                            //SERVICE_WIN32_OWN_PROCESS or SERVICE_WIN32_SHARE_PROCESS.
                            switch (services->ServiceStatus.dwServiceType)
                            {
                            case SERVICE_FILE_SYSTEM_DRIVER:
                                 servType="File system driver";
                                break;
                            case SERVICE_KERNEL_DRIVER:
                                servType="kernel";
                                break;
                            case SERVICE_WIN32_OWN_PROCESS:
                                servType="Owner";
                                break;
                            case SERVICE_WIN32_SHARE_PROCESS:
                                servType="Share";
                                break;
                            default:
                                servType="unknown,User share or user owner";
                                break;
                            }
                            switch (services->ServiceStatus.dwCurrentState)
                            {
                            case SERVICE_CONTINUE_PENDING:
                                servState="continue pending";
                                break;
                            case SERVICE_PAUSE_PENDING:
                                servState="pause is pending";
                                break;
                            case SERVICE_PAUSED:
                                servState="paused";
                                break;
                            case SERVICE_RUNNING:
                                servState="running";
                                break;
                            case SERVICE_START_PENDING:
                                servState="service is starting";
                                break;
                            case SERVICE_STOP_PENDING:
                                servState="service is stopping";
                                break;
                            case SERVICE_STOPPED:
                                servState="stopped";
                                break;
                            default:
                                servState="unkown";
                                break;


        }
            servdata.append(QString::number(count)+QString(".)%1\t\t\t%2\t\t\t%3\t\t\t%4").arg(QString::fromStdWString(services->lpServiceName)).arg(
                        QString::fromStdWString(services->lpDisplayName)).arg(
                        servType).
                    arg(servState));
            services++;
            Counter++;
        }while(Counter< returned);

    return servdata;
    }
    else{
        msgBox.setText(QString("Service enumeration failed!. Error %1.").arg(GetLastError()));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return QStringList();
    }
    }

}

void ServiceDialog::reject()
{
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;

        resBtn = QMessageBox::question( this, "Services",
                                        tr("Are you sure?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        QDialog::reject();
    }
}

void ServiceDialog::on_ClearBtn_clicked()
{
      strServModel->setStringList(QStringList{});
}

void ServiceDialog::on_ServicesBtn_clicked()
{
    QStringList data=GetServices();
    strServModel=new QStringListModel(this);
    strServModel->setStringList(data);
    ui->ServicesListView->setModel(strServModel);
    ui->ServicesListView->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents);

}
