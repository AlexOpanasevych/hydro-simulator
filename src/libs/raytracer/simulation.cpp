#include "simulation.h"
#include <QAudioDeviceInfo>
#include <QQmlApplicationEngine>
#include <qaudiooutput.h>
#include <src/lib/audio/ksoundmix.h>
#include <math.h>
#include <src/audio/afsound.h>
#include <src/audio/sinsound.h>
#include <src/lib/utility/kadditional.h>
#include <src/lib/utility/krandomstep.h>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>

QPair<QVector<double>, QVector<double> > *Simulation::getSoundSpeedCurve()
{
    return &soundSpeedCurve;
}

QPair<QVector<double>, QVector<double> > *Simulation::getRayCurve()
{
    return &rayCurve;
}

QList<QPair<QVector<double>, QVector<double> > > *Simulation::getGraphPointsList()
{
    return &graphPointsList;
}

QList<QPair<QVector<double>, QVector<double> > > *Simulation::getBackGraphPointsList()
{
    return &backGraphPointsList;
}

QPair<QVector<double>, QVector<double> > *Simulation::getBackRayCurve()
{
    return &rayCurve;
}

void Simulation::createBuffer()
{
    this->setSeason(0);
    calculateSpeed();
    int progress = 0;

    QVector<QPair<int, QVector<QPair<QVector<double>, QVector<double>>>>> seasonVect;
    //season one
    for (int i = 0; i < 300; i++)
    {
        this->setDepth(i);
        calculate(true);

        QListIterator<QPair<QVector<double>, QVector<double>>> iter(graphPointsList);
        QVector<QPair<QVector<double>, QVector<double>>> vector;
        while(iter.hasNext())
        {
            QVector<double> x, y;
            QPair<QVector<double>, QVector<double>> tmp = iter.next();
            x = tmp.first;
            y = tmp.second;
            vector.push_back(QPair<QVector<double>,QVector<double>>(x, y));
        }
        seasonVect.push_back(QPair(i,vector));
        progress++;
        qDebug() << "Progress" << KMath::getInstance()->dmap(progress, 0, 600, 0, 100);
    }
    buffer.push_back(seasonVect);

    seasonVect.clear();
    this->setSeason(5);
    calculateSpeed();
    //season six
    for (int i = 0; i < 300; i++)
    {
        this->setDepth(i);
        calculate(true);
        QListIterator<QPair<QVector<double>, QVector<double>>> iter(graphPointsList);
        QVector<QPair<QVector<double>, QVector<double>>> vector;
        while(iter.hasNext())
        {
            QVector<double> x, y;
            QPair<QVector<double>, QVector<double>> tmp = iter.next();
            x = tmp.first;
            y = tmp.second;
            vector.push_back(QPair<QVector<double>,QVector<double>>(x, y));
        }
        seasonVect.push_back(QPair(i,vector));
        progress++;
        qDebug() << "Progress" << KMath::getInstance()->dmap(progress, 0, 600, 0, 100);
    }
    buffer.push_back(seasonVect);
    //transform to variant, that look like:
    //
    // List[i].toMap[i].toList[j].toMap[j].toList[k].toList[m]
    //
    QVariantList result;
    for (int i = 0; i < buffer.size(); i++)
    {
        QVariantList depthList;
        QVariantMap seasonMap;
        auto firstL = buffer[i];
        for (int j = 0; j < firstL.size(); j++)
        {
            auto secondL = firstL[j];
            auto thirdL = secondL.second;
            QVariantList coordList;
            QVariantMap depthMap;
            for (int k = 0; k < thirdL.size(); k++)
            {
                QVariantList tmpList;
                auto fourthL = thirdL[k].second;
                for (int m = 0; m < fourthL.size(); m += MAGIC_COEFFICIENT)
                {
                    tmpList.push_back(fourthL[m]);
                }
                coordList.push_back(tmpList);
            }
            depthMap.insert(QString::number(secondL.first), coordList);
            depthList.insert(j, depthMap);
        }
        seasonMap.insert(QString::number(i), depthList);
        result.insert(i, seasonMap);
    }

    QFile file(bufferPath());
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(QJsonDocument::fromVariant(result).toJson());
        file.close();
    }
    else
    {
        qDebug() << "Error, file doesn't opened";
    }
}

