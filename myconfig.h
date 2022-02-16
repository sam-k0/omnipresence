#ifndef MYCONFIG_H_INCLUDED
#define MYCONFIG_H_INCLUDED
//#define __USE_DEBUG_MODE__
#define STRINGIFY(x) #x

typedef struct{
    DWORD pid;
    std::string path;
    bool valid;
} pInfo;

const double APPVERSION = 1.0;



#endif // MYCONFIG_H_INCLUDED
