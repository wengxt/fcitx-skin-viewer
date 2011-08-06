#include "MyLoadConfig.h"
#include <fcitx-config/xdg.h>
#include <fcitx-utils/log.h>
#include <libintl.h>
#include <limits.h>
#include "config.h"

#define _(x) gettext(x)

static void FilterPlacement(GenericConfig* config, ConfigGroup *group, ConfigOption *option, void* value, ConfigSync sync, void* arg);
static void ParsePlacement(UT_array* sps, char* placment);

static const UT_icd place_icd = {sizeof(SkinPlacement), NULL, NULL, NULL };

CONFIG_BINDING_BEGIN(FcitxSkin)
CONFIG_BINDING_REGISTER("SkinInfo","Name",skinInfo.skinName)
CONFIG_BINDING_REGISTER("SkinInfo","Version",skinInfo.skinVersion)
CONFIG_BINDING_REGISTER("SkinInfo","Author",skinInfo.skinAuthor)
CONFIG_BINDING_REGISTER("SkinInfo","Desc",skinInfo.skinDesc)
    
CONFIG_BINDING_REGISTER("SkinFont","FontSize",skinFont.fontSize)
CONFIG_BINDING_REGISTER("SkinFont","MenuFontSize",skinFont.menuFontSize)
CONFIG_BINDING_REGISTER("SkinFont","TipColor",skinFont.fontColor[MSG_TIPS])
CONFIG_BINDING_REGISTER("SkinFont","InputColor",skinFont.fontColor[MSG_INPUT])
CONFIG_BINDING_REGISTER("SkinFont","IndexColor",skinFont.fontColor[MSG_INDEX])
CONFIG_BINDING_REGISTER("SkinFont","UserPhraseColor",skinFont.fontColor[MSG_USERPHR])
CONFIG_BINDING_REGISTER("SkinFont","FirstCandColor",skinFont.fontColor[MSG_FIRSTCAND])
CONFIG_BINDING_REGISTER("SkinFont","CodeColor",skinFont.fontColor[MSG_CODE])
CONFIG_BINDING_REGISTER("SkinFont","OtherColor",skinFont.fontColor[MSG_OTHER])
CONFIG_BINDING_REGISTER("SkinFont","ActiveMenuColor",skinFont.menuFontColor[MENU_ACTIVE])
CONFIG_BINDING_REGISTER("SkinFont","InactiveMenuColor",skinFont.menuFontColor[MENU_INACTIVE])
    
CONFIG_BINDING_REGISTER("SkinMainBar","BackImg",skinMainBar.backImg)
CONFIG_BINDING_REGISTER("SkinMainBar","Logo",skinMainBar.logo)
CONFIG_BINDING_REGISTER("SkinMainBar","Eng",skinMainBar.eng)
CONFIG_BINDING_REGISTER("SkinMainBar","Active",skinMainBar.active)
CONFIG_BINDING_REGISTER("SkinMainBar","MarginLeft", skinMainBar.marginLeft)
CONFIG_BINDING_REGISTER("SkinMainBar","MarginRight", skinMainBar.marginRight)
CONFIG_BINDING_REGISTER("SkinMainBar","MarginTop", skinMainBar.marginTop)
CONFIG_BINDING_REGISTER("SkinMainBar","MarginBottom", skinMainBar.marginBottom)
CONFIG_BINDING_REGISTER_WITH_FILTER("SkinMainBar","Placement", skinMainBar.placement, FilterPlacement)

CONFIG_BINDING_REGISTER("SkinInputBar","BackImg",skinInputBar.backImg)
CONFIG_BINDING_REGISTER("SkinInputBar","MarginTop", skinInputBar.marginTop)
CONFIG_BINDING_REGISTER("SkinInputBar","MarginBottom", skinInputBar.marginBottom)
CONFIG_BINDING_REGISTER("SkinInputBar","MarginLeft", skinInputBar.marginLeft)
CONFIG_BINDING_REGISTER("SkinInputBar","MarginRight", skinInputBar.marginRight)
CONFIG_BINDING_REGISTER("SkinInputBar","CursorColor",skinInputBar.cursorColor)
CONFIG_BINDING_REGISTER("SkinInputBar","InputPos",skinInputBar.iInputPos)
CONFIG_BINDING_REGISTER("SkinInputBar","OutputPos",skinInputBar.iOutputPos)
CONFIG_BINDING_REGISTER("SkinInputBar","BackArrow",skinInputBar.backArrow)
CONFIG_BINDING_REGISTER("SkinInputBar","ForwardArrow",skinInputBar.forwardArrow)
CONFIG_BINDING_REGISTER("SkinInputBar","BackArrowX",skinInputBar.iBackArrowX)
CONFIG_BINDING_REGISTER("SkinInputBar","BackArrowY",skinInputBar.iBackArrowY)
CONFIG_BINDING_REGISTER("SkinInputBar","ForwardArrowX",skinInputBar.iForwardArrowX)
CONFIG_BINDING_REGISTER("SkinInputBar","ForwardArrowY",skinInputBar.iForwardArrowY)

