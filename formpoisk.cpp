#include "formpoisk.h"
#include "ui_formpoisk.h"

FormPoisk::FormPoisk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPoisk)
{
    ui->setupUi(this);
}

FormPoisk::~FormPoisk()
{
    delete ui;
}

void FormPoisk::on_pushButton_clicked()
{
    QHBoxLayout*hbl=new QHBoxLayout();
    le_list<<new QLineEdit();
    hbl->addWidget(le_list.last());
    pb_list<<new QPushButton("Выбрать");
    connect(pb_list.last(),&QPushButton::clicked,this,&FormPoisk::on_pushButton_clicked);
    hbl->addWidget(pb_list.last());
    ui->verticalLayout->addLayout(hbl);
}
