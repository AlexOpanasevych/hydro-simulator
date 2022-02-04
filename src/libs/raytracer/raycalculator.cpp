#include "raycalculator.h"

#include <tuple>
#include <math.h>
#include <QDebug>
#include <complex>

SoundSpeedProfile::SoundSpeedProfile()
{
}

void SoundSpeedProfile::init(double step, std::vector<double> ssp)
{
    m_ssp = ssp;
    m_step = step;
    m_last = step*(ssp.size()-1);
}

double SoundSpeedProfile::getSpeed(double z)
{
    if (z < 0) z = 0;
    if (z > m_last) z = m_last;
    auto i = static_cast<unsigned int>(z / m_step);
    double zi = i * m_step;
    double k = (m_ssp[i+1] - m_ssp[i]) / m_step;
    auto res = k*(z - zi) + m_ssp[i];
    return res;
}

double SoundSpeedProfile::getDerivative(double z)
{
    auto i = static_cast<unsigned int>(z / m_step);
    double k = (m_ssp[i+1] - m_ssp[i]) / m_step;
    return k;
}

void SoundSpeedProfile::calcAll(double z)
{
    if (z < 0) z = 0;
    if (z > m_last) z = m_last;
    auto i = static_cast<unsigned int>(z / m_step);
    double zi = i * m_step;
    double k = (m_ssp[i+1] - m_ssp[i]) / m_step;

    speed = k*(z - zi) + m_ssp[i];
    derivativeZFirst = k;
    slownessR = 0;
    slownessZ = -(derivativeZFirst) / (speed * speed);
    sigmaI = 1/speed;
}

RayCalculator::RayCalculator()
{
    m_ray = new Ray();
    Munc munc;

    sourceZ = 100;
    lastR = 30000;
    bottom = 300;
    munc.calc(int(bottom+1), 1);
    int step = 100;
    QVector<double> bottomVecY;
    QVector<double> bottomVecX;
    for (int i(0); i<lastR; i+=step){
        bottomVecY.append(bottom-10 + 10*sin(i*M_PI/180));
        bottomVecX.append(i);
    }
    bat.init(bottomVecX.toStdVector(), bottomVecY.toStdVector());
    ssp.init(1, munc.c.toStdVector());
}

