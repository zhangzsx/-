#ifndef MAINFORM_H
#define MAINFORM_H
#include <QWidget>
#include <QTextCharFormat>
#include <QtGui>
#include <QCoreApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QToolButton>
#include "widget.h"
#include "addqlwidget.h"
#include "qzinvitedialog.h"
#include "qlwidget.h"
#include "rwxml.h"
#include "addqlfriendwidget.h"
#include "setdialog.h"

class QUdpSocket;
const int Maxnum = 100;
namespace Ui {
class MainForm;
}
enum MsgType{Msg, UsrEnter, UsrLeft, FileName, Refuse ,QlMsg, QlInvite, QlEnter, QlQuit};

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    QStringList usrnamelist;
    QStringList iplist;
//    QStringList childWdgetlist;
    ~MainForm();


public slots:
    bool eventFilter(QObject *, QEvent *);

protected:
    void usrEnter(QString,QString);
    void usrEnterQz(QStringList,QString);
    void usrLeft(QString ipAddr);
    void usrLeftqz(QString,QString);
    void sndentMsg(MsgType type);
//    void initfriendwidget();
    void insertfriendwidget(QString insertname, QString insertip);
    void inMsg(QString ip,QString receiveip,QString msg);
    void receivMsg(MsgType,QString,QString,QString);
    void receivQlMsg(MsgType,QString,QString,QString);
    void receivFile(QString srvaddr,QString clntaddr, QString filename);
    void initQl();                                     //初始化群聊功能
    void receivinvite(QString,QString,QStringList);
    void addqzlb(QString,QString);
    void updateqzWdgtmember(QStringList);
    QString getIP();
    QString getUsr();
    QString getMsg();
    void createRightMenu();

private:
    Ui::MainForm *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QString uName;
    QString address;
    QString fileName;
    QString xmlfile;
    QStringList Cfglist;      //配置信息列表
    QToolButton *toolBtn;       //私聊列表按钮
    QToolButton *addqltoolBtn;  //增加群组按钮
    QToolButton *qltoolBtn;     //群聊列表按钮
    Widget *chatWidget;          //私聊对话框指针
    QlWidget *qlWidget;          //群聊对话框指针
    QList<Widget*> chatWidgetList; //保存正在对话的私聊对话框指针
    QList<QlWidget*> qlWidgetList; //保存正在对话的群聊对话框指针
    QList<QStringList> Qzlblist;   //保存已加入的群组及群组详细信息
    QList<QStringList> Qzzxmemberlist;   //记录已加入的群组在线好友：at 0：群组id;单数为ip、双数为name
    QStringList Qzxxlist;          //待加入的群组详细信息
    AddqlWidget *addqlwidget;      //新增群组对话框指针
    QzinviteDialog *qzinvitdiag;   //群组邀请消息提示框指针
    AddqlfriendWidget *addfrienddg;  //添加群组好友对话框指针
    QColor color;
    QVBoxLayout *xllayout;         //私聊列表布局
    QVBoxLayout *qllayout;         //群聊列表布局
//    int usrnumber;
    QWidget* delewidget;
    RwXml *rwxml;
    SetDialog *setdlg;
//    QWidget* GetWidgetPtrByName(QString strWidget);
    QMenu *rightMenu;         //右键菜单
    QAction *addfriend;         //添加好友
    QAction *quitqz;       //退出群聊
    QString qzid_RightBtn;  //鼠标右键点击的群组id

private slots:
    void processPendingDatagrams();
    void showChatWidget(int);    //显示私聊对话框
    void showQlChatWidget(int);  //显示群聊对话框
    void sndMsg(MsgType type,QString ip,QString msg);
    void sndMesage(int type,QString ip,QString msg);
    void on_refurbish_clicked();
    void on_slBtn_clicked();
    void on_qlBtn_clicked();
    void addqlWidget();
    void sendinvite(QStringList,QStringList);
    void sendinvite_addfriend(QString,QStringList);
    void updateqzlb(QString,QString);
    void on_set_clicked();
    void acceptinvit(QString,QString);
    void clicked_rightMenu(const QPoint &pos);
    void addfriends();
    void quitqzs();
    void restart_set();
};

#endif // MAINFORM_H
