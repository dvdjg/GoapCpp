#include <gmock/gmock.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <stdexcept>
#include <sqlite_modern_cpp.h>
#include <sqlite_modern_cpp/log.h>
using namespace sqlite;
using namespace std;


class DataTest : public ::testing::Test
{

public:
    DataTest()
    {
    }
protected:
    // Per-test-case set-up.
    // Called before the first test in this test case.
    // Can be omitted if not needed.
    static void SetUpTestCase()
    {
        //shared_resource_ = new ...;
    }

    // Per-test-case tear-down.
    // Called after the last test in this test case.
    // Can be omitted if not needed.
    static void TearDownTestCase()
    {
        //delete shared_resource_;
        //shared_resource_ = NULL;
    }

    // You can define per-test set-up and tear-down logic as usual.
    virtual void SetUp() {  }
    virtual void TearDown() {  }
};

TEST_F(DataTest, Test1)
{
    bool error_detected = false;
    error_log(
        [&](errors::constraint)
    {
        cerr << "Wrong error detected!" << endl;
    },
    [&](errors::constraint_primarykey e)
    {
        cerr << e.get_code() << '/' << e.get_extended_code() << ": " << e.what() << endl;
        error_detected = true;
    }
    );
    database db(":memory:");
    db << "CREATE TABLE person (id integer primary key not null, name TEXT);";

    try
    {
        db << "INSERT INTO person (id,name) VALUES (?,?)" << 1 << "jack";
        // inserting again to produce error
        db << "INSERT INTO person (id,name) VALUES (?,?)" << 1 << "jack";
    }
    catch (errors::constraint &e)
    {
    }

    EXPECT_TRUE(error_detected);
}

TEST_F(DataTest, Test2)
{

}