Ray *RayCalculator::calcRay(double theta)
{
    theta = M_PI * theta / 180;
    double yOld[4], fOld[4], yNew[4], fNew[4];
    m_ray->theta = theta;
    ssp.calcAll(sourceZ);
    yOld[0] = 0;
    yOld[1] = sourceZ;
    yOld[2] = (1/ssp.speed) * cos(theta);
    yOld[3] = (1/ssp.speed) * sin(theta);

    fOld[0] = cos(theta);
    fOld[1] = sin(theta);
    fOld[2] = ssp.slownessR;
    fOld[3] = ssp.slownessZ;
    m_ray->r.clear();
    m_ray->z.clear();
    m_ray->numberOfReflections = 0;
    m_ray->travelledDistance = 0;
    m_ray->zones.clear();
    bool state = true;
    m_ray->currZone = 0;
    m_ray->clearZone();
    m_ray->decay.clear();
    m_ray->decay.push_back(std::complex<double>(1,0));
    m_ray->p.clear();
    m_ray->q.clear();
    m_ray->caustc.clear();
    m_ray->p.push_back(1);
    m_ray->q.push_back(0);
    m_ray->caustc.push_back(0);
    m_ray->amp.clear();
    m_ray->amp.push_back(double(NAN));
    m_ray->isRefl.clear();
    m_ray->isRefl.push_back(false);
    m_ray->phase.clear();
    m_ray->phase.push_back(0.0);
    m_ray->ic.clear();
    m_ray->ic.push_back(0);
    m_ray->r.push_back(0);
    m_ray->z.push_back(src.depthOfTheSource);
    m_ray->thetaRefl.clear();
    m_ray->thetaRefl.push_back(0);
    m_ray->phiV.clear();
    m_ray->phiV.append(1.0); // starting value for volumetric loss
    m_ray->phiR = std::complex<double>(1.0,0.0);// starting value for boundary reflection loss
    m_ray->amp_tmp.clear();

    int cnt = 0;
    while ((yOld[0] >= 0) && (yOld[0] <= lastR)) {
        //calc ray one steprkf45WithPrecission(totalErr, iterCnt, dsi, yOld, fOld, yNew, fNew)
        rkf45WithPrecission(0.1, 100, 1, yOld, fOld, yNew, fNew);
        m_ray->bottomReflection = false;
        state = calcReflection(yOld, yNew, fOld, fNew);
        if (!state)
            break;
        if (cnt++ > 30000) {
            break;
        }

        if (!m_zones.isEmpty()){
            if (m_ray->currZone != 0){
                if(m_zones[m_ray->currZone].boundRect.contains(yNew[0],yNew[1])){
                    m_ray->zn.r.push_back(yNew[0]);
                    m_ray->zn.z.push_back(yNew[1]);
                    m_ray->zn.distance.push_back(m_ray->travelledDistance);
                } else {
                    if(m_ray->zones.contains(m_ray->currZone)){
                        m_ray->zones[m_ray->currZone].insert(m_ray->zones[m_ray->currZone].lastKey()+1,m_ray->zn);
                    } else{
                        m_ray->zones.insert(m_ray->currZone, QMap<int, Ray::ZoneInfo>());
                        m_ray->zones[m_ray->currZone].insert(1,m_ray->zn);
                    }
                    m_ray->currZone = 0;
                    m_ray->clearZone();
                }
            } else {
                for (auto it = m_zones.begin(), end = m_zones.end();it != end; it++) {
                    if(it.value().boundRect.contains(yNew[0],yNew[1])){
                        m_ray->currZone = it.key();
                        m_ray->zn.r.push_back(yNew[0]);
                        m_ray->zn.z.push_back(yNew[1]);
                        m_ray->zn.distance.push_back(m_ray->travelledDistance);
                        break;
                    }
                }
            }
        }
        update(yOld, yNew,fOld,fNew);
        m_ray->ds = sqrt(pow((m_ray->r.last() - m_ray->r[m_ray->r.size()-2]),2)
                + pow((m_ray->z.last() - m_ray->z[m_ray->z.size()-2]),2));
//        qDebug()<< m_ray->r.last() << " " << m_ray->r[m_ray->r.size()-2];
//        qDebug()<< m_ray->z.last() << " " << m_ray->z[m_ray->z.size()-2];
        m_ray->travelledDistance+=m_ray->ds;
        m_ray->ic.push_back(m_ray->ic.last() + m_ray->ds * ssp.getSpeed(m_ray->z.last()));
//        qDebug()<< m_ray->travelledDistance << " " << m_ray->ds << " " << m_ray->ic.last();
        calcRayAmplitude();

    }
    return m_ray;
}

