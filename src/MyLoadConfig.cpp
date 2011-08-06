#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "MyLoadConfig.h"

#define MID 64
#define LONG 128

using namespace std;

LoadConfig::LoadConfig() {
    generalFile=new _GeneralParseStruct [LONG];
    initialGeneralFile(generalFile, LONG);
};
LoadConfig::LoadConfig(char *configFilePath) {
    if ( (configFile=fopen(configFilePath, "r"))==NULL ) {
        perror("Open file error");
        exit(1);
    }
    generalFile=new _GeneralParseStruct [LONG];

    initialGeneralFile(generalFile, LONG);
    GeneralParse(configFile, generalFile);

}

void LoadConfig::LoadConfig::initialGeneralFile(_GeneralParseStruct generalFile[], int fileLenth)
{
    for (int i=0; i<LONG; i++) {
        strcpy(generalFile[i].key, "\0");
        strcpy(generalFile[i].value, "\0");
    }
}

void LoadConfig::GeneralParse(FILE *configFile, _GeneralParseStruct generalFile[])
{
    char tempSave[LONG];
    char *whereIsEqual;
    char *whereIsValue;

    int i=0;
    while ( fgets(tempSave, LONG, configFile)!=NULL ) {
        if ( tempSave[0]=='\n') {
            cout << "# blank line" << endl;
            continue;
        }
        if ( strchr(tempSave, '#')!=NULL ) {
            cout << "# commit" << endl;
            continue;
        }
        whereIsEqual=strchr(tempSave, '=');
        if ( whereIsEqual==NULL ) {
            cout << "Null!\n";
            sscanf(tempSave, " %s ", &generalFile[i].key);
        } else {
            // cout << whereIsEqual << endl;
            *whereIsEqual='\0';
            whereIsValue=whereIsEqual+sizeof(char);
            // cout << whereIsValue << endl;
            strcpy( generalFile[i].key, tempSave);
            strncpy( generalFile[i].value, whereIsValue, (strlen(whereIsValue)-1) );
        }
        cout << generalFile[i].key << "\t" << generalFile[i].value << endl;
        i++;
    }
}