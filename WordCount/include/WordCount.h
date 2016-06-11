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
        WordCount( sql::Connection* c );//contructor gets connection to mysql DB
        int readText( string text );//read a text from a long plain string
        int procData( unsigned int maxi );//process data stored in strs, giving the max number of sentences to return (0=all)
    protected:
    private:
        vector<vector<vector<string>>> strs;//[text][satz][wort] -> scalable -> multiple texts at once
        sql::Connection* con;//store that connection in class
};

#endif // WORDCOUNT_H