void Simulation::loadFromBuffer()
{
    int progress = 0;
    QFile file(bufferPath());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QVariantList result;
        result = QJsonDocument().fromJson(file.readAll()).toVariant().toList();
        file.close();

        for (int i = 0; i < result.size(); i++)
        {
            QVariantList depthList = result[i].toMap()[QString::number(i)].toList();
            QVariantMap seasonMap;
            QVector<QPair<int, QVector<QPair<QVector<double>, QVector<double>>>>> depthVect;
            for (int j = 0; j < depthList.size(); j++)
            {
                QVariantList coordList = depthList[j].toMap()[QString::number(j)].toList();
                QVector<QPair<QVector<double>, QVector<double>>> coordsVect;
                for (int k = 0; k < coordList.size(); k++)
                {
                    QVariantList tmpList = coordList[k].toList();
                    QVector<double> x, y;
                    for (int m = 0; m < tmpList.size(); m++)
                    {
                        x.push_back(m * MAGIC_COEFFICIENT);
                        y.push_back(tmpList[m].toDouble());
                    }
                    coordsVect.push_back(QPair(x, y));


                }
                depthVect.push_back(QPair(j, coordsVect));
                progress++;
                qDebug() << "Progress buffer load" << KMath::getInstance()->dmap(progress, 0, 600, 0, 100);
            }
            buffer.push_back(depthVect);
        }
    }
    else
    {
        qDebug() << "Error, file doesn't opened";
    }

    qDebug() << "goto";
}

QVector<QPair<QVector<double>, QVector<double>>> Simulation::findCoordsByDepth(int depth)
{
    QVector<QPair<QVector<double>, QVector<double>>> result;
    // 0 -> first hydrology type
    // 1 -> sixth hydrology type
    int seas = season() == 0 ? 0 : 1;
    result = buffer[seas][depth].second;
    return result;
}

QString Simulation::bufferPath() {
    return KAdditional::absolutePath("../resourses/raytracer/hydrology_buffer.json");
}

Simulation::Simulation(QObject *parent) : QObject(parent)
{
    setDepth(0);
    setLinesCount(5);
    setSeason(0);
}

void Simulation::start()
{
    calculateSpeed();
    //createBuffer();
    calculate(false);
    loadFromBuffer();
    calculateWave();

    connect(this, &Simulation::seasonChanged, this, [this](){
        calculateSpeed();
        calculateWave();
        emit repaint();
    });

    //connect(this, &Simulation::submarinePosChanged, this, [this](){
    //    if (isReflected(submarinePos().x(), submarinePos().y())) calculateBackWave();
    //    emit repaint();
    //});
}

void Simulation::calculate(bool calc)
{
    Ray * r = nullptr;
    RayCalculator rc;
    rc.sourceZ = depth();
    rc.lastR = lastR;
    rc.addPressureCalcZone(5000, 50, 10000, 100);
    graphPointsList.clear();

    rc.ssp.init(0.1, soundSpeedCurve.first.toStdVector());

    QVector<double> rectX(2), rectY(2);
    rectX[0] = rc.m_zones[1].boundRect.x();
    rectX[1] = rc.m_zones[1].boundRect.x() + rc.m_zones[1].boundRect.width();
    rectY[0] = rc.m_zones[1].boundRect.y();
    rectY[1] = rc.m_zones[1].boundRect.y() + rc.m_zones[1].boundRect.height();

    // surface of the bottom
    int step = 1000;
    QVector<double> bottomY;
    QVector<double> bottomX;

    for (int i(0); i<rc.lastR; i+=step)
    {
        bottomY.append(300);
        bottomX.append(i);
    }

    rc.bat.init(bottomX.toStdVector(), bottomY.toStdVector());
    QVector<double> newBotX, newBotY;

    for (int i = 0; i<rc.lastR;i++){
        newBotX.append(i);
        newBotY.append(rc.bat.spln(i));
    }

    double theta0 = linesCount();
    int thetaN = 6;
    rc.src.frequency = 100;    // Freaquency of the pulse
    double thetaDelta = qAbs(2 *(theta0 / thetaN));
    QVector<double> times(thetaN);

    if (calc)
    {
        for (int i = 0; i < thetaN ; i++)
        {
            auto th = -theta0 + i * thetaDelta;
            r = rc.calcRay(th); // do ray calculation
            //qDebug() << "Theta: " <<th;
            r->z[0] = r->z[1];
            graphPointsList.append(QPair<QVector<double>, QVector<double>>(r->r, r->z));
        }
    }

    backRayCurve = QPair<QVector<double>, QVector<double>>(newBotX, newBotY);
    rayCurve = QPair<QVector<double>, QVector<double>>(newBotX, newBotY);
}

