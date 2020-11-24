#ifndef RWXML_H
#define RWXML_H
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QTreeWidget>

enum DateTimeType{Time,Date,DateTime};
class RwXml
{
public:
    RwXml();
    QList<QStringList> Qzlblist;
    QStringList confg;
    bool cfgdocRead();
    bool docRead(QString);
    bool zonedocRead(QString);
    void writeXml(QString,QStringList);
    void deletDomnotlist(QString,QString);
    void creatzonexml(QString);
    void afreshConfg(QStringList);
    void initcnfg(QString);

private:
    QDomDocument doc;
    QTreeWidget *treeWidget;


protected:
    bool docWrite(QString);
    void createNodes(QDomElement &,QStringList);
    QString getDateTime(DateTimeType type);
};

#endif // RWXML_H
