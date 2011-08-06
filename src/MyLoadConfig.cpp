#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "MyLoadConfig.h"

#define MID 64
#define LONG 128

using namespace std;

_GeneralParseStruct::_GeneralParseStruct()
{
    strcpy(key, "// blank");
    strcpy(value, "// blank");
}

LoadConfig::LoadConfig() {
    generalFile=new _GeneralParseStruct [LONG];
};

LoadConfig::LoadConfig(char *configFilePath) {
    if ( (configFile=fopen(configFilePath, "r"))==NULL ) {
        perror("Open file error");
        exit(1);
    }
    generalFile=new _GeneralParseStruct [LONG];

    GeneralParse(configFile, generalFile);
    ParseSkinInputBar(generalFile, LONG);

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
            (generalFile[i].value)[strlen(whereIsValue)-1]='\0';
        }
        cout << generalFile[i].key << "\t" << generalFile[i].value << endl;
        i++;
    }
    rewind(configFile);
}

void LoadConfig::ParseSkinInputBar(_GeneralParseStruct generalFile[], int arrayLenth)
{
    int i;
    for (i=0; i<arrayLenth; i++) {
        if ( strstr(generalFile[i].key, "[SkinInputBar]")!=NULL ) {
            break;
        }
    }
    
    i++;
    for ( ; ( (strchr(generalFile[i].key, '[')==NULL) && (strchr(generalFile[i].key, '/')==NULL) ); i++ ) {
        if ( strstr(generalFile[i].key, "BackImg")!=NULL ) {
            sscanf(generalFile[i].value, "%s", SkinInputBar.BackImg);
        }
        
        if ( strstr(generalFile[i].key, "MarginLeft")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.MarginLeft);
        }
        
        if ( strstr(generalFile[i].key, "MarginRight")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.MarginRight);
        }
        
        if ( strstr(generalFile[i].key, "MarginTop")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.MarginTop);
        }
        
        if ( strstr(generalFile[i].key, "MarginBottom")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.MarginBottom);
        }
        
        if ( strstr(generalFile[i].key, "CursorColor")!=NULL ){
            sscanf(generalFile[i].value, "%d %d %d",
                   &SkinInputBar.CursorColor.r,
                   &SkinInputBar.CursorColor.g,
                   &SkinInputBar.CursorColor.b
                  );
        }
        
        if ( strstr(generalFile[i].key, "InputPos")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.InputPos);
        }
        
        if ( strstr(generalFile[i].key, "OutputPos")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.OutputPos);
        }
        
        if ( strstr(generalFile[i].key, "BackArrow")!=NULL ){
            sscanf(generalFile[i].value, "%s", &SkinInputBar.BackArrow);
        }
        
        if ( strstr(generalFile[i].key, "ForwardArrow")!=NULL ){
            sscanf(generalFile[i].value, "%s", SkinInputBar.FowardArrow);
        }
        
        if ( strstr(generalFile[i].key, "BackArrowX")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.BackArrowX);
        }
        
        if ( strstr(generalFile[i].key, "BackArrowY")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.BackArrowY);
        }
        
        if ( strstr(generalFile[i].key, "ForwardArrowX")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.ForwardArrowX);
        }
        
        if ( strstr(generalFile[i].key, "ForwardArrowY")!=NULL ){
            sscanf(generalFile[i].value, "%d", &SkinInputBar.ForwardArrowY);
        }
    }
    
}