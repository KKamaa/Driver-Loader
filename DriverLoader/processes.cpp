#include "processes.h"
#include "ui_processes.h"
#include "mainwindow.h"
Processes::Processes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Processes)
{
    ui->setupUi(this);

    QStringList data=GetProcessList();
    strModel=new QStringListModel(this);
    strModel->setStringList(data);
    ui->ProcessesListView->setModel(strModel);
    ui->ProcessesListView->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents);



}

Processes::~Processes()
{
    delete ui;
}

QStringList Processes:: GetProcessList()
{
     PROCESSENTRY32 pe32;
     HANDLE hProcessSnap;
    QStringList proc;
     int count=0;


     // Take a snapshot of all processes in the system.

       hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );


       // Retrieve information about the first process,
         // and exit if unsuccessful
       if (hProcessSnap==INVALID_HANDLE_VALUE)
       {
       QMessageBox(QMessageBox::Warning, "Error", "Invalid handle  ",
                   QMessageBox::Ok).exec();

}
       // Set the size of the structure before using it.
         pe32.dwSize = sizeof( PROCESSENTRY32 );


         if( !Process32First( hProcessSnap, &pe32 ) )
         {
             QMessageBox(QMessageBox::Warning, "Error", "Invalid handle"+GetLastError(),
                         QMessageBox::Ok).exec(); // show cause of failure
           CloseHandle( hProcessSnap );          // clean the snapshot object
           return QStringList();
         }
else{

                proc.append(QString("%0\t\t%1").arg("Process").arg("PID"));
             do{
                 ++count;
                 proc.append(QString("%0\t\t%1").arg(QString::number(count)+".)"+QString::fromStdWString(pe32.szExeFile))
                                                           .arg(QString::number(pe32.th32ProcessID)));


       }while( Process32Next( hProcessSnap, &pe32 ) );


       }

return proc;
}


void Processes::on_ProcessBtn_clicked()
{
    QStringList data=GetProcessList();
    strModel=new QStringListModel(this);
    strModel->setStringList(data);
    ui->ProcessesListView->setModel(strModel);
    ui->ProcessesListView->viewport()->setAttribute(Qt::WA_TransparentForMouseEvents);

}

void Processes::on_ClearProcButton_clicked()
{
    strModel->setStringList(QStringList{});
}

void Processes::reject()
{
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;

        resBtn = QMessageBox::question( this, "Processes",
                                        tr("Are you sure?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        QDialog::reject();
    }

}

