#include <QtTest>

#include <libs/PluginsBase/tools/trajectoryparser.h>

class TestRegexMatch : public QObject
{
    Q_OBJECT

public:
    TestRegexMatch();
    ~TestRegexMatch();

private slots:
    void test_case1();

};

TestRegexMatch::TestRegexMatch()
{

}

TestRegexMatch::~TestRegexMatch()
{

}

void TestRegexMatch::test_case1()
{
    TrajectoryParser parser;
    parser.parse("rotate a+100 r5 z100;"
                 "forward d100 v100 dz10;"
                 "forward d100 v100 -z10 dz10 acc5;");
}

QTEST_APPLESS_MAIN(TestRegexMatch)

#include "tst_testregexmatch.moc"
