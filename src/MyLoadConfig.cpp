#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <MyLoadConfig.h>

#define MID 64
#define LONG 128

using namespace std;

LoadConfig::LoadConfig() {};
LoadConfig::LoadConfig(char *configFilePath) {
    if (configFile=fopen(configFilePath, "r")==NULL) {
        perror("Open file error");
        exit(1);
    }
    _GeneralParseStruct *generalFile=new _GeneralParseStruct[LONG];

    initialGeneralFile(generalFile);
    this->GeneralParse(configFile, generalFile);

}

void LoadConfig::LoadConfig::initialGeneralFile(_GeneralParseStruct generalFile[])
{
    for (int i=0; i<LONG; i++) {
        strcpy(generalFile[i].key, "\0\0");
        strcpy(generalFile[i].value, "\0\0");
    }
}

void LoadConfig::GeneralParse(FILE *configFile, _GeneralParseStruct generalFile[])
{
    char tempSave[LONG];

    int i=0;
    while ( fgets(tempSave, LONG, configFile)!=NULL ) {
        if ( strlen(tempSave)==0) {
            continue;
        }
        if ( strchr(tempSave, '#')!=NULL ) {
            continue;
        }
        scanf(" %s=%s", &generalFile[i].key, &generalFile[i].value);
        i++;
    }
}