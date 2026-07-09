#include "chartdialog.h"
#include "ui_chartdialog.h"

ChartDialog::ChartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChartDialog)
{
    ui->setupUi(this);
}

ChartDialog::~ChartDialog()
{
    delete ui;
}

void ChartDialog::on_selectMonth_currentTextChanged(const QString &text)
{
    QString start = "";
    QString end = "";

    if(text == "Сентябрь"){
        start = "2016-08-31";
        end = "2016-09-30";
    }else if(text == "Октябрь"){
        start = "2016-09-30";
        end = "2016-10-31";
    }else if(text == "Ноябрь"){
        start = "2016-10-31";
        end = "2016-11-30";
    }else if(text == "Декабрь"){
        start = "2016-11-30";
        end = "2016-12-31";
    }else if(text == "Январь"){
        start = "2016-12-31";
        end = "2017-01-30";
    }else if(text == "Февраль"){
        start = "2017-01-30";
        end = "2017-02-28";
    }else if(text == "Март"){
        start = "2017-02-28";
        end = "2017-03-31";
    }else if(text == "Апрель"){
        start = "2017-03-31";
        end = "2017-04-30";
    }else if(text == "Май"){
        start = "2017-04-30";
        end = "2017-05-31";
    }else if(text == "Июнь"){
        start = "2017-05-31";
        end = "2017-06-30";
    }else if(text == "Июль"){
        start = "2017-06-30";
        end = "2017-07-31";
    }else{
        start = "2017-07-31";
        end = "2017-08-31";
    }

    emit ChartDialog::searchMonthStat(ui->insertCode->text(), start, end, ui->tableView);
}

void ChartDialog::showEvent(QShowEvent *event){
    QDialog::showEvent(event);
    ui->insertCode->setText("YKS");
    emit ChartDialog::searchMonthStat("YKS", "2016-12-31", "2017-01-30", ui->tableView);
}
