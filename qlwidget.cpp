#include "qlwidget.h"
#include "ui_qlwidget.h"
//#include <QUdpSocket>
#include <QScrollBar>
#include "mainform.h"
//#include <QFile>
//#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>

QlWidget::QlWidget(QWidget *parent,QStringList qzinfor,QStringList memberinfor) :
    QWidget(parent),
    ui(new Ui::QlWidget)
{
    ui->setupUi(this);
    qzid = qzinfor.at(0);
    qzname = qzinfor.at(1);
    qzprofil = qzinfor.at(2);
    qzprocl = qzinfor.at(3);
    memberlist = memberinfor;
    setWindowTitle(qzname+"\n"+qzid);
    ui->qzID->setText(qzid);
    ui->qzname->setText(qzname);
    ui->qzprofil->setText(qzprofil);
    ui->qzprocl->setText(qzprocl);

    setAttribute(Qt::WA_QuitOnClose,false);
    initqzmembset(memberinfor);
/*    serv = new Server();
//    clien = new Client();
    ui->rcevLayout->addWidget(clien);
    ui->sendLayout->addWidget(serv);
    ui->stackedWidget->setCurrentIndex(0);*/
//    ui->msgTxtEdit->setAcceptDrops(false);
//    setAcceptDrops(true);
//    connect(serv, SIGNAL(sendfilmOk()), this, SLOT(sendfilmOk()));
//    connect(clien, SIGNAL(receivfilmOk()), this, SLOT(receivfilmOk()));
    connect(ui->msgTxtEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),this, SLOT(curFmtChanged(const QTextCharFormat)));
}

QlWidget::~QlWidget()
{
    delete ui;
}
void QlWidget::initqzmembset(QStringList memberinfor)
{
    ui->qzmembertable->setColumnCount(2);
    QStringList headlist;
    headlist  << tr("用户名") << tr("用户IP");
    ui->qzmembertable->setHorizontalHeaderLabels(headlist);
//    ui->qzmembertable->resizeColumnsToContents();

    int Num = (memberinfor.count()-1)/2;
    ui->qzmembertable->setRowCount(Num);
    for (int i = 0;i<Num;i++)
    {
        ui->qzmembertable->setItem(i,0,new QTableWidgetItem(memberinfor.at(i*2+2)));
        ui->qzmembertable->setItem(i,1,new QTableWidgetItem(memberinfor.at(i*2+1)));
    }
    ui->qzmembertable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->qzmembertable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    ui->qzmembertable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}
void QlWidget::on_sendBtn_clicked()
{
    QString msg = ui->msgTxtEdit->toPlainText();
//    QString fileNametemp = NULL;
/*    if(serv->sendtable->rowCount()>0)
    {
        sendfilepath = serv->sendtable->item(0,3)->text();
        fileNametemp = serv->sendtable->item(0,0)->text();
    }*/
    if(msg == "")
    {
        QMessageBox::warning(0, tr("警告"), tr("发送内容为空，无法发送！"), QMessageBox::Ok);
        return;
    }
    else {
//        if(msg != ""){
            msg = ui->msgTxtEdit->toHtml();
            emit sndMessage(QlMsg,qzid,msg);
            QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            ui->msgBrowser->setTextColor(Qt::darkGreen);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->msgBrowser->append("[ " +memberlist.at(2)+memberlist.at(1)+" ] " + time);
            ui->msgBrowser->append(msg);
            ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
            ui->msgTxtEdit->clear();
            ui->msgTxtEdit->setFocus();
//        }
/*        if (fileNametemp != "") {
            serv->Sendfilm(sendfilepath);
            for(int i = 0; i< serv->sendtable->rowCount();i++)
            {
                fileNametemp = serv->sendtable->item(i,0)->text();
                emit sndMessage(FileName,qzid,fileNametemp);
                QTime t;
                t.start();
                while(t.elapsed()<100);            //延时100ms
            }

            ui->msgBrowser->append(tr("文件已发送，等待对方接收..."));
        }*/
    }
}

void QlWidget::on_exitBtn_clicked()
{
    close();
}

void QlWidget::on_fontCbx_currentFontChanged(const QFont &f)
{
    ui->msgTxtEdit->setCurrentFont(f);
    ui->msgTxtEdit->setFocus();
}

void QlWidget::on_sizeCbx_currentIndexChanged(const QString &arg1)
{
    ui->msgTxtEdit->setFontPointSize(arg1.toInt());
    ui->msgTxtEdit->setFocus();
}

