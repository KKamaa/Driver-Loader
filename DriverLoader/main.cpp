#include "mainwindow.h"
#include "scm.h"
#include"defs.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (SCM::init() == false) {
        QMessageBox msgBox;
        msgBox.setText("You need priviledges to run.\nRun application as admin.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        QApplication::quit();

        return 1;
    }else{
    MainWindow w;
    w.show();
    return a.exec();
}

}
