#include "mainform.h"
#include "ui_mainform.h"
#include <QUdpSocket>
#include <QHostInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QNetworkInterface>
#include <QProcess>
#include <QColorDialog>

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    setWindowTitle(tr("内网通2020版"));                   //设置主窗体的标题
    setWindowIcon(QPixmap(":/images/qq.png"));              //设置主窗体标题栏图标
    setAttribute(Qt::WA_QuitOnClose,true);
    address = getIP();
    Qzlblist.clear();
    xmlfile = address+".xml";
    rwxml = new RwXml();
    if(rwxml->cfgdocRead())
    {
        Cfglist = rwxml->confg;
        QString nametemp = Cfglist.at(1);
        QString iptemp = Cfglist.at(2);
        if(iptemp == address)
            uName = nametemp;
        else
        {
            uName = "Administrator";
            rwxml->initcnfg(address);
            Cfglist = rwxml->confg;
        }
    }
    else{
        uName = "Administrator";
        rwxml->initcnfg(address);
        Cfglist = rwxml->confg;
    }
    if(rwxml->zonedocRead(xmlfile))
        Qzlblist = rwxml->Qzlblist;
    else
        rwxml->creatzonexml(xmlfile);
    udpSocket = new QUdpSocket(this);
    port = 21208;

//    usrnumber = 0;
    ui->MyName->setText(uName);
    ui->MyIP->setText(address);
    ui->MyImage->setPixmap(QPixmap(":/images/spqy.png"));
    ui->MyImage->setFixedSize(QPixmap(":/images/spqy.png").size());
    ui->set->setIcon(QPixmap(":/images/set.png"));
    ui->set->setFixedSize(QPixmap(":/images/set.png").size());
    ui->refurbish->setIcon(QPixmap(":/images/refurbish.PNG"));
    ui->refurbish->setFixedSize(QPixmap(":/images/refurbish.PNG").size());
    ui->Online->setText(tr("在线人数：%1").arg(iplist.count()+1));

    ui->siliaoBtn->setIcon(QPixmap(":/images/siliao.png"));
    ui->siliaoBtn->setFixedSize(QPixmap(":/images/siliao.png").size());
    ui->qunliaoBtn->setIcon(QPixmap(":/images/qunliao.png"));
    ui->qunliaoBtn->setFixedSize(QPixmap(":/images/qunliao.png").size());
    connect(ui->siliaoBtn, SIGNAL(clicked()), this, SLOT(on_slBtn_clicked()));
    connect(ui->qunliaoBtn, SIGNAL(clicked()), this, SLOT(on_qlBtn_clicked()));

    xllayout=new QVBoxLayout(ui->friendwidget);
    xllayout->setMargin(10);//布局中各窗体的显示间距
    xllayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);//布局中各窗体的显示位置

    initQl();

    udpSocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    sndMsg(UsrEnter,NULL,NULL);
}

MainForm::~MainForm()
{
    delete ui;
}
/*void MainForm::initfriendwidget()
{
    if(usrnumber>=1)
    {
        for(int i = usrnumber-1; i>=0;--i)
        {
            toolBtn[i]->setParent(NULL);
            layout->removeWidget(toolBtn[i]);
            toolBtn[i]->removeEventFilter(this);
        }
        usrnumber = 0;
    }
    if(usrnamelist.count()>0)
    {
        for (int i = 0; i<usrnamelist.count();i++) {
    //        toolBtn[i] =new QToolButton;
            toolBtn[i]->setText(usrnamelist.at(i)+"\n"+iplist.at(i));
            toolBtn[i]->setIcon(QPixmap(":/images/spqy.png"));
            toolBtn[i]->setIconSize(QPixmap(":/images/spqy.png").size());
            toolBtn[i]->setAutoRaise(true);
            toolBtn[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //        layout->addWidget(toolBtn[i]);
            layout->insertWidget(i,toolBtn[i]);
            toolBtn[i]->installEventFilter(this);
        }
    //    layout->addStretch();//插入一个占位符
        usrnumber = usrnamelist.count();
    }
    update();
}*/