void QlWidget::on_boldTBtn_clicked(bool checked)
{
    if(checked)
        ui->msgTxtEdit->setFontWeight(QFont::Bold);
    else
        ui->msgTxtEdit->setFontWeight(QFont::Normal);
    ui->msgTxtEdit->setFocus();
}

void QlWidget::on_italicTBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontItalic(checked);
    ui->msgTxtEdit->setFocus();
}

void QlWidget::on_underlineTBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontUnderline(checked);
    ui->msgTxtEdit->setFocus();
}

void QlWidget::on_colorTBtn_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid()){
        ui->msgTxtEdit->setTextColor(color);
        ui->msgTxtEdit->setFocus();
    }
}

void QlWidget::on_saveTBtn_clicked()
{
/*    if (ui->msgBrowser->document()->isEmpty()) {
        QMessageBox::warning(0, tr("警告"), tr("聊天记录为空，无法保存！"), QMessageBox::Ok);
    } else {
        QString fname = QFileDialog::getSaveFileName(this,tr("保存聊天记录"), tr("聊天记录"), tr("文本(*.txt);;所有文件(*.*)"));
        if(!fname.isEmpty())
            saveFile(fname);
    }*/
}
void QlWidget::showmsgBrowser(QString usrip,QString msg)
{
    int i = memberlist.indexOf(usrip);
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->msgBrowser->setTextColor(Qt::blue);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->msgBrowser->append("[ " +memberlist.at(i+1)+usrip+" ] " + time);
    ui->msgBrowser->append(msg);
}
QString QlWidget::getMessage()  //获得要发送的信息
{
    QString msg = ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();
    return msg;
}
/*bool QlWidget::saveFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("保存文件"),tr("无法保存文件 %1:\n %2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->msgBrowser->toPlainText();

    return true;
}*/

