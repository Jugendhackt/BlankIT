#include <iostream>
#include <WordCount.h>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;


int main( int argc, char* argv[] )
{
    srand( time( NULL ) );
    driver = get_driver_instance();
    con = driver->connect( "tcp://127.0.0.1:3306", "jh", "" );
    con->setSchema( "WC" );
    sql::Statement* stmt = con->createStatement();
    stmt->execute( "SET collation_connection = 'latin1_general_cs'" );
    delete stmt;

    string full = "";
    string line;
    unsigned int i = 0;

    while( getline( cin, line ) )
    {
        if( line == "EOF" )
            break;

        full += line + "\n";
        i++;
    }

    WordCount wc( con );
    wc.readFile( full );

    if( argc == 2 )//if a max number of sentences is given -> use it -> if it is a string atoi returns 0
        wc.procData( atoi( argv[1] ) );
    else//if there is no max given, print all (0)
        wc.procData( 0 );

    return 0;
}
