#include <gtest/gtest.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>

#include <sqlite_modern_cpp.h>
#include <sqlite_modern_cpp/log.h>

using namespace sqlite;
using namespace std;

static void insert(database &db, bool is_null)
{
    int id = 1;
    boost::optional<int> val;
    if (!is_null)
    {
        val = 5;
    }

    db << "delete from test where id = 1";
    db << "insert into test(id,val) values(?,?)" << id << val;
}

static void select(database &db, bool should_be_null)
{
    db << "select id,val from test">> [&](long long, boost::optional<int> val)
    {
        if (should_be_null)
        {
            if (val)
            {
                EXPECT_FALSE("exit(EXIT_FAILURE)");
            }
        }
        else
        {
            if (!val)
            {
                EXPECT_FALSE("exit(EXIT_FAILURE)");
            }
        }
    };
}

class SQLiteModern : public ::testing::Test
{

public:
    SQLiteModern()
    {
    }
protected:
    static void SetUpTestCase()
    {
    }

    static void TearDownTestCase()
    {
    }

    // You can define per-test set-up and tear-down logic as usual.
    virtual void SetUp() {  }
    virtual void TearDown() {  }
};

TEST_F(SQLiteModern, error_log)
{
    bool error_detected = false;
    {
        error_log(
            [&](errors::constraint e)
        {
            cerr << "Wrong error detected! " << e.get_code() << '/' << e.get_extended_code() << ": " << e.what() << endl;
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
        catch (errors::constraint e)
        {
            cerr << "Catched error.- " << e.get_code() << '/' << e.get_extended_code() << ": " << e.what() << endl;
        }
    }
    EXPECT_TRUE(error_detected);
}

TEST_F(SQLiteModern, blob_example)
{
    try
    {
        database db(":memory:");

        db << "CREATE TABLE person (name TEXT, numbers BLOB);";
        db << "INSERT INTO person VALUES (?, ?)" << "bob" << vector<int> { 1, 2, 3, 4};
        db << "INSERT INTO person VALUES (?, ?)" << "jack" << vector<char> { '1', '2', '3', '4'};
        db << "INSERT INTO person VALUES (?, ?)" << "sara" << vector<double> { 1.0, 2.0, 3.0, 4.0};

        vector<int> numbers_bob;
        db << "SELECT numbers from person where name = ?;" << "bob">> numbers_bob;

        if (numbers_bob.size() != 4 || numbers_bob[0] != 1
            || numbers_bob[1] != 2 || numbers_bob[2] != 3 || numbers_bob[3] != 4)
        {
            cout << "Bad result on line " << __LINE__ << endl;
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }
        //else { for(auto e : numbers_bob) cout << e << ' '; cout << endl; }

        vector<char> numbers_jack;
        db << "SELECT numbers from person where name = ?;" << "jack">> numbers_jack;

        if (numbers_jack.size() != 4 || numbers_jack[0] != '1'
            || numbers_jack[1] != '2' || numbers_jack[2] != '3' || numbers_jack[3] != '4')
        {
            cout << "Bad result on line " << __LINE__ << endl;
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }
        //else { for(auto e : numbers_jack) cout << e << ' '; cout << endl; }

        vector<double> numbers_sara;
        db << "SELECT numbers from person where name = ?;" << "sara">> numbers_sara;

        if (numbers_sara.size() != 4 || numbers_sara[0] != 1
            || numbers_sara[1] != 2 || numbers_sara[2] != 3 || numbers_sara[3] != 4)
        {
            cout << "Bad result on line " << __LINE__ << endl;
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }
        //else {
        //db << "SELECT numbers from person where name = ?;" << "sara" >> [](vector<double> numbers_sara){
        //for(auto e : numbers_sara) cout << e << ' '; cout << endl;
        //};
        //}

    }
    catch (sqlite_exception e)
    {
        cout << "Unexpected error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

TEST_F(SQLiteModern, boost_optional)
{
    try
    {
        // creates a database file 'dbfile.db' if it does not exists.
        database db(":memory:");

        db << "drop table if exists test";
        db <<
           "create table if not exists test ("
           "   id integer primary key,"
           "   val int"
           ");";

        insert(db, true);
        select(db, true);

        insert(db, false);
        select(db, false);

    }
    catch (exception &e)
    {
        cout << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

TEST_F(SQLiteModern, exception_dont_execute)
{
    database db(":memory:");
    db << "CREATE TABLE person (id integer primary key not null, name TEXT not null);";

    try
    {
        auto stmt = db << "INSERT INTO person (id,name) VALUES (?,?)";
        throw 1;
    }
    catch (int)
    {
    }
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}


TEST_F(SQLiteModern, exceptions)
{
    database db(":memory:");
    db << "CREATE TABLE person (id integer primary key not null, name TEXT);";
    bool expception_thrown = false;

    try
    {
        db << "INSERT INTO person (id,name) VALUES (?,?)" << 1 << "jack";
        // inserting again to produce error
        db << "INSERT INTO person (id,name) VALUES (?,?)" << 1 << "jack";
    }
    catch (errors::constraint &e)
    {
        cerr << e.get_code() << ": " << e.what() << " during "
             << quoted(e.get_sql()) << endl;
        expception_thrown = true;
#if SQLITE_VERSION_NUMBER >= 3014000
        if (e.get_sql() != "INSERT INTO person (id,name) VALUES (1,'jack')")
        {
#else
        if (e.get_sql() != "INSERT INTO person (id,name) VALUES (?,?)")
        {
#endif
            cerr << "Wrong statement failed\n";
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }
    }

    if (!expception_thrown)
    {
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

TEST_F(SQLiteModern, extended_exceptions)
{
    database db(":memory:");
    db << "CREATE TABLE person (id integer primary key not null, name TEXT);";
    bool expception_thrown = false;

    try
    {
        db << "INSERT INTO person (id,name) VALUES (?,?)" << 1 << "jack";
        // inserting again to produce error
        db << "INSERT INTO person (id,name) VALUES (?,?)" << 1 << "jack";
    }
    catch (errors::constraint_primarykey &e)
    {
        cerr << e.get_code() << '/' << e.get_extended_code() << ": " << e.what() << " during "
             << quoted(e.get_sql()) << endl;
        expception_thrown = true;
#if SQLITE_VERSION_NUMBER >= 3014000
        if (e.get_sql() != "INSERT INTO person (id,name) VALUES (1,'jack')")
        {
#else
        if (e.get_sql() != "INSERT INTO person (id,name) VALUES (?,?)")
        {
#endif
            cerr << "Wrong statement failed\n";
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }
    }

    if (!expception_thrown)
    {
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

struct TmpFile
{
    string fname;

    TmpFile(): fname("./flags.db") { }
    ~TmpFile()
    {
        remove(fname.c_str());
    }
};

#if __BYTE_ORDER == __BIG_ENDIAN
#define OUR_UTF16 "UTF-16be"
#else
#define OUR_UTF16 "UTF-16le"
#endif

TEST_F(SQLiteModern, flags)
{
    try
    {
        TmpFile file;
        sqlite::sqlite_config cfg;
        std::string enc;
        {
            database db(":memory:", cfg);
            db << "PRAGMA encoding;">> enc;
            if (enc != "UTF-8")
            {
                cout << "Unexpected encoding on line " << __LINE__ << '\n';
            }
            EXPECT_FALSE(enc != "UTF-8");
        }
        {
            database db(u":memory:", cfg);
            db << "PRAGMA encoding;">> enc;
            if (enc != OUR_UTF16)
            {
                cout << "Unexpected encoding on line " << __LINE__ << '\n';
            }
            EXPECT_FALSE(enc != OUR_UTF16);
        }
        {
            cfg.encoding = Encoding::UTF8;
            database db(u":memory:", cfg);
            db << "PRAGMA encoding;">> enc;
            if (enc != "UTF-8")
            {
                cout << "Unexpected encoding on line " << __LINE__ << '\n';
            }
            EXPECT_FALSE(enc != "UTF-8");
        }
        {
            cfg.encoding = Encoding::UTF16;
            database db(u":memory:", cfg);
            db << "PRAGMA encoding;">> enc;
            if (enc != OUR_UTF16)
            {
                cout << "Unexpected encoding on line " << __LINE__ << '\n';
            }
            EXPECT_FALSE(enc != OUR_UTF16);
        }
        {
            database db(file.fname, cfg);
            db << "PRAGMA encoding;">> enc;
            if (enc != OUR_UTF16)
            {
                cout << "Unexpected encoding on line " << __LINE__ << '\n';
            }
            EXPECT_FALSE(enc != OUR_UTF16);

            db << "CREATE TABLE foo (a string);";
            db << "INSERT INTO foo VALUES (?)" << "hello";
        }
        {
            cfg.flags = sqlite::OpenFlags::READONLY;
            database db(file.fname, cfg);

            string str;
            db << "SELECT a FROM foo;">> str;

            if (str != "hello")
            {
                cout << "Bad result on line " << __LINE__ << endl;
            }
            EXPECT_FALSE(str != "hello");

            try
            {
                db << "INSERT INTO foo VALUES (?)" << "invalid";
                cout << "Unexpected success on line " << __LINE__ << endl;
                EXPECT_FALSE("exit(EXIT_FAILURE)");
            }
            catch (errors::readonly &) {}
        }
    }
    catch (sqlite_exception e)
    {
        cout << "Unexpected error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

static int add_integers(int i, int j)
{
    return i + j;
}

TEST_F(SQLiteModern, functions)
{
    try
    {
        database db(":memory:");

        db.define("my_new_concat", [](std::string i, std::string j)
        {
            return i + j;
        });
        db.define("my_new_concat", [](std::string i, std::string j, std::string k)
        {
            return i + j + k;
        });
        db.define("add_integers", &add_integers);
        std::string test1, test3;
        int test2 = 0;
        db << "select my_new_concat('Hello ','world!')">> test1;
        db << "select add_integers(1,1)">> test2;
        db << "select my_new_concat('a','b','c')">> test3;

        if (test1 != "Hello world!" || test2 != 2 || test3 != "abc")
        {
            cout << "Wrong result\n";
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }

        db.define("my_count", [](int &i, int)
        {
            ++i;
        }, [](int &i)
        {
            return i;
        });
        db.define("my_concat_aggregate", [](std::string & stored, std::string current)
        {
            stored += current;
        }, [](std::string & stored)
        {
            return stored;
        });
        db << "create table countable(i, s)";
        db << "insert into countable values(1, 'a')";
        db << "insert into countable values(2, 'b')";
        db << "insert into countable values(3, 'c')";
        db << "select my_count(i) from countable">> test2;
        db << "select my_concat_aggregate(s) from countable order by i">> test3;

        if (test2 != 3 || test3 != "abc")
        {
            cout << "Wrong result\n";
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }

        db.define("tgamma", [](double i)
        {
            return std::tgamma(i);
        });
        db << "CREATE TABLE numbers (number INTEGER);";

        for (auto i = 0; i != 10; ++i)
        {
            db << "INSERT INTO numbers VALUES (?);" << i;
        }

        db << "SELECT number, tgamma(number+1) FROM numbers;">> [](double number, double factorial)
        {
            cout << number << "! = " << factorial << '\n';
        };
    }
    catch (sqlite_exception e)
    {
        cout << "Unexpected error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

struct tbl_functor
{
    explicit tbl_functor(vector<pair<int, string>> &vec_) : vec(vec_) { }

    void operator()(int id, string name)
    {
        vec.push_back(make_pair(id, move(name)));
    }
    vector<pair<int, string>> &vec;
};

TEST_F(SQLiteModern, functors)
{
    try
    {
        database db(":memory:");
        db << "CREATE TABLE tbl (id integer, name string);";
        db << "INSERT INTO tbl VALUES (?, ?);" << 1 << "hello";
        db << "INSERT INTO tbl VALUES (?, ?);" << 2 << "world";

        vector<pair<int, string>> vec;
        db << "select id,name from tbl;">> tbl_functor(vec);

        if (vec.size() != 2)
        {
            cout << "Bad result on line " << __LINE__ << endl;
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }

        vec.clear();

        tbl_functor functor(vec);
        db << "select id,name from tbl;">> functor;

        if (vec.size() != 2 || vec[0].first != 1 || vec[0].second != "hello")
        {
            cout << "Bad result on line " << __LINE__ << endl;
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }

    }
    catch (sqlite_exception e)
    {
        cout << "Unexpected error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}


template<typename Target, typename... AttrTypes>
struct builder
{
    vector<Target> results;

    void operator()(AttrTypes... args)
    {
        results.emplace_back(std::forward < AttrTypes && > (args)...);
    };
};


struct user
{
    int age;
    string name;
    double weight;

    user(int age, string name, double weight) : age(age), name(name), weight(weight) { }

    static std::vector<user> all(sqlite::database &db)
    {
        builder<user, int, std::string, double> person_builder;
        db << "SELECT * FROM user;"
           >> person_builder;
        return std::move(person_builder.results); // move to avoid copying data ;-)
    };
};

TEST_F(SQLiteModern, lvalue_functor_example)
{
    try
    {
        database db(":memory:");

        db <<
           "create table if not exists user ("
           "   age int,"
           "   name text,"
           "   weight real"
           ");";

        db << "insert into user (age,name,weight) values (?,?,?);" << 20 << u"chandler" << 83.25;
        db << "insert into user (age,name,weight) values (?,?,?);" << 21 << u"monika" << 86.25;
        db << "insert into user (age,name,weight) values (?,?,?);" << 22 << u"ross" << 88.25;

        auto users = user::all(db);

        for (auto u : users)
        {
            cout << u.age << ' ' << u.name << ' ' << u.weight << endl;
        }
        if (users.size() != 3)
        {
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

}

struct dbFront
{
    std::unique_ptr<database_binder> storedProcedure;
    database db;
    dbFront(): db(":memory:")
    {
        db << "CREATE TABLE tbl (id integer, name string);";
        // the temporary moved object should not run _execute() function on destruction.
        storedProcedure = std::make_unique<database_binder>(
                              db << "INSERT INTO tbl VALUES (?, ?);"
                          );
    }
};


TEST_F(SQLiteModern, mov_ctor)
{
    try
    {
        dbFront dbf;
    }
    catch (sqlite_exception e)
    {
        cout << "Unexpected error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

TEST_F(SQLiteModern, nullptr_uniqueptr)
{
    try
    {
        database db(":memory:");
        db << "CREATE TABLE tbl (id integer,age integer, name string, img blob);";
        db << "INSERT INTO tbl VALUES (?, ?, ?, ?);" << 1 << 24 << "bob" << vector<int> { 1, 2, 3};
        unique_ptr<string> ptr_null;
        db << "INSERT INTO tbl VALUES (?, ?, ?, ?);" << 2 << nullptr << ptr_null << nullptr;

        db << "select age,name,img from tbl where id = 1">> [](unique_ptr<int> age_p, unique_ptr<string> name_p, unique_ptr<vector<int>> img_p)
        {
            if (age_p == nullptr || name_p == nullptr || img_p == nullptr)
            {
                cerr << "ERROR: values should not be null" << std::endl;
                EXPECT_FALSE("exit(EXIT_FAILURE)");
            }

            cout << "age:" << *age_p << " name:" << *name_p << " img:";
            for (auto i : *img_p)
            {
                cout << i << ",";
            }
            cout << endl;
        };

        db << "select age,name,img from tbl where id = 2">> [](unique_ptr<int> age_p, unique_ptr<string> name_p, unique_ptr<vector<int>> img_p)
        {
            if (age_p != nullptr || name_p != nullptr || img_p != nullptr)
            {
                cerr << "ERROR: values should be nullptr" << std::endl;
                EXPECT_FALSE("exit(EXIT_FAILURE)");
            }

            cout << "OK all three values are nullptr" << endl;
        };

    }
    catch (sqlite_exception e)
    {
        cout << "Sqlite error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

TEST_F(SQLiteModern, prepared_statment)
{
    try
    {

        database db(":memory:");

        auto pps = db << "select ?"; // get a prepared parsed and ready statment

        int test = 4;
        pps << test; // set a bound var

        pps >> test; // execute statement
        EXPECT_EQ(4, test);

        pps << 4; // bind a rvalue
        pps++; // and execute

        pps << 8>> test;
        EXPECT_EQ(8, test);

        auto pps2 = db << "select 1,2,3,4,5"; // multiple extract test

        pps2 >> [](int a, int b, int c, int d, int e)
        {
            EXPECT_TRUE(1 == a && 2 == b && 3 == c && 4 == d && 5 == e);
            std::cout << "L " << a << b << c << d << e << "\n"; // still works as intended
        };

        auto pps3 = db << "select ?,?,?";

        pps3 << 1 << test << 5>> [](int a, int b, int, int c)
        {
            std::cout << "L2 " << a << b << c << "\n"; // still works as intended
        };

        db << "select ?,?" << test << 5>> test; // and mow everything together

        db << "select ?, ?, ?" << 1 << test << 1>> [](int a, int b, int, int c)
        {
            std::cout << "L3 " << a << b << c << "\n"; // still works as intended
        };

        db << "select ?" << test; 		// noVal
        db << "select ?,?" << test << 1;
        db << "select ?,?" << 1 << test;
        db << "select ?,?" << 1 << 1;
        db << "select ?,?" << test << test;

        db << "select ?" << test>> test; 		// lVal
        db << "select ?,?" << test << 1>> test;
        db << "select ?,?" << 1 << test>> test;
        db << "select ?,?" << 1 << 1>> test;
        db << "select ?,?" << test << test>> test;

        int q = 0;

        db << "select ?" << test>> [&](int t)
        {
            q = t++;
        }; 		// rVal
        db << "select ?,?" << test << 1>> [&](int t, int p)
        {
            q = t + p;
        };
        db << "select ?,?" << 1 << test>> [&](int t, int p)
        {
            q = t + p;
        };
        db << "select ?,?" << 1 << 1>> [&](int t, int p)
        {
            q = t + p;
        };
        db << "select ?,?" << test << test>> [&](int t, int p)
        {
            q = t + p;
        };

        db << "select ?,?,?" << test << 1 << test; // mix
        db << "select ?,?,?" << 1 << test << 1;
        db << "select ?,?,?" << 1 << 1 << test;
        db << "select ?,?,?" << 1 << 1 << 1;
        db << "select ?,?,?" << test << test << test;

        {
            auto pps4 = db << "select ?,?,?"; // reuse

            (pps4 << test << 1 << test)++;
            (pps4 << 1 << test << 1)++;
            (pps4 << 1 << 1 << test)++;
            (pps4 << 1 << 1 << 1)++;
            (pps4 << test << test << test)++;
        }

        {
            auto prep = db << "select ?";

            prep << 5;
            prep.execute();
            prep << 6;
            prep.execute();
        }


    }
    catch (sqlite_exception e)
    {
        cout << "Unexpected error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

TEST_F(SQLiteModern, readme_example)
{
    try
    {
        // creates a database file 'dbfile.db' if it does not exists.
        database db(":memory:");

        // executes the query and creates a 'user' table
        db <<
           "create table if not exists user ("
           "   _id integer primary key autoincrement not null,"
           "   age int,"
           "   name text,"
           "   weight real"
           ");";

        // inserts a new user record.
        // binds the fields to '?' .
        // note that only types allowed for bindings are :
        //      int ,long, long long, float, double
        //      string , u16string
        // sqlite3 only supports utf8 and utf16 strings, you should use std::string for utf8 and std::u16string for utf16.
        // note that u"my text" is a utf16 string literal of type char16_t * .
        db << "insert into user (age,name,weight) values (?,?,?);"
           << 20
           << u"bob"
           << 83.25;

        int age = 22;
        float weight = 68.5;
        string name = "jack";
        db << u"insert into user (age,name,weight) values (?,?,?);" // utf16 query string
           << age
           << name
           << weight;

        cout << "The new record got assigned id " << db.last_insert_rowid() << endl;

        // selects from user table on a condition ( age > 18 ) and executes
        // the lambda for each row returned .
        db << "select age,name,weight from user where age > ? ;"
           << 21
           >> [&](int _age, string _name, double _weight)
        {
            if (_age != age || _name != name)
            {
                EXPECT_FALSE("exit(EXIT_FAILURE)");
            }
            cout << _age << ' ' << _name << ' ' << _weight << endl;
        };

        // selects the count(*) from user table
        // note that you can extract a single culumn single row result only to : int,long,long,float,double,string,u16string
        int count = 0;
        db << "select count(*) from user">> count;
        cout << "cout : " << count << endl;

        // you can also extract multiple column rows
        db << "select age, name from user where _id=1;">> tie(age, name);
        cout << "Age = " << age << ", name = " << name << endl;

        // this also works and the returned value will be automatically converted to string
        string str_count;
        db << "select count(*) from user">> str_count;
        cout << "scount : " << str_count << endl;
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
}

TEST_F(SQLiteModern, shared_connection)
{
    try
    {

        database db(":memory:");
        {
            auto con = db.connection();
            {
                database db2(con);
                int test = 0;
                db2 << "select 1">> test;
                if (test != 1)
                {
                    EXPECT_FALSE("exit(EXIT_FAILURE)");
                }
            }

            int test = 0;
            db << "select 1">> test;
            if (test != 1)
            {
                EXPECT_FALSE("exit(EXIT_FAILURE)");
            }
        }
    }
    catch (sqlite_exception e)
    {
        cout << "Unexpected error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

TEST_F(SQLiteModern, simple_examples)
{
    try
    {
        database db(":memory:");

        db << "CREATE TABLE foo (a integer, b string);";
        db << "INSERT INTO foo VALUES (?, ?)" << 1 << "hello";
        db << "INSERT INTO foo VALUES (?, ?)" << 2 << "world";

        string str;
        db << "SELECT b from FOO where a=?;" << 2L>> str;

        if (str != "world")
        {
            cout << "Bad result on line " << __LINE__ << endl;
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }

        std::string sql("select 1+1");
        long test = 0;
        db << sql>> test;

        if (test != 2)
        {
            EXPECT_FALSE("exit(EXIT_FAILURE)");
        }

    }
    catch (sqlite_exception e)
    {
        cout << "Unexpected error " << e.what() << endl;
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }
    catch (...)
    {
        cout << "Unknown error\n";
        EXPECT_FALSE("exit(EXIT_FAILURE)");
    }

    cout << "OK\n";
    EXPECT_TRUE("exit(EXIT_SUCCESS)");
}

struct AutoDeleteFile
{
    string fname;

    AutoDeleteFile(): fname("./trycatchblocks.db") {}
    ~AutoDeleteFile()
    {
        remove(fname.c_str());
    }
};


class DBInterface
{
    database db;

public:
    DBInterface(const string &fileName) : db(fileName) { }

    void LogRequest(const string &username, const string &ip, const string &request)
    {
        try
        {
            auto timestamp = std::to_string(time(nullptr));

            db  <<
                "create table if not exists log_request ("
                "   _id integer primary key autoincrement not null,"
                "   username text,"
                "   timestamp text,"
                "   ip text,"
                "   request text"
                ");";
            db  << "INSERT INTO log_request (username, timestamp, ip, request) VALUES (?,?,?,?);"
                << username
                << timestamp
                << ip
                << request;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    bool TestData(void)
    {
        try
        {
            string username, timestamp, ip, request;

            db  << "select username, timestamp, ip, request from log_request where username = ?"
                << "test"
                >> std::tie(username, timestamp, ip, request);

            if (username == "test" && ip == "127.0.0.1" && request == "hello world")
            {
                return true;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }

        return false;
    }
};

TEST_F(SQLiteModern, trycatchblocks)
{
    // --------------------------------------------------------------------------
    // -- Test if writing to disk works properly from within a catch block.
    // --------------------------------------------------------------------------
    try
    {
        throw "hello";
    }
    catch (...)
    {
        AutoDeleteFile tmpF;
        DBInterface interf(tmpF.fname);
        interf.LogRequest("test", "127.0.0.1", "hello world");
        if (!interf.TestData())
        {
            EXPECT_FALSE("exit( EXIT_FAILURE )");
        }
    }

    EXPECT_TRUE("exit( EXIT_SUCCESS )");
}
