#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include "license.h"

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    License *l;
    ~About();

private slots:
    void on_pushButton_clicked();

private:
    Ui::About *ui;
};

#endif // ABOUT_H
