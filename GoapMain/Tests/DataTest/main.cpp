#include <gmock/gmock.h>


// DDP
// https://github.com/meteor/meteor/blob/devel/packages/ddp/DDP.md
// https://github.com/andrei-markeev/ddpserver
// https://github.com/alexhancock/ddp-cpp
// https://github.com/almightycouch/meteorpp
// https://github.com/dhbaird/easywsclient
// https://almightycouch.org/blog/meteor-ddp-minimongo-implementation-cpp/

//
// SQL
// https://github.com/rbock/sqlpp11
// http://boost.2283326.n4.nabble.com/sqlpp11-SQL-for-C-td4654642.html
// http://soci.sourceforge.net/doc/3.2/
// https://github.com/aminroosta/sqlite_modern_cpp
// https://www.sqlite.org/cvstrac/wiki?p=SqliteWrappers
// https://github.com/SRombauts/SQLiteCpp
// http://sqlitewrapper.kompex-online.com/


int main(int argc, char **argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
