#ifndef RPCPREFERENCES_H_INCLUDED
#define RPCPREFERENCES_H_INCLUDED
#include <vector>
#include <string>
using namespace std;


struct rpcPref
{
    std::string path;
    std::string displayName;
    std::string appid;
    int priority;
    std::string icon;

    rpcPref(std::string p, std::string d, std::string id, int pr, std::string ico)
    {
        path = p;
        displayName = d;
        appid = id;
        priority = pr;
        icon = ico;
    }

};

/**
*  @brief Initializes Vector and presences
*/
vector<rpcPref*>* initPresences()
{
    // Holds all preferences
    vector<rpcPref*>* prefVec = new vector<rpcPref*>;

    // Create a Preference
    rpcPref* pref = new rpcPref("E:\\Code__Blocks\\CodeBlocks\\codeblocks.exe","Code::Blocks","576668163350200321", 99, "logo160");
    rpcPref* pref2 = new rpcPref("E:\\Notepad++\\notepad++.exe","Notepad++","662720961979744297", 100, "bhlogo_discord_large");

    // Add to vector
    prefVec->push_back(pref);
    prefVec->push_back(pref2);

    return prefVec;
}

/**
* @brief Destroys the vector and objects inside
*/
void destroyPresencesVec(vector<rpcPref*>* v)
{
    for(int i = 0; i < (int)v->size(); i++)
    {
        delete v->at(i);
    }
    delete v;
}

#endif // RPCPREFERENCES_H_INCLUDED
