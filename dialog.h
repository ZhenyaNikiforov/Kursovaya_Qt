#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;

private slots:
    void on_showYear_clicked();
    void on_cleanYear_clicked();

signals:
    void startShow(QChartView* chartView, QString code);
    void startClean(QChartView* chartView);
};

#endif // DIALOG_H
