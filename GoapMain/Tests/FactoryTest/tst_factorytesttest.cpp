#include <QString>
#include <QtTest>

class FactoryTestTest : public QObject
{
    Q_OBJECT

public:
    FactoryTestTest();

private Q_SLOTS:
    void testCase1();
};

FactoryTestTest::FactoryTestTest()
{
}

void FactoryTestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(FactoryTestTest)

#include "tst_factorytesttest.moc"