CONFIG_BINDING_REGISTER("SkinTrayIcon","Active",skinTrayIcon.active)
CONFIG_BINDING_REGISTER("SkinTrayIcon","Inactive",skinTrayIcon.inactive)

CONFIG_BINDING_REGISTER("SkinMenu", "BackImg", skinMenu.backImg)
CONFIG_BINDING_REGISTER("SkinMenu", "MarginTop", skinMenu.marginTop)
CONFIG_BINDING_REGISTER("SkinMenu", "MarginBottom", skinMenu.marginBottom)
CONFIG_BINDING_REGISTER("SkinMenu", "MarginLeft", skinMenu.marginLeft)
CONFIG_BINDING_REGISTER("SkinMenu", "MarginRight", skinMenu.marginRight)
CONFIG_BINDING_REGISTER("SkinMenu", "ActiveColor", skinMenu.activeColor)
CONFIG_BINDING_REGISTER("SkinMenu", "LineColor", skinMenu.lineColor)

CONFIG_BINDING_REGISTER("SkinKeyboard", "BackImg", skinKeyboard.backImg)
CONFIG_BINDING_REGISTER("SkinKeyboard", "KeyColor", skinKeyboard.keyColor)

CONFIG_BINDING_END()

void FilterPlacement(GenericConfig* config, ConfigGroup *group, ConfigOption *option, void* value, ConfigSync sync, void* arg)
{
    FcitxSkin* sc = (FcitxSkin*) config;
    if (sync == Raw2Value)
    {        
        ParsePlacement(&sc->skinMainBar.skinPlacement, *(char**) value);
    }
}

void ParsePlacement(UT_array* sps, char* placment)
{
    UT_array* array = SplitString(placment, ';');
    char** str;
    utarray_clear(sps);
    for(str = (char**) utarray_front(array);
        str != NULL;
        str = (char**) utarray_next(array, str))
    {
        char* s = *str;
        char* p = strchr(s, ':');
        if (p == NULL)
            continue;
        if ((strchr(s, ':') - s) > MAX_STATUS_NAME)
            continue;
        
        int len = p - s;
        SkinPlacement sp;
        strncpy(sp.name, s, len);
        sp.name[len] = '\0';
        int ret = sscanf(p+1, "%d,%d", &sp.x, &sp.y);
        if (ret != 2)
            continue;
        utarray_push_back(sps, &sp);
    }
    
    utarray_free(array);
}

CONFIG_DESC_DEFINE(GetSkinDesc, "skin.desc")

int LoadSkinConfig(FcitxSkin* sc, const QString& skinType)
{
    FILE    *fp;
    char  buf[PATH_MAX]={0};
    boolean    isreload = false;
    int ret = 0;

    snprintf(buf, PATH_MAX, "%s/fcitx_skin.conf", skinType.toLatin1().data());
    buf[PATH_MAX-1] ='\0';
    size_t len;
    char ** path = GetXDGPath(&len, "XDG_CONFIG_HOME", ".config", PACKAGE "/skin" , DATADIR, PACKAGE "/skin" );

    fp = GetXDGFile(buf, path, "r", len, NULL);
    FreeXDGPath(path);

    if (fp)
    {
        ConfigFile *cfile;
        ConfigFileDesc* skinDesc = GetSkinDesc();
        if (sc->config.configFile == NULL)
        {
            cfile = ParseConfigFileFp(fp, skinDesc);
        }
        else
        {
            cfile = sc->config.configFile;
            cfile = ParseIniFp(fp, cfile);
        }
        if (!cfile)
        {
            fclose(fp);
            fp = NULL;
        }
        else
        {
            FcitxSkinConfigBind(sc, cfile, skinDesc);
            ConfigBindSync((GenericConfig*)sc);
        }
    }

    if (!fp)
        ret = 1;    // 如果安装目录里面也没有配置文件，那就只好告诉用户，无法运行了

    if (fp)
        fclose(fp);

    return ret;

}

MyLoadConfig::MyLoadConfig(const QString& skinName)
{
    memset(&this->skin, 0, sizeof(FcitxSkin));
    utarray_init(&skin.skinMainBar.skinPlacement, &place_icd);
    LoadSkinConfig(&this->skin, skinName);
}
