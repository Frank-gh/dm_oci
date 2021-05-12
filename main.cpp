#include <iostream>
using namespace std;

#include <cstdio>

// #define OTL_ORA12C // Compile OTL 4.0/OCI10gR2

#define OTL_ODBC // CompileOTL 4.0/ODBC  
#define OTL_ODBC_UNIX

#define OCI_ATTR_DEFAULT_LOBPREFETCH_SIZE 512
#define OTL_BIGINT long long // GNU C++  
#define OTL_UBIGINT unsigned long long // GNU C++  




// #include <stdlib.h>
// #define OTL_STR_TO_BIGINT(str,n) \
// { \
// n=strtoll(str,0,10); \
// }
// #define OTL_BIGINT_TO_STR(n,str) \
// { \
// sprintf(str,"%lld",n); \
// }

// #define OTL_STR_TO_BIGINT(str,n) \
// { \
// n=strtoll(str,0,10); \
// }
// #define OTL_BIGINT_TO_STR(n,str) \
// { \
// sprintf(str,"%lld",n); \
// }


#define OTL_STREAM_NO_PRIVATE_UNSIGNED_LONG_OPERATORS 

#include "otlv4.h" // include the OTL 4.0 header file

otl_connect db; // connect object


void insert()
// insert rows into table
{
    otl_stream o(50, // buffer size
            "insert into test_tab values(:f1<BIGINT>,:f2<char[31]>)",
            // SQL statement
            db // connect object
            );
    o.set_commit(0); // turn off stream's "auto-commit"

    char tmp[32];

    for (long long i = 1; i <= 123; ++i) {
        sprintf(tmp, "Name%lld", i);
        o << i << tmp;
    }
    o.flush(); // flush the stream's dirty buffer: 
    // execute the INSERT for the rows 
    // that are still in the stream buffer

    // db.commit_nowait(); // commit with no wait (new feature of Oracle 10.2)
    db.commit();
}

void select() {
    otl_stream i(50, // buffer size
            "select * from test_tab where f1>=:f<int> and f1<=:f*2",
            // SELECT statement
            db // connect object
            );
    // create select stream

    float f1;
    char f2[31];

    i << 8; // assigning :f = 8
    // SELECT automatically executes when all input variables are
    // assigned. First portion of output rows is fetched to the buffer

    while (!i.eof()) { // while not end-of-data
        i >> f1 >> f2;
        cout << "f1=" << f1 << ", f2=" << f2 << endl;
    }

    i << 4; // assigning :f = 4
    // SELECT automatically executes when all input variables are
    // assigned. First portion of output rows is fetched to the buffer

    while (!i.eof()) { // while not end-of-data
        i >> f1 >> f2;
        cout << "f1=" << f1 << ", f2=" << f2 << endl;
    }

}

int main() {
    otl_connect::otl_initialize(); // initialize OCI environment
    try {

        db.rlogon("SYSDBA/xk.xmx190035"); // connect to Oracle

        otl_cursor::direct_exec(db, "drop table test_tab",
                otl_exception::disabled // disable OTL exceptions
        ); // drop table

        otl_cursor::direct_exec(db,
                "create table test_tab(f1 number, f2 varchar2(30))"); // create table

        insert(); // insert records into table
        select(); // select records from table

    }

    catch (otl_exception& p) { // intercept OTL exceptions
        cerr << p.msg << endl; // print out error message
        cerr << p.stm_text << endl; // print out SQL that caused the error
        cerr << p.var_info << endl; // print out the variable that caused the error
    }

    db.logoff(); // disconnect from Oracle

    return 0;

}