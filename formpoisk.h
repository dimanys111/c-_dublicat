#ifndef FORMPOISK_H
#define FORMPOISK_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>

namespace Ui {
class FormPoisk;
}

class FormPoisk : public QWidget
{
    Q_OBJECT

public:
    QList<QLineEdit*> le_list;
    explicit FormPoisk(QStringList *sl, QWidget *parent = 0);
    ~FormPoisk();

public slots:
    void add(QString s);
private slots:
    void ol_clicked();
    void ol_clicked_c();
private:
    QStringList *SL;
    QList<QHBoxLayout*>hbl_list;
    QList<QPushButton*> pb_list;
    QList<QPushButton*> pb_cl_list;
    Ui::FormPoisk *ui;
    void zag(QString s);
};

#endif // FORMPOISK_H