void RayCalculator::rkf45(double dsi, double *yOld, double *fOld, double *yNew, double *fNew, double *ds4, double *ds5)
{
    static double      k1[4],k2[4],k3[4],k4[4],k5[4],k6[4];
    static double      yk[4],yrk4[4],yrk5[4];
    double      ri;
    double      zi;

    auto prepare = [&](double *yk_in, double *ki) {
        ri     = yk_in[0];
        zi     = yk_in[1];
        auto sigmaR = yk_in[2];
        auto sigmaZ = yk_in[3];
        auto sigmaI = sqrt( pow(sigmaR,2) + pow(sigmaZ,2));
        auto es_r = (sigmaR)/(sigmaI);
        auto es_z = (sigmaZ)/(sigmaI);
        ssp.calcAll(zi);
        ki[0] = es_r;
        ki[1] = es_z;
        ki[2] = ssp.slownessR;
        ki[3] = ssp.slownessZ;
    };

    ri = yOld[0];
    zi = yOld[1];
    /* determine k1:                                      */
    //ssp.calcAll(zi);
    //csValues(settings, ri, zi, &ci, &cc, &sigmaI, &cri, &czi, &slowness, &crri, &czzi, &crzi);
    for(auto j=0; j<4; j++){
        k1[j] = fOld[j];
        yk[j] = yOld[j] + 0.25 * dsi * k1[j];
    }

    /* determine k2:                                            */
    prepare(yk, k2);
    for(auto j=0; j<4; j++){
        yk[j] = yOld[j] + dsi *(3.0/32.0 * k1[j] + 9.0/32.0 * k2[j]);
    }

    /* determine k3:                                            */
    prepare(yk, k3);
    for(auto j=0; j<4; j++){
        yk[j] = yOld[j] + dsi * ( 1932.0/2197.0*k1[j] -7200.0/2197.0*k2[j] + 7296.0/2197.0*k3[j]);
    }

    /* determine k4:                                            */
    prepare(yk, k4);
    for(auto j=0; j<4; j++){
        yk[j] = yOld[j] + dsi * (439.0/216.0*k1[j] - 8.0*k2[j] + 3680.0/513.0*k3[j] - 845.0/4104*k4[j]);
    }

    /* determine k5:         (last RK4 step)                */
    prepare(yk, k5);
    for(auto j=0; j<4; j++){
        yk[j] = yOld[j] + dsi * (2.0*k2[j] - 8.0/27.0*k1[j] + 3544.0/2565.0*k3[j] + 1859.0/4104*k4[j] - 11.0/40.0*k5[j]);
    }

    /* determine k6:        (last RK5 step)                 */
    //define coeficients required for Runge-Kutta-Fehlberg method:
    //RK4:
const double A1 =   (25.0/216.0);
const double A3 = (1408.0/2565.0);
const double A4 = (2197.0/4101.0);
const double A5 =   (-1.0/5.0);
    //RK5:
const double B1 =    (16.0/135.0);
const double B3 =  (6656.0/12825.0);
const double B4 = (28561.0/56430.0);
const double B5 =    (-9.0/50.0);
const double B6 =     (2.0/55.0);
    prepare(yk, k6);
    for(auto j=0; j<4; j++){
        yrk4[j] = yOld[j] + dsi * ( A1 * k1[j] + A3 * k3[j] + A4 * k4[j] + A5 * k5[j]);
        yrk5[j] = yOld[j] + dsi * ( B1 * k1[j] + B3 * k3[j] + B4 * k4[j] + B5 * k5[j] + B6 * k6[j]);
        yNew[j] = yrk5[j];
    }

    /* Calculate the actual output value:       */
    prepare(yNew, fNew);


    /* Determine ds4 and ds5:       */
    auto dr = yrk4[0] - yOld[0];
    auto dz = yrk4[1] - yOld[1];
    *ds4 = sqrt(dr*dr + dz*dz);

    dr = yrk5[0] - yOld[0];
    dz = yrk5[1] - yOld[1];
    *ds5 = sqrt(dr*dr + dz*dz);
    m_ray->thetaRefl.push_back(atan((yNew[1]-yOld[1])/(yNew[0]-yOld[0])));
}

void RayCalculator::rkf45WithPrecission(double totalErr, int iterCnt, double dsi, double *yOld, double *fOld, double *yNew, double *fNew)
{
    int i = 0;
    double stepErr = 1;
    double ds4, ds5;
    while (stepErr > totalErr) {
        rkf45(dsi, yOld, fOld, yNew, fNew, &ds4, &ds5);
        stepErr = fabs( ds4 - ds5) / (0.5 * (ds4 + ds5));
        dsi *= 0.5;
        i++;
        if (i >= iterCnt) qFatal("Iteration more than max cnt");
    }
}

void Munc::calc(QVector<double> depth)
{
    this->depth = depth;
    this->c.resize(depth.size());
    for (int i = 0, l = depth.size(); i < l; ++i) {
        auto eta = 2.0 * (depth[i] - z1)/b;
        this->c[i] = c1 * (1.0 + epsilon * (eta + exp(-eta)-1.0));
    }
}

void Munc::calc(int n, double step)
{
    QVector<double> z(n);
    for (int  i=0;i < n; ++i) z[i] = step*i;
    calc(z);
}

Bathymetry::Bathymetry(){
    cp = 1550.0;
    cs = 0.0;
    rho = 2.0;
    ap = 0.0;
    as = 0.0;
}

void Bathymetry::init(std::vector<double>distance, std::vector<double> surface){
    spln.set_points(distance,surface);
}

