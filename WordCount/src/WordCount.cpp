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
    string ful = text;

    vector<string> tete;
    boost::algorithm::split_regex(tete,ful,boost::regex("[[][[].*?[]][]]"));
    cout<<tete.size()<<endl;
    for(string fu : tete){
        vector<vector<string>> lstrs;
        cout<<fu.length()<<endl;

        boost::regex re1("[\?!;,\()\"\':]|(?<![0-9])[.]");
        string full = boost::regex_replace(fu, re1, " $& ");
        boost::regex re2("[ ]+");
        full = boost::regex_replace(full, re2, " ");
        boost::regex re3("([\[][\[])(.*?)([]][]])");
        full = boost::regex_replace(full, re3, "$&$2\n");


        vector<string> tem;
        boost::algorithm::split_regex(tem,full,boost::regex("(?<![0-9])[.]|[!?\n\r]"));
        for(unsigned int j = 0; j<tem.size(); j++)
        {
            vector<string> tem2;
            boost::algorithm::split(tem2,tem[j],is_any_of(" \n\r"));
            lstrs.push_back(tem2);
        }

        ///leere Wörter löschen
            for(unsigned int k = 0; k<lstrs.size(); k++)
            {
                for(unsigned int j = 0; j<lstrs[k].size(); j++)
                {
                    if(lstrs[k][j].length()<=0)
                    {
                        lstrs[k].erase(lstrs[k].begin()+j);
                        j--;
                    }
                }
                if(lstrs[k].size()==0)
                {
                    lstrs.erase(lstrs.begin()+k);
                    k--;
                }
            }

        ///Wörter zählen
            for(unsigned int j = 0; j<lstrs.size(); j++)
            {
                for(unsigned int k = 0; k<lstrs[j].size(); k++)
                {
                    wcount[lstrs[j][k]]++;
                }
            }
        cout<<"lstrs.size()"<<lstrs.size()<<endl;
        strs.push_back(lstrs);
    }
    cout<<"strs.size()"<<strs.size()<<endl;

    return 0;
}
/*
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
}*/