void Simulation::calculateWave()
{
    auto vect = findCoordsByDepth(depth());
    graphPointsList.clear();
    for (int i = 0; i < vect.size(); i++)
    {
        graphPointsList.push_back(vect[i]);
    }
}

void Simulation::calculateBackWave()
{
    auto vect = findCoordsByDepth(submarinePos().y());
    backGraphPointsList.clear();

    for (int i = 0; i < vect.size(); i++)
    {
        int pos = FindCoordinate(submarinePos().x() / 10, vect[i].first);
        if (pos < vect[i].first.size())
        {
            vect[i].first.remove((int)pos + 1, (int)(vect[i].first.size() - pos - 1));
        }
        if (pos < vect[i].second.size())
        {
            vect[i].second.remove((int)pos + 1, (int)(vect[i].second.size() - pos - 1));
        }
        std::reverse(vect[i].second.begin(), vect[i].second.end());
        backGraphPointsList.push_back(vect[i]);
    }
}

void Simulation::calculateSpeed()
{
    switch (season() + 1)
    {
        case 1:
        {
            //int N = int (rc->bottom * 10);
            int N = 3000;
            double zStep = 0.1;
            QVector<double> z(N), c(N);
            c[0] = 1450.1;
            c[1] = 1502;

            c = constrainVector(1450.1, 1502, N);

            for (int i = 0; i < N; i++)
            {
                z[i] = i * zStep;
            }
            soundSpeedCurve.first = c;
            soundSpeedCurve.second = z;
            //rc->ssp.init(zStep, c.toStdVector());
            break;
        }
        case 2:
        {
            //int N = int (rc->bottom * 10);
            int N = 3000;
            double zStep = 0.1;
            QVector<double> z(N), c(N);
            QVector<double> xData(3), yData(3);
            xData[0] = 1495;
            xData[1] = 1510;
            xData[2] = 1480;

            yData[0] = 0;
            yData[1] = 50;
            yData[2] = 300;

            for (int i = 0; i < N; i++)
            {
                z[i] = i * zStep;
                c[i] = interpolate(yData, xData, z[i]);
            }
            soundSpeedCurve.first = c;
            soundSpeedCurve.second = z;
            //rc->ssp.init(zStep, c.toStdVector());
            break;
        }
        case 3:
        {
            //int N = int (rc->bottom * 10);
            int N = 3000;
            double zStep = 0.1;
            QVector<double> z(N), c(N);
            QVector<double> xData(4), yData(4);
            xData[0] = 1495;
            xData[1] = 1510;
            xData[2] = 1480;
            xData[3] = 1510;

            yData[0] = 0;
            yData[1] = 50;
            yData[2] = 200;
            yData[3] = 300;

            for (int i = 0; i < N; i++)
            {
                z[i] = i * zStep;
                c[i] = interpolate(yData, xData, z[i]);
            }
            //rc->ssp.init(zStep, c.toStdVector());
            soundSpeedCurve.first = c;
            soundSpeedCurve.second = z;
            break;
        }
        case 4:
        {
            //int N = int (rc->bottom * 10);
            int N = 3000;
            double zStep = 0.1;
            QVector<double> z(N), c(N);
            QVector<double> xData(5), yData(5);
            xData[0] = 1510;
            xData[1] = 1520;
            xData[2] = 1495;
            xData[3] = 1505;
            xData[4] = 1480;

            yData[0] = 0;
            yData[1] = 50;
            yData[2] = 155;
            yData[3] = 240;
            yData[4] = 300;

            for (int i = 0; i < N; i++)
            {
                z[i] = i * zStep;
                c[i] = interpolate(yData, xData, z[i]);
            }
            //rc->ssp.init(zStep, c.toStdVector());
            soundSpeedCurve.first = c;
            soundSpeedCurve.second = z;
            break;
        }
        case 5:
        {
            //int N = int (rc->bottom * 10);
            int N = 3000;
            double zStep = 0.1;
            QVector<double> z(N), c(N);
            QVector<double> xData(2), yData(2);
            xData[0] = 1520;
            xData[1] = 1450.1;

            yData[0] = 0;
            yData[1] = 300;

            for (int i = 0; i < N; i++)
            {
                z[i] = i * zStep;
                c[i] = interpolate(yData, xData, z[i]);
            }
            //rc->ssp.init(zStep, c.toStdVector());
            soundSpeedCurve.first = c;
            soundSpeedCurve.second = z;
            break;
        }
        case 6:
        {
            //int N = int (rc->bottom * 10);
            int N = 3000;
            double zStep = 0.1;
            QVector<double> z(N), c(N);
            QVector<double> xData(3), yData(3);
            xData[0] = 1520;
            xData[1] = 1490;
            xData[2] = 1520;

            yData[0] = 0;
            yData[1] = 175;
            yData[2] = 300;

            for (int i = 0; i < N; i++)
            {
                z[i] = i * zStep;
                c[i] = interpolate(yData, xData, z[i]);
            }
            //rc->ssp.init(zStep, c.toStdVector());
            soundSpeedCurve.first = c;
            soundSpeedCurve.second = z;
            break;
        }
        case 7:
        {
            //int N = int (rc->bottom * 10);
            int N = 3000;
            double zStep = 0.1;
            QVector<double> z(N), c(N);
            QVector<double> xData(4), yData(4);
            xData[0] = 1520;
            xData[1] = 1490;
            xData[2] = 1510;
            xData[3] = 1480;

            yData[0] = 0;
            yData[1] = 120;
            yData[2] = 200;
            yData[3] = 300;

            for (int i = 0; i < N; i++)
            {
                z[i] = i * zStep;
                c[i] = interpolate(yData, xData, z[i]);
            }
            //rc->ssp.init(zStep, c.toStdVector());
            soundSpeedCurve.first = c;
            soundSpeedCurve.second = z;
            break;
        }
    }
}

