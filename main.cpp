//#include <iostream>  // cout, cin, etc
//#include <stdio.h>   // printf
#include <windows.h> // Import to use psapi
#include "gms.h"     // GameMaker specific functionality

//#include <psapi.h>   // Depends on windows.h
#include <list>      // List include
#include <string>    // Strings



//using namespace std;
/**
* Sources: https://forums.codeguru.com/showthread.php?545543-WinAPI-How-to-enumerate-processes
*          https://stackoverflow.com/questions/1933113/c-windows-how-to-get-process-path-from-its-pid
*/

//
///* Register Callback */
//const int EVENT_OTHER_SOCIAL = 70;
//
//// defines function pointers for the DS map creation
//void (*CreateAsynEventWithDSMap)(int, int) = NULL;
//int (*CreateDsMap)(int _num, ...) = NULL;
//bool (*DsMapAddDouble)(int _index, const char* _pKey, double value) = NULL;
//bool (*DsMapAddString)(int _index, const char* _pKey, const char* pVal) = NULL;
//
///**
//* @brief Registers Callbacks for GameMaker API
//*/
//gmx void RegisterCallbacks(char* arg1, char* arg2, char* arg3, char* arg4) {
//    void (*CreateAsynEventWithDSMapPtr)(int, int) = (void (*)(int, int))(arg1);
//    int(*CreateDsMapPtr)(int _num, ...) = (int(*)(int _num, ...)) (arg2);
//    CreateAsynEventWithDSMap = CreateAsynEventWithDSMapPtr;
//    CreateDsMap = CreateDsMapPtr;
//
//    bool (*DsMapAddDoublePtr)(int _index, const char* _pKey, double value) = (bool(*)(int,  const char*, double))(arg3);
//    bool (*DsMapAddStringPtr)(int _index, const char* _pKey, const char* pVal) = (bool(*)(int, const char*, const char*))(arg4);
//
//    DsMapAddDouble = DsMapAddDoublePtr;
//    DsMapAddString = DsMapAddStringPtr;
//}
//
///**
//*   @short Struct to hold process information
//*/
//struct pInfo{
//    DWORD pid;
//    std::string path;
//    bool valid;
//};
//
//std::list<pInfo*> listPInfos; // The list to hold pInfos
//

/**
* @brief Writes all processIDs to Std List
*/
//
//DWORD PSAPI_EnumProcesses(std::list<DWORD>& listProcessIDs, DWORD dwMaxProcessCount)
//{
//    DWORD dwRet = NO_ERROR;             // Set default ret to false
//    listProcessIDs.clear();             // Clear list in case there is still something in there
//    DWORD *pProcessIds = new DWORD[dwMaxProcessCount]; // Array to write the PIDs into
//    DWORD cb = dwMaxProcessCount * sizeof(DWORD);      // The max bytesize
//    DWORD dwBytesReturned = 0;                         // The returned bytesize
//
//    // call PSAPI EnumProcesses
//    if (EnumProcesses(pProcessIds, cb, &dwBytesReturned))
//    {
//        // push returned process IDs into the output list
//        const int nSize = dwBytesReturned / sizeof(DWORD); // Calculate size of array
//        for(int nIndex = 0; nIndex < nSize; nIndex++)      // Loop addresses in array and add to list
//        {
//            listProcessIDs.push_back(pProcessIds[nIndex]);
//        }
//    }
//    else // Function call failed, return error
//    {
//        dwRet = ::GetLastError();
//    }
//    delete[]pProcessIds; // Free memory
//    return dwRet;
//}

/**
* @brief Returns struct holding the information
*/
//
//pInfo* getProcessInformation(DWORD pid)
//{
//    HANDLE procHandle = NULL; // Will hand the Handle to process we're gonna examine
//    TCHAR filename[MAX_PATH]; // Will hold the filepath to executable file
//
//    // Struct will hold returned Information
//    pInfo* processInformation = new pInfo();
//    processInformation->path = "";
//    processInformation->pid = NULL;
//    processInformation->valid = false;
//
//    // Call OpenProcess with given access rights to query information and retrieve a handle to this process
//    procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
//
//    if(procHandle == NULL) // Error opening procHandle
//    {
//        printf("Failed to open process handle to process %lu \n",pid);
//        return processInformation;
//    }
//
//    // Try resolving the filepath and write to filename from handle
//    if(GetModuleFileNameEx(procHandle, NULL, filename, MAX_PATH) == 0)
//    {
//        std::cout << "Failed to getModuleFilename" << std::endl;
//    }
//    else
//    {
//        std::cout << "Module filename is "<<filename<< std::endl;
//        // Convert strings
//        processInformation->path = filename;
//        processInformation->valid = true;
//        processInformation->pid = pid;
//    }
//    // Close handle to proc.
//    CloseHandle(procHandle);
//    // Return processInformation
//    return processInformation;
//}

