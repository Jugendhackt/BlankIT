#include "WordCount.h"

WordCount::WordCount( sql::Connection* c )
{
    con = c;
}

int WordCount::readFile( string text )
{
    strs.clear();
    string fu = text;

    vector<vector<string>> lstrs;

    boost::regex re1( "[\?!;,\()\"\':]|(?<![0-9])[.]" );
    string full = boost::regex_replace( fu, re1, " $& " );
    boost::regex re2( "[ ]+" );
    full = boost::regex_replace( full, re2, " " );
    boost::regex re3( "([\[][\[])(.*?)([]][]])" );
    full = boost::regex_replace( full, re3, "$&$2\n" );


    vector<string> tem;
    boost::algorithm::split_regex( tem, full, boost::regex( "(?<![0-9])[.]|[!?\n\r]" ) );

    for( unsigned int j = 0; j < tem.size(); j++ )
    {
        vector<string> tem2;
        boost::algorithm::split( tem2, tem[j], is_any_of( " \n\r" ) );
        lstrs.push_back( tem2 );
    }

    ///leere Wörter löschen
    for( unsigned int k = 0; k < lstrs.size(); k++ )
    {
        for( unsigned int j = 0; j < lstrs[k].size(); j++ )
        {
            if( lstrs[k][j].length() <= 0 )
            {
                lstrs[k].erase( lstrs[k].begin() + j );
                j--;
            }
        }

        if( lstrs[k].size() == 0 )
        {
            lstrs.erase( lstrs.begin() + k );
            k--;
        }
    }

    strs.push_back( lstrs );

    return 0;
}

string makeSentence( vector<string> sen )
{
    string ret = "";

    for( size_t i = 0; i < sen.size(); i++ )
    {
        ret += sen[i];
        ret += " ";
    }

    ret += ".";
    return ret;
}

int WordCount::procData()
{
    sql::Statement* stmt = con->createStatement();

    sql::ResultSet* res = stmt->executeQuery( "select sum(anz) from Nomen;" );
    unsigned int su = 0;

    while( res->next() )
        su = res->getUInt( "sum(anz)" );

    delete res;
    res = stmt->executeQuery( "select count(*) from Nomen;" );
    unsigned int si = 0;

    while( res->next() )
        si = res->getUInt( "count(*)" );

    double mid = ( double )su / ( double )si;
    sparse_hash_map<string, unsigned int> wos;

    for( unsigned int l = 0; l < strs.size(); l++ )
    {
        for( unsigned int k = 0; k < strs[l].size(); k++ )
        {
            for( unsigned int j = 0; j < strs[l][k].size(); j++ )
                wos[strs[l][k][j]] = 0;
        }
    }

    sql::PreparedStatement* loadAnz = con->prepareStatement( "select D.ID,D.Wort,W.anz from dict as D join worte as W on D.ID=W.ID AND D.Wort = ?;" );

    for( auto t = wos.begin(); t != wos.end(); ++t )
    {
        loadAnz->setString( 1, t->first );
        sql::ResultSet* res = loadAnz->executeQuery();

        while( res->next() )
            wos[t->first] = res->getUInt( "anz" );

        delete res;
    }

    delete loadAnz;
    unsigned int c1 = 0;
    unsigned int c2 = 0;

    for( unsigned int l = 0; l < strs.size(); l++ )
    {
        for( unsigned int k = 0; k < strs[l].size(); k++ )
        {
            unsigned int counter = 0;

            for( unsigned int j = 0; j < strs[l][k].size(); j++ )
            {
                if( wos[strs[l][k][j]] <= mid )
                    counter++;
            }

            if( strs[l][k].size() > 3 && ( double )counter / ( double )strs[l][k].size() > 0.25 && ( double )counter / ( double )strs[l][k].size() < 0.75 )
            {
                string s = makeSentence( strs[l][k] );

                if( !boost::regex_match( s.c_str(), boost::regex( ".*?[=*)(:\\]\\[/0-9]+.*?|^[^A-Z].*?" ) ) )
                {
                    c2++;
                    cout << s << endl;
                }
            }

            c1++;
        }
    }

    cout << "[@24]" << endl;

    return 0;
}
