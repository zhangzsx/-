#include "addqlwidget.h"
#include "ui_addqlwidget.h"
#include <QTime>
#include <QHeaderView>
#include <QMessageBox>

AddqlWidget::AddqlWidget(QWidget *parent,QStringList usrnamelist,QStringList usriplist) :
    QWidget(parent),
    ui(new Ui::AddqlWidget)
{
    ui->setupUi(this);
    QString id = createrand();
    setAttribute(Qt::WA_QuitOnClose,false);
    ui->qzId->setText(id);
    initqzmembset(usrnamelist,usriplist);
}

AddqlWidget::~AddqlWidget()
{
    delete ui;
}
QString AddqlWidget::createrand()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QString id;
    for (int i = 0;i<8;i++) {
        int test = qrand()%10;
        id +=QString::number(test);
    }
    return id;
}
void AddqlWidget::initqzmembset(QStringList usrnamelist,QStringList usriplist)
{
    ui->qzmembertable->setColumnCount(3);
    QStringList headlist;
    headlist  << tr("选择") << tr("用户名") << tr("用户IP");
    ui->qzmembertable->setHorizontalHeaderLabels(headlist);
    int Num = usriplist.count();
    ui->qzmembertable->setRowCount(Num);
    for (int i = 0;i<Num;i++)
    {
        QCheckBox *checkbox = new QCheckBox();
        checkbox->setCheckState(Qt::Unchecked);
        checklist.append(checkbox);
        ui->qzmembertable->setCellWidget(i,0,checkbox);
        ui->qzmembertable->setItem(i,1,new QTableWidgetItem(usrnamelist.at(i)));
        ui->qzmembertable->setItem(i,2,new QTableWidgetItem(usriplist.at(i)));
    }
    ui->qzmembertable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->qzmembertable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    ui->qzmembertable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

void AddqlWidget::on_qxBtn_clicked()
{
    int num = checklist.count();
    for (int i = 0;i<num;i++) {
        checklist.at(i)->setCheckState(Qt::Checked);
    }
}

void AddqlWidget::on_qbxBtn_clicked()
{
    int num = checklist.count();
    for (int i = 0;i<num;i++) {
        checklist.at(i)->setCheckState(Qt::Unchecked);
    }
}

void AddqlWidget::on_okButton_clicked()
{
    QString qzname = ui->qzName->text();
    if(qzname =="")
        QMessageBox::warning(this,tr("提示"),tr("群名称不可为空，请填写！"),QMessageBox::Ok);
    int num = checklist.count();
    for (int i = 0;i<num;i++) {
        if(checklist.at(i)->checkState())
            iplist.append(ui->qzmembertable->item(i,2)->text());
    }
    if(iplist.count() == 0)
    {
        QMessageBox::warning(this,tr("提示"),tr("未添加任何好友，请确认！"),QMessageBox::Ok);
        return;
    }
    QStringList qzzllist;
    qzzllist.append(ui->qzId->text());
    qzzllist.append(qzname);
    qzzllist.append(ui->qzprofile->toPlainText());
    qzzllist.append(ui->qzprocl->toPlainText());
    emit addqz(qzzllist,iplist);
    close();
}

void AddqlWidget::on_quitButton_clicked()
{
    close();
}
