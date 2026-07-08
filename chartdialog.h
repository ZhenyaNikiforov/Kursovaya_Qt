#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include <QTableView>

namespace Ui {
class ChartDialog;
}

class ChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartDialog(QWidget *parent = nullptr);
    ~ChartDialog();
    void startValue();

private:
    Ui::ChartDialog *ui;

private slots:
    void on_selectMonth_currentTextChanged(const QString &text);

signals:
    void searchMonthStat(QString code, QString start, QString end, QTableView *table);
};

#endif // CHARTDIALOG_H
