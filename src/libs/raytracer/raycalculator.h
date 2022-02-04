#ifndef RAYCALCULATOR_H
#define RAYCALCULATOR_H

#include <QStack>
#include <QVector>
#include <vector>
#include "spline.h"
#include <QLineF>
#include <QPointF>
#include <QRectF>
#include <QMap>
#include <complex>

class SoundSpeedProfile
{
public:
    SoundSpeedProfile();
    void init(double step, std::vector<double>);

    double getSpeed(double z);
    double getSpeed(int i){
        return m_ssp[i];
    }
    double getStep(){
        return m_step;
    }
    double getLast(){
        return m_last;
    }
    double getDerivative(double z);
    void calcAll(double z);
    double slownessR;
    double slownessZ;
    double speed;
    double derivativeZFirst;
    double sigmaI;


private:
    std::vector<double> m_ssp;
    double m_last;
    double m_step;
};

class Rkf45Calculator
{
public:


};

class Ray
{
public:
    QVector<double> phiV;
    std::complex<double> phiR;
    int numberOfReflections = 0;
    QVector<double> ic;
    QVector<double> r;
    QVector<double> z;
    QVector<std::complex<double>> amp;
    QVector<double> amp_tmp;
    QVector<std::complex<double>> decay;
    bool bottomReflection = false;
    double old_Y, old_X;
    bool isForward = true;
    double ds;
    QVector<QLineF> falling;
    QVector<QPointF> intersection;
    double travelledDistance;
    int currZone = 0;
    QVector<double> thetaRefl;
    QVector<std::complex<double>> reflCoeff;
    QVector<double> p;
    QVector<double> q;
    QVector<double> caustc;
    QVector <double> gradC = {0,0};
    QVector <double> nGradC = {0,0};
    QVector <double> dGradC = {0,0};
    QVector <double> es = {0,0};
    QVector <double> sigma = {0,0};
    QVector <double> phase;
    QVector <bool> isRefl;
    QVector <double> tangentX;
    QVector <double> tangentY;
    std::complex<double> ap_aq;
    double theta;
    int rayBoundaryType = 0; // 0 - no boundary, -1 - abve surface, 1 - below bottom
    struct ZoneInfo{
        QVector<double> r;
        QVector<double> z;
        QVector<double> pressure;
        QVector<double> phase;
        QVector<double> distance;
    }zn;

    QMap<int, QMap<int, ZoneInfo>> zones;
    void clearZone();
};

class Munc
{
public:
    double c1 = 1500;
    double z1 = 100;
    //double b = 1300;
    double b = 500;
    double epsilon = 0.00737;
     // depth
    void calc(QVector<double> depth);
    void calc(int n, double step);

    QVector<double> depth;
    QVector<double> c;
};

class Bathymetry{
public:
    Bathymetry();
    void init(std::vector<double>, std::vector<double>);
    double getSrfcPoint(double r);
    void calcIntersect();
    spline spln;
    double rho, ap, as, cp, cs; // for rigid surface type
private:
    int numberOfCoords;
    double m_step;
    double m_last;
};

class Source{
public:
    double theta0;  // width of the beam
    double thteaN;  // quantity of rays
    double depthOfTheSource;
    double frequency;
};

class RayCalculator
{
public:
    RayCalculator();
    double sourceFrequency;
    SoundSpeedProfile ssp;
    Bathymetry bat;
    double sourceZ;
    double lastR;
    double bottom;
    Ray* calcRay(double theta);
    void update(double * yOld,double * yNew, double * fOld, double * fNew);
    bool calcReflection(double * yOld,double * yNew, double * fOld, double * fNew);
    struct Zone{
        int id;
        QRectF boundRect;
    };
    QMap<int, Zone> m_zones;
    void addPressureCalcZone(double x, double y, double width, double height);
    void calcRayAmplitude();
    Source src;
private:
    void rkf45(double dsi, double* yOld, double* fOld, double* yNew, double* fNew, double* ds4, double* ds5);
    void rkf45WithPrecission(double totalErr, int iterCnt, double dsi, double* yOld, double* fOld, double* yNew, double* fNew);
    Ray* m_ray;
    void calcReflCoeff(double * yOld);
};



#endif // RAYCALCULATOR_H
