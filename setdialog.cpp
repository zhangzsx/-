#include "setdialog.h"
#include "ui_setdialog.h"
#include <QDir>
#include <QFileDialog>
#include <QProcess>

SetDialog::SetDialog(QWidget *parent,QStringList Cfglist) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);
    confglist =Cfglist;
    ui->nameEdit->setText(Cfglist.at(1));
    ui->headBox->setCurrentIndex(Cfglist.at(3).toInt());
    ui->mywordEdit->setText(Cfglist.at(4));
    bool state = (Qt::CheckState)Cfglist.at(5).toInt();
    ui->AutoruncheckBox->setChecked(state);
    state = (Qt::CheckState)Cfglist.at(6).toInt();
    ui->AutomincheckBox->setChecked(state);
//    state = (afg.at(7).toInt() == 1)?"YES":"NO";
    state = (Qt::CheckState)Cfglist.at(7).toInt();
    ui->AutochecknewBox->setChecked(state);
    state = (Qt::CheckState)Cfglist.at(8).toInt();
    ui->AutonodeBox->setChecked(state);
    ui->notekeepEdit->setText(Cfglist.at(9));
    state = (Qt::CheckState)Cfglist.at(10).toInt();
    ui->RuntoldotherBox->setChecked(state);
    state = (Qt::CheckState)Cfglist.at(11).toInt();
    ui->LefttoldotherBox->setChecked(state);
    state = (Qt::CheckState)Cfglist.at(12).toInt();
    ui->OtherleftcheckBox->setChecked(state);
    ui->filekeepEdit->setText(Cfglist.at(13));
    state = (Qt::CheckState)Cfglist.at(14).toInt();
    ui->AutorecvfileBox->setChecked(state);
    ui->autofilekeepEdit->setText(Cfglist.at(15));
    ui->okBtn->setFocus();
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
}

SetDialog::~SetDialog()
{
    delete ui;
}

void SetDialog::on_notekeepBtn_clicked()
{
    QString path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("view file"),QDir::currentPath()));
    ui->notekeepEdit->setText(path);
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot |QDir::Dirs);
/*    QFileInfoList list = dir.entryInfoList();
    for (int i=0;i<list.length();i++) {
        QString file = list.at(i).fileName();
        ui->notekeepEdit->setText(file);
    }*/
}

void SetDialog::on_filekeepBtn_clicked()
{
    QString path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("view file"),QDir::currentPath()));
    ui->filekeepEdit->setText(path);
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot |QDir::Dirs);
}

void SetDialog::on_autofilekeepBtn_clicked()
{
    QString path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("view file"),QDir::currentPath()));
    ui->autofilekeepEdit->setText(path);
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot |QDir::Dirs);
}

void SetDialog::on_okBtn_clicked()
{
    confglist.replace(1,ui->nameEdit->text());
    confglist.replace(3,QString::number(ui->headBox->currentIndex()));
    confglist.replace(4,ui->mywordEdit->toPlainText());
    confglist.replace(5,QString::number(ui->AutoruncheckBox->checkState()));
    confglist.replace(6,QString::number(ui->AutomincheckBox->checkState()));
    confglist.replace(7,QString::number(ui->AutochecknewBox->checkState()));
    confglist.replace(8,QString::number(ui->AutonodeBox->checkState()));
    confglist.replace(9,ui->notekeepEdit->text());
    confglist.replace(10,QString::number(ui->RuntoldotherBox->checkState()));
    confglist.replace(11,QString::number(ui->LefttoldotherBox->checkState()));
    confglist.replace(12,QString::number(ui->OtherleftcheckBox->checkState()));
    confglist.replace(13,ui->filekeepEdit->text());
    confglist.replace(14,QString::number(ui->AutorecvfileBox->checkState()));
    confglist.replace(15,ui->autofilekeepEdit->text());
    rwxml = new RwXml();
    rwxml->afreshConfg(confglist);
    emit setok();
//    qApp->quit();
//    QProcess::startDetached(qApp->applicationFilePath(),QStringList());
//    close();
}

void SetDialog::on_cancleBtn_clicked()
{
    close();
}