void MainForm::insertfriendwidget(QString insertname, QString insertip)
{
    int usrnumber = usrnamelist.count();
    toolBtn =new QToolButton;
    toolBtn->setText(insertname+"\n"+insertip);
    toolBtn->setIcon(QPixmap(":/images/ytouxiang.png"));
    toolBtn->setIconSize(QPixmap(":/images/ytouxiang.png").size());
    toolBtn->setAutoRaise(true);
    toolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    xllayout->insertWidget(usrnumber,toolBtn);
    toolBtn->installEventFilter(this);

    update();
}

void MainForm::showChatWidget(int num)
{
    QStringList usrinfor;
    usrinfor.append(usrnamelist.at(num));
    usrinfor.append(iplist.at(num));
    usrinfor.append(uName);
    usrinfor.append(address);
    chatWidget = new Widget(0,Cfglist,usrinfor);
    chatWidget->show();
    connect(chatWidget, SIGNAL(sndMessage(int,QString,QString)), this, SLOT(sndMesage(int,QString,QString)));
    chatWidget->installEventFilter(this);
    chatWidgetList.append(chatWidget);
}
QString MainForm::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
        {
            if(addr.toString().contains("127.0.")||addr.toString().contains("192.168."))
                continue;
            else
            return addr.toString();
        }
    }
    return 0;
}
void MainForm::sndentMsg(MsgType type)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
//    QString address = getIP();
    out << type << uName << address;
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);
}
void MainForm::usrEnter(QString usrname, QString ipaddr)
{
    if(ipaddr == address)
        return;
    else{
        for (int i = 0;i<iplist.count();i++) {
            if(ipaddr == iplist.at(i))
                return;
        }
        usrnamelist.append(usrname);
        iplist.append(ipaddr);
        insertfriendwidget(usrname,ipaddr);
        ui->Online->setText(tr("在线人数：%1").arg(iplist.count()+1));
//        sndentMsg(UsrEnter);
        sndMsg(UsrEnter,NULL,NULL);
    }

}
void MainForm::usrEnterQz(QStringList qzidlist,QString usrip)
{
    if(usrip == address) return;
    if(qzidlist.count()==0) return;
    for (int i = 0;i<Qzlblist.count();i++) {
        QStringList qztemp = Qzlblist.at(i);
        QString qzid = qztemp.at(0);
        int num = qzidlist.indexOf(qzid);
        if(num!=-1)
        {
            for (int j =0;j<Qzzxmemberlist.count();j++) {
                QStringList qzmem = Qzzxmemberlist.at(j);
                if(qzid ==qzmem.at(0))
                {
                    int ipindex = qzmem.indexOf(usrip);
                    if(ipindex!=-1)
                        return;
                    QString usrna = usrnamelist.at(iplist.indexOf(usrip));
                    qzmem << usrip << usrna;
                    Qzzxmemberlist.replace(j,qzmem);
                    updateqzWdgtmember(qzmem);
                }
            }
        }
    }
}
void MainForm::updateqzWdgtmember(QStringList qzmemberlist)   //更新正在对话的群组对话框的在线好友列表
{
    for (int i = 0; i<qlWidgetList.count();i++)
    {
        QlWidget *qlWidgettemp = qlWidgetList.at(i);
        QString str = qlWidgettemp->windowTitle();
        int index= str.indexOf("\n");
        QString chatip = str.right(str.length()-index-1);
        if(qzmemberlist.at(0) == chatip)
        {
            qlWidgettemp->updatemembertable(qzmemberlist);
            return;
        }
    }
}
void MainForm::usrLeft(QString ipAddr)
{
    if(ipAddr == address)
        return;
    QWidget *pWidget = NULL;
    for (int i = 0;i<iplist.count();i++) {
        if(ipAddr == iplist.at(i))
        {
            usrnamelist.removeAt(i);
            iplist.removeAt(i);

            pWidget = xllayout->itemAt(i)->widget();
            pWidget->removeEventFilter(this);
            pWidget->setParent(NULL);
            xllayout->removeWidget(pWidget);
            break;
        }
    }
    ui->Online->setText(tr("在线人数：%1").arg(iplist.count()+1));
    if(Qzzxmemberlist.count()==0)
        return;
    for (int j = 0;j<Qzzxmemberlist.count();j++) {
        QStringList memlist = Qzzxmemberlist.at(j);
        int num = memlist.indexOf(ipAddr);
        if(num !=-1)
        {
            memlist.removeAt(num+1);
            memlist.removeAt(num);
            Qzzxmemberlist.replace(j,memlist);
            updateqzWdgtmember(memlist);
        }
    }
}
void MainForm::usrLeftqz(QString ipAddr,QString qzid)
{
    for (int i = 0;i<Qzzxmemberlist.count();i++) {
        QStringList memlist = Qzzxmemberlist.at(i);
        if(memlist.at(0) != qzid)
            continue;
        int num = memlist.indexOf(ipAddr);
        if(num !=-1)
        {
            memlist.removeAt(num+1);
            memlist.removeAt(num);
            Qzzxmemberlist.replace(i,memlist);
            updateqzWdgtmember(memlist);
        }
    }
}
void MainForm::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int msgType;
        in >> msgType;
        QString usrName,ipAddr,receiveip,msg,fileName,qzid;
        QStringList qzList,qzidlist;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch(msgType)
        {
        case Msg:
            in >> receiveip >> ipAddr >> msg;
//            inMsg(ipAddr,receiveip,msg);
            receivMsg(Msg,ipAddr,receiveip,msg);
            break;

        case UsrEnter:
            in >> usrName >> ipAddr >> qzidlist;
            usrEnter(usrName,ipAddr);
            usrEnterQz(qzidlist,ipAddr);
            break;

        case UsrLeft:
            in >> ipAddr;
            usrLeft(ipAddr);
            break;

        case FileName:
            in >> receiveip >> ipAddr >> fileName;                //接收方ip  //发送方ip
//            receivFile(ipAddr,receiveip,fileName);
            receivMsg(FileName,ipAddr,receiveip,fileName);
            break;

        case Refuse:
            in >> receiveip >> ipAddr;
            receivMsg(Refuse,ipAddr,receiveip,NULL);
            break;

        case QlMsg:
            in >> qzid >> ipAddr >> msg;       //群组id  //发送方ip
            if(ipAddr != address)
                receivQlMsg(QlMsg,ipAddr,qzid,msg);
            break;

        case QlInvite:
            in >> receiveip >> ipAddr >> qzList;
            receivinvite(receiveip,ipAddr,qzList);
            break;

        case QlEnter:
            in >> qzid >> ipAddr;
            qzidlist << qzid;
            usrEnterQz(qzidlist,ipAddr);
            break;

        case QlQuit:
            in >> qzid >> ipAddr;
            usrLeftqz(ipAddr,qzid);
            break;
        }
    }
}
void MainForm::closeEvent(QCloseEvent *event)
{
    sndMsg(UsrLeft,NULL,NULL);
    event->accept();
//    QWidget::closeEvent(e);
}
bool MainForm::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *pWidget = NULL;
    //鼠标双击弹出私聊对话框事件
    for(int i=0;i<xllayout->count();i++)
    {
        pWidget = xllayout->itemAt(i)->widget();
        if(watched==pWidget)	                		//首先判断当前发生事件的对象
        {
            //判断发生的事件类型
            if(event->type()==QEvent::MouseButtonDblClick)
            {
                //将事件event转化为鼠标事件
                QMouseEvent *mouseEvent=(QMouseEvent *)event;
                QString iptemp = iplist.at(i);
                /* 以下根据鼠标的按键类型分别显示 */
                if(mouseEvent->buttons()&Qt::LeftButton)
                {
                    for (int j = 0; j<chatWidgetList.count();j++) {
                        Widget *chatWidgettemp = chatWidgetList.at(j);
                        QString str = chatWidgettemp->windowTitle();
                        int index= str.indexOf("\n");
                        QString chatip = str.right(str.length()-index-1);
                        if(chatip == iptemp)
                        {
                            return false;
                        }
                    }
                    showChatWidget(i);
                    return false;
                }
            }
        }
    }
    //鼠标点击退出私聊对话框，删除对应私聊对话框指针事件
    for (int i = 0; i<chatWidgetList.count();i++)
    {
        Widget *chatWidgettem = chatWidgetList.at(i);
        if(watched==chatWidgettem)			//首先判断当前发生事件的对象
        {
        //判断发生的事件类型
            if(event->type()==QEvent::Close)
            {
                chatWidgetList.removeAt(i);
                break;
            }
        }
    }
    //群聊列表事件
    for(int i=1;i<qllayout->count();i++)
    {
        pWidget = qllayout->itemAt(i)->widget();
        if(watched==pWidget)	                		//首先判断当前发生事件的对象
        {
            //判断发生的事件类型
            if(event->type()==QEvent::MouseButtonDblClick)   //鼠标双击事件
            {
                //将事件event转化为鼠标事件
                QMouseEvent *mouseEvent=(QMouseEvent *)event;
                QStringList qzlist = Qzlblist.at(i-1);
                QString idtemp = qzlist.at(0);
                /* 以下根据鼠标的按键类型分别显示 */
                if(mouseEvent->buttons()&Qt::LeftButton)
                {
                    for (int j = 0; j<qlWidgetList.count();j++) {
                        QlWidget *qlWidgettemp = qlWidgetList.at(j);
                        QString str = qlWidgettemp->windowTitle();
                        int index= str.indexOf("\n");
                        QString chatid = str.right(str.length()-index-1);
                        if(chatid == idtemp)
                        {
                            return false;
                        }
                    }
                    showQlChatWidget(i-1);
                    return false;
                }

            }
            if(event->type()==QEvent::MouseButtonPress)   //鼠标单击事件
            {
                //将事件event转化为鼠标事件
                QMouseEvent *mouseEvent=(QMouseEvent *)event;
                QStringList qzlist = Qzlblist.at(i-1);
                QString idtemp = qzlist.at(0);
                /* 以下根据鼠标的按键类型分别显示 */
                if(mouseEvent->buttons()&Qt::RightButton)  //右键单击
                {
                    qzid_RightBtn = idtemp;
                    return false;
                }

            }

        }
    }
    //鼠标点击退出群聊对话框，删除对应群聊对话框指针事件
    for (int i = 0; i<qlWidgetList.count();i++)
    {
        QlWidget *qlWidget = qlWidgetList.at(i);
        if(watched==qlWidget)			//首先判断当前发生事件的对象
        {
        //判断发生的事件类型
            if(event->type()==QEvent::Close)
            {
                qlWidgetList.removeAt(i);
                break;
            }
        }
    }
    return false;
}
void MainForm::inMsg(QString ip,QString receiveip,QString msg)
{
    if(receiveip != address)
        return;
    for (int i = 0; i<chatWidgetList.count();i++) {
        Widget *chatWidgettemp = chatWidgetList.at(i);
        QString str = chatWidgettemp->windowTitle();
        int index= str.indexOf("\n");
        QString chatip = str.right(str.length()-index-1);
        if(ip == chatip)
        {
            chatWidgettemp->showmsgBrowser(msg);
            return;
        }
    }
    int i = iplist.indexOf(ip);
    showChatWidget(i);
    chatWidget->showmsgBrowser(msg);
}
void MainForm::sndMsg(MsgType type,QString ip,QString msg)
{
    QHostAddress HostAddress;
    HostAddress.setAddress(ip);
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
//    int count;
    QStringList qzidlist;
    out << type;
    switch(type)
    {
    case Msg:
        out << ip << address << msg;
        udpSocket->writeDatagram(data,data.length(),HostAddress, port);
        return;

    case UsrEnter:
        for (int i = 0;i<Qzlblist.count();i++) {
            QStringList qztemp = Qzlblist.at(i);
            qzidlist << qztemp.at(0);
        }
        out << uName << address << qzidlist;
        break;

    case UsrLeft:
        out << address;
        break;

    case FileName: {
        out << ip << address  << msg;
        udpSocket->writeDatagram(data,data.length(),HostAddress, port);
        return;
    }

    case Refuse: {
        out << ip << address;
        udpSocket->writeDatagram(data,data.length(),HostAddress, port);
        return;
    }

    case QlMsg:
        out << ip << address << msg;    //消息类型、群组id、发送消息ip、消息内容
        break;

    case QlEnter:
        out << ip << address;       //进入的群组、本地ip
        break;

    case QlInvite:
        out << ip << address << Qzxxlist;
        break;

    case QlQuit:
        out << ip << address;
        break;
    }

    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);
}
/*QWidget* MainForm::GetWidgetPtrByName(QString strWidget)
{
    QWidget *pWidget = NULL;
    QString strName;
    QWidgetList chatWidgetList2 = qApp->allWidgets();
    for(int i = 0; i<chatWidgetList2.size();i++)
    {
        pWidget = chatWidgetList2.at(i);
//        strName = pWidget->accessibleName();
        strName = pWidget->windowTitle();
//        int index= strName.indexOf("\n");
//        QString chatip = strName.right(strName.length()-index-1);
        if(strName == strWidget)
        {
            pWidget = chatWidgetList2.at(i);
//            pWidget->send
//            chatWidgetList->at(i)->showmsgBrowser(strWidget);
//            pWidget->activateWindow();
            break;
        }
    }
    return pWidget;
}*/

