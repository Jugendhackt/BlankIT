#include "WordCount.h"

WordCount::WordCount(sql::Connection *c)
{
    con=c;
    //locworte.set_empty_key("%%%-leer-%%%");
    locworte.set_deleted_key("%%%-gelöscht-%%%");
    //globDictS.set_empty_key("%%%-leer-%%%");
    globDictS.set_deleted_key("%%%-gelöscht-%%%");
    //globDictI.set_empty_key(4294967294);
    globDictI.set_deleted_key(4294967295);
}

WordCount::~WordCount()
{
    //dtor
}

int WordCount::readFile(string text)
{
    globDictI.clear();
    globDictS.clear();
    strs.clear();
    wcount.clear();
    locworte.clear();
    string full = text;

    std::regex re1("[\?!;,\()\"\':]|(?<![0-9])[.]");
    full = std::regex_replace(full, re1, " $& ");
    std::regex re2("[ ]+");
    full = std::regex_replace(full, re2, " ");
    std::regex re3("([\[][\[])(.*?)([]][]])");
    full = std::regex_replace(full, re3, "$&$2\n");


    vector<vector<string>> ret;
    vector<string> tem;
    boost::algorithm::split_regex(tem,full,boost::regex("(?<![0-9])[.]|[!?\n\r]"));
    for(unsigned int j = 0; j<tem.size(); j++)
    {
        vector<string> tem2;
        boost::algorithm::split(tem2,tem[j],is_any_of(" \n\r"));
        ret.push_back(tem2);
    }
    strs.push_back(ret);

    ///leere Wörter löschen
    for(unsigned int l = 0; l<strs.size(); l++)
    {
        for(unsigned int k = 0; k<strs[l].size(); k++)
        {
            for(unsigned int j = 0; j<strs[l][k].size(); j++)
            {
                if(strs[l][k][j].length()<=0)
                {
                    strs[l][k].erase(strs[l][k].begin()+j);
                    j--;
                }
            }
            if(strs[l][k].size()==0)
            {
                strs[l].erase(strs[l].begin()+k);
                k--;
            }
        }
        if(strs[l].size()==0)
        {
            strs.erase(strs.begin()+l);
            l--;
        }
    }

    ///Wörter zählen
    for(unsigned int i = 0; i<strs.size(); i++)
    {
        for(unsigned int j = 0; j<strs[i].size(); j++)
        {
            for(unsigned int k = 0; k<strs[i][j].size(); k++)
            {
                wcount[strs[i][j][k]]++;
            }
        }
    }

    sql::PreparedStatement *dictIns = con->prepareStatement("INSERT IGNORE INTO dict(Wort) VALUES(?);");
    for(auto t = wcount.begin(); t!=wcount.end(); ++t)
    {
        if(t->first.length()>0)
        {
            dictIns->setString(1,t->first);
            dictIns->execute();
        }
    }
    delete dictIns;
    sql::PreparedStatement *textIns = con->prepareStatement("INSERT IGNORE INTO text(Titel, Text) VALUES(?,?);");
    sql::PreparedStatement *getId = con->prepareStatement("SELECT TID,Titel FROM text WHERE Titel=?");

    string tit="";
    unsigned int anz = 0;
    for(unsigned int j = 0; j<strs[0][0].size(); j++)
    {
        if(j!=strs[0][0].size()-1)
        {
            tit+=strs[0][0][j];
            tit+=" ";
            anz++;
        }
        else
        {
            tit+=strs[0][0][j];
        }
    }
    if(tit.length()>anz)
    {
        textIns->setString(1,tit);
        textIns->setString(2,full);
        textIns->execute();
    }
    getId->setString(1,tit);
    sql::ResultSet *res = getId->executeQuery();
    while(res->next())
    {
        TID=res->getInt("TID");
    }
    delete res;

    delete textIns;
    delete getId;
    sql::PreparedStatement *dictGet = con->prepareStatement("SELECT ID,Wort FROM dict WHERE Wort=?");
    for(auto t = wcount.begin(); t!=wcount.end(); ++t)
    {
        dictGet->setString(1,t->first);
        sql::ResultSet *res = dictGet->executeQuery();
        while(res->next())
        {
            globDictI[res->getInt("ID")]=res->getString("Wort");
            globDictS[res->getString("Wort")]=res->getInt("ID");
        }
        delete res;
    }
    delete dictGet;
    return 0;
}