double Bathymetry::getSrfcPoint(double r){
    return spln(r);
}

void RayCalculator::update(double * yOld,double * yNew, double * fOld, double * fNew){
    for(int j=0; j<4; j++){
        yOld[j] = yNew[j];
        fOld[j] = fNew[j];
    }
    m_ray->r.append(yNew[0]);
    m_ray->z.append(yNew[1]);
    m_ray->old_Y = -1;
    m_ray->old_X = -1;
}

bool RayCalculator::calcReflection(double *yOld, double *yNew, double *fOld, double *fNew){
    double ri,zi;
    double normalr, normalz;
    bool calc = false;
    if (yNew[1] < 0) {
        calc = true;
        zi = 0;         // pointisectZ
        ri = (yNew[0]-yOld[0])/(yNew[1] - yOld[1]) * (zi - yOld[1]) + yOld[0]; // pointisectR
        double taubr = cos(atan(0));
        double taubz = sin(atan(0));
        if ( abs(taubr) == 1.0 ){
            taubz = 0;
        }else if ( abs(taubz) == 1.0 ){
            taubr = 0;
        }
        normalr = - taubz;
        normalz = taubr;
        m_ray->reflCoeff.push_back(std::complex<double>(-1,0));
        m_ray->bottomReflection = false;
    } else if (yNew[1] > bat.spln(yNew[0])) {
        calc = true;
        QVector<double> result(2);
        m_ray->bottomReflection = true;
        bat.spln.calcIntersectWithLine(yOld[0],yNew[0],yOld[1],yNew[1], &result);
        if (result[0] > 0){
            ri = result[0];
            zi = result[1];
            double splineTangent = atan(bat.spln.deriv(1,ri));
            normalr = - sin(splineTangent);
            normalz = cos (splineTangent);

        } else {
            return false;
        }
    }
    if (calc){
        double c = normalr*fOld[0] + normalz * fOld[1]; // dot product normal and esr
        double tauRr = fNew[0] - 2 * c * normalr;
        double tauRz = fNew[1] - 2 * c * normalz;
        yNew[0] = ri;
        yNew[1] = zi;
        fNew[0] = tauRr;
        fNew[1] = tauRz;
        ssp.calcAll(zi);
        yNew[2] = ssp.sigmaI*tauRr;
        yNew[3] = ssp.sigmaI*tauRz;
        fNew[2] = ssp.slownessR;
        fNew[3] = ssp.slownessZ;
        m_ray->thetaRefl.push_back(acos(c));
        m_ray->isRefl.push_back(true);
        if (m_ray->bottomReflection)
            calcReflCoeff(yOld);
        m_ray->phase.push_back(m_ray->phase.last()-atan2(m_ray->reflCoeff.last().imag(), m_ray->reflCoeff.last().real()));
    } else {
        m_ray->isRefl.push_back(false);
        m_ray->phase.push_back(m_ray->phase.last());
    }
    return true;
}

void RayCalculator::addPressureCalcZone(double x, double y, double width, double height){
    Zone zn;
    zn.boundRect.setRect(x,y,width,height);
    if(m_zones.isEmpty()){
        zn.id = 1;
    } else {
        zn.id = m_zones.lastKey() + 1;
    }
    m_zones.insert(zn.id,zn);
}

