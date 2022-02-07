#ifndef FORM_ELEM_H
#define FORM_ELEM_H

#include <QWidget>

namespace Ui {
    class Form_Elem;
}

class Form_Elem : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Elem(QString s_,int n,int cm, QWidget *parent = 0);
    ~Form_Elem();

private slots:
    void on_pushButton_clicked();
signals:
    void emitMy(Form_Elem* e);
private:
    QString s;
    Ui::Form_Elem *ui;
    void resizeImage(QResizeEvent *e);
};

#endif // FORM_ELEM_H
