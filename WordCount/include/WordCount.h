#ifndef WORDCOUNT_H
#define WORDCOUNT_H

#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <google/sparse_hash_map>
#include <regex>
#include <Wort.h>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


using namespace std;
using namespace boost::algorithm;


using google::sparse_hash_map;

class WordCount
{
public:
    WordCount(sql::Connection *c);
    virtual ~WordCount();
    sparse_hash_map<unsigned int,string> globDictI;
    sparse_hash_map<string,unsigned int> globDictS;
    unsigned int TID;
    vector<vector<vector<string>>> strs;
    sparse_hash_map<string,unsigned int> wcount;
    sparse_hash_map<string,Wort> locworte;
    sql::Connection *con;
    int readFile(string text);
    int procData();
    int storeData();
protected:
private:
};

#endif // WORDCOUNT_H
