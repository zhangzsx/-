#include "rwxml.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <QString>
#include <QObject>
RwXml::RwXml()
{

}
//读取 配置 文档
bool RwXml::cfgdocRead(){
    confg.clear();
    if(!docRead("IntwebCfg.xml"))
        return false;
    QDomElement root = doc.documentElement();  //获取dom树的根标签
    //遍历所有标签
    QDomNodeList list = root.childNodes();
    for(int i=0;i<list.count();i++){
        QString qzid = list.at(i).toElement().text();
        confg << qzid;
    }
    return true;
}
//重写 配置 文档
void RwXml::afreshConfg(QStringList configlist)
{
    if(!docRead("IntwebCfg.xml"))
        return;

    QDomElement root = doc.documentElement();
    QDomNodeList list = root.childNodes();
    QDomNode node = list.at(1);
    QDomNode oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(1));
    QDomNode newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(2);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(2));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(3);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(3));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(4);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(4));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(5);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(5));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(6);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(6));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(7);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(7));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(8);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(8));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(9);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(9));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(10);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(10));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(11);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(11));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(12);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(12));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(13);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(13));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(14);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(14));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

    node = list.at(15);
    oldnode = node.firstChild();
    node.firstChild().setNodeValue(configlist.at(15));
    newnode = node.firstChild();
    node.replaceChild(newnode,oldnode);

        //写入到文件
    docWrite("IntwebCfg.xml");
}
//读取 XML 文档
bool RwXml::docRead(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    if(!doc.setContent(&file)){
        file.close();
        return false;
    }
    file.close();
    return true;
}
//读取 zone.XML 文档
bool RwXml::zonedocRead(QString zonefile)
{
    Qzlblist.clear();
    if(!docRead(zonefile))
        return false;
    QDomElement root = doc.documentElement();  //获取dom树的根标签
    if (root.tagName() != "qzlb")
        {
//            QMessageBox::critical(this, QObject::tr("Error"),QObject::tr("Not a qzlb file"));
            return false;
        }
    //遍历所有群组
    QDomNodeList children = root.childNodes();
    for(int i=0;i<children.count();i++){
        QDomNode node = children.at(i);
        QString order = node.toElement().attribute("order");
        QDomNodeList list = node.childNodes();
        QString qzid = list.at(0).toElement().text();
        QString qzname = list.at(1).toElement().text();
        QString qzprofil = list.at(2).toElement().text();
        QString qzprocl = list.at(3).toElement().text();
        QString qzcreatedate = list.at(4).toElement().text();
        QString qzupdtdate = list.at(5).toElement().text();
        QStringList qzxx;
        qzxx << qzid << qzname << qzprofil << qzprocl << qzcreatedate << qzupdtdate;
        Qzlblist << qzxx;
    }
    return true;
}

//写入 XML 文档
bool RwXml::docWrite(QString zonefile)
{
    QFile file(zonefile);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))  //只读、清空
        return false;
    QTextStream out(&file);
    doc.save(out,4);
//    doc.save()
    file.close();
    return true;
}

//将群组信息保存到 XML 文件中
void RwXml::writeXml(QString zonefile,QStringList qzxx)
{
    if(!docRead(zonefile))
        return;
    //先从文件读取
//    if(zonedocRead(zonefile)){
        QString currentDate = getDateTime(Date);
        QDomElement root = doc.documentElement();
        //根据是否有群组节点进行处理
        if(!root.hasChildNodes()){
            QDomElement qz = doc.createElement(QString("qz"));
            QDomAttr order = doc.createAttribute("order");
            order.setValue("1");
            qz.setAttributeNode(order);
            root.appendChild(qz);
            createNodes(qz,qzxx);
        }else{
            int num = root.childNodes().count();
            QDomElement qz2 = doc.createElement(QString("qz"));
            QDomAttr order = doc.createAttribute("order");
            order.setValue(QString("%1").arg(num+1));
            qz2.setAttributeNode(order);
            root.appendChild(qz2);
            createNodes(qz2,qzxx);
        }
        //写入到文件
        docWrite(zonefile);
//    }
}

