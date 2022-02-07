#include "form_elem.h"
#include "ui_form_elem.h"
#include <QDebug>
#include "QResizeEvent"

Form_Elem::Form_Elem(QString s_, int n, int cm, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Elem)
{
    s=s_;
    ui->setupUi(this);
    ui->lineEdit->setText(s);
    ui->lineEdit_2->setText("n="+QString().number(n)+"; cm="+QString().number(cm));
    ui->label->setPixmap(s);
}

Form_Elem::~Form_Elem()
{
    delete ui;
}

void Form_Elem::on_pushButton_clicked()
{
    QFile::remove(s);
    emit emitMy(this);
}