/*void QlWidget::on_openwjBtn_clicked()
{
    sendfilepath = QFileDialog::getOpenFileName(this);
    if(!sendfilepath.isEmpty())
    {
        sendFileName = sendfilepath.right(sendfilepath.size() - sendfilepath.lastIndexOf('/')-1);
        int row = serv->sendtable->rowCount();
        serv->sendtable->setRowCount(row+1);
        serv->sendtable->setItem(row,0,new QTableWidgetItem(sendFileName));
        serv->sendtable->setItem(row,1,new QTableWidgetItem(tr("文件")));
        serv->sendtable->setItem(row,3,new QTableWidgetItem(sendfilepath));
    }
}
void QlWidget::receivfile(QString strfilename)
{
    int row = clien->recvtable->rowCount();
    clien->recvtable->setRowCount(row+1);
    clien->recvtable->setItem(row,0,new QTableWidgetItem(strfilename));
    clien->recvtable->setItem(row,1,new QTableWidgetItem(tr("文件")));
    ui->stackedWidget->setCurrentIndex(1);
}
void QlWidget::on_sendcancleBtn_clicked()
{
    serv->sendtable->setRowCount(0);
    serv->progressBar->setValue(0);
}

void QlWidget::on_receivcancelBtn_clicked()
{
    clien->recvtable->setRowCount(0);
    clien->progressBar->setValue(0);
//    clien->CancleBtn_clicked();
//    emit sndMessage(Refuse,ip,NULL);
}

void QlWidget::on_saveasBtn_clicked()
{
    recvfileName = clien->recvtable->item(0,0)->text();
    recvfilepath = QFileDialog::getSaveFileName(0,tr("保存文件"),recvfileName);
    if(!recvfilepath.isEmpty())
    {
        clien->setFileName(recvfilepath);
        clien->setHostAddr(QHostAddress(usrip));
    }
}
void QlWidget::sendfilmOk()
{
//    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->msgBrowser->setTextColor(Qt::blue);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->msgBrowser->append(tr(" 文件 ")+sendFileName+tr(" 发送成功"));
    serv->sendtable->removeRow(0);
    if(serv->sendtable->rowCount()>0)
    {
        sendfilepath = serv->sendtable->item(0,3)->text();
        serv->Sendfilm(sendfilepath);
    }

}
void QlWidget::receivfilmOk()
{
//    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->msgBrowser->setTextColor(Qt::blue);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->msgBrowser->append(tr(" 文件")+recvfileName+tr(" 接收成功，已保存至：")+recvfilepath);
    clien->recvtable->removeRow(0);
    if(clien->recvtable->rowCount()==0)
        ui->stackedWidget->setCurrentIndex(0);
    else{
        recvfileName = clien->recvtable->item(0,0)->text();
        QString Filepath = recvfilepath.left(recvfilepath.lastIndexOf('/'));  //文件保存路径
        recvfilepath = Filepath+tr("/%1").arg(recvfileName);
        if(!recvfilepath.isEmpty())
        {
            clien->setFileName(recvfilepath);
            clien->setHostAddr(QHostAddress(usrip));
        }
    }

}*/
void QlWidget::curFmtChanged(const QTextCharFormat &fmt)
{
    ui->fontCbx->setCurrentFont(fmt.font());

    if (fmt.fontPointSize() < 8) {
        ui->sizeCbx->setCurrentIndex(4);
    } else {
        ui->sizeCbx->setCurrentIndex(ui->sizeCbx->findText(QString::number(fmt.fontPointSize())));
    }
    ui->boldTBtn->setChecked(fmt.font().bold());
    ui->italicTBtn->setChecked(fmt.font().italic());
    ui->underlineTBtn->setChecked(fmt.font().underline());
    color = fmt.foreground().color();
}
void QlWidget::updatemembertable(QStringList memlist)
{
    ui->qzmembertable->setRowCount(0);
    int Num = (memlist.count()-1)/2;
    ui->qzmembertable->setRowCount(Num);
    for (int i = 0;i<Num;i++)
    {
        ui->qzmembertable->setItem(i,0,new QTableWidgetItem(memlist.at(i*2+2)));
        ui->qzmembertable->setItem(i,1,new QTableWidgetItem(memlist.at(i*2+1)));
    }
}
/*void QlWidget::on_receivBtn_clicked()
{
    recvfileName = clien->recvtable->item(0,0)->text();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh_mm_ss");
    QString dirpath = QDir::currentPath()+"/Recv Files/"+usrname+"("+usrip+")/"+time;
    QDir dir;
    if(!dir.exists(dirpath))
        dir.mkpath(dirpath);
    recvfilepath = dirpath+tr("/%1").arg(recvfileName);
    if(!recvfilepath.isEmpty())
    {
        clien->setFileName(recvfilepath);
        clien->setHostAddr(QHostAddress(usrip));
    }
}
void QlWidget::closesrv()
{
    serv->refused();
}
bool QlWidget::IsValidDragFile(QDropEvent *e)
{
    QString text = e->mimeData()->text();
    bool bre = false;
    do
    {
        // 存在多个文件时，只判断第一个
        QStringList files = text.split("\n");
        if(files.size() < 1)
        {
            break;
        }

        int pos = files[0].indexOf("file:///");
        if(0 != pos)
        {
            break;
        }

        //去掉拖拽头标 file:///
        QString filename = files[0].mid(8);

        // 接受目录
        QDir dir(filename);
        if (dir.exists())
        {
            bre = true;
            break;
        }

        if(QFile::exists(filename))
        {
            bre = true;
            break;
        }

    }while(false);

    return bre;
}

void QlWidget::AcceptFolder(QString folder)
{
    if(!folder.isEmpty())
    {
        sendFileName = folder.right(folder.size() - folder.lastIndexOf('/')-1);
        int row = serv->sendtable->rowCount();
        serv->sendtable->setRowCount(row+1);
        serv->sendtable->setItem(row,0,new QTableWidgetItem(sendFileName));
        serv->sendtable->setItem(row,1,new QTableWidgetItem(tr("文件夹")));
        serv->sendtable->setItem(row,3,new QTableWidgetItem(folder));
    }
}

void QlWidget::AcceptFile(QString pathfile)
{
    if(!pathfile.isEmpty())
    {
        sendFileName = pathfile.right(pathfile.size() - pathfile.lastIndexOf('/')-1);
        int row = serv->sendtable->rowCount();
        serv->sendtable->setRowCount(row+1);
        serv->sendtable->setItem(row,0,new QTableWidgetItem(sendFileName));
        serv->sendtable->setItem(row,1,new QTableWidgetItem(tr("文件")));
        serv->sendtable->setItem(row,3,new QTableWidgetItem(pathfile));
    }
}

void QlWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
    else event->ignore();
}
void QlWidget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())return;
//    QString fileName = urls.first().toLocalFile();
//    if(fileName.isEmpty())return;
//    ui->msgTxtEdit->setText(fileName);
//    QStringList files = event->mimeData()->text().split("\n");
    for(int i = 0; i < urls.size(); i++)
    {
//        QString fileName = files[i].mid(8); //  去掉头file:///
        QString fileName = urls.at(i).toLocalFile(); //  去掉头file:///
        // 接受目录
        QDir dir(fileName);
        if (dir.exists())
        {
//            AcceptFolder(fileName);
            continue;
        }
        else
        {
            AcceptFile(fileName);
            continue;
        }
    }
}*/
