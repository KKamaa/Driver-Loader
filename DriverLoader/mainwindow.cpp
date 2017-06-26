#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scm.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->StartComboBox->addItems(QStringList() << "Automatic" << "Boot" << "Demand" << "Disable"<<"System" );
    ui->StartComboBox->setCurrentIndex(2);
    ui->ErrorLevelComboBox->addItems(QStringList()<< "Critical"<<"Ignore"<<"Normal"<<"Severe");
    ui->ServiceComboBox->addItem("Driver");
//CSS Stylesheets for the Group Box
ui->FromGrpBox->setStyleSheet("QGroupBox { border: 2px   solid #4CAF50; \
                                                                   border-radius: 25px; \
                                                                         } ");
ui->CtrlAGrpBox->setStyleSheet("QGroupBox { border: 2px   solid #4CAF50; \
                               border-radius: 25px; \
                                     } ");
ui->CrtlBGrpBox->setStyleSheet("QGroupBox { border: 2px   solid #4CAF50; \
                               border-radius: 25px; \
                                     } ");

ui->PlainTxtEdit->appendHtml("<p><font size=5 color=#0A9C30 face=verdana>**Driver Loader 1.0 \
                                                Loads **drivers, registers, **unregisters, starts, stops ** \
                                                as SCM to the **windows SCM.</font> </p>");


ui->PlainTxtEdit->setStyleSheet("QPlainTextEdit{ border: 2px   solid #4CAF50; \
                                }");

}

MainWindow::~MainWindow()
{
    delete ui;

}

/*void MainWindow::changeBackground(QString imgPath)
{
    QPixmap bkgnd(imgPath);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

}*/

void MainWindow::on_ProcessBtn_clicked()
{
    processDialog=new Processes(this);
    processDialog->setModal(true);
    processDialog->show();

}

void MainWindow::on_BrowseBtn_clicked()
{
    Filename=QFileDialog::getOpenFileName(
               this,"Driver File",QDir::currentPath(),"Driver files (*.sys)");
    QFileInfo file=QFileInfo(Filename);
   if(Filename.isNull()){
       QMessageBox(QMessageBox::Information, "Info", "Select driver file!",
                   QMessageBox::Ok).exec();

   }
   else{
       QMessageBox(QMessageBox::Information, "Info", "Driver "+file.fileName()+" selected!",
                   QMessageBox::Ok).exec();
       ui->DriverPathLineEdit->setText(Filename);
       ui->ServiceNameLineEdit->setText(file.fileName());
       ui->DisplayNameLineEdit->setText(file.fileName());
       ui->DriverSizeLineEdit->setText(QString::number(Defs::GetDriverFileSize(Filename)) + " bytes");
       ui->DriverTimeLineEdit->setText(Defs::GetFileLastWriteTime(Filename));
   }
}

