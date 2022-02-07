#include <form_dubl.h>
#include <ui_form_dubl.h>
#include <QLayout>

Form_Dubl::Form_Dubl(Element e, Form_DDD* ddd) :
    ui(new Ui::Form_Dubl)
{
    ui->setupUi(this);
    connect(this,&Form_Dubl::emit_del,ddd,&Form_DDD::on_pushButton_clicked);
    for(int i=0;i<e.sl.size();i++)
    {
        Form_Elem* fe=new Form_Elem(e.sl.at(i),e.sn.at(i),e.scm.at(i),this);
        connect(fe,&Form_Elem::emitMy,this,&Form_Dubl::clos_pr);
        ui->horizontalLayout->addWidget(fe);
    }
    colElem=e.sl.size();
}

Form_Dubl::~Form_Dubl()
{
    delete ui;
}

void Form_Dubl::clos_pr(Form_Elem *e)
{
    layout()->removeWidget(e);
    e->deleteLater();
    colElem--;
    if(colElem==1)
    {
        emit emit_del();
    }
}