void MainForm::on_refurbish_clicked()
{
    //清空用户记录
    usrnamelist.clear();
    iplist.clear();
    //清空窗体列表
    for (int i = xllayout->count()-1;i >= 0;i--) {
        QWidget *pWidget = NULL;
        pWidget = xllayout->itemAt(i)->widget();
        pWidget->removeEventFilter(this);
        pWidget->setParent(NULL);
        xllayout->removeWidget(pWidget);
    }
    int count = Qzzxmemberlist.count();
    for (int i = 0;i<count ;i++) {
        QStringList qzxx = Qzzxmemberlist.at(i);
        QStringList qzxxtemp;
        qzxxtemp << qzxx.at(0)  << address << uName;
        Qzzxmemberlist.replace(i,qzxxtemp);
    }
    ui->Online->setText(tr("在线人数：%1").arg(iplist.count()+1));
    //发送离开消息
    sndMsg(UsrLeft,NULL,NULL);
    //发送进入消息
    sndMsg(UsrEnter,NULL,NULL);
}
void MainForm::receivFile(QString srvaddr,QString clntaddr, QString filename)
{
    if(clntaddr != address)
        return;
    else{
        for (int i = 0; i<chatWidgetList.count();i++) {
            Widget *chatWidgettemp = chatWidgetList.at(i);
            QString str = chatWidgettemp->windowTitle();
            int index= str.indexOf("\n");
            QString chatip = str.right(str.length()-index-1);
            if(srvaddr == chatip)
            {
                chatWidgettemp->receivfile(filename);
                return;
            }
        }
        int i = iplist.indexOf(srvaddr);
        showChatWidget(i);
        chatWidget->receivfile(filename);
    }
}
void MainForm::sndMesage(int type,QString ip,QString msg)
{
    if(type == Msg)
        sndMsg(Msg,ip,msg);
    else if(type == FileName)
        sndMsg(FileName,ip,msg);
    else if(type == Refuse)
        sndMsg(Refuse,ip,NULL);
    else if(type == QlMsg)
        sndMsg(QlMsg,ip,msg);
}
void MainForm::receivMsg(MsgType type,QString srvaddr,QString clntaddr,QString msg)
{
    if(clntaddr != address)return;
    for (int i = 0; i<chatWidgetList.count();i++)
    {
        Widget *chatWidgettemp = chatWidgetList.at(i);
        QString str = chatWidgettemp->windowTitle();
        int index= str.indexOf("\n");
        QString chatip = str.right(str.length()-index-1);
        if(srvaddr == chatip)
        {
            if(type == Msg)
                chatWidgettemp->showmsgBrowser(msg);
            else if(type == FileName)
                chatWidgettemp->receivfile(msg);
            else if(type == Refuse)
                chatWidgettemp->closesrv();
            return;
        }
    }
    int i = iplist.indexOf(srvaddr);
    showChatWidget(i);
    if(type == Msg)
        chatWidget->showmsgBrowser(msg);
    else if(type == FileName)
        chatWidget->receivfile(msg);
    else if(type == Refuse)
        chatWidget->closesrv();
}
void MainForm::receivQlMsg(MsgType type,QString sendaddr,QString receivqz,QString msg)
{
    for (int i = 0;i<Qzlblist.count();i++)
    {
        QStringList qztemp = Qzlblist.at(i);
        if(receivqz == qztemp.at(0))
        {
            for (int j = 0; j<qlWidgetList.count();j++)
            {
                QlWidget *qlWidgettemp = qlWidgetList.at(j);
                QString str = qlWidgettemp->windowTitle();
                int index= str.indexOf("\n");
                QString chatip = str.right(str.length()-index-1);
                if(receivqz == chatip)
                {
//                    if(type == Msg)
                        qlWidgettemp->showmsgBrowser(sendaddr,msg);
/*                    else if(type == FileName)
                        qlWidgettemp->receivfile(msg);
                    else if(type == Refuse)
                        qlWidgettemp->closesrv();*/
                    return;
                }
            }
//            int i = iplist.indexOf(srvaddr);
            showQlChatWidget(i);
//            if(type == Msg)
                qlWidget->showmsgBrowser(sendaddr,msg);
/*            else if(type == FileName)
                qlWidget->receivfile(msg);
            else if(type == Refuse)
                qlWidget->closesrv();  */
        }

    }
}
void MainForm::on_slBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void MainForm::on_qlBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void MainForm::initQl()
{
    qllayout=new QVBoxLayout(ui->qunliao_widget);
    qllayout->setMargin(10);//布局中各窗体的显示间距
    qllayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);//布局中各窗体的显示位置

    addqltoolBtn =new QToolButton;
    addqltoolBtn->setText(tr("创建群组"));
    addqltoolBtn->setIcon(QPixmap(":/images/xinqunzu.png"));
    addqltoolBtn->setIconSize(QPixmap(":/images/xinqunzu.png").size());
    addqltoolBtn->setAutoRaise(true);
    addqltoolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    qllayout->insertWidget(0,addqltoolBtn);
    connect(addqltoolBtn, SIGNAL(clicked()), this, SLOT(addqlWidget()));
    ui->stackedWidget->setCurrentIndex(0);

    int num = Qzlblist.count();
    for(int i = 0; i<num;i++)
    {
        QStringList qzxx = Qzlblist.at(i);
        addqzlb(qzxx.at(0),qzxx.at(1));
        QStringList qzmemberip;
        qzmemberip << qzxx.at(0) << address << uName;
        Qzzxmemberlist << qzmemberip;
    }
}

