#ifndef HELPER_H
#define HELPER_H

#include <QWidget>
#include "cube.h"

namespace Ui {
class helper;
}

class helper : public QWidget
{
    Q_OBJECT

public:
    explicit helper(QWidget *parent = 0);
    ~helper();



private slots:
    void on_pushButton_clicked();

private:
    Ui::helper *ui;
};

#endif // HELPER_H