//创建群组信息的节点
void RwXml::createNodes(QDomElement &qz,QStringList qzxx)
{
//    QDomElement time = doc.createElement(QString("时间"));
//    QDomAttr curTime = doc.createAttribute("time");
    QDomElement id = doc.createElement(QString("qzid"));
    QDomElement name = doc.createElement(QString("qzname"));
    QDomElement profil = doc.createElement(QString("qzprofil"));
    QDomElement procl = doc.createElement(QString("qzprocl"));
    QDomElement createdate = doc.createElement(QString("createdate"));
    QDomElement updtdate = doc.createElement(QString("createdate"));
    QDomText text;
    text = doc.createTextNode(QString("%1").arg(qzxx.at(0)));
    id.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(qzxx.at(1)));
    name.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(qzxx.at(2)));
    profil.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(qzxx.at(3)));
    procl.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(qzxx.at(4)));
    createdate.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(qzxx.at(5)));
    updtdate.appendChild(text);
    qz.appendChild(id);
    qz.appendChild(name);
    qz.appendChild(profil);
    qz.appendChild(procl);
    qz.appendChild(createdate);
    qz.appendChild(updtdate);
}

void RwXml::deletDomnotlist(QString zonefile,QString qzid)
{
    if(!docRead(zonefile))
        return;
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.childNodes();
    for(int i = 0;i<list.count();i++)
    {
        QDomNodeList node = list.at(i).childNodes();
        QString qzidtemp = node.at(0).toElement().text();
        if(qzidtemp==qzid)
        {
            root.removeChild(list.at(i));
            break;
        }
    }
    docWrite(zonefile);
/*    QFile file(zonefile);
    file.open(QIODevice::ReadOnly); //读
    QDomDocument docu;
    docu.setContent(&file);
    file.close();
    QDomElement root = docu.documentElement();
    QDomNodeList list = root.childNodes();
    for(int i = 0;i<list.count();i++)
    {
        QDomNodeList node = list.at(i).childNodes();
        QString qzidtemp = node.at(0).toElement().text();
        if(qzidtemp==qzid)
        {
            docu.removeChild(list.at(i));
            break;
        }
    }
    file.open(QIODevice::WriteOnly|QIODevice::Truncate); //只写、清空
    QTextStream out(&file);
    docu.save(out,0);
    file.close();*/
}
void RwXml::creatzonexml(QString zonefile)
{
    QFile file(zonefile);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate); //只写、清空
    QDomDocument docum;
    QDomProcessingInstruction instruction;
    instruction = docum.createProcessingInstruction("xml", "version = \'1.0\' encoding=\'UTF-8\'");
    docum.appendChild(instruction);
    QDomElement root = docum.createElement(QString("qzlb"));
    docum.appendChild(root);
    QTextStream out(&file);
    docum.save(out,0);
    file.close();
}
QString RwXml::getDateTime(DateTimeType type)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString date = datetime.toString("yyyy-MM-dd");
    QString time = datetime.toString("hh:mm");
    QString dateAndTime = datetime.toString("yyyy-MM-dd dddd hh:mm");
    if(type == Date)return date;
    else if(type == Time)return time;
    else return dateAndTime;
}
void RwXml::initcnfg(QString ip)
{
    confg.clear();
    QString dirpath = QDir::currentPath();
    confg << "V1.0.1" << "Administrator" << ip << "-1" << "" << "2" << "2" << "2" << "2" << dirpath << "2" << "2" << "2" << dirpath+"/AutoRecv Files" << "2" << dirpath+"/AutoRecv Files";
    afreshConfg(confg);
    QFile file("IntwebCfg.xml");
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    QTextStream out(&file);
    out << "<?xml version='1.0' encoding='UTF-8'?> \n"
        << "<IntwebConfig> \n"
        << " <Version>V1.0.1</Version> \n"
        <<"  <Myname>Administrator</Myname> \n"
        << "  <Myip>"+ip+"</Myip> \n"
        << "  <headimage>-1</headimage> \n"
        << "  <Myword> </Myword> \n"
        << "  <IsAutoRun>2</IsAutoRun> \n"
        << "  <IsAutoRunMin>2</IsAutoRunMin> \n"
        << "  <IsAutoCheckUpdate>2</IsAutoCheckUpdate> \n"
        << "  <IsAutoKeepNode>2</IsAutoKeepNode> \n"
        << "  <AutoKeepNodePath>"+dirpath+"</AutoKeepNodePath> \n"
        << "  <IsTellothersWhenrun>2</IsTellothersWhenrun> \n"
        << "  <IsTellothersWhenleft>2</IsTellothersWhenleft> \n"
        << "  <IsNoticeWhenotherleft>2</IsNoticeWhenotherleft> \n"
        << "  <CustomRecvPath>"+dirpath+"/AutoRecv Files</CustomRecvPath> \n"
        << "  <IsAutoRecvfile>0</IsAutoRecvfile> \n"
        << "  <AutoRecvFilePath>"+dirpath+"/AutoRecv Files</AutoRecvFilePath> \n"
        << "</IntwebConfig> \n";
    file.close();
}
