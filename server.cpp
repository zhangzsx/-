#include "server.h"

#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QApplication>
#include <QHeaderView>

Server::Server(QWidget *parent) :
    QDialog(parent)
//    ui(new Ui::Server)
{
//    ui->setupUi(this);
//    setFixedSize(400,207);
    progressBar = new QProgressBar();
    tPort = 4526;
    tSrv = new QTcpServer(this);
    connect(tSrv, SIGNAL(newConnection()), this, SLOT(sndMsg()));
    initsendview();
    initSrv();
}

Server::~Server()
{
//    delete ui;
}

void Server::initSrv()
{
    payloadSize = 8*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesTobeWrite = 0;

/*    ui->sStatusLbl->setText(tr("请选择要传送的文件"));
    ui->progressBar->reset();
    ui->sOpenBtn->setEnabled(true);
    ui->sSendBtn->setEnabled(false);*/

    tSrv->close();
}

void Server::sndMsg()
{
    totalBytes = 0;
    bytesWritten = 0;
    bytesTobeWrite = 0;
    clntConn = tSrv->nextPendingConnection();  //获取一个已经连接tcp套接字
    connect(clntConn,SIGNAL(bytesWritten(qint64)),this,SLOT(updClntProgress(qint64)));
    locFile = new QFile(filePath);
    if(!locFile->open((QFile::ReadOnly))){
        QMessageBox::warning(this, tr("应用程序"), tr("无法读取文件 %1:\n%2").arg(filePath).arg(locFile->errorString()));
        return;
    }
    totalBytes = locFile->size();  //文件的总字节数
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_8);
    time.start();  // 开始计时
    QString curFile = filePath.right(filePath.size() - filePath.lastIndexOf('/')-1);  //待发送文件名
    sendOut << qint64(0) << qint64(0) << curFile;
    totalBytes += outBlock.size();  //文件名大小信息+实际文件大小
    sendOut.device()->seek(0);      //设置文件指针从0开始
    sendOut << totalBytes << qint64(outBlock.size() - sizeof(qint64)*2);
    bytesTobeWrite = totalBytes - clntConn->write(outBlock);  //发送缓冲区数据，同时修改待发送的数据长度
    outBlock.resize(0);
}

void Server::updClntProgress(qint64 numBytes)
{
    qApp->processEvents();
    bytesWritten += (int)numBytes;
    if (bytesTobeWrite > 0)    //没发送完
    {
        outBlock = locFile->read(qMin(bytesTobeWrite, payloadSize));  //每次最多发送8k
        bytesTobeWrite -= (int)clntConn->write(outBlock);
        outBlock.resize(0);
    } else {
        locFile->close();
    }
    progressBar->setMaximum(totalBytes);
    progressBar->setValue(bytesWritten);

/*//    float useTime = time.elapsed();
//    double speed = bytesWritten / useTime;
    ui->sStatusLbl->setText(tr("已发送 %1MB (%2MB/s) \n共%3MB 已用时:%4秒\n估计剩余时间：%5秒")
                   .arg(bytesWritten / (1024*1024))
                   .arg(speed*1000 / (1024*1024), 0, 'f', 2)
                   .arg(totalBytes / (1024 * 1024))
                   .arg(useTime/1000, 0, 'f', 0)
                   .arg(totalBytes/speed/1000 - useTime/1000, 0, 'f', 0));*/

    if(bytesWritten == totalBytes) {
        locFile->close();
        tSrv->close();
        emit sendfilmOk();
        progressBar->setValue(0);
//        sendtable->setRowCount(0);
    }
}
/*void Server::on_sOpenBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        theFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
        int row = sendtable->rowCount();
        sendtable->insertRow(row);
        sendtable->item(row,0)->setText(theFileName);
        sendtable->item(row,1)->setText(tr("文件"));
        sendtable->item(row,2)->setText(fileName);*/

/*        ui->sStatusLbl->setText(tr("要传送的文件为：%1 ").arg(theFileName));
        ui->sSendBtn->setEnabled(true);
        ui->sOpenBtn->setEnabled(false);
    }
}*/

void Server::Sendfilm(QString filepath)
{
    filePath = filepath;
    if(!tSrv->listen(QHostAddress::Any,tPort))//开始监听
    {
        qDebug() << tSrv->errorString();
//        close();
        return;
    }
}

void Server::on_sClose()
{
    if(tSrv->isListening())
    {
        tSrv->close();  //当tcp正在监听时，关闭tcp服务器应用，即按下close键时就不监听tcp请求了
        if (locFile->isOpen())
            locFile->close();
        clntConn->abort();  //Socket::终止当前的连接，禁止写入缓冲区
    }
//    close();
}

void Server::refused()
{
    tSrv->close();
//    ui->sStatusLbl->setText(tr("对方拒绝接收！"));
}

void Server::closeEvent(QCloseEvent *)
{
    on_sClose();
}
void Server::initsendview()
{
    progressBar = new QProgressBar();
    progressBar->setMaximumSize(180,5);
    sendtable = new QTableWidget();
    sendtable->setColumnCount(4);
    QStringList headlist;
    headlist << tr("名称") << tr("类型") << tr("大小") << tr("路径");
    sendtable->setHorizontalHeaderLabels(headlist);
//    sendtable->resizeColumnsToContents();
    sendtable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    sendtable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    QGridLayout *mainlayout =new QGridLayout(this);
    mainlayout->addWidget(progressBar,0,0);
    mainlayout->addWidget(sendtable,1,0);
    mainlayout->setSizeConstraint(QLayout::SetFixedSize);
}
