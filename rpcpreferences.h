#ifndef RPCPREFERENCES_H_INCLUDED
#define RPCPREFERENCES_H_INCLUDED
#include <vector>
#include <fstream>
#include <string>
#include "myconfig.h"
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
*  @brief Initializes Vector and presences with dummies !DO NOT USE IN DEPLOY!
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

string getCurrentDir() // Returns EXE directory
{
    /*
    https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
    */

    // get working directory
    char cCurrentPath[FILENAME_MAX]; // Buffer

    if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
    {
        //showErr("Could not get current directory", "Something went wrong.");
        exit(-1);
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    //printf("The current working directory is %s\n", cCurrentPath);
    char* s = cCurrentPath;
    return string(s);
}


vector<rpcPref*>* loadPresences()
{
    std::string target = getCurrentDir() + "\\pref.txt";
    cout << "Loading from "<< target<<endl;

    // File streaming
    std::ifstream myfile;
    myfile.open(target);

    std::string filebuffer; // the str to read to

    if(!myfile.is_open()) // Err catching
    {
        cout << "Could not find preferences file at:" << target << endl;
        system("pause");
        exit(-1);
    }


    int readpos = 0; // Checks what to read next
    // The buffers to read to
    std::string __path = "";
    std::string __status = "";
    std::string __appid = "";
    int __prio = 0;
    std::string __icon = "";
    // The prefVec
    vector<rpcPref*>* pVecLoaded = new vector<rpcPref*>;


    while(myfile.good())// Loop the contents
    {
        filebuffer == "";
        std::getline (myfile, filebuffer);
        cout << filebuffer<< endl;

        switch(readpos)
        {
        case 0:
            __path = filebuffer;
            readpos ++;
            break;

        case 1:
            __status = filebuffer;
            readpos ++;
            break;

        case 2:
            __appid = filebuffer;
            readpos ++;
            break;

        case 3:
            __prio = std::stoi(filebuffer);
            readpos ++;
            break;

        case 4:
            __icon = filebuffer;
            pVecLoaded->push_back(new rpcPref(__path, __status, __appid, __prio, __icon));
            readpos = 0;
            break;
        }
    }
    cout << "Reader ended with : " << readpos << endl;
    return pVecLoaded;
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
    const char* details = "Using Omnipresence";
    const char* largeImage = gmu::string_to_constcharptr(p->icon);

    gmrpc_setPresence(state, details, "", largeImage);
}

#endif // RPCPREFERENCES_H_INCLUDED
