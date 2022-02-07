#include "work.h"
#include "QDir"
#include "QApplication"
#include "QStringList"
#include "mainwindow.h"
#include "QMessageBox"

QMutex Work::m;
QMap<QString,QByteArray> Work::map;
QMap<QString,QByteArray> Work::map_prov;
QString Work::D;
bool Work::stop;

Work::Work()
{
    QDir path;
    path.mkpath(qApp->applicationDirPath()+"/cache");
    D=qApp->applicationDirPath()+"/cache/";

    for(int i=0;i<6;i++)
    {
        Work_Item* wi=new Work_Item();
        wi_list<<wi;
        connect(wi,&Work_Item::MyEmit1,this,&Work::prov_kol);
    }
    connect(&t,&QTimer::timeout,this,&Work::timout);
    connect(&t1,&QTimer::timeout,this,&Work::timout1);
    loadMap();
    Mythread=new QThread();
    moveToThread(Mythread);
    t.moveToThread(Mythread);
    t1.moveToThread(Mythread);
    Mythread->start();
}

void Work::prov_kol()
{
    minOch++;
    emit emitPB(maxOch+listEmit.size(),minOch);
    kol_sch--;
    if (kol_sch==0 && listEmit.isEmpty())
    {
        saveMap();
        emit emitPut("Поиск дубликатов...");
        emit emitPB(1,1);
        qDebug() << map.size();
        t.stop();
        t1.stop();
        poisk_dubl();
    }
}

void Work::poisk_dubl()
{
    listNomUd.clear();
    QList<QString> sss = map_prov.keys();
    QList<QByteArray> ppp = map_prov.values();
    emit emitPB(1,0);

    for(int i=0;i<ppp.size();i++)
    {
        if(stop)
            return;
//        if(!listNomUd.contains(i))
        {
            QStringList sl;
            QList<int> sn;
            QList<int> scm;
            QByteArray img1=ppp.at(i);
            for(int j=i+1;j<ppp.size();j++)
            {
                if(stop)
                    return;
                QByteArray img2=ppp.at(j);
                int n=0;
                char cold=0;
                char cmax=0;
                bool b=true;
                for(int ii=0;ii<img1.size();ii++)
                {
                    char c=img1.at(ii)-img2.at(ii);
                    char ac=abs(c);
                    if(cmax<=ac)
                        cmax=ac;
                    if(b)
                    {
                        if(ii>0)
                        {
                            if(c!=cold)
                            {
                                b=false;
                            }
                        }
                    }
                    n=n+ac;
                    cold=c;
                }

                if(b)
                {
                    sn.append(n);
                    scm.append(cmax);
                    sl.append(sss.at(j));
                    listNomUd.append(j);
                }
                else
                {
                    if(n>-1 && n<=2000 && cmax<100)
                    {
                        sn.append(n);
                        scm.append(cmax);
                        sl.append(sss.at(j));
                        listNomUd.append(j);
                    }
                }
            }
            if(sl.size())
            {
                sl.append(sss.at(i));
                sn.append(0);
                scm.append(0);
                Element e(sl,sn,scm);
                emit form_d(e);
            }
        }
        emit emitPB(ppp.size(),i+1);
    }
    emit emitPut("Поиск дубликатов Завершон");
    qDebug() << "Усё2";
}

void Work::timout()
{
    saveMap();
}

void Work::timout1()
{
    if(stop)
    {
        listEmit.clear();
        return;
    }
    if(!listEmit.isEmpty())
    {
        for(int i=0;i<wi_list.size();i++)
        {
            if(!listEmit.isEmpty())
            {
                Work_Item* wi=wi_list.at(i);
                while(wi->getCol()<5 && !listEmit.isEmpty())
                {
                    kol_sch++;
                    QStringList fileNames=listEmit.takeFirst();
                    maxOch++;
                    wi->addCol();
                    emit wi->obrItem_emit(fileNames);
                }
            }
        }
    }
}

void Work::stopF()
{
    stop=true;
    saveMap();
    for(int i=0;i<wi_list.size();i++)
    {
        Work_Item* wi=wi_list.at(i);
        wi->stop=true;
    }
    for(int i=0;i<wi_list.size();i++)
    {
        Work_Item* wi=wi_list.at(i);
        wi->Mythread->exit();
        wi->Mythread->wait();
    }
    Mythread->exit();
    Mythread->wait();
}

bool Work::contMap(QString item)
{
    m.lock();
    bool s=map.contains(item);
    m.unlock();
    return s;
}

void Work::insertMap(QString fileNames,QByteArray& s)
{
    m.lock();
    map.insert(fileNames,s);
    map_prov.insert(fileNames,s);
    m.unlock();
}

void Work::insertMap_prov(QString fileNames)
{
    m.lock();
    map_prov.insert(fileNames,map.value(fileNames));
    m.unlock();
}

void Work::removeMap(QString fileNames)
{
    m.lock();
    map.remove(fileNames);
    m.unlock();
}

void Work::loadMap()
{
    QFile file(D+"cache.cache");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_6);
    m.lock();
    in >> map;
    m.unlock();
    in >> Dir_Tek;
    file.close();
}

void Work::saveMap()
{
    QFile file(D+"cache.cache");
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_6);
    m.lock();
    out << map;
    m.unlock();
    out << Dir_Tek;
    file.close();
}

void Work::nach_getDirFiles(QStringList sl)
{
    Dir_Tek=sl;
    stop=false;
    kol_sch=0;
    maxOch=0;
    minOch=0;
    listEmit.clear();
    t1.stop();
    t1.start(1000);
    t.stop();
    t.start(10000);
    map_prov.clear();
    for(int i=0;i<wi_list.size();i++)
    {
        Work_Item* wi=wi_list.at(i);
        wi->stop=false;
    }
    for(int i=0;i<Dir_Tek.size();i++)
    {
        getDirFiles(Dir_Tek.at(i));
    }
    emit emitPut("Создание ХЭША");
}

void Work::getDirFiles(QString s)
{
    emit emitPut(s);
    if(stop)
    {
        return;
    }
    QDir dir(s);
    if (!dir.exists())
    {
        return;
    }
    QStringList fileNames;
    QStringList l;
    l<<"*.bmp"<<"*.jpg"<<"*.jpeg"<<"*.png"<<"*.gif"<<"*.jpeg";
    QStringList fileList = dir.entryList(l, QDir::Files);
    QStringList dirList = dir.entryList(QDir::Dirs);
    if(stop)
    {
        return;
    }
    for (int i=0;i<fileList.size();i++)
    {
        QString s1;
        if(s.lastIndexOf("/")==s.size()-1)
            s1=s + fileList.at(i);
        else
            s1=s + "/" + fileList.at(i);
        fileNames<<s1;
    }
    if(!fileNames.isEmpty())
    {
        listEmit<<fileNames;
        timout1();
    }
    if(stop)
    {
        return;
    }
    dirList.removeOne(".");
    dirList.removeOne("..");
    for (int i=0;i<dirList.size();i++) {
        if(!dirList.at(i).contains(".lnk"))
        {
            QString s1;
            if(s.lastIndexOf("/")==s.size()-1)
                s1=s+dirList.at(i);
            else
                s1=s+"/"+dirList.at(i);
            getDirFiles(s1);
            if(stop)
            {
                return;
            }
        }
    }
}
