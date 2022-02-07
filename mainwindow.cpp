#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QLabel"
#include "QDesktopServices"
#include "QUrl"
#include "QFileDialog"
#include "QTextCodec"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    w=new Work();
    qRegisterMetaType<Element>("Element");

    connect(this,&MainWindow::emitMy,w,&Work::nach_getDirFiles);
    connect(w,&Work::emitPB,this,&MainWindow::setPB);
    connect(w,&Work::emitPut,this,&MainWindow::setCHTO);
    connect(w,&Work::form_d,&FDDD,&Form_DDD::addDubl);
    connect(&FDDD,&Form_DDD::emit_closeALL,this,&MainWindow::close);
    //connect(this,&MainWindow::emitProv,w->wf,&Work_Form::timout);

    QPushButton *le=new QPushButton();
    connect(le, &QPushButton::clicked, [=]() {
        QString str = QFileDialog::getExistingDirectory(0, "Directory Dialog", "");
        map.insert(le,str);
        le->setText(str);
    } );
    QString str="/media/dima/ARHIV/ФОТО И ВИДЕО";
    map.insert(le,str);
    le->setText(str);
    ui->widget->setLayout(new QVBoxLayout());
    ui->widget->layout()->addWidget(le);
}

MainWindow::~MainWindow()
{
    w->stopF();
    delete ui;
}

void MainWindow::setPB(int ii, int jj)
{
    ui->progressBar->setMaximum(ii);
    ui->progressBar->setValue(jj);
}

void MainWindow::setCHTO(QString s)
{
    ui->label->setText(s);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    w->stopF();
}

void MainWindow::on_pushButton_2_clicked()
{
    QPushButton *le=new QPushButton();
    connect(le, &QPushButton::clicked, [=]() {
        QString str = QFileDialog::getExistingDirectory(0, "Directory Dialog", "");
        map.insert(le,str);
        le->setText(str);
    } );
    ui->widget->setLayout(new QVBoxLayout());
    ui->widget->layout()->addWidget(le);
}

void MainWindow::on_pushButton_clicked()
{
    emit emitMy(map.values());
}
