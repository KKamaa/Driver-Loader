#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"defs.h"
#include "processes.h"
#include "servicedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //void changeBackground(QString imgPath);


    ~MainWindow();

private slots:
    void on_ProcessBtn_clicked();

    void on_BrowseBtn_clicked();

    void on_RegBtn_clicked();

    void on_UnregBtn_clicked();

    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void on_ClearBtn_clicked();

    void on_ServiceComboBox_activated(const QString &arg1);

    void on_ServicesBtn_clicked();
    void closeEvent(QCloseEvent*event);

private:
    Ui::MainWindow *ui;
    Processes *processDialog;
    ServiceDialog*servicesDialog;
    QString Filename;
    QString LoadOption;
    QString ErrorOption;
    QString serviceName="ProtectDriver";
    QString displayName="ProtectDriver";
    QMessageBox msgBox;

};

#endif // MAINWINDOW_H
