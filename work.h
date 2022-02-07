#ifndef WORK_H
#define WORK_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QListWidgetItem>
#include "QDebug"
#include "work_item.h"
#include "element.h"

class Work : public QObject
{
    Q_OBJECT
public:
    QStringList Dir_Tek;
    //Work_Form* wf;
    Work();
    static bool stop;
    void saveMap();
    static void insertMap(QString fileNames, QByteArray &s);
    static void insertMap_prov(QString fileNames);
    static void removeMap(QString fileNames);
    static bool contMap(QString item);
    static QString D;
    void stopF();
signals:
    void emitPut(QString s);
    void emitPB(int i,int i1);
    void form_d(Element e);
public slots:
    void nach_getDirFiles(QStringList sl);
private slots:
    void prov_kol();
    void timout();
    void timout1();
private:
    static QMap<QString,QByteArray> map;
    static QMap<QString,QByteArray> map_prov;
    QThread* Mythread;
    int maxOch;
    int minOch;
    QList<int> listNomUd;
    QList<QStringList> listEmit;
    QTimer t,t1;
    int kol_sch;
    QList<Work_Item*> wi_list;
    static QMutex m;
    void getDirFiles(QString s);
    void loadMap();
    void poisk_dubl();
};

#endif // WORK_H
