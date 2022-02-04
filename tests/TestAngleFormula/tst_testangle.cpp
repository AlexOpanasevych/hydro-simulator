#include <QtTest>

// add necessary includes here

class testAngle : public QObject
{
    Q_OBJECT

public:
    testAngle();
    ~testAngle();

private slots:
    void test_case1();

};

testAngle::testAngle()
{

}

testAngle::~testAngle()
{

}

void testAngle::test_case1()
{
    for (int angle = -180; angle <= 180; angle++) {
        double A = angle * M_PI / 180.;
        if(A < 0) {
            A += 2 * M_PI;
        }

        double x = sin(A);
        double y = cos(A);
        double newAngle = 0.0f;


        if(x < y) {
            newAngle = atan(x / y);
        }
        else {
            newAngle = M_PI_2 - atan(y / x);
        }

        if(newAngle < 0) {
            newAngle += 2 * M_PI;
        }

        if(x < 0 && y < 0) {
            newAngle += M_PI;
        }
        if(floor(newAngle) != floor(A)) {
            qDebug() << "angle:" << angle << "x:" << x << "y:" << y << newAngle << A;
        }
    }
}

QTEST_APPLESS_MAIN(testAngle)

#include "tst_testangle.moc"
