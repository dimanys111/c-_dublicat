#include "form_ddd.h"
#include "ui_form_ddd.h"

Form_DDD::Form_DDD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_DDD)
{
    ui->setupUi(this);
    col_dubl=0;
}

Form_DDD::~Form_DDD()
{
    delete ui;
}

void Form_DDD::addDubl(Element e)
{
    if(col_dubl==0)
    {
        this->showMaximized();
        ui->pushButton->setEnabled(true);
        Form_Dubl* fd=new Form_Dubl(e,this);
        this->layout()->addWidget(fd);
        fd_Tek=fd;
        col_dubl++;
    }
    else
    {
        if (col_dubl<6)
        {
            Form_Dubl* fd=new Form_Dubl(e,this);
            list_FormDubl<<fd;
            col_dubl++;
        }
        else
            list_elem.append(e);
    }
    ui->pushButton->setText(">>>  "+QString::number(list_FormDubl.size()+list_elem.size()));
}

void Form_DDD::on_pushButton_clicked()
{
    col_dubl--;
    fd_Tek->deleteLater();
    if(!list_FormDubl.isEmpty())
    {
        Form_Dubl* fd=list_FormDubl.takeFirst();
        this->layout()->addWidget(fd);
        fd_Tek=fd;

        if(!list_elem.isEmpty())
        {
            Element e=list_elem.takeFirst();
            Form_Dubl* fd=new Form_Dubl(e,this);
            list_FormDubl<<fd;
            col_dubl++;
        }
        ui->pushButton->setText(">>>  "+QString::number(list_FormDubl.size()+list_elem.size()));
    }
    else
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton->setText("Ждите");
    }
}

void Form_DDD::closeEvent(QCloseEvent *event)
{
    emit emit_closeALL();
}
