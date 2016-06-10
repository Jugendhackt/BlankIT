#ifndef WORD_H
#define WORD_H

#include <google/sparse_hash_map>

using google::sparse_hash_map;



using namespace std;


class Wort
{
public:
    string w;
    unsigned int WID;
    unsigned int c;
    sparse_hash_map<string, unsigned int> attr;//Geschlecht -> 1=männl, 2 = weibl etc.
    sparse_hash_map<string, unsigned int> flags;//Satzanf -> Anzahl
    sparse_hash_map<unsigned int, bool> glW;///gleichgesetzte Wörter
    sparse_hash_map<unsigned int, unsigned int> oftInSatz;
    sparse_hash_map<unsigned int, unsigned int> oftInDirV;
    sparse_hash_map<unsigned int, unsigned int> oftInDirH;
    sparse_hash_map<unsigned int, unsigned int> oftInText;

    Wort(string wo, unsigned int co, unsigned int id);
    Wort();
    virtual ~Wort();
protected:
private:
};

#endif // WORD_H

