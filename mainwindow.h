#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QPushButton>
#include <work.h>
#include "form_ddd.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void setPB(int ii,int jj);
    void setCHTO(QString s);
private:
    Form_DDD FDDD;
    Work* w;
    Ui::MainWindow *ui;
    QMap<QPushButton*,QString> map;

signals:
    void emitMy(QStringList sl);
    void emitProv();
protected:

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
