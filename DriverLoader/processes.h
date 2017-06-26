#ifndef PROCESSES_H
#define PROCESSES_H
# include "defs.h"

namespace Ui {

class Processes;
}

class Processes :  public QDialog
{
    Q_OBJECT

public:
    explicit Processes(QWidget *parent = 0);
   QStringList GetProcessList(void);
    ~Processes();

private slots:
    void on_ProcessBtn_clicked();
    void reject();
    void on_ClearProcButton_clicked();

private:
    Ui::Processes *ui;
    QStringListModel*strModel;

};

#endif // PROCESSES_H
