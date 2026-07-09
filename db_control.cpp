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
        if(!this->aviaFlights.open()){
            QString errorText = this->aviaFlights.lastError().text();
            qDebug() << "Критическая ошибка подключения к БД:" << errorText;
            QMessageBox::critical(nullptr,
            QString::fromUtf8("Ошибка подключения"),
            QString::fromUtf8("Не удалось соединиться с базой данных.\n") + errorText);
            return;
        }
        emit this->conn();
    }
}

void DB_Control::showAll(QTableView* table){

    QSqlQuery query(this->aviaFlights);

    query.prepare(R"(
        SELECT airport_name->>'ru' as airportName, airport_code
        FROM bookings.airports_data
    )");

    if(!query.exec()){
        QString errorMessage = query.lastError().text();
        qDebug()<<"Ошибка выполнения запроса: "<<errorMessage;
        return;
    }

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
        SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as "Name" from bookings.flights f
        JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport
        where f.arrival_airport  = :code
    )");

    query.bindValue(":code", code);

    if(!query.exec()){
        QString errorMessage = query.lastError().text();
        qDebug()<<"Ошибка выполнения запроса: "<<errorMessage;
        return;
    }

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
        SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as "Name" from bookings.flights f
        JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport
        WHERE f.departure_airport  = :code
    )");

    query.bindValue(":code", code);

    if(!query.exec()){
        QString errorMessage = query.lastError().text();
        qDebug()<<"Ошибка выполнения запроса: "<<errorMessage;
        return;
    }

    this->model->setQuery(std::move(query));

    this->model->setHeaderData(0, Qt::Horizontal, "Номер");
    this->model->setHeaderData(1, Qt::Horizontal, "Время вылета");
    this->model->setHeaderData(2, Qt::Horizontal, "Название");

    table->setModel(this->model);
}

void DB_Control::searchYearStat(QChartView *chartView, QString code)
{
    QSqlQuery query(this->aviaFlights);

    query.prepare(R"(
        SELECT count(flight_no), date_trunc('month', scheduled_departure) as "Month"
        FROM bookings.flights f
        WHERE (scheduled_departure::date > date('2016-08-31')
        AND scheduled_departure::date <= date('2017-08-31'))
        AND ( departure_airport = :code or arrival_airport = :code)
        group by "Month"
    )");

    query.bindValue(":code", code);

    if(!query.exec()){
        QString errorMessage = query.lastError().text();
        qDebug()<<"Ошибка выполнения запроса: "<<errorMessage;
        return;
    }

    QBarSet *barSet = new QBarSet("Количество рейсов");
    QStringList categories;
    int maxFlights = 0;

    while (query.next()) {
        int count = query.value(0).toInt();
        QDateTime monthDate = query.value(1).toDateTime();

        QString monthStr = monthDate.toString("yyyy-MM");

        *barSet << count;
        categories << monthStr;

        if (count > maxFlights) {
            maxFlights = count;
        }
    }

    QBarSeries *series = new QBarSeries();
    series->append(barSet);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Статистика перелетов за год для порта: " + code);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxFlights * 1.1);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    if (chartView->chart()) {
        delete chartView->chart();
    }

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

void DB_Control::cleanYearStat(QChartView* chartView){

    QChart *oldChart = chartView->chart();
    chartView->setChart(new QChart());

    if (oldChart) {
        oldChart->deleteLater();
    }
}

void DB_Control::searchMonthStat(QString code, QString start, QString end, QTableView *table)
{
    QSqlQuery query(this->aviaFlights);
    query.prepare(R"(
        SELECT count(flight_no), date_trunc('day', scheduled_departure) as "Day"
        FROM bookings.flights f
        WHERE (scheduled_departure::date > date(:start)
        AND scheduled_departure::date <= date(:end))
        AND (departure_airport = :code OR arrival_airport = :code)
        GROUP BY "Day"
    )");

    query.bindValue(":code", code);
    query.bindValue(":start", start);
    query.bindValue(":end", end);

    if(!query.exec()){
        QString errorMessage = query.lastError().text();
        qDebug()<<"Ошибка выполнения запроса: "<<errorMessage;
        return;
    }

    this->model->setQuery(std::move(query));
    table->setModel(this->model);
}