#ifndef SERVICEDIALOG_H
#define SERVICEDIALOG_H
#include "defs.h"
#include <QDialog>

namespace Ui {
class ServiceDialog;
}

class ServiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceDialog(QWidget *parent = 0);
    QStringList GetServices(void);
    SC_HANDLE Init(void);
    void Uninit(SC_HANDLE serv);
    void reject();
    ~ServiceDialog();

private slots:
    void on_ClearBtn_clicked();

    void on_ServicesBtn_clicked();

private:
    Ui::ServiceDialog *ui;
    QStringListModel*strServModel;
};

#endif // SERVICEDIALOG_H
