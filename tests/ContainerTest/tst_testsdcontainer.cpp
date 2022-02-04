#include <QtTest>
#include <sampladatacontainer.h>
//#include <sampledata/torpedosampledata.h>
//#include <sampledata/vesselsampledata.h>
//#include "sampledata/torpedocontrollersampledata.h"
#include "logger.h"
// add necessary includes here
class TestSDContainer : public QObject
{
    Q_OBJECT

public:
    TestSDContainer();
    ~TestSDContainer();

private slots:
    void test_case1();

};

TestSDContainer::TestSDContainer()
{

}

TestSDContainer::~TestSDContainer()
{

}

void TestSDContainer::test_case1()
{
//    SamplaDataContainer* con = new SamplaDataContainer;
//    TorpedoSampleData* tor = new TorpedoSampleData;
//    TorpedoControllerSampleData * cont = new TorpedoControllerSampleData;
//    tor->uSecond = 1;
//    tor->index = 0;
//    con->CreateNewRecord(0 , 1 , true);
//    con->pushSample(0 , tor);
//    cont->uSecond = 1;
//    cont->index = 1;
//    con->CreateNewRecord(0 , 5 , true);
//    con->pushSample(0 , cont);
}
QTEST_APPLESS_MAIN(TestSDContainer)
#include "tst_testsdcontainer.moc"
