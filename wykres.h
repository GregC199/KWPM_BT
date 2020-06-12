#ifndef WYKRES_H
#define WYKRES_H

#include <QWidget>
#include <QChartView>
#include <QtCharts>

class Wykres : public QWidget
{
    Q_OBJECT
public:
    explicit Wykres(QWidget *parent = nullptr);

signals:

public slots:

private:
    QChartView* zmiana_rozmiaru;
};

#endif // WYKRES_H
