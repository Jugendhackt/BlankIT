#ifndef WORDCOUNT_H
#define WORDCOUNT_H

#include <iostream>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

#include <google/sparse_hash_set>
#include <google/sparse_hash_map>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


using namespace std;
using namespace boost::algorithm;
using namespace boost;


using google::sparse_hash_map;
using google::sparse_hash_set;

class WordCount {
    public:
        WordCount( sql::Connection* c );
        vector<vector<vector<string>>> strs;//[text][satz][wort]
        sql::Connection* con;
        int readFile( string text );
        int procData( unsigned int maxi );
    protected:
    private:
};

#endif // WORDCOUNT_H