void RayCalculator::calcRayAmplitude(){
    /*
     * The formula for calculating ray amplitude is taken from cTraceo manual p.10-14
     * it consists from Amplitude coefficient A multiplied by boundary loss coeff,
     * in programm called phiR and multiplied by volumetric loss coeff, called phiV
     */
     /* Calculating Thorpe's attenuation coeff, it's used in calculating phiV,
      * the formula is taken from cTraceo code, not from the cTraceo user manual,
      * because they are bit different. the aim of calculating thorpe's coeff is
      * obtaining the apha coeff
    */

    //Calc phiV
    double fxf = pow((src.frequency/1000),2);
    double alpha = 0.0033 + 0.11 * fxf/(1+fxf) + 44 * fxf/(4100 + fxf) + 0.0003 * fxf;
    alpha = alpha/8685.8896;
    /*
     * Formula for obtaining the phiV is exp(-alpha * S), where S is the full distance
     * travelled by the ray
     */
    m_ray->phiV.append(exp(-alpha * m_ray->travelledDistance));

    // Calc phiR
    /*
     * This type of loss is only calculated at the boundary reflections
     */
    if (m_ray->bottomReflection){
        double tilap = bat.ap /(40.0 * M_PI * M_LOG10E); // stands for tilda ap, p.14 of the manual
        double tilas = bat.as /(40.0 * M_PI * M_LOG10E); // stands for tilda as, p.14 of the manual
        // stands for tilda cp, eq 2.22 p.13 of the manual
        std::complex<double> tilcpBot = bat.cp * (1 - 1i * tilap)/(1 + pow(tilap,2));
        // stands for tilda cs, eq 2.22 p.13 of the manual
        std::complex<double> tilcsBot = bat.cs * (1 - 1i * tilas)/(1 + pow(tilas,2));
        // Calculating coefficients A1..A7 from the equation 2.21, p.13
        double a1 = bat.rho/1.0;
        double tmpDepth = m_ray->z[m_ray->z.size()-2];
        double tmpSpeed = ssp.getSpeed(tmpDepth);
        std::complex<double> a2 = tilcpBot/tmpSpeed;
        std::complex<double> a3 = tilcsBot/tmpSpeed;
        std::complex<double> a4 = a3 * sin(m_ray->thetaRefl.last());
        std::complex<double> a5 = 2.0 * pow (a4,2);
        std::complex<double> a6 = a2 * sin(m_ray->thetaRefl.last());
        std::complex<double> a7 = 2.0 * a5 - pow(a5,2);
        // Calculating reflection coefficients using equation 2.20, p.13
        std::complex<double> d = a1 * (a2 * (1.0 - a7)/sqrt(1.0-pow(a6,2)) + a3 * a7 / sqrt(1.0-a5/2.0));
        std::complex<double> r = (d * cos(m_ray->thetaRefl.last()))/
                (d * cos(m_ray->thetaRefl.last())+1.0);
        //Updating the phiR using new coeff
        m_ray->phiR *= r;
    }



    double dr,dz,cnn,cnj,csj,rm,rn;
    m_ray->nGradC[0] = 0;
    m_ray->nGradC[1] = ssp.getDerivative(m_ray->z[m_ray->z.size()-1]);
    m_ray->dGradC[0] = 0;
    m_ray->dGradC[1] = m_ray->nGradC[1] - m_ray->gradC[1];
    int i = m_ray->r.size()-1;
    dr = m_ray->r[i] - m_ray->r[i-1];
    dz = m_ray->z[i] - m_ray->z[i-1];
    m_ray->es[0] = dr/m_ray->ds;
    m_ray->es[1] = dz/m_ray->ds;
    m_ray->sigma[0] = (1/ssp.getSpeed(m_ray->z[m_ray->z.size()-2])) * m_ray->es[0];
    m_ray->sigma[1] = (1/ssp.getSpeed(m_ray->z[m_ray->z.size()-2])) * m_ray->es[1];
    cnn = 0;
    if (!m_ray->isRefl.last()){
        m_ray->p.push_back( m_ray->p.last() - m_ray->q.last()
                            * (cnn/pow(ssp.getSpeed(m_ray->z[m_ray->z.size()-2]),2)) * m_ray->ds);
        m_ray->q.push_back(m_ray->q.last() - m_ray->p[m_ray->p.size()-2]
                * ssp.getSpeed(m_ray->z[m_ray->z.size()-2])*m_ray->ds);
        cnj = m_ray->dGradC[0] * (-m_ray->sigma[1]) + m_ray->dGradC[1] * m_ray->sigma[0];
        csj = m_ray->dGradC[0] * m_ray->sigma[0] + m_ray->dGradC[1] * m_ray->sigma[1];

        if (m_ray->sigma[1] != 0.0){
            rm = m_ray->sigma[0]/m_ray->sigma[1];
            rn = - (rm * (2 * cnj - rm * csj)/ssp.getSpeed(m_ray->z[m_ray->z.size()-2]));
            m_ray->p[m_ray->p.size()-1] = m_ray->p[m_ray->p.size()-2] + m_ray->q[m_ray->q.size()-2] * rn;
        }
    } else {
        double tauBr, tauBz;
        if(!m_ray->bottomReflection){
            tauBr = cos(atan(0));
            tauBz = sin(atan(0));
        } else {
            double tangent = atan(bat.spln.deriv(1,m_ray->z.last()));
            tauBr = cos(tangent);
            tauBz = sin(tangent);
        }
        double nBdyR = -tauBz;
        double nBdyZ = tauBr;
        if (m_ray->bottomReflection){
            nBdyR = - nBdyR;
            nBdyZ = - nBdyZ;
        }
        double sigmaNr = -m_ray->sigma[1];
        double sigmaNz = m_ray->sigma[0];
        double tg = m_ray->sigma[0] * tauBr + m_ray->sigma[1] * tauBz;
        double th = m_ray->sigma[0] * nBdyR + m_ray->sigma[1] * nBdyZ;
        double cn = m_ray->gradC[0] * sigmaNr + m_ray->gradC[1] * sigmaNz;
        double cs = m_ray->gradC[0] * m_ray->sigma[0] + m_ray->gradC[1] * m_ray->sigma[1];

        rm = tg/th;
        if (m_ray->bottomReflection)
            cn = -cn;
        rn = rm * (4 * cn - 2 * rm * cs)/ssp.getSpeed(m_ray->z[m_ray->z.size()-2]);
        double tmpP = m_ray->p[m_ray->p.size()-1] + m_ray->q[m_ray->q.size()-1] * rn;
        double tmpQ = m_ray->q[m_ray->q.size()-1];
        m_ray->p.push_back(tmpP);
        m_ray->q.push_back(tmpQ);
    }
    m_ray->ap_aq = std::complex<double>(ssp.getSpeed(src.depthOfTheSource)* cos(m_ray->theta*M_PI/180)*
                                        (ssp.getSpeed(m_ray->z[m_ray->z.size()-2])
                                        /(m_ray->ic[m_ray->ic.size()-2]
                                        * m_ray->q[m_ray->q.size()-2])));
    m_ray->amp.push_back(sqrt(m_ray->ap_aq) * m_ray->decay.last() * m_ray->phiV.last());
    m_ray->gradC = m_ray->nGradC;
    m_ray->amp_tmp.push_back(abs(m_ray->amp.last()));
}

