#ifndef MYLOADCONFIG_H
#define MYLOADCONFIG_H

#include <cstdio>
#include <fcitx-config/fcitx-config.h>
#include <fcitx/ui.h>
#include <QString>


typedef struct _SkinInfo
{
    char *skinName;
    char *skinVersion;
    char *skinAuthor;
    char *skinDesc;
} SkinInfo;

typedef struct _SkinFont
{
    int fontSize;
    int menuFontSize;
    ConfigColor fontColor[7];
    ConfigColor menuFontColor[2];
} SkinFont;

typedef struct _SkinMenu
{
    char* backImg;
    int marginTop;
    int marginBottom;
    int marginLeft;
    int marginRight;
    ConfigColor activeColor;
    ConfigColor lineColor;
} SkinMenu;

/**
 * @brief The Main Window Skin description
 **/
typedef struct _SkinMainBar
{
    char* backImg;
    char* logo;
    char* eng;
    char* active;
    int marginTop;
    int marginBottom;
    int marginLeft;
    int marginRight;
    char *placement;
    UT_array skinPlacement;
} SkinMainBar;

typedef struct _SkinInputBar
{    
    char* backImg;
    ConfigColor cursorColor;
    int marginTop;
    int marginBottom;
    int marginLeft;
    int marginRight;
    char* backArrow;
    char* forwardArrow;
    int iBackArrowX;
    int iBackArrowY;
    int iForwardArrowX;
    int iForwardArrowY;
    int iInputPos;
    int iOutputPos;
} SkinInputBar;

typedef struct _SkinPlacement
{
    char name[MAX_STATUS_NAME + 1];
    int x;
    int y;
    UT_hash_handle hh;
} SkinPlacement;

/**
 * @brief Tray Icon Image
 **/
typedef struct _SkinTrayIcon
{
    /**
     * @brief Active Tray Icon Image
     **/
    char* active;
    
    /**
     * @brief Inactive Tray Icon Image
     **/
    char* inactive;
} SkinTrayIcon;

typedef struct _SkinKeyboard
{
    char* backImg;
    ConfigColor keyColor;
} SkinKeyboard;

/** 
* 配置文件结构,方便处理,结构固定
*/
typedef struct _FcitxSkin
{
    GenericConfig config;
    SkinInfo skinInfo;
    SkinFont skinFont;
    SkinMainBar skinMainBar;
    SkinInputBar skinInputBar;
    SkinTrayIcon skinTrayIcon;    
    SkinMenu skinMenu;
    SkinKeyboard skinKeyboard;
} FcitxSkin;

CONFIG_BINDING_DECLARE(FcitxSkin);

class MyLoadConfig {
public:
    MyLoadConfig(const QString& skinName);
    FcitxSkin skin;
    
private:
};

#endif
