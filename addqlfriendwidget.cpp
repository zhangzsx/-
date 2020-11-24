#include "addqlfriendwidget.h"
#include "ui_addqlfriendwidget.h"
#include <QMessageBox>

AddqlfriendWidget::AddqlfriendWidget(QWidget *parent,QStringList usrnamelist,QStringList usriplist,QStringList qzmemberlist) :
    QWidget(parent),
    ui(new Ui::AddqlfriendWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("添加群组好友（")+qzmemberlist.at(0)+"）");
    qzid = qzmemberlist.at(0);
    QStringList qzmemip,qzmemname;
    int Num = (qzmemberlist.count()-1)/2;
    for (int i = 0;i<Num;i++)
    {
        qzmemip << qzmemberlist.at(i*2+1);
        qzmemname << qzmemberlist.at(i*2+2);
    }
    initoldtb(qzmemip,qzmemname);

    QStringList usrname = usrnamelist;
    QStringList usrip = usriplist;
    for (int i = 0;i<qzmemip.count();i++) {
        int index = usrip.indexOf(qzmemip.at(i));
        usrip.removeAt(index);
        usrname.removeAt(index);
    }
    initnewtb(usrname,usrip);
}

AddqlfriendWidget::~AddqlfriendWidget()
{
    delete ui;
}
void AddqlfriendWidget::initoldtb(QStringList qzmemip,QStringList qzmemname)
{
    ui->oldtable->setColumnCount(2);
    QStringList headlist;
    headlist  << tr("选择") << tr("用户名") << tr("用户IP");
    ui->oldtable->setHorizontalHeaderLabels(headlist);
    int Num = qzmemip.count();
    ui->oldtable->setRowCount(Num);
    for (int i = 0;i<Num;i++)
    {
//        QCheckBox *checkbox = new QCheckBox();
//        checkbox->setCheckState(Qt::Unchecked);
//        checklist.append(checkbox);
//        ui->oldtable->setCellWidget(i,0,checkbox);
        ui->oldtable->setItem(i,0,new QTableWidgetItem(qzmemname.at(i)));
        ui->oldtable->setItem(i,1,new QTableWidgetItem(qzmemip.at(i)));
    }
    ui->oldtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->oldtable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    ui->oldtable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}
void AddqlfriendWidget::initnewtb(QStringList usrnamelist,QStringList usriplist)
{

    ui->newtable->setColumnCount(3);
    QStringList headlist;
    headlist  << tr("选择") << tr("用户名") << tr("用户IP");
    ui->newtable->setHorizontalHeaderLabels(headlist);
    int Num = usriplist.count();
    ui->newtable->setRowCount(Num);
    for (int i = 0;i<Num;i++)
    {
        QCheckBox *checkbox = new QCheckBox();
        checkbox->setCheckState(Qt::Unchecked);
        checklist.append(checkbox);
        ui->newtable->setCellWidget(i,0,checkbox);
        ui->newtable->setItem(i,1,new QTableWidgetItem(usrnamelist.at(i)));
        ui->newtable->setItem(i,2,new QTableWidgetItem(usriplist.at(i)));
    }
    ui->newtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->newtable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    ui->newtable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}
void AddqlfriendWidget::on_qxBtn_clicked()
{
    int num = checklist.count();
    for (int i = 0;i<num;i++) {
        checklist.at(i)->setCheckState(Qt::Checked);
    }
}

void AddqlfriendWidget::on_qbxBtn_clicked()
{
    int num = checklist.count();
    for (int i = 0;i<num;i++) {
        checklist.at(i)->setCheckState(Qt::Unchecked);
    }
}

void AddqlfriendWidget::on_cancleBtn_clicked()
{
    close();
}

void AddqlfriendWidget::on_okBtn_clicked()
{
    QStringList iplist;
    int num = checklist.count();
    for (int i = 0;i<num;i++) {
        if(checklist.at(i)->checkState())
            iplist.append(ui->newtable->item(i,2)->text());
    }
    if(iplist.count() == 0)
    {
        QMessageBox::warning(this,tr("提示"),tr("未添加任何好友，请确认！"),QMessageBox::Ok);
        return;
    }
    emit addfriend(qzid,iplist);
    close();
}
