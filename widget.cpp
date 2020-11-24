#include "widget.h"
#include "ui_widget.h"
#include <QUdpSocket>
#include <QScrollBar>
#include "mainform.h"
#include <QFile>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>

Widget::Widget(QWidget *parent,QStringList confg,QStringList usrinfor)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    usrname = usrinfor.at(0);
    usrip = usrinfor.at(1);
    name = usrinfor.at(2);
    ip = usrinfor.at(3);
    cnfiglist = confg;
    setWindowTitle(usrname+"\n"+usrip);
    setAttribute(Qt::WA_QuitOnClose,false);
    ui->usrnamelabel->setText(tr("对方用户名：")+usrname);
    ui->usriplabel->setText(tr("对方IP：")+usrip);
    serv = new Server();
    clien = new Client();
    ui->rcevLayout->addWidget(clien);
    ui->sendLayout->addWidget(serv);
    ui->stackedWidget->setCurrentIndex(0);
    ui->msgTxtEdit->setAcceptDrops(false);
    setAcceptDrops(true);
    connect(serv, SIGNAL(sendfilmOk()), this, SLOT(sendfilmOk()));
    connect(clien, SIGNAL(receivfilmOk()), this, SLOT(receivfilmOk()));
    connect(ui->msgTxtEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),this, SLOT(curFmtChanged(const QTextCharFormat)));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_sendBtn_clicked()
{
    QString msg = ui->msgTxtEdit->toPlainText();
    QString fileNametemp = NULL;
    if(serv->sendtable->rowCount()>0)
    {
        sendfilepath = serv->sendtable->item(0,3)->text();
        fileNametemp = serv->sendtable->item(0,0)->text();
    }
    if(msg == "" && fileNametemp == "")
    {
        QMessageBox::warning(0, tr("警告"), tr("发送内容为空，无法发送！"), QMessageBox::Ok);
        return;
    }
    else {
        if(msg != ""){
            msg = ui->msgTxtEdit->toHtml();
            emit sndMessage(Msg,usrip,msg);
            QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            ui->msgBrowser->setTextColor(Qt::darkGreen);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->msgBrowser->append("[ " +name+" ] " + time);
            ui->msgBrowser->append(msg);
            ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
            ui->msgTxtEdit->clear();
            ui->msgTxtEdit->setFocus();
        }
        if (fileNametemp != "") {
            serv->Sendfilm(sendfilepath);
            for(int i = 0; i< serv->sendtable->rowCount();i++)
            {
                fileNametemp = serv->sendtable->item(i,0)->text();
                emit sndMessage(FileName,usrip,fileNametemp);
                QTime t;
                t.start();
                while(t.elapsed()<100);            //延时100ms
            }
            ui->msgBrowser->append(tr("文件已发送，等待对方接收..."));
        }
    }
}

void Widget::on_exitBtn_clicked()
{
    close();
}

void Widget::on_fontCbx_currentFontChanged(const QFont &f)
{
    ui->msgTxtEdit->setCurrentFont(f);
    ui->msgTxtEdit->setFocus();
}

void Widget::on_sizeCbx_currentIndexChanged(const QString &arg1)
{
    ui->msgTxtEdit->setFontPointSize(arg1.toInt());
    ui->msgTxtEdit->setFocus();
}

void Widget::on_boldTBtn_clicked(bool checked)
{
    if(checked)
        ui->msgTxtEdit->setFontWeight(QFont::Bold);
    else
        ui->msgTxtEdit->setFontWeight(QFont::Normal);
    ui->msgTxtEdit->setFocus();
}

void Widget::on_italicTBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontItalic(checked);
    ui->msgTxtEdit->setFocus();
}

void Widget::on_underlineTBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontUnderline(checked);
    ui->msgTxtEdit->setFocus();
}

void Widget::on_colorTBtn_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid()){
        ui->msgTxtEdit->setTextColor(color);
        ui->msgTxtEdit->setFocus();
    }
}

void Widget::on_saveTBtn_clicked()
{
    if (ui->msgBrowser->document()->isEmpty()) {
        QMessageBox::warning(0, tr("警告"), tr("聊天记录为空，无法保存！"), QMessageBox::Ok);
    } else {
        QString fname = QFileDialog::getSaveFileName(this,tr("保存聊天记录"), tr("聊天记录"), tr("文本(*.txt);;所有文件(*.*)"));
        if(!fname.isEmpty())
            saveFile(fname);
    }
}
void Widget::showmsgBrowser(QString msg)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->msgBrowser->setTextColor(Qt::blue);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->msgBrowser->append("[ " +name+" ] " + time);
    ui->msgBrowser->append(msg);

}
QString Widget::getMessage()  //获得要发送的信息
{
    QString msg = ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();
    return msg;
}
bool Widget::saveFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("保存文件"),tr("无法保存文件 %1:\n %2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->msgBrowser->toPlainText();

    return true;
}

void Widget::on_openwjBtn_clicked()
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
void Widget::receivfile(QString strfilename)
{
    int row = clien->recvtable->rowCount();
    clien->recvtable->setRowCount(row+1);
    clien->recvtable->setItem(row,0,new QTableWidgetItem(strfilename));
    clien->recvtable->setItem(row,1,new QTableWidgetItem(tr("文件")));
    ui->stackedWidget->setCurrentIndex(1);
    if(cnfiglist.at(14) == "0")
        return;
    else {
        if(clien->recvtable->rowCount()==1)
        {
            QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh_mm_ss");
            QString dirpath = cnfiglist.at(15)+"/"+usrname+"("+usrip+")/"+time;
            QDir dir;
            if(!dir.exists(dirpath))
               dir.mkpath(dirpath);
            recvfilepath = dirpath+tr("/%1").arg(strfilename);
            if(!recvfilepath.isEmpty())
            {
                clien->setFileName(recvfilepath);
                clien->setHostAddr(QHostAddress(usrip));
            }
        }
    }
}
void Widget::on_sendcancleBtn_clicked()
{
    serv->sendtable->setRowCount(0);
    serv->progressBar->setValue(0);
}

