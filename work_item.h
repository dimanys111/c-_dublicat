#ifndef WORKITEM_H
#define WORKITEM_H

#include <QObject>
#include <QThread>
#include <QListWidgetItem>
#include <QMutex>

class Work_Item : public QObject
{
    Q_OBJECT
public:
    explicit Work_Item();
    QThread* Mythread;
    bool stop;
    int getCol();
    void addCol();
signals:
    void MyEmit1();
    void obrItem_emit(QStringList fileNamess);
public slots:
    void obrItem(QStringList fileNamess);
private:
    QMutex m;
    static QMutex m_stat;
    int col;
};

#endif // WORKITEM_H