void MainWindow::on_RegBtn_clicked()
{
    unsigned long registrationResult = 0;
    QMessageBox msgBox;

    if (ui->DriverPathLineEdit->text().trimmed().isEmpty()) {
        msgBox.setText("Please provide a valid driver file path.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    if (ui->ServiceNameLineEdit->text().trimmed().isEmpty() ||
            ui->ServiceNameLineEdit->text().trimmed().length() > 256) {
        msgBox.setText("Please provide a name for the service.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    if (ui->DisplayNameLineEdit->text().trimmed().isEmpty() ||
            ui->DisplayNameLineEdit->text().trimmed().length() > 256) {
        msgBox.setText("Please provide a service name to display.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }


/* msgBox.setText(ui->DisplayNameLineEdit->text().trimmed()+"\n"+
                ui->DriverPathLineEdit->text().trimmed()+"\n"+
                ui->ErrorLevelComboBox->itemText(ui->ErrorLevelComboBox->currentIndex())+"\n"+
                ui->ServiceComboBox->itemText(ui->ServiceComboBox->currentIndex())+"\n"+
                ui->StartComboBox->itemText(ui->StartComboBox->currentIndex()));
 msgBox.setStandardButtons(QMessageBox::Ok);
 msgBox.setIcon(QMessageBox::Warning);
 msgBox.exec();*/


 registrationResult = SCM::Register(ui->DriverPathLineEdit->text().trimmed(),
                                            ui->ServiceNameLineEdit->text().trimmed(),
                                            ui->DisplayNameLineEdit->text().trimmed(),
                                            ui->StartComboBox->itemText(ui->StartComboBox->currentIndex()),
                                            ui->ErrorLevelComboBox->itemText(ui->ErrorLevelComboBox->currentIndex()));

    switch (registrationResult) {
        case ERROR_SERVICE_EXISTS:
            msgBox.setText("Service registration failed. The service already exists.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case 1:
            msgBox.setText("Service registration failed. Empty or invalid parameters have been provided.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;

        case 0:
            msgBox.setText("Service registration succeeded.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;

        default:
            msgBox.setText(QString("Service registration failed. Error code %1.").arg(registrationResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
    }
}



void MainWindow::on_startBtn_clicked()
{
    QMessageBox msgBox;
    unsigned long startResult = 0;

    if (ui->ServiceNameLineEdit->text().trimmed().isEmpty() ||
            ui->ServiceNameLineEdit->text().trimmed().length() > 256) {
        msgBox.setText("Please provide service name.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    startResult = SCM::Start(ui->ServiceNameLineEdit->text().trimmed());

    switch (startResult) {
        case 1:
            msgBox.setText("Starting service failed.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SHARING_VIOLATION:
            msgBox.setText("The process cannot access the file because it is being used by another process.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_DOES_NOT_EXIST:
            msgBox.setText("The specified service does not exist as an installed service.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_ALREADY_RUNNING:
            msgBox.setText("An instance of the service is already running.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case 0:
            msgBox.setText("Service started.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;

        default:
            msgBox.setText(QString("Starting service failed. Error code %1.").arg(startResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
    }
}

void MainWindow::on_stopBtn_clicked()
{
    QMessageBox msgBox;
    unsigned long stopResult = 0;

    if (ui->ServiceNameLineEdit->text().trimmed().isEmpty() ||
            ui->ServiceNameLineEdit->text().trimmed().length() > 256) {
        msgBox.setText("Please provide service name.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    stopResult = SCM::Stop(ui->ServiceNameLineEdit->text().trimmed());

    switch (stopResult) {
        case 1:
            msgBox.setText("Stopping service failed.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_NOT_ACTIVE:
            msgBox.setText("The service has not been started.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_DOES_NOT_EXIST:
            msgBox.setText("The specified service does not exist as an installed service.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case 0:
            msgBox.setText("Service stopped.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;

        default:
            msgBox.setText(QString("Stopping service failed. Error code %1.").arg(stopResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
    }

}

void MainWindow::on_UnregBtn_clicked()
{
    unsigned long unregistrationResult = 0;
    QMessageBox msgBox;

    if (ui->ServiceNameLineEdit->text().trimmed().isEmpty() ||
            ui->ServiceNameLineEdit->text().trimmed().length() > 256) {
        msgBox.setText("Please provide service name.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    unregistrationResult = SCM::Unregister(ui->ServiceNameLineEdit->text().trimmed());

    switch (unregistrationResult) {
        case 1:
            msgBox.setText("Service unregistration failed.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_DOES_NOT_EXIST:
            msgBox.setText("The specified service does not exist as an installed service.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case 0:
            msgBox.setText("Service unregistration succeeded.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;

        default:
            msgBox.setText(QString("Service unregistration failed. Error code %1.").arg(unregistrationResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
    }

}

void MainWindow::on_ClearBtn_clicked()
{
    ui->DisplayNameLineEdit->clear();
    ui->DriverPathLineEdit->clear();
    ui->DriverSizeLineEdit->clear();
    ui->DriverTimeLineEdit->clear();
    ui->ServiceNameLineEdit->clear();
    ui->DriverVersionLineEdit->clear();
}

void MainWindow::on_ServiceComboBox_activated(const QString &arg1)
{
    ui->ServiceComboBox->setCurrentText(arg1);
}

void MainWindow::on_ServicesBtn_clicked()
{
    servicesDialog=new ServiceDialog(this);
    servicesDialog->setModal(true);
    servicesDialog->show();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Driver Loader 1.0",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {

        event->accept();
    }
}
