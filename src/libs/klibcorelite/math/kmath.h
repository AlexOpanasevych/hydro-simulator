#ifndef KMATH_H
#define KMATH_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QVector>
#include <QVector2D>

#include <kmacro.h>

#define K_INCLUDE_MATH \
private: \
    const static inline KMath *kmath = KMath::instance();

class KLIBCORELITE_EXPORT KMath : public QObject {
    Q_OBJECT
    K_SINGLETON(KMath)
    K_QML_SINGLETON(KMath)
    KMath(QObject *parent = nullptr);
public:
    template<typename T> static
    void bubbleSort(QVector<T> *array);
    template<typename container_t> static
    QVector<quint64> customContainerIndices(container_t container);

    template<typename container_t> static
    typename container_t::value_type customContainerAverage(container_t container) {
        typedef typename container_t::value_type item_t;
        if(container.size() <= 0)
            return item_t();

        item_t sum = item_t();
        quint64 i = 0;
        for(auto c :container) {
            sum += c;
            i++;
        }
        return sum / i;
    }    
public slots:
    static float constrainAngle(float value) ;
    static float constrainDegrees(float value) ;
    static double constrainDegrees(double value) ;
    static float determinant(QVector2D vector1, QVector2D vector2) ;
    static float getAngle(QVector2D vector1, QVector2D vector2) ;
    static float getDegrees(QVector2D vector1, QVector2D vector2) ;
    static bool direction(float dstAngle, float angle) ;
    static bool direction(double dstAngle, double angle) ;
    static float distance(float dstAngle, float angle);
    static double degreesToRadians(double degrees);
    static double radiansToDegrees(double radians);

    static float degreesDistance(float angle1, float angle2) ;
    static float degreesDifference(float angle1, float angle2) ;
    static double degreesDistance(double angle1, double angle2) ;
    static double degreesDifference(double angle1, double angle2) ;

    static float map(float value, float inMin, float inMax, float outMin, float outMax) ;
    static double dmap(double value, double inMin, double inMax, double outMin, double outMax) ;
    static double limitValue(double value, double min, double max) ;
    static double limitedMap(double value, double inMin, double inMax, double outMin, double outMax) ;
    static double sigm(double value) ;
    static double gaus(double value) ;
    static QVector<double> bubbleSort(const QVector<double> &array) ;
    static QVector<quint64> vectorIndices(const QVector<double> &vector) ;
    static double random() ;
    static double randomStep() ;
    static double toKnots(double value) ;
    static double fromKnots(double value) ;
    static int toTop(double value) ;
    static QVector<double> resampling(const QVector<double> &input, int sizeMultiplier) ;
    static QVector<double> integrate(QVector<double> input, double alpha) ;
    static QVector<double> smooth(const QVector<double> &input, double alpha, int sizeMultiplier) ;

    static double metersDistance(int x1, int y1, int x2, int y2);

    static bool isInBounds(double min, double value, double max);
//    static bool isInBoundsPeriod(double min, double value, double max);

    static double getAngle(double x, double y);

    static double signPiPeriod(double angle);

};

template<typename T>
void KMath::bubbleSort(QVector<T> *array) {
    T temp;
    auto&& size = array->size();
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array->operator[](j) > array->operator[](j + 1)) {
                temp = array->operator[](j);
                array->operator[](j) = array->operator[](j + 1);
                array->operator[](j + 1) = temp;
            }
        }
    }
}


template<typename container_t>
QVector<quint64> KMath::customContainerIndices(container_t container) {
    if(container.size() <= 0)
        return QVector<quint64>();
    int currentIndex = 0;
    auto currentValue = container[currentIndex];
    QVector<quint64> result;

    typedef typename std::remove_reference<decltype (currentValue)>::type item_t;

    result.push_back(currentIndex);
    for (int i = 1; i < container.size(); ++i) {
        if(!KCompareEngine<item_t>::compare(currentValue, container[i])) {
            ++currentIndex;
            currentValue = container[i];
        }
        result.push_back(currentIndex);
    }
    return result;
}


#endif // KMATH_H
