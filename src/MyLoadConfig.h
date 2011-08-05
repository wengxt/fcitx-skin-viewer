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
    midChar name;
    midChar key;
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
    midChar BacImg;
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
    int FowardArrowX;
    int FowardArrowY;
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

    LoadConfig();
    LoadConfig(char *configFilePath);

    void GeneralParse(char *configFileParse, tempSave[]); // 把信息读到GeneralParseStruct中，返回到tempSave数组里。
};


#endif