#ifndef QLWIDGET_H
#define QLWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
//#include "server.h"
//#include "client.h"
#include <QTextCharFormat>
class QFile;

namespace Ui {
class QlWidget;
}

class QlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QlWidget(QWidget *parent ,QStringList qzinfor ,QStringList memberinfor);
    ~QlWidget();
    void showmsgBrowser(QString,QString);
    void updatemembertable(QStringList);
//    void receivfile(QString strfilename);
//    void closesrv();
signals:
//    void sndFileName(QString fileName);
    void sndMessage(int type,QString ip,QString msg);

private slots:
    void on_sendBtn_clicked();
    void on_exitBtn_clicked();
    void on_fontCbx_currentFontChanged(const QFont &f);
    void on_sizeCbx_currentIndexChanged(const QString &arg1);
    void on_boldTBtn_clicked(bool checked);
    void on_italicTBtn_clicked(bool checked);
    void on_underlineTBtn_clicked(bool checked);
    void on_colorTBtn_clicked();
    void on_saveTBtn_clicked();
//    void on_openwjBtn_clicked();
//    void on_sendcancleBtn_clicked();
//    void on_receivcancelBtn_clicked();
//    void on_saveasBtn_clicked();
//    void sendfilmOk();
//    void receivfilmOk();
    void curFmtChanged(const QTextCharFormat &fmt);

//    void on_receivBtn_clicked();
    void initqzmembset(QStringList);

private:
    Ui::QlWidget *ui;
    QString myip;             //本机ip
    QString myname;           //本机名称
    QString qzid;             //群号码
    QString qzname;           //群名称
    QString qzprofil;         //群简介
    QString qzprocl;          //群公告
    QString qzcredate;        //群创建时间
    QStringList memberlist;   //群在线成员列表
//    QString sendfilepath;           //发送文件路径
//    QString sendFileName;        //发送文件名称
//    QFile *locFile;
//    QString recvfilepath;           //接收文件路径
//    QString recvfileName;        //接收文件名称
//    Server *serv;
//    Client *clien;
    QColor color;

protected:
    QString getMessage();

//    bool saveFile(const QString &filename);
//    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dropEvent(QDropEvent *event) override;

    //判断是否为有效的文件
//    virtual bool IsValidDragFile(QDropEvent *e);

    //接受目录
    // @note 遍例目录，调用AcceptFile
//    virtual void AcceptFolder(QString folder);

    //接受文件
//    virtual void AcceptFile(QString pathfile);

//    virtual void dragEnterEvent(QDragEnterEvent *e) override;
//    virtual void dragMoveEvent(QDragMoveEvent *e) override;
//    virtual void dropEvent(QDropEvent *e) override;
};

#endif // QLWIDGET_H