/**
* @brief Returns List containing pInfos
* @returns 0 is no error
*/

//
//int getPInfosList(std::list<pInfo*> *pInfoList)
//{
//    /// Loop over all PIDs in list
//    std::list<DWORD> listProcessIDs;            // The list to write the PIDs into
//    const DWORD dwMaxProcessCount = 1024;       // The max. amount of processes to loop
//
//
//    // Call the EnumProcesses function to write into list
//    DWORD dwRet = PSAPI_EnumProcesses(listProcessIDs, dwMaxProcessCount); // Takes list and max. count
//
//    // Evaluate response
//    if(NO_ERROR == dwRet)
//    {
//        // No Error returned
//        const std::list<DWORD>::const_iterator end = listProcessIDs.end(); // Get end iterator
//        std::list<DWORD>::const_iterator iter = listProcessIDs.begin();    // Get current iterator
//        // Loop list
//        for( ; iter != end; ++iter)
//        {
//            DWORD dwProcessID = *iter;                                  // Iterator for DWORD list
//            pInfo* myPInfo = getProcessInformation(dwProcessID);        // The Pinfo struct for this Process
//
//            // Check if we really want to add it.
//            // we dont want to add empty strings and duplicates
//            bool doadd = true;
//
//            if(myPInfo->path.size() != 0)
//            {
//                for(auto const& i : *pInfoList)
//                {
//                    if(i->path == myPInfo->path)
//                    {
//                        doadd = false;
//                    }
//                }
//                if(doadd)
//                {
//                    pInfoList->push_back(myPInfo);                              // Add to pInfo List
//                }
//            }
//            else
//            {
//                delete myPInfo;
//            }
//
//            std::cout << "Process ID: " << dwProcessID << std::endl;
//        }
//    }
//    else
//    {
//        std::cout << "PSAPI_GetProcessesList failed. Error: " << dwRet << std::endl;
//        return -99;
//    }
//    return 0;
//    // Loop over all pInfos
//    for (auto const& i : listPInfos)
//    {
//        std::cout << i->path<<std::endl;
//    }
//}
//}
//
//int main()
//{
//    // clear list
//    if(!listPInfos.empty())
//    {
//        for(auto &item: listPInfos)
//        {
//            delete item;
//        }
//        listPInfos.clear();
//    }
//
//    int result = getPInfosList(&listPInfos); // Request infos
//
//    if(result == 0) // noerror
//    {
//        int counter = 0;
//        for (auto const& i : listPInfos)
//        {
//            std::cout << "------"<< std::endl;
//            std::cout << i->path << std::endl;
//            std::cout << i->path.size() << std::endl;
//            counter ++;
//        }
//
//    }
//    else
//    {
//        printf("Error.");
//    }
//
//    system("pause");
//    return 0;
//}




///! DLL API functions
/*gmx gmint gm_get_executablepaths()
{
    // clear list
    if(!listPInfos.empty())
    {
        for(auto &item: listPInfos)
        {
            delete item;
        }
        listPInfos.clear();
    }

    int mymap = CreateDsMap(0);   // Create Dsmap

    int result = getPInfosList(&listPInfos); // Request infos

    if(result == 0) // noerror
    {
        int counter = 0;
        for (auto const& i : listPInfos)
        {
            DsMapAddString(mymap, gmu::string_to_constcharptr(std::to_string(counter)), gmu::string_to_charptr(i->path));
            counter ++;
        }
        DsMapAddDouble(mymap, "length", (double)listPInfos.size());
        DsMapAddString(mymap, "event_type", "PROCLIST_READY");
        CreateAsynEventWithDSMap(mymap, EVENT_OTHER_SOCIAL);
        return gmtrue;
    }
    else
    {
        return gmfalse;
    }

}*/


gmx gmbool check()
{
    return gmtrue;
}