double Simulation::interpolate(QVector<double> xData, QVector<double> yData, double x, bool extrapolate)
{
    int size = xData.size();

    int i = 0;                                                                  // find left end of interval for interpolation
    if ( x >= xData[size - 2] )                                                 // special case: beyond right end
    {
       i = size - 2;
    }
    else
    {
       while ( x > xData[i+1] ) i++;
    }
    double xL = xData[i], yL = yData[i], xR = xData[i+1], yR = yData[i+1];      // points on either side (unless beyond ends)
    if ( !extrapolate )                                                         // if beyond ends of array and not extrapolating
    {
       if ( x < xL ) yR = yL;
       if ( x > xR ) yL = yR;
    }

    double dydx = ( yR - yL ) / ( xR - xL );                                    // gradient

    return yL + dydx * ( x - xL );                                              // linear interpolation
}

QVector<double> Simulation::constrainVector(double start, double end, int size)
{
    QVector<double> result(size);
    double step = abs(start - end) / size;

    if (start < end)
    {
        result[0] = start + step;
        for (int i = 1; i < size; i++)
        {
            result[i] = result[i - 1] + step;
        }
    }
    else
    {
        result[0] = start - step;
        for (int i = 1; i < size; i++)
        {
            result[i] = result[i - 1] - step;
        }
    }

    return result;
}

