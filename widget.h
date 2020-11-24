#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include "server.h"
#include "client.h"
#include <QTextCharFormat>
class QFile;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QStringList confg,QStringList usrinfor);
    ~Widget();
    void showmsgBrowser(QString msg);
    void receivfile(QString strfilename);
    void closesrv();
signals:
    void sndFileName(QString fileName);
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
    void on_openwjBtn_clicked();
    void on_sendcancleBtn_clicked();
    void on_receivcancelBtn_clicked();
    void on_saveasBtn_clicked();
    void sendfilmOk();
    void receivfilmOk();
    void curFmtChanged(const QTextCharFormat &fmt);

    void on_receivBtn_clicked();

private:
    Ui::Widget *ui;
    QString name;
    QString ip;
    QString usrname;
    QString usrip;
    QStringList cnfiglist;             //配置信息
    QString sendfilepath;           //发送文件路径
    QString sendFileName;        //发送文件名称
    QFile *locFile;
    QString recvfilepath;           //接收文件路径
    QString recvfileName;        //接收文件名称
    Server *serv;
    Client *clien;
    QColor color;

protected:
    QString getMessage();
    bool saveFile(const QString &filename);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    //判断是否为有效的文件
    virtual bool IsValidDragFile(QDropEvent *e);

    //接受目录
    // @note 遍例目录，调用AcceptFile
    virtual void AcceptFolder(QString folder);

    //接受文件
    virtual void AcceptFile(QString pathfile);

//    virtual void dragEnterEvent(QDragEnterEvent *e) override;
//    virtual void dragMoveEvent(QDragMoveEvent *e) override;
//    virtual void dropEvent(QDropEvent *e) override;


};
#endif // WIDGET_H