void Widget::on_receivcancelBtn_clicked()
{
    clien->recvtable->setRowCount(0);
    clien->progressBar->setValue(0);
//    clien->CancleBtn_clicked();
//    emit sndMessage(Refuse,ip,NULL);
}

void Widget::on_saveasBtn_clicked()
{
    recvfileName = clien->recvtable->item(0,0)->text();
    recvfilepath = QFileDialog::getSaveFileName(0,tr("保存文件"),recvfileName);
    if(!recvfilepath.isEmpty())
    {
        clien->setFileName(recvfilepath);
        clien->setHostAddr(QHostAddress(usrip));
    }
}
void Widget::on_receivBtn_clicked()
{
    recvfileName = clien->recvtable->item(0,0)->text();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh_mm_ss");
//    QString dirpath = QDir::currentPath()+"/Recv Files/"+usrname+"("+usrip+")/"+time;
    QString dirpath = cnfiglist.at(13)+"/"+usrname+"("+usrip+")/"+time;
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
void Widget::sendfilmOk()
{
//    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->msgBrowser->setTextColor(Qt::blue);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
    sendFileName = serv->sendtable->item(0,0)->text();
    ui->msgBrowser->append(tr(" 文件 ")+sendFileName+tr(" 发送成功"));
    serv->sendtable->removeRow(0);
    if(serv->sendtable->rowCount()>0)
    {
        sendfilepath = serv->sendtable->item(0,3)->text();
        serv->Sendfilm(sendfilepath);
    }

}
void Widget::receivfilmOk()
{
//    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->msgBrowser->setTextColor(Qt::blue);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
//    recvfileName = clien->recvtable->item(0,0)->text();
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

}
void Widget::curFmtChanged(const QTextCharFormat &fmt)
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

void Widget::closesrv()
{
    serv->refused();
}
bool Widget::IsValidDragFile(QDropEvent *e)
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

void Widget::AcceptFolder(QString folder)
{
    if(!folder.isEmpty())
    {
        QString FileName = folder.right(folder.size() - folder.lastIndexOf('/')-1);
        int row = serv->sendtable->rowCount();
        serv->sendtable->setRowCount(row+1);
        serv->sendtable->setItem(row,0,new QTableWidgetItem(FileName));
        serv->sendtable->setItem(row,1,new QTableWidgetItem(tr("文件夹")));
        serv->sendtable->setItem(row,3,new QTableWidgetItem(folder));
    }
/*    QDir dir(folder);
    //文件
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);

    for(int i = 0; i < file_list.size(); i++)
    {
        QString fileName = file_list[i].filePath();
        AcceptFile(fileName);
    }
    //目录
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < folder_list.size(); i++)
    {
        AcceptFolder(folder_list[i].absoluteFilePath());
    }*/
}

void Widget::AcceptFile(QString pathfile)
{
    if(!pathfile.isEmpty())
    {
        QString FileName = pathfile.right(pathfile.size() - pathfile.lastIndexOf('/')-1);
        int row = serv->sendtable->rowCount();
        serv->sendtable->setRowCount(row+1);
        serv->sendtable->setItem(row,0,new QTableWidgetItem(FileName));
        serv->sendtable->setItem(row,1,new QTableWidgetItem(tr("文件")));
        serv->sendtable->setItem(row,3,new QTableWidgetItem(pathfile));
    }
}

/*void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    if(IsValidDragFile(event))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void Widget::dragMoveEvent(QDragMoveEvent *event)
{
    if(IsValidDragFile(event))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}*/

/*void Widget::dropEvent(QDropEvent *event)
{
    QStringList files = event->mimeData()->text().split("\n");
    for(int i = 0; i < files.size(); i++)
    {
        QString fileName = files[i].mid(8); //  去掉头file:///

        // 接受目录
        QDir dir(fileName);
        if (dir.exists())
        {
            AcceptFolder(fileName);
            continue;
        }

        if(QFile::exists(fileName))
        {
            AcceptFile(fileName);
            continue;
        }
    }
}*/
void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
    else event->ignore();
}
void Widget::dropEvent(QDropEvent *event)
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
/*        if(QFile::exists(fileName))
        {
            AcceptFile(fileName);
            continue;
        }*/
    }
}
/*
void Widget::dropEvent(QDropEvent *event)
{
    const QMimeData *mineData = event->mimeData();
    if(mineData->hasUrls())
    {
        QList<QUrl>urlList=mineData->urls();
        if(urlList.isEmpty())return;
        QString fileName = urlList.at(0).toLocalFile();
        if(fileName.isEmpty())return;
        if(!fileName.isEmpty())
        {
            QFile file(fileName);
            if(!file.open(QIODevice::ReadOnly))return;
            QTextStream in(&file);
            ui->msgTxtEdit->setText(in.readAll());
        }
    }
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())return;
    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty())return;
    ui->msgTxtEdit->setText(fileName);
}
*/
