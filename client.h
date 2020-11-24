#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QHostAddress>
#include <QFile>
#include <QTime>
#include <QTableView>
#include <QStandardItemModel>
#include <QProgressBar>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>

class QTcpSocket;

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT
    
public:
    explicit Client(QWidget *parent = 0);
    ~Client();

    void setHostAddr(QHostAddress addr);
    void setFileName(QString name);
//    QStandardItemModel *recvmodel;
    QTableWidget *recvtable;
    QProgressBar *progressBar;
    void CancleBtn_clicked();
protected:
    void closeEvent(QCloseEvent *);
    void initreceiveview();
    void on_cClose();
private:
//    Ui::Client *ui;

    QTcpSocket *tClnt;
    quint16 blockSize;
    QHostAddress hostAddr;
    qint16 tPort;

    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 fileNameSize;
    QString fileName;
    QFile *locFile;
    QByteArray inBlock;

    QTime time;
//    QTableView *recvView;

    QGridLayout *mainlayout;

//    QTableWidgetItem *recvtableitem;
private slots:

    void newConn();
    void readMsg();
    void displayErr(QAbstractSocket::SocketError);
//    void on_cCloseBtn_clicked();
signals:
    void receivfilmOk();
};

#endif // CLIENT_H
