/**
* Sources: https://forums.codeguru.com/showthread.php?545543-WinAPI-How-to-enumerate-processes
*          https://stackoverflow.com/questions/1933113/c-windows-how-to-get-process-path-from-its-pid
*/
#include "processfunctions.h"
#include "myconfig.h"
#include "rpcinvoke.h"
#include "rpcpreferences.h"
#include <unistd.h>
#include <ctime>
using namespace std;

std::list<pInfo*> listPInfos; // The list to hold pInfos
std::vector<rpcPref*>* prefVec; // Holds all rpcPrefs
rpcPref* currSetPres = nullptr; // The current set presence



/**
* @brief Loops all processes and preferenced Presences to determine which presence to set next
*/
void checksetpresence(std::list<pInfo*>* listPInfos, std::vector<rpcPref*>* prefVec )
{
    cout << prefVec->size() << " preferences found."<<endl;

    int currPresPrio = -99;
    rpcPref* nextPref = nullptr;

    if(prefVec->empty() || listPInfos->empty())
    {
        cout << " Empty ."<< endl;
        return;
    }

    cout << "Start looping all preferences"<<endl;

    for (auto iter : *prefVec) // Loop all configured presences
    {
        cout << "prefloop! "<<endl;

        if(iter->priority > currPresPrio )   // Comp prios
        {
            // new one is higher
            for (auto const& procInfo : *listPInfos) // Loop all running processes
            {
                //cout << "Comparing: \n"<< procInfo->path << "\n to\n"<< iter->path<< endl;
                if(procInfo->path == iter->path)     // Same exe path
                {
                    cout << "Found presence " << iter->displayName<< endl;
                    nextPref = iter;             // Set currsetpres
                    currPresPrio = iter->priority;  // Set currprio
                    break;
                }
            }
        }
    }


    if(currSetPres == nullptr && nextPref != nullptr) // The first time a pref was found / app launch
    {
        cout << "Case 1: CurrSet is NONE! Next will be: "<< nextPref->displayName<<endl;
        // Set the presence
        currSetPres = nextPref;
        // Using wrapper
        setPresenceWrapped(currSetPres);
    }
    else if(nextPref == nullptr) // All pref procs closed
    {
        cout << "Case 2: Next Presence is NONE!"<< endl;
        //if(gmrpc_get_ready())
        //{
        gmrpc_exit();
        //}
        currSetPres = nullptr;
    }
    else if(currSetPres != nextPref) // A different than current
    {
        cout << "Case 3: CurrSet is "<< currSetPres->displayName <<" Next will be different: "<< nextPref->displayName<<endl;
        currSetPres = nextPref;

        if(currSetPres != nullptr)
        {
            gmrpc_exit();
            // Set the presence
            setPresenceWrapped(currSetPres);
        }
    }
    else if(currSetPres == nextPref)
    {
        cout << "Case 4: Did not find any other or more important presence."<<endl;
    }
}

int main()
{

    if(!checkConnectionWrapped()) // Check module connection
    {
        // Connection loss
        printf("Terminating Program.");
        system("pause");
        return -1;
    }

    /// Init Presences TODO: Add file reading to read presences
    prefVec = initPresences();

    while(true)
    {
        // clear list
        clearProcList(&listPInfos);

        // Get the pInfo structs to list
        if(getPInfosList(&listPInfos) == 0) // noerror
        {
            // Loop all preferences to check and set presence on rpc
            checksetpresence(&listPInfos, prefVec);
        }
        else
        {
            printf("Error.");
        }
        // Wait n seconds because rate limit of discord
        clock_t start = clock();
        while(clock() - start <= CLOCKS_PER_SEC*3);

    }
    // Cleanup
    destroyPresencesVec(prefVec);
    return 0;
}





