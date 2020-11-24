#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QTime>
#include <QProgressBar>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QGridLayout>
class QFile;
class QTcpServer;
class QTcpSocket;

/*namespace Ui {
class Server;
}*/

class Server : public QDialog
{
    Q_OBJECT
    
public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    
    void initSrv();
    void refused();
    void Sendfilm(QString);
//    QStandardItemModel *sendmodel;
    QTableWidget *sendtable;
    QProgressBar *progressBar;

protected:
    void closeEvent(QCloseEvent *);
    void initsendview();
    void initreceiveview();
    void on_sClose();

private:
//    Ui::Server *ui;
    qint16 tPort;
    QTcpServer *tSrv;
    QString filePath;   //待发送文件路径
//    QString theFileName;
    QFile *locFile;

    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesTobeWrite;
    qint64 payloadSize;
    QByteArray outBlock;

    QTcpSocket *clntConn;

    QTime time;

//    QTableView *sendView;


private slots:
    void sndMsg();
    void updClntProgress(qint64 numBytes);
//    void on_sOpenBtn_clicked();

signals:
    void sndFileName(QString fileName);
    void sendfilmOk();
};

#endif // SERVER_H
