#include "helper.h"
#include "ui_helper.h"
#include "cube.h"
#include "ui_cube.h"
#include<QPainter>
#include<QTimer>
#include<QKeyEvent>
helper::helper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::helper)
{
    ui->setupUi(this);
}

helper::~helper()
{
    delete ui;
}

void helper::on_pushButton_clicked()
{
    this->close();
}