void MainForm::addqlWidget()
{
    addqlwidget = new AddqlWidget(0,usrnamelist,iplist);
    addqlwidget->show();
    connect(addqlwidget,SIGNAL(addqz(QStringList,QStringList)),this,SLOT(sendinvite(QStringList,QStringList)));
}
void MainForm::sendinvite(QStringList qzzlList,QStringList ipList)
{
    Qzxxlist = qzzlList;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    Qzxxlist << time << time;
    for(int i = 0;i<ipList.count();i++)
    {
        QString iptemp = ipList.at(i);
        sndMsg(QlInvite,iptemp,NULL);
    }
    updateqzlb(qzzlList.at(0),qzzlList.at(1));
}
void MainForm::receivinvite(QString recvip,QString ipaddr,QStringList qzxxlist)
{
    if(recvip != address)
    {
        return;
    }
    int num = -1;
    num = iplist.indexOf(ipaddr);
    if(num == -1)
        return;
    Qzxxlist = qzxxlist;
    QStringList infor;
    infor << ipaddr << usrnamelist.at(num) << qzxxlist.at(0) << qzxxlist.at(1);
    qzinvitdiag = new QzinviteDialog(0,infor);
    qzinvitdiag->show();
    connect(qzinvitdiag,SIGNAL(acceptinvit(QString,QString)),this,SLOT(acceptinvit(QString,QString)));
}
void MainForm::acceptinvit(QString qzid,QString qzname)
{
    updateqzlb(qzid,qzname);
    on_refurbish_clicked();
//    sndMsg(UsrEnter,NULL,NULL);
}
void MainForm::updateqzlb(QString qzid,QString qzname)
{
    addqzlb(qzid,qzname);
    Qzlblist << Qzxxlist;
    QStringList qzmember;
    qzmember << Qzxxlist.at(0) << address << uName;
    Qzzxmemberlist << qzmember;
    rwxml->writeXml(xmlfile,Qzxxlist);
}
void MainForm::addqzlb(QString qzid,QString qzname)
{
    int num = qllayout->count();
    qltoolBtn =new QToolButton;
    qltoolBtn->setText(qzname+"\n"+qzid);
    qltoolBtn->setIcon(QPixmap(":/images/qunzu.png"));
    qltoolBtn->setIconSize(QPixmap(":/images/qunzu.png").size());
    qltoolBtn->setAutoRaise(true);
    qltoolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    qllayout->insertWidget(num,qltoolBtn);
    qltoolBtn->installEventFilter(this);

    qltoolBtn->setContextMenuPolicy(Qt::CustomContextMenu);      //设置右键菜单事项
    createRightMenu();
    connect(qltoolBtn,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(clicked_rightMenu(QPoint)));  //右键菜单响应
}

