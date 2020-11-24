#ifndef ADDQLFRIENDWIDGET_H
#define ADDQLFRIENDWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QList>
namespace Ui {
class AddqlfriendWidget;
}

class AddqlfriendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddqlfriendWidget(QWidget *parent ,QStringList usrnamelist,QStringList usriplist,QStringList qzmemberlist);
    ~AddqlfriendWidget();
signals:
    void addfriend(QString,QStringList);
private slots:

    void on_qxBtn_clicked();

    void on_qbxBtn_clicked();

    void on_cancleBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::AddqlfriendWidget *ui;
    void initoldtb(QStringList,QStringList);
    void initnewtb(QStringList,QStringList);
    QList<QCheckBox *> checklist;
    QString qzid;
};

#endif // ADDQLFRIENDWIDGET_H
