#include "client.h"

#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>
#include <QHeaderView>

Client::Client(QWidget *parent) :
    QDialog(parent)
//    ui(new Ui::Client)
{
    setFixedSize(180,240);
    initreceiveview();
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    tClnt = new QTcpSocket(this);
    tPort = 4526;
    connect(tClnt, SIGNAL(readyRead()), this, SLOT(readMsg()));
    connect(tClnt, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(displayErr(QAbstractSocket::SocketError)));
}

Client::~Client()
{
//    delete ui;
}

void Client::setFileName(QString name)
{
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    locFile = new QFile(name);
}

void Client::setHostAddr(QHostAddress addr)
{
    hostAddr = addr;
    newConn();
}

void Client::newConn()
{
    blockSize = 0;
    tClnt->abort();
    tClnt->connectToHost(hostAddr, tPort);
    time.start();
}

void Client::readMsg()
{
    QDataStream in(tClnt);
    in.setVersion(QDataStream::Qt_5_8);

//    float useTime = time.elapsed();

    if (bytesReceived <= sizeof(qint64)*2) {
        if ((tClnt->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0))
        {
            //接收数据总大小信息和数据文件名大小
            in>>totalBytes>>fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((tClnt->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)){
            //开始接收文件，并建立文件
            in>>fileName;
            bytesReceived +=fileNameSize;

            if(!locFile->open(QFile::WriteOnly)){
                QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2.").arg(fileName).arg(locFile->errorString()));
                locFile->close();
                tClnt->close();
                return;
            }
        } else {
            return;
        }
    }
    if (bytesReceived < totalBytes) {
        bytesReceived += tClnt->bytesAvailable();  //把缓冲区内容写入文件
        inBlock = tClnt->readAll();
        locFile->write(inBlock);
        inBlock.resize(0);                         //清空缓冲区
    }
    progressBar->setMaximum(totalBytes);
    progressBar->setValue(bytesReceived);

//    double speed = bytesReceived / useTime;
/*    ui->cStatusLbl->setText(tr("已接收 %1MB (%2MB/s) \n共%3MB 已用时：%4秒\n估计剩余时间：%5秒")
                                      .arg(bytesReceived / (1024*1024))
                                      .arg(speed*1000/(1024*1024),0,'f',2)
                                      .arg(totalBytes / (1024 * 1024))
                                      .arg(useTime/1000,0,'f',0)
                                      .arg(totalBytes/speed/1000 - useTime/1000,0,'f',0));*/

    if(bytesReceived == totalBytes)
    {
        locFile->close();
        tClnt->close();
        emit receivfilmOk();
        progressBar->setValue(0);
//        recvtable->setRowCount(0);
    }
}

void Client::displayErr(QAbstractSocket::SocketError sockErr)
{
    switch(sockErr)
    {
        //RemoteHostClosedError为远处主机关闭了连接时发出的错误信号
        case QAbstractSocket::RemoteHostClosedError : break;
        default : qDebug() << tClnt->errorString();
    }
}
void Client::CancleBtn_clicked()
{
    tClnt->abort();
    if (locFile->isOpen())
        locFile->close();
}

void Client::on_cClose()
{
    tClnt->abort();
    if (locFile->isOpen())
        locFile->close();
//    close();
}

void Client::closeEvent(QCloseEvent *)
{
    on_cClose();
}
void Client::initreceiveview()
{
    progressBar = new QProgressBar();
    progressBar->setMaximumSize(180,20);
    recvtable = new QTableWidget();
    recvtable->setColumnCount(4);
    QStringList headlist;
    headlist << tr("名称") << tr("类型") << tr("大小") << tr("路径");
    recvtable->setHorizontalHeaderLabels(headlist);
//    recvtable->resizeColumnsToContents();
    recvtable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    recvtable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    mainlayout =new QGridLayout(this);
    mainlayout->addWidget(progressBar,0,0);
    mainlayout->addWidget(recvtable,1,0);
    mainlayout->setSizeConstraint(QLayout::SetFixedSize);
}
