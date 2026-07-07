#ifndef DB_CONTROL_H
#define DB_CONTROL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QObject>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlQuery>

class DB_Control: public QObject
{
    Q_OBJECT
public:
    explicit DB_Control(QObject *parent = nullptr);
    ~DB_Control();

    void connect();
    void showAll(QTableView* table);
    void clean(QTableView* table);

    void inAirport(QTableView* table, QString code);
    void outAirport(QTableView* table, QString code);

private:
    QSqlDatabase aviaFlights;
    QSqlQueryModel *model;

signals:
    void conn();
    void disConn();
};

#endif // DB_CONTROL_H
