#ifndef FORM_DUBL_H
#define FORM_DUBL_H

#include <QWidget>
#include <form_elem.h>
#include "element.h"
#include "form_ddd.h"

class Form_DDD;

namespace Ui {
    class Form_Dubl;
}

class Form_Dubl : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Dubl(Element e, Form_DDD *ddd);
    ~Form_Dubl();

public slots:

private slots:
    void clos_pr(Form_Elem *e);

signals:
    void emit_del();
private:
    int colElem;
    Ui::Form_Dubl *ui;
};

#endif // FORM_DUBL_H
