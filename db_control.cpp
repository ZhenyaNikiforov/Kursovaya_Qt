#include "db_control.h"

DB_Control::DB_Control(QObject *parent): QObject(parent) {
    this->aviaFlights = QSqlDatabase::addDatabase("QPSQL");
    this->aviaFlights.setHostName("981757-ca08998.tmweb.ru");
    this->aviaFlights.setDatabaseName("demo");
    this->aviaFlights.setUserName("netology_usr_cpp");
    this->aviaFlights.setPassword("CppNeto3");
    this->aviaFlights.setPort(5432);

    this->model = new QSqlQueryModel(this);
}

DB_Control::~DB_Control(){
    this->aviaFlights.close();
}

void DB_Control::connect(){
    if(this->aviaFlights.isOpen()){
        this->aviaFlights.close();
        emit this->disConn();
    }else{
        this->aviaFlights.open();
        emit this->conn();
    }
}

void DB_Control::showAll(QTableView* table){

    QSqlQuery query(this->aviaFlights);

    query.prepare(R"(
        SELECT airport_name->>'ru' as airportName, airport_code
        FROM bookings.airports_data
    )");

    query.exec();

    this->model->setQuery(std::move(query));

    this->model->setHeaderData(0, Qt::Horizontal, "Название");
    this->model->setHeaderData(1, Qt::Horizontal, "Код");

    table->setModel(this->model);
}

void DB_Control::clean(QTableView* table){
    table->setModel(nullptr);
}

void DB_Control::inAirport(QTableView* table, QString code)
{
    QSqlQuery query(this->aviaFlights);

    query.prepare(R"(
        SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as Name from bookings.flights f
        JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport
        where f.arrival_airport  = :code
    )");

    query.bindValue(":code", code);

    query.exec();
    this->model->setQuery(std::move(query));

    this->model->setHeaderData(0, Qt::Horizontal, "Номер");
    this->model->setHeaderData(1, Qt::Horizontal, "Время прилёта");
    this->model->setHeaderData(2, Qt::Horizontal, "Название");

    table->setModel(this->model);
}

void DB_Control::outAirport(QTableView* table, QString code)
{
    QSqlQuery query(this->aviaFlights);

    query.prepare(R"(
        SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as Name                                              from bookings.flights f
        JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport
        WHERE f.departure_airport  = :code
    )");

    query.bindValue(":code", code);

    query.exec();
    this->model->setQuery(std::move(query));

    this->model->setHeaderData(0, Qt::Horizontal, "Номер");
    this->model->setHeaderData(1, Qt::Horizontal, "Время вылета");
    this->model->setHeaderData(2, Qt::Horizontal, "Название");

    table->setModel(this->model);
}