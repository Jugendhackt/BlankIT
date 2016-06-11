#include <iostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <iterator>
#include <Wort.h>
#include <pthread.h>
#include <exception>
#include <future>
#include <google/sparse_hash_map>
#include <google/dense_hash_map>
#include <stdio.h>
#include <WordCount.h>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace boost::algorithm;
using namespace boost;


using google::sparse_hash_map;
using google::dense_hash_map;


sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;


int main( int argc, char* argv[] )
{
    ///Create-Statements in Code!

    driver = get_driver_instance();
    con = driver->connect( "tcp://127.0.0.1:3306", "jh", "" );
    con->setSchema( "WC" );
    /*sql::Statement *stmt = con->createStatement();
    stmt->execute("SET collation_connection = 'latin1_general_cs'");
    delete stmt;*/

    string full = "";
    string line;
    ifstream myfile( "/home/dvs23/Wikipedia-txt/20140616-wiki-de_000000.txt" );

    if( myfile.is_open() )
    {
        unsigned int i = 0;

        while( getline( myfile, line ) )
        {
            full += line + "\n";
            i++;
        }

        myfile.close();
    }

    cout << "Hello world!" << endl;

    WordCount wc( con );
    wc.readFile( full );
    cout << "eingelesen" << endl;
    wc.procData();
    cout<<"verarbeitet"<<endl;
    /*wc.storeData();
    cout<<"gespeichert"<<endl;
    stmt->execute("SELECT D.ID,D.Wort,W.anz FROM dict AS D JOIN worte AS W ON W.ID=D.ID AND W.anz>50 JOIN flags AS F ON F.ID=D.ID AND F.flag = 'satzanf' AND F.anz < W.anz*0.2 WHERE D.Wort REGEXP '^[A-ZÄÖÜ][a-zäöüÄÖÜ]*$';");*/

    return 0;
}
