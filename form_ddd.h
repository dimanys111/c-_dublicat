#ifndef FORM_DDD_H
#define FORM_DDD_H

#include <QWidget>
#include <form_dubl.h>

class Form_Dubl;

namespace Ui {
class Form_DDD;
}

class Form_DDD : public QWidget
{
    Q_OBJECT

public:
    explicit Form_DDD(QWidget *parent = 0);
    ~Form_DDD();

public slots:
    void addDubl(Element e);
    void on_pushButton_clicked();
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void emit_closeALL();
private:
    Form_Dubl* fd_Tek;
    int col_dubl;
    QList<Form_Dubl*> list_FormDubl;
    QList<Element> list_elem;
    Ui::Form_DDD *ui;
};

#endif // FORM_DDD_H