int WordCount::procData()
{
    for(auto t = wcount.begin(); t!=wcount.end(); ++t)
    {
        if(locworte.find(t->first)==locworte.end() && t->first.length()>0)
        {
            locworte[t->first]=Wort(t->first,wcount[t->first],globDictS[t->first]);
        }
    }
    for(unsigned int l = 0; l<strs.size(); l++)
    {
        if(l%50==0)
            cout<<l<<endl;

        for(unsigned int k = 0; k<strs[l].size(); k++)
        {
            locworte[strs[l][k][0]].flags["satzanf"]++;
            locworte[strs[l][k][strs[l][k].size()-1]].flags["satzend"]++;
            for(unsigned int i = 0; i<strs[l][k].size(); i++)
            {
                for(unsigned int j = 0; j<strs[l][k].size(); j++)
                {
                    if(i!=j)
                    {
                        locworte[strs[l][k][i]].oftInSatz[globDictS[strs[l][k][j]]]++;
                    }
                }
                if(i!=0&&i!=strs[l][k].size()-1)
                {
                    locworte[strs[l][k][i]].oftInDirV[globDictS[strs[l][k][i-1]]]++;
                    locworte[strs[l][k][i]].oftInDirH[globDictS[strs[l][k][i+1]]]++;
                }
                if(i==0&&strs[l][k].size()>1)
                    locworte[strs[l][k][i]].oftInDirH[globDictS[strs[l][k][i+1]]]++;
                if(i==strs[l][k].size()-1&&strs[l][k].size()>1)
                    locworte[strs[l][k][i]].oftInDirV[globDictS[strs[l][k][i-1]]]++;

                locworte[strs[l][k][i]].oftInText[TID]++;
            }

        }
    }
    return 0;
}
int WordCount::storeData()
{
    unsigned int counter=0;
    for(auto i = locworte.begin(); i!=locworte.end(); ++i)
    {
        counter++;
        cout<<endl<<counter<<"/"<<locworte.size()<<endl;
        sql::PreparedStatement *pstmt = con->prepareStatement("INSERT INTO `oiS` VALUES(?,?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        sql::PreparedStatement *pstmt2 = con->prepareStatement("INSERT INTO `oiS2` VALUES(?,?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        for(auto t = ((i->second).oftInSatz).begin(); t!=(i->second).oftInSatz.end(); ++t)
        {
            if((i->second).WID>t->first)
            {
                pstmt->setInt(1,(i->second).WID);
                pstmt->setInt(2,t->first);
                pstmt->setInt(3,t->second);
                pstmt->execute();
                pstmt2->setInt(2,(i->second).WID);
                pstmt2->setInt(1,t->first);
                pstmt2->setInt(3,t->second);
                pstmt2->execute();
            }
            else
            {
                pstmt->setInt(2,(i->second).WID);
                pstmt->setInt(1,t->first);
                pstmt->setInt(3,t->second);
                pstmt->execute();
                pstmt2->setInt(1,(i->second).WID);
                pstmt2->setInt(2,t->first);
                pstmt2->setInt(3,t->second);
                pstmt2->execute();
            }

        }
        delete pstmt;
        delete pstmt2;
        cout<<"oiS"<<endl;
        pstmt = con->prepareStatement("INSERT INTO `oiDV` VALUES(?,?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        pstmt2 = con->prepareStatement("INSERT INTO `oiDV2` VALUES(?,?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        for(auto t = ((i->second).oftInDirV).begin(); t!=(i->second).oftInDirV.end(); ++t)
        {
            if((i->second).WID>t->first)
            {
                pstmt->setInt(1,(i->second).WID);
                pstmt->setInt(2,t->first);
                pstmt->setInt(3,t->second);
                pstmt->execute();
                pstmt2->setInt(2,(i->second).WID);
                pstmt2->setInt(1,t->first);
                pstmt2->setInt(3,t->second);
                pstmt2->execute();
            }
            else
            {
                pstmt->setInt(2,(i->second).WID);
                pstmt->setInt(1,t->first);
                pstmt->setInt(3,t->second);
                pstmt->execute();
                pstmt2->setInt(1,(i->second).WID);
                pstmt2->setInt(2,t->first);
                pstmt2->setInt(3,t->second);
                pstmt2->execute();
            }
        }
        delete pstmt;
        delete pstmt2;
        cout<<"oiDV"<<endl;
        pstmt = con->prepareStatement("INSERT INTO `oiDH` VALUES(?,?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        pstmt2 = con->prepareStatement("INSERT INTO `oiDH2` VALUES(?,?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        for(auto t = ((i->second).oftInDirH).begin(); t!=(i->second).oftInDirH.end(); ++t)
        {
            if((i->second).WID>t->first)
            {
                pstmt->setInt(1,(i->second).WID);
                pstmt->setInt(2,t->first);
                pstmt->setInt(3,t->second);
                pstmt->execute();
                pstmt2->setInt(2,(i->second).WID);
                pstmt2->setInt(1,t->first);
                pstmt2->setInt(3,t->second);
                pstmt2->execute();
            }
            else
            {
                pstmt->setInt(2,(i->second).WID);
                pstmt->setInt(1,t->first);
                pstmt->setInt(3,t->second);
                pstmt->execute();
                pstmt2->setInt(1,(i->second).WID);
                pstmt2->setInt(2,t->first);
                pstmt2->setInt(3,t->second);
                pstmt2->execute();
            }
        }
        delete pstmt;
        delete pstmt2;
        cout<<"oiDH"<<endl;
        pstmt = con->prepareStatement("INSERT INTO oiT VALUES(?,?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        for(auto t = ((i->second).oftInText).begin(); t!=(i->second).oftInText.end(); ++t)
        {
            pstmt->setInt(1,(i->second).WID);
            pstmt->setInt(2,t->first);
            pstmt->setInt(3,t->second);
            pstmt->execute();
        }
        delete pstmt;
        cout<<"oiT"<<endl;
        pstmt = con->prepareStatement("INSERT INTO worte VALUES(?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        pstmt->setInt(1,(i->second).WID);
        pstmt->setInt(2,(i->second).c);
        pstmt->execute();
        delete pstmt;
        cout<<"worte"<<endl;
        pstmt = con->prepareStatement("INSERT INTO flags VALUES(?,?,?) ON DUPLICATE KEY UPDATE anz=anz+VALUES(anz);");
        for(auto t = ((i->second).flags).begin(); t!=(i->second).flags.end(); ++t)
        {
            pstmt->setInt(1,(i->second).WID);
            pstmt->setString(2,t->first);
            pstmt->setInt(3,t->second);
            pstmt->execute();
        }
        delete pstmt;
        cout<<"flags"<<endl;
        pstmt = con->prepareStatement("INSERT INTO attr VALUES(?,?,?) ON DUPLICATE KEY UPDATE val=VALUES(val);");
        for(auto t = ((i->second).attr).begin(); t!=(i->second).attr.end(); ++t)
        {
            pstmt->setInt(1,(i->second).WID);
            pstmt->setString(2,t->first);
            pstmt->setInt(3,t->second);
            pstmt->execute();
        }
        delete pstmt;
        cout<<"attr"<<endl;
        pstmt = con->prepareStatement("INSERT IGNORE INTO glw VALUES(?,?);");
        for(auto t = ((i->second).glW).begin(); t!=(i->second).glW.end(); ++t)
        {
            if(t->second==true)
            {
                pstmt->setInt(1,(i->second).WID);
                pstmt->setInt(2,t->first);
                pstmt->execute();
            }
        }
        delete pstmt;
        cout<<"glw"<<endl;
    }
    return 0;
}
