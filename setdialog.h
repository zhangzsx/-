#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include "rwxml.h"

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetDialog(QWidget *parent,QStringList Cfglist);
    ~SetDialog();
    QStringList confglist;

signals:
    void setok();

private slots:
    void on_notekeepBtn_clicked();

    void on_filekeepBtn_clicked();

    void on_autofilekeepBtn_clicked();

    void on_okBtn_clicked();

    void on_cancleBtn_clicked();

private:
    Ui::SetDialog *ui;
    RwXml *rwxml;

};

#endif // SETDIALOG_H
