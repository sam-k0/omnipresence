#ifndef RPCPREFERENCES_H_INCLUDED
#define RPCPREFERENCES_H_INCLUDED
#include <vector>
#include <fstream>
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

    rpcPref(){}

};

/**
*  @brief Initializes Vector and presences
*/
vector<rpcPref*>* initPresences()
{
    // Holds all preferences
    vector<rpcPref*>* prefVec = new vector<rpcPref*>;

    // Create a Preference
    rpcPref* pref = new rpcPref("E:\\Code__Blocks\\CodeBlocks\\codeblocks.exe","Editing Code","576668163350200321", 1, "logo160");
    rpcPref* pref2 = new rpcPref("E:\\Notepad++\\notepad++.exe","Notepad++","662720961979744297", 2, "bhlogo_discord_large");
    rpcPref* pref3 = new rpcPref("C:\\Users\\kooba\\AppData\\Roaming\\GameMaker-Studio\\5piceIDE.exe","Using GMS 1.4","821056056003919924", 3, "electron");

    // Add to vector
    prefVec->push_back(pref);
    prefVec->push_back(pref2);
    prefVec->push_back(pref3);
    return prefVec;
}

void loadPresences(vector<rpcPref*>* pvec, string filepath)
{
    char buffer[500];
    ifstream source;
    source.open(filepath.c_str(), ios::in);

    if(!source)
    {
        return;
    }

    int currmode = 0;

    std::string path = "";
    std::string displayName = "";
    std::string appid = "";
    int priority = -99;
    std::string icon = "";

    rpcPref* cur;

    while(source.getline(buffer, 499))
    {
        if(source.eof())
        {
            source.close();
            return;
        }

        if(currmode == 0)
        {
            cur = new rpcPref();
            path = buffer;
        }
        else if(currmode == 1)
        {
             displayName = buffer;
        }
        else if(currmode == 2)
        {
            appid = buffer;
        }
        else if(currmode == 3)
        {
             std::string priostr = buffer;
                priority = std::stoi(priostr);
        }
        else if(currmode == 4)
        {
            icon = buffer;

            cur->path = path;
            cur->displayName = displayName;
            cur->appid = appid;
            cur->priority = priority;
            cur->icon = icon;

            pvec->push_back(cur);
        }

        currmode = currmode % 5;
    }

    cout << "Readptr: "<<currmode<<endl;
    if(currmode != 0)
    {
        delete cur;
    }
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

/**
*   @brief Sets the presence to a struct rpcPref
*/
void setPresenceWrapped(rpcPref* p)
{
    gmrpc_init(gmu::string_to_constcharptr(p->appid));

    const char* state = gmu::string_to_constcharptr(p->displayName);
    const char* details = "omnipresence";
    const char* largeImage = gmu::string_to_constcharptr(p->icon);

    gmrpc_setPresence(state, details, "", largeImage);
}

#endif // RPCPREFERENCES_H_INCLUDED
