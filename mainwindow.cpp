#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //-Установим начальное состояние кнопок

    ui->StatDay->setEnabled(false);
    ui->StatYear->setEnabled(false);
    ui->cleanDisplay->setEnabled(false);
    ui->showOneAirport->setEnabled(false);
    ui->showAllAirports->setEnabled(false);

    //- законнектим сигналы соединения с базой данных

    connect(&(this->dbControl), &DB_Control::conn, this, &MainWindow::yes_connect);
    connect(&(this->dbControl), &DB_Control::disConn, this, &MainWindow::no_connect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::yes_connect(){
    ui->StatDay->setEnabled(true);
    ui->StatYear->setEnabled(true);
    ui->cleanDisplay->setEnabled(true);
    ui->showOneAirport->setEnabled(true);
    ui->showAllAirports->setEnabled(true);

    ui->connectButton->setText("Отключиться");
    ui->connectStatus->setText("Подключено");
}
void MainWindow::no_connect(){
    ui->StatDay->setEnabled(false);
    ui->StatYear->setEnabled(false);
    ui->cleanDisplay->setEnabled(false);
    ui->showOneAirport->setEnabled(false);
    ui->showAllAirports->setEnabled(false);

    ui->connectButton->setText("Подключиться");
    ui->connectStatus->setText("Отключено");
}

void MainWindow::on_connectButton_clicked(){
    this->dbControl.connect();
}

void MainWindow::on_showAllAirports_clicked()
{
    this->dbControl.showAll(ui->infoOutput);
}

void MainWindow::on_cleanDisplay_clicked()
{
    this->dbControl.clean(ui->infoOutput);
}

void MainWindow::on_showOneAirport_clicked()
{
    QString codeAirport = ui->insertAirportName->text();

    if(ui->searchVarible->currentText() == "Прилёт")
    {
        /*qDebug()<<"Prilet!";
        qDebug()<<codeAirport;*/
        this->dbControl.inAirport(ui->infoOutput, codeAirport);
    }
    else
    {
        /*qDebug()<<"Otlet!";
        qDebug()<<codeAirport;*/
        this->dbControl.outAirport(ui->infoOutput, codeAirport);
    }
}