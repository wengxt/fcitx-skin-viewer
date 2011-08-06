#ifndef MYLOADCONFIG_H
#define MYLOADCONFIG_H

#include <cstdio>


typedef char shortChar[16];
typedef char midChar[64];
typedef char longChar[256];

struct _Color
{
    int r;
    int g;
    int b;
};

struct _GeneralParseStruct
{
    midChar key;
    midChar value;
    
    /** 把key和value都写入“// blank” */
    _GeneralParseStruct();
};

struct _SkinInfo
{
    midChar Name;
    shortChar Version;
    midChar Author;
    longChar Desc;
};
struct _SkinFont
{
    int FontSize;
    int MenuFontSize;
    _Color TipColor;
    _Color InputColor;
    _Color IndexColor;
    _Color FirstCandColor;
    _Color UserPhraseColor;
    _Color CodeColor;
    _Color OtherColor;
    _Color ActiveMenuColor;
    _Color InactiveMenuColor;
};
struct _SkinMainBar
{
    midChar BackImg;
    midChar Logo;
    midChar Eng;
    midChar Active;
    int MarginLeft;
    int MarginRight;
    int MarginTop;
    int MarginBottom;
};
struct _SkinInputBar
{
    midChar BackImg;
    int MarginLeft;
    int MarginRight;
    int MarginTop;
    int MarginBottom;
    _Color CursorColor;
    int InputPos;
    int OutputPos;
    midChar BackArrow;
    midChar FowardArrow;
    int BackArrowX;
    int BackArrowY;
    int ForwardArrowX;
    int ForwardArrowY;
};
struct _SkinTrayIcon
{
    midChar Active;
    midChar Inactive;
};
struct _SkinMenu
{
    midChar BackImg;
    int MarginLeft;
    int MarginRight;
    int MarginTop;
    int MarginBottom;
    _Color ActiveColor;
    _Color LineColor;
};
struct _SkinKeyboard
{
    midChar BackImg;
    _Color KeyColor;
};


class LoadConfig
{
public:
    _SkinInfo SkinInfo;
    _SkinFont SkinFont;
    _SkinMainBar SkinMainBar;
    _SkinInputBar SkinInputBar;
    _SkinTrayIcon SkinTrayIcon;
    _SkinMenu SkinMenu;
    _SkinKeyboard SkinKeyboard;

    FILE *configFile;
    _GeneralParseStruct *generalFile;

    LoadConfig();
    LoadConfig(char *configFilePath);
    
private:
    /** 信息读成key-value的形式，返回到tempSave数组里。 */
    void GeneralParse(FILE *configFile, _GeneralParseStruct generalFile[]); 
    
    /** 把tempSave里存的文件读到这个class的SkinInputBar结构中，下面的函数类似。 */
    void ParseSkinInputBar(_GeneralParseStruct generalFile[], int arrayLenth);
    
};


#endif