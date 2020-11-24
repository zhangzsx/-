#include "qzinvitedialog.h"
#include "ui_qzinvitedialog.h"
#include <QDesktopWidget>

QzinviteDialog::QzinviteDialog(QWidget *parent,QStringList invitinfor) :
    QDialog(parent),
    ui(new Ui::QzinviteDialog)
{
    ui->setupUi(this);
    qzip = invitinfor.at(2);
    qzname = invitinfor.at(3);
    setWindowTitle(tr("来自好友 ")+invitinfor.at(1)+"("+invitinfor.at(0)+tr(")的邀请"));
    ui->contentedit->setText(tr("邀请加入群组：")+"\n"+invitinfor.at(3)+"("+invitinfor.at(2)+")");
    QDesktopWidget *deskWgt = QApplication::desktop();
    setGeometry(deskWgt->width()-160,deskWgt->height()-100,160,100);
}

QzinviteDialog::~QzinviteDialog()
{
    delete ui;
}

void QzinviteDialog::on_pushButton_clicked()
{
    emit acceptinvit(qzip,qzname);
    close();
}

void QzinviteDialog::on_pushButton_2_clicked()
{
    close();
}
