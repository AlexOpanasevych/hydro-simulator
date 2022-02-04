#include "kmath.h"
#include <QVector>
#include <QVector2D>
#include <math.h>

#define NOISE_LEVEL 0.2
#define GAIN1_MAX 0.8

KMath::KMath(QObject *parent) : QObject (parent) {}

float KMath::constrainAngle(float value) {
    const float max = static_cast<float>(M_PI * 2);
    value = fmod(value, max);
    if (value < 0)
        value += max;
    return value;
}

float KMath::constrainDegrees(float value) {
    const float max = 360.0;
    value = fmod(value, max);
    if (value < 0)
        value += max;
    return value;
}

double KMath::constrainDegrees(double value)  {
    const double max = 360.0;
    value = fmod(value, max);
    if (value < 0)
        value += max;
    return value;
}

float KMath::determinant(QVector2D vector1, QVector2D vector2)  {
    return vector1.x() * vector2.y() - vector1.y() * vector2.x();
}

float KMath::getAngle(QVector2D vector1, QVector2D vector2)  {
    const float dot = QVector2D::dotProduct(vector1, vector2);
    const float det = determinant(vector1, vector2);
    return -std::atan2(det, dot);
}

float KMath::getDegrees(QVector2D vector1, QVector2D vector2)  {
    const float dot = QVector2D::dotProduct(vector1, vector2);
    const float det = determinant(vector1, vector2);
    return -std::atan2(det, dot) * static_cast<float>(180 / M_PI);
}

bool KMath::direction(float dstAngle, float angle) {
    if((dstAngle - angle) < 0) {
        if(std::abs(dstAngle - angle) < 180) {
            return 1; // //
        } else {
            return 0; //right//
        }
    } else {
        if(std::abs(dstAngle - angle) < 180) {
            return 0; //right//
        } else {
            return 1; //left//
        }
    }
}

bool KMath::direction(double dstAngle, double angle) {
    if((dstAngle - angle) < 0) {
        if(std::abs(dstAngle - angle) < 180) {
            return 1; // //
        } else {
            return 0; //right//
        }
    } else {
        if(std::abs(dstAngle - angle) < 180) {
            return 0; //right//
        } else {
            return 1; //left//
        }
    }
}

float KMath::distance(float dstAngle, float angle) {
    if((dstAngle - angle) < 0) {
        if(std::abs(dstAngle - angle) < 180) {
            return std::abs(dstAngle - angle);
        } else {
            return std::abs(dstAngle - angle);
        }
    } else {
        if(std::abs(dstAngle - angle) < 180) {
            return std::abs(dstAngle - angle);
        } else {
            return std::abs(dstAngle - angle);
        }
    }
}

double KMath::degreesToRadians(double degrees)
{
    return degrees * M_PI / 180.;
}

double KMath::radiansToDegrees(double radians)
{
    return radians * 180. / M_PI;
}

float KMath::degreesDistance(float angle1, float angle2) {
    float max = 360;
    float abs = qAbs(angle1 - angle2);
    return abs > (max * 0.5f) ? (max - abs) : abs;
}

float KMath::degreesDifference(float angle1, float angle2) {
    float max = 360;
    float diff = angle1 - angle2;
    return qAbs(diff) > (max * 0.5f)
            ? ((diff >= 0)
               ? (diff - max)
               : (diff + max)
                 )
            : diff;}

double KMath::degreesDistance(double angle1, double angle2) {
    double max = 360;
    double abs = qAbs(angle1 - angle2);
    return abs > (max * 0.5) ? (max - abs) : abs;
}

double KMath::degreesDifference(double angle1, double angle2) {
    double max = 360;
    double diff = angle1 - angle2;
    return qAbs(diff) > (max * 0.5)
            ? ((diff >= 0)
               ? (diff - max)
               : (diff + max)
                 )
            : diff;
}


float KMath::map(float value, float inMin, float inMax, float outMin, float outMax) {
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

double KMath::dmap(double value, double inMin, double inMax, double outMin, double outMax) {
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

double KMath::limitValue(double value, double min, double max) {
    return std::max(std::min(value, max), min);
}

double KMath::limitedMap(double value, double inMin, double inMax, double outMin, double outMax) {
    return dmap(limitValue(value, inMin, inMax), inMin, inMax, outMin, outMax);
}

double KMath::sigm(double value) {
    return 1 / (1 + std::exp(-value));
}

double KMath::gaus(double value) {
    return exp(-value * value);
}

QVector<double> KMath::bubbleSort(const QVector<double> &array) {
    QVector<double> result = array;
    bubbleSort(&result);
    return result;
}
QVector<quint64> KMath::vectorIndices(const QVector<double> &vector) {
    return customContainerIndices(vector);
}


double KMath::random() {
    return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

double KMath::randomStep() {
    static int i = std::rand() % 32;
    static double noise = random();

    if(i-- == 0) {
        i = std::rand() % 32;
        noise = random();
    }
    return noise;
}

double KMath::toKnots(double value) {
    return value * 1.9438444924574;
}

double KMath::fromKnots(double value) {
    return value * 0.5144444444444;
}

int KMath::toTop(double value) {
    int i = static_cast<int>(value);
    if(!KCompareEngine<double>::compare(static_cast<double>(i), value)) {
        i++;
    }
    return i;
}

QVector<double> KMath::resampling(const QVector<double> &input, int sizeMultiplier) {
    if(sizeMultiplier > 1) {
        QVector<double> result;
        for(int i = 0; i < input.size() - 1; i++) {
            result.push_back(input[i]);
            for(int j = 1; j < sizeMultiplier; j++) {
                result.push_back((input[i + 1] - input[i]) / sizeMultiplier * j + input[i]);
            }
        }

        for(int j = 1; j < sizeMultiplier; j++) {
            result.push_back((input[0] - input[input.size() - 1]) / sizeMultiplier * j + input[input.size() - 1]);
        }
        return result;
    }
    return input;
}

QVector<double> KMath::integrate(QVector<double> input, double alpha) {
    for(int i = 1; i < input.size(); ++i) {
        input[i] = alpha * input[i - 1] + (1 - alpha) * input[i];
    }
    input[0] = alpha * input[input.size() - 1] + (1 - alpha) * input[0];
    return input;
}

QVector<double> KMath::smooth(const QVector<double> &input, double alpha, int sizeMultiplier) {
    return integrate(resampling(input, sizeMultiplier), alpha);
}

double KMath::metersDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool KMath::isInBounds(double min, double value, double max)
{
    return value >= min && value <= max;
}

//bool KMath::isInBoundsPeriod(double min, double value, double max)
//{
//    if(min )
//}

double KMath::getAngle(double x, double y)
{
    if(x > 0 && y > 0) return M_PI_2 - atan(y / x);
    else if (x == 0 && y > 0) return 0;
    else if (x < 0 && y > 0) return -(M_PI_2 + atan(y / x));
    else if (x < 0 && y == 0) return -M_PI_2;
    else if (x < 0 && y < 0) return -(M_PI_2 + atan(y / x));
    else if (x == 0 && y < 0) return -M_PI;
    else if (x > 0 && y < 0) return M_PI - atan(y / x);
    else if (x > 0 && y == 0) return M_PI_2;
    else return 0;
}

double KMath::signPiPeriod(double angle)
{
    angle = fmod(angle, 2* M_PI);
    if(angle > M_PI) return angle - 2 * M_PI;
    else if(angle < -M_PI) return angle + 2*M_PI;
    return angle;

}



