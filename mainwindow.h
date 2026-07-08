#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "db_control.h"
#include "dialog.h"
#include "chartdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    DB_Control dbControl;
    Dialog modalWindow;
    ChartDialog chartWindow;

private slots:
    void on_connectButton_clicked();//- подключение/отключение
    void on_showAllAirports_clicked();//- показывает все аэропорты
    void on_cleanDisplay_clicked();//-очищает дисплей
    void on_showOneAirport_clicked();//- показывает по выбранному аэропорту

    void on_StatYear_clicked();//- открывает статистику за год
    void on_StatDay_clicked();//- открывает статистику за месяц

    void yes_connect();//- разблокировка кнопок
    void no_connect();//- блокировка кнопок
};

#endif // MAINWINDOW_H
