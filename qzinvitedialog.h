#ifndef QZINVITEDIALOG_H
#define QZINVITEDIALOG_H

#include <QDialog>

namespace Ui {
class QzinviteDialog;
}

class QzinviteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QzinviteDialog(QWidget *parent,QStringList invitinfor);
    ~QzinviteDialog();

signals:
    void acceptinvit(QString,QString);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::QzinviteDialog *ui;
    QString qzip;
    QString qzname;
};

#endif // QZINVITEDIALOG_H