void MainForm::showQlChatWidget(int num)
{
    QStringList qzinfor = Qzlblist.at(num);
    QStringList memberinfor;
    for (int i = 0;i<Qzzxmemberlist.count();i++) {
        memberinfor = Qzzxmemberlist.at(i);
        if(memberinfor.at(0) == qzinfor.at(0))
            break;
    }
    qlWidget = new QlWidget(0,qzinfor,memberinfor);
    qlWidget->show();
    connect(qlWidget, SIGNAL(sndMessage(int,QString,QString)), this, SLOT(sndMesage(int,QString,QString)));
    qlWidget->installEventFilter(this);
    qlWidgetList.append(qlWidget);
}

void MainForm::on_set_clicked()
{
    setdlg = new SetDialog(0,Cfglist);
    setdlg->show();
    connect(setdlg,SIGNAL(setok()),this,SLOT(restart_set()));
}
void MainForm::restart_set()
{
    sndMsg(UsrLeft,NULL,NULL);
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath(),QStringList());
}
void MainForm::createRightMenu()
{
    rightMenu = new QMenu;
    addfriend = new QAction(tr("添加好友"),this);
    quitqz = new QAction(tr("退出群组"),this);

    rightMenu->addAction(addfriend);
    rightMenu->addAction(quitqz);
    connect(addfriend, SIGNAL(triggered()), this, SLOT(addfriends()));
    connect(quitqz, SIGNAL(triggered()), this, SLOT(quitqzs()));
}
void MainForm::clicked_rightMenu(const QPoint &pos)
{
    rightMenu->exec(QCursor::pos());
}
void MainForm::addfriends()
{
    QStringList qzzxtemp;
    for (int i = 0;i<Qzzxmemberlist.count();i++)
    {
        QStringList qzmemxx = Qzzxmemberlist.at(i);
        if(qzmemxx.at(0) == qzid_RightBtn)
        {
            qzzxtemp = qzmemxx;
            break;
        }
    }
    addfrienddg = new AddqlfriendWidget(0,usrnamelist,iplist,qzzxtemp);
    addfrienddg->show();
    connect(addfrienddg,SIGNAL(addfriend(QString,QStringList)),this,SLOT(sendinvite_addfriend(QString,QStringList)));
}
void MainForm::sendinvite_addfriend(QString qzid,QStringList ipList)
{
    QStringList qzzlList;
    for (int i = 0;i<Qzlblist.count();i++) {
        qzzlList = Qzlblist.at(i);
        if(qzzlList.at(0)==qzid)
        {
            Qzxxlist = qzzlList;
            break;
        }
    }
    for(int i = 0;i<ipList.count();i++)
    {
        QString iptemp = ipList.at(i);
        sndMsg(QlInvite,iptemp,NULL);
    }
}
void MainForm::quitqzs()
{
    sndMsg(QlQuit,qzid_RightBtn,NULL);
    rwxml->deletDomnotlist(xmlfile,qzid_RightBtn);
    for (int i = 0; i<qlWidgetList.count();i++) {
        QlWidget *qlWidgettemp = qlWidgetList.at(i);
        QString str = qlWidgettemp->windowTitle();
        int index= str.indexOf("\n");
        QString qzip = str.right(str.length()-index-1);
        if(qzip == qzid_RightBtn)
            qlWidgettemp->close();
    }
    QWidget *qlwidget = NULL;
    for (int i = 0;i<Qzlblist.count();i++) {
        QStringList qztemp = Qzlblist.at(i);
        if(qztemp.at(0)==qzid_RightBtn)
        {
            qlwidget = qllayout->itemAt(i+1)->widget();
            qlwidget->removeEventFilter(this);
            qlwidget->setParent(NULL);
            qllayout->removeWidget(qlwidget);
            Qzlblist.removeAt(i);
            return;
        }
    }
    for (int i = 0;i<Qzzxmemberlist.count();i++)
    {
        QStringList qzmemxx = Qzzxmemberlist.at(i);
        if(qzmemxx.at(0) == qzid_RightBtn)
            Qzzxmemberlist.removeAt(i);
    }
}