void Ray::clearZone(){
    zn.r.clear();
    zn.z.clear();
    zn.pressure.clear();
    zn.phase.clear();
    zn.distance.clear();
}

void RayCalculator::calcReflCoeff(double * yOld){
    double tilap, tilas;
    double a1;
    std::complex<double> a2, a3, a4, a5, a6, a7, d;
    std::complex<double> tilcp2, tilcs2;

    tilap = bat.ap/(40.0 * M_PI * M_LOG10E);
    tilas = bat.as/(40.0 * M_PI * M_LOG10E);

    tilcp2 = bat.cp * std::complex<double>(1.0, - tilap)/ (1.0 + tilap * tilap);
    tilcs2 = bat.cs * std::complex<double>(1.0, - tilas)/ (1.0 + tilas * tilas);

    a1 = bat.rho/1.0;
    a2 = tilcp2/ssp.getSpeed(yOld[1]);
    a3 = tilcs2/ssp.getSpeed(yOld[1]);
    a4 = a3 * sin(m_ray->thetaRefl.last());
    a5 = 2.0 * a4 * a4;
    a6 = a2 * sin(m_ray->thetaRefl.last());
    a7 = 2.0 * a5 - a5 * a5;

    d = a1 * ( a2 * (1.0 - a7 ) / sqrt( 1.0 - a6 * a6 ) + a3 * a7 / sqrt(1.0 - 0.5*a5));
    m_ray->reflCoeff.push_back((d * cos(m_ray->thetaRefl.last())-1.0)/(d * cos(m_ray->thetaRefl[m_ray->thetaRefl.size()-2])+1.0));
    m_ray->decay.push_back(m_ray->decay.last() * m_ray->reflCoeff.last());
}
