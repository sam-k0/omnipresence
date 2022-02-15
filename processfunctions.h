#ifndef PROCESSFUNCTIONS_H_INCLUDED
#define PROCESSFUNCTIONS_H_INCLUDED
#include <iostream>  // cout, cin, etc
#include <stdio.h>   // printf
#include <windows.h> // Import to use psapi
#include "gms.h"     // GameMaker specific functionality
#include <psapi.h>   // Depends on windows.h
#include <list>      // List include
#include <string>    // Strings
#include "myconfig.h"// types

/**
* @brief Writes all processIDs to Std List
*/
DWORD PSAPI_EnumProcesses(std::list<DWORD>& listProcessIDs, DWORD dwMaxProcessCount)
{
    DWORD dwRet = NO_ERROR;             // Set default ret to false
    listProcessIDs.clear();             // Clear list in case there is still something in there
    DWORD *pProcessIds = new DWORD[dwMaxProcessCount]; // Array to write the PIDs into
    DWORD cb = dwMaxProcessCount * sizeof(DWORD);      // The max bytesize
    DWORD dwBytesReturned = 0;                         // The returned bytesize

    // call PSAPI EnumProcesses
    if (EnumProcesses(pProcessIds, cb, &dwBytesReturned))
    {
        // push returned process IDs into the output list
        const int nSize = dwBytesReturned / sizeof(DWORD); // Calculate size of array
        for(int nIndex = 0; nIndex < nSize; nIndex++)      // Loop addresses in array and add to list
        {
            listProcessIDs.push_back(pProcessIds[nIndex]);
        }
    }
    else // Function call failed, return error
    {
        dwRet = ::GetLastError();
    }
    delete[]pProcessIds; // Free memory
    return dwRet;
}

/**
* @brief Returns struct holding the information
*/

pInfo* getProcessInformation(DWORD pid)
{
    HANDLE procHandle = NULL; // Will hand the Handle to process we're gonna examine
    TCHAR filename[MAX_PATH]; // Will hold the filepath to executable file

    // Struct will hold returned Information
    pInfo* processInformation = new pInfo();
    processInformation->path = "";
    processInformation->pid = NULL;
    processInformation->valid = false;

    // Call OpenProcess with given access rights to query information and retrieve a handle to this process
    procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if(procHandle == NULL) // Error opening procHandle
    {
        printf("Failed to open process handle to process %lu \n",pid);
        return processInformation;
    }

    // Try resolving the filepath and write to filename from handle
    if(GetModuleFileNameEx(procHandle, NULL, filename, MAX_PATH) == 0)
    {
        std::cout << "Failed to getModuleFilename" << std::endl;
    }
    else
    {
        std::cout << "Module filename is "<<filename<< std::endl;
        // Convert strings
        processInformation->path = filename;
        processInformation->valid = true;
        processInformation->pid = pid;
    }
    // Close handle to proc.
    CloseHandle(procHandle);
    // Return processInformation
    return processInformation;
}

/**
* @brief Returns List containing pInfos
* @returns 0 is no error
*/
int getPInfosList(std::list<pInfo*> *pInfoList)
{
    /// Loop over all PIDs in list
    std::list<DWORD> listProcessIDs;            // The list to write the PIDs into
    const DWORD dwMaxProcessCount = 1024;       // The max. amount of processes to loop


    // Call the EnumProcesses function to write into list
    DWORD dwRet = PSAPI_EnumProcesses(listProcessIDs, dwMaxProcessCount); // Takes list and max. count

    // Evaluate response
    if(NO_ERROR == dwRet)
    {
        // No Error returned
        const std::list<DWORD>::const_iterator end = listProcessIDs.end(); // Get end iterator
        std::list<DWORD>::const_iterator iter = listProcessIDs.begin();    // Get current iterator
        // Loop list
        for( ; iter != end; ++iter)
        {
            DWORD dwProcessID = *iter;                                  // Iterator for DWORD list
            pInfo* myPInfo = getProcessInformation(dwProcessID);        // The Pinfo struct for this Process

            // Check if we really want to add it.
            // we dont want to add empty strings and duplicates
            bool doadd = true;

            if(myPInfo->path.size() != 0)
            {
                for(auto const& i : *pInfoList)
                {
                    if(i->path == myPInfo->path)
                    {
                        doadd = false;
                    }
                }
                if(doadd)
                {
                    pInfoList->push_back(myPInfo);                              // Add to pInfo List
                }
            }
            else
            {
                delete myPInfo;
            }

            std::cout << "Process ID: " << dwProcessID << std::endl;
        }
    }
    else
    {
        std::cout << "PSAPI_GetProcessesList failed. Error: " << dwRet << std::endl;
        return -99;
    }
    return 0;
}

/**
* @brief Clears the passed list and destroys objects.
*/
void clearProcList(std::list<pInfo*> *listPInfos)
{
    if(!listPInfos->empty())
    {
        int counter = 0;
        for(auto &item: *listPInfos)
        {
            delete item;
            counter ++;
        }
        listPInfos->clear();
        cout << "Cleared " << counter << " Items." << endl;
    }
}

#endif // PROCESSFUNCTIONS_H_INCLUDED
