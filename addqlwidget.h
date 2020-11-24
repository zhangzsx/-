#ifndef ADDQLWIDGET_H
#define ADDQLWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QList>
namespace Ui {
class AddqlWidget;
}

class AddqlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddqlWidget(QWidget *parent,QStringList usrnamelist,QStringList usriplist);
    ~AddqlWidget();
signals:
    void addqz(QStringList,QStringList);
private:
    Ui::AddqlWidget *ui;
    QStringList namelist;
    QStringList iplist;
    QList<QCheckBox *> checklist;

protected:
    QString createrand();
    void initqzmembset(QStringList,QStringList);
private slots:
    void on_qxBtn_clicked();
    void on_qbxBtn_clicked();
    void on_okButton_clicked();
    void on_quitButton_clicked();

};

#endif // ADDQLWIDGET_H