bool Simulation::isReflected(double distance, double depth)
{
    if (graphPointsList.size() == 0) return false;
    QListIterator<QPair<QVector<double>, QVector<double>>>
            iter(graphPointsList);

    QPair<QVector<double>, QVector<double>> gd1 = iter.next();
    QVector<double> x1v = gd1.first;
    QVector<double> y1v = gd1.second;

    QPair<QVector<double>, QVector<double>> gd2;
    QVector<double> x2v;
    QVector<double> y2v;

    while (iter.hasNext())
    {
        gd2 = iter.next();
        x2v = gd2.first;
        y2v = gd2.second;

        for (int i = 0; i < 3; i++)
        {
            double x1 = FindCoordinate(distance / MAGIC_COEFFICIENT + i, x1v);
            double y1 = (x1 + i < y1v.length()) ? y1v[x1 + i] : -1;

            double x2 = FindCoordinate(distance / MAGIC_COEFFICIENT + i, x2v);
            double y2 = (x2 + i < y2v.length()) ? y2v[x2 + i] : -1;

            if (y1 >= 0 && y2 >= 0)
            {
                if (depth <= y1 && depth >= y2)
                {
                    setReflected(true);
                    return true;
                }
                else if (depth >= y1 && depth <= y2)
                {
                    setReflected(true);
                    return true;
                }
            }
        }

        gd1 = gd2;
        x1v = x2v;
        y1v = y2v;
    }
    setReflected(false);
    return false;
}

bool Simulation::isRecieved(double distance, double depth)
{
    Q_UNUSED(distance)
    if (backGraphPointsList.size() == 0) return false;
    QListIterator<QPair<QVector<double>, QVector<double>>>
            iter(backGraphPointsList);

    QPair<QVector<double>, QVector<double>> gd1 = iter.next();
    QVector<double> x1v = gd1.first;
    QVector<double> y1v = gd1.second;

    QPair<QVector<double>, QVector<double>> gd2;
    QVector<double> x2v;
    QVector<double> y2v;

    while (iter.hasNext())
    {
        gd2 = iter.next();
        x2v = gd2.first;
        y2v = gd2.second;

        for (int i = 0; i < 3; i++)
        {
            double x1 = x1v[i] / MAGIC_COEFFICIENT;
            double y1 = (x1 + i < y1v.length()) ? y1v[x1 + i] : -1;

            double x2 = x2v[i] / MAGIC_COEFFICIENT;
            double y2 = (x2 + i < y2v.length()) ? y2v[x2 + i] : -1;

            if (y1 >= 0 && y2 >= 0)
            {
                if (depth <= y1 && depth >= y2)
                {
                    return true;
                }
                else if (depth >= y1 && depth <= y2)
                {
                    return true;
                }
            }
        }

        gd1 = gd2;
        x1v = x2v;
        y1v = y2v;
    }
    setReflected(false);
    return false;
}

double Simulation::FindCoordinate(double value, QVector<double> vec)
{
    int i = 0;
    if (value < 0) value = 0;
    while (true)
    {
        int p = value + i;
        if (p >= vec.size() - 1) break;
        if (vec[p] > value) break;
        i++;
    }
    return value + i;
}

void Simulation::repaintClicked()
{
    //qDebug() << "repaint";

    emit repaint();
}

QCustomPlot *Simulation::getSoundSpeedPlot() const
{
    return soundSpeedPlot;
}

void Simulation::setSoundSpeedPlot(QCustomPlot *value)
{
    soundSpeedPlot = value;
}

QCustomPlot *Simulation::getRayPlot() const
{
    return rayPlot;
}

void Simulation::setRayPlot(QCustomPlot *value)
{
    rayPlot = value;
}


