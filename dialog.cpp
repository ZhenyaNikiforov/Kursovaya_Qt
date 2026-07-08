#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_showYear_clicked(){
    emit startShow(ui->chartView, ui->insertCode->text());
}

void Dialog::on_cleanYear_clicked(){
    emit startClean(ui->chartView);
}