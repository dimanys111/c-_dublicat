#ifndef ELEMENT_H
#define ELEMENT_H

#include <QStringList>

class Element
{
public:
    Element();
    Element(QStringList sl_,QList<int> sn_,QList<int> scm_);
    QStringList sl;
    QList<int> sn;
    QList<int> scm;
};

#endif // ELEMENT_H
