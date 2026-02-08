#include "gameoverdialog.h"
#include "ui_gameoverdialog.h"

gameoverdialog::gameoverdialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameoverdialog)
{
    ui->setupUi(this);
}

gameoverdialog::~gameoverdialog()
{
    delete ui;
}
