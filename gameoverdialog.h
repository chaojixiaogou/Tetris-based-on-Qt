#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QWidget>

namespace Ui {
class gameoverdialog;
}

class gameoverdialog : public QWidget
{
    Q_OBJECT

public:
    explicit gameoverdialog(QWidget *parent = 0);
    ~gameoverdialog();

private:
    Ui::gameoverdialog *ui;
};

#endif // GAMEOVERDIALOG_H
