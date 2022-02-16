#ifndef RPCINVOKE_H_INCLUDED
#define RPCINVOKE_H_INCLUDED
#include "gms.h"
#include <windows.h>
#include <direct.h>
#include <iostream>
// Discord GMRPC
// Init function pointers
typedef gmbool(*DISCORD_INIT)(const char* APPLICATION_ID);
typedef gmbool(*DISCORD_SETPRESENCE)(const char* state, const char* details, const char* smallImage, const char* largeImage);
typedef gmint(*DISCORD_CHECK)(gmint n);
typedef gmbool(*DISCORD_EXIT)();
typedef gmbool(*DISCORD_READY)();

static DISCORD_INIT gmrpc_init;              // Pointers to the functions in GMRPC dll
static DISCORD_SETPRESENCE gmrpc_setPresence;// Pointers to the functions in GMRPC dll
static DISCORD_CHECK gmrpc_checkConnection;  // Pointers to the functions in GMRPC dll
static DISCORD_EXIT gmrpc_exit;
static DISCORD_READY gmrpc_get_ready;


void loadGMRPC()
{
    // Get Directory
    char* _buf;
    _buf = _getcwd(NULL, 0);
    string __buf = string(_buf);
    __buf = __buf + "\\" + "GMRPC.dll";
    const char* GMRPCDLLPATH = __buf.c_str();

    // Load module
    HMODULE hModule = LoadLibrary(TEXT(GMRPCDLLPATH));

    if(!hModule)
    {
        cout << "Error. Terminating. Could not load gmrpc"<<endl;
        system("pause");
    }
    // Alloc Functions
    /*static DISCORD_INIT*/ gmrpc_init = (DISCORD_INIT) GetProcAddress(hModule, "gmrpc_init");

    /*static DISCORD_SETPRESENCE*/ gmrpc_setPresence = (DISCORD_SETPRESENCE) GetProcAddress(hModule, "gmrpc_setPresence");

   /* static DISCORD_CHECK*/ gmrpc_checkConnection = (DISCORD_CHECK) GetProcAddress(hModule, "gmrpc_checkConnection");

    /*static DISCORD_EXIT*/ gmrpc_exit = (DISCORD_EXIT) GetProcAddress(hModule, "gmrpc_exit");

    /*static DISCORD_READY*/ gmrpc_get_ready = (DISCORD_READY) GetProcAddress(hModule, "gmrpc_get_ready");
}



/**
* @brief Checks connection to gmrpc.
*/
bool checkConnectionWrapped()
{
    if(gmrpc_checkConnection(4.0) != 8)
    {
        std::cout << "GMRPC Connection Checksum does not match with expected. \n The module could not be loaded properly."<<std::endl;
        return false;
    }
    return true;
}

#endif // RPCINVOKE_H_INCLUDED
