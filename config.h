#include <X11/XF86keysym.h>

static int showsystray                   = 1;         /* 是否显示托盘栏 */
static const int newclientathead         = 0;         /* 定义新窗口在栈顶还是栈底 */
static const unsigned int borderpx       = 2;         /* 窗口边框大小 */
static const unsigned int systraypinning = 1;         /* 托盘跟随的显示器 0代表不指定显示器 */
static const unsigned int systrayspacing = 1;         /* 托盘间距 */
static const unsigned int systrayspadding = 5;        /* 托盘和状态栏的间隙 */
static int gappi                         = 2;        /* 窗口与窗口 缝隙大小 */
static int gappo                         = 8;        /* 窗口与边缘 缝隙大小 */
static const int _gappo                  = 8;        /* 窗口与窗口 缝隙大小 不可变 用于恢复时的默认值 */
static const int _gappi                  = 8;        /* 窗口与边缘 缝隙大小 不可变 用于恢复时的默认值 */
static const int vertpad                 = 5;         /* vertical padding of bar */
static const int sidepad                 = 5;         /* horizontal padding of bar */
static const int overviewgappi           = 24;        /* overview时 窗口与边缘 缝隙大小 */
static const int overviewgappo           = 50;        /* overview时 窗口与窗口 缝隙大小 */
static const int showbar                 = 1;         /* 是否显示状态栏 */
static const int topbar                  = 1;         /* 指定状态栏位置 0底部 1顶部 */
static const float mfact                 = 0.6;       /* 主工作区 大小比例 */
static const int   nmaster               = 1;         /* 主工作区 窗口数量 */
static const unsigned int snap           = 10;        /* 边缘依附宽度 */
static const unsigned int baralpha       = 0xc0;      /* 状态栏透明度 */
static const unsigned int borderalpha    = 0xdd;      /* 边框透明度 */
static const char *fonts[]               = { "SauceCodePro Nerd Font:size=16", "monospace:size=15" };
static const char *colors[][3]           = {          /* 颜色设置 ColFg, ColBg, ColBorder */ 
    [SchemeNorm] = { "#bbbbbb", "#333333", "#444444" },
    [SchemeSel] = { "#ffffff", "#37474F", "#42A5F5" },
    [SchemeSelGlobal] = { "#ffffff", "#37474F", "#FFC0CB" },
    [SchemeHid] = { "#dddddd", NULL, NULL },
    [SchemeSystray] = { NULL, "#7799AA", NULL },
    [SchemeUnderline] = { "#7799AA", NULL, NULL }, 
    [SchemeNormTag] = { "#bbbbbb", "#333333", NULL },
    [SchemeSelTag] = { "#eeeeee", "#333333", NULL },
    [SchemeBarEmpty] = { NULL, "#111111", NULL },
};
static const unsigned int alphas[][3]    = {          /* 透明度设置 ColFg, ColBg, ColBorder */ 
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSel] = { OPAQUE, baralpha, borderalpha },
    [SchemeSelGlobal] = { OPAQUE, baralpha, borderalpha },
    [SchemeNormTag] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSelTag] = { OPAQUE, baralpha, borderalpha },
    [SchemeBarEmpty] = { NULL, 0xa0a, NULL },
    [SchemeStatusText] = { OPAQUE, 0x88, NULL },
};

/* 自定义脚本位置 */
static const char *autostartscript = "~/scripts/autostart.sh";
static const char *statusbarscript = "$DWM/statusbar/statusbar.sh";

/* 自定义 scratchpad instance */
static const char scratchpadname[] = "scratchpad";

/* 自定义tag名称 */
/* 自定义特定实例的显示状态 */
//            ﮸  ﭮ 切
//     对应的tag序号以及快捷键:   0:1  1:2  2:3  3:4  4:5  5:9  6:c  7:m  8:0  9:w 10:l
//     "", "", ""
static const char *tags[] = { "", "", "", "", "", "","","","","","","ﬄ"};
static const char *tagsel[][2] = {
	{ "#ffffff", "#ff0000" },
	{ "#ffffff", "#ff7f00" },
	{ "#000000", "#ffff00" },
	{ "#000000", "#00ff00" },
	{ "#ffffff", "#0000ff" },
	{ "#ffffff", "#4b0082" },
	{ "#ffffff", "#9400d3" },
	{ "#000000", "#ffffff" },
	{ "#ffffff", "#000000" },
	{ "#ffffff", "#000000" },
	{ "#ffffff", "#000000" },
	{ "#ffffff", "#000000" },
};
static const unsigned int tagalpha[] = { OPAQUE, baralpha };

static const Rule rules[] = {
    /* class                 instance              title             tags mask     isfloating  isglobal    isnoborder monitor */
    // { NULL,                 "wechat.exe",          NULL,             1 << 9,       0,          0,          0,        -1 },
    // { NULL,                 "wxwork.exe",          NULL,             1 << 10,      0,          0,          0,        -1 },
    {"obs",                  NULL,                 NULL,             1 << 8,       0,          0,          0,        -1 },
    {"chrome",               NULL,                 NULL,             1 << 9,       0,          0,          0,        -1 },
    {"Chromium",             NULL,                 NULL,             1 << 9,       0,          0,          0,        -1 },
    {"music",                NULL,                 NULL,             1 << 10,      1,          0,          1,        -1 },
    { NULL,                 "qq",                  NULL,             1 << 11,       0,          0,          1,        -1 },
    { NULL,                  NULL,                "broken",          0,            1,          0,          0,        -1 },
    { "图片查看",           "图片查看",           "图片查看",        0,            1,          0,          0,        -1 },
    { "图片预览",           "图片预览",           "图片预览",        0,            1,          0,          0,        -1 },
    { NULL,                  NULL,                "crx_",            0,            1,          0,          0,        -1 },
    {"flameshot",            NULL,                 NULL,             0,            1,          0,          0,        -1 },
    {"wemeetapp",            NULL,                 NULL,             TAGMASK,      1,          0,          0,        -1 }, // 腾讯会议在切换tag时有诡异bug导致退出 变成global来规避该问题
    {"peek",                 NULL,                 NULL,             TAGMASK,      1,          0,          0,        -1 }, 
    {"float",                NULL,                 NULL,             0,            1,          0,          0,        -1 }, // 特殊class client默认浮动
    {"noborder",             NULL,                 NULL,             0,            1,          0,          1,        -1 }, // 特殊class client默认无边框
    {"global",               NULL,                 NULL,             TAGMASK,      1,          1,          0,        -1 }, // 特殊class client全局于所有tag
};
static const char *overviewtag = "OVERVIEW";
static const Layout overviewlayout = { "舘",  overview };

/* 自定义布局 */
static const Layout layouts[] = {
    { "﬿",  tile },         /* 主次栈 */
    { "﩯",  magicgrid },    /* 网格 */
    { "",  monocle}        /* 叠加模式 */
};

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG, cmd) \
    { MODKEY,              KEY, view,       {.ui = 1 << TAG, .v = cmd} }, \
    { MODKEY|ShiftMask,    KEY, tag,        {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,  KEY, toggleview, {.ui = 1 << TAG} }, \

static Key keys[] = {
    /* modifier            key              function          argument */
    { MODKEY,              XK_equal,        togglesystray,    {0} },                     /* super +            |  切换 托盘栏显示状态 */
    { MODKEY,              XK_Tab,          view,             {0}},                      /* super tab          |  切换上一次使用的tag*/
    { MODKEY,              XK_j,            focusstack,       {.i = +1} },               /* super j            |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_k,            focusstack,       {.i = -1} },               /* super k            |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_Up,           focusstack,       {.i = -1} },               /* super up           |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_Down,         focusstack,       {.i = +1} },               /* super down         |  本tag内切换聚焦窗口 */

    { MODKEY,              XK_Left,         viewtoleft,       {0} },                     /* super left         |  聚焦到左边的tag */
    { MODKEY,              XK_Right,        viewtoright,      {0} },                     /* super right        |  聚焦到右边的tag */
    { MODKEY|ShiftMask,    XK_Left,         tagtoleft,        {0} },                     /* super shift left   |  将本窗口移动到左边tag */
    { MODKEY|ShiftMask,    XK_Right,        tagtoright,       {0} },                     /* super shift right  |  将本窗口移动到右边tag */
    { MODKEY,              XK_w,            toggleoverview,   {0} },                     /* super a            |  显示所有tag 或 跳转到聚焦窗口的tag */
    { MODKEY|ShiftMask,    XK_j,            rotatestack, {.i = -1}},                     /* super + shift + k                 |  顺时针旋转窗口 */
    { MODKEY|ShiftMask,    XK_k,            rotatestack, {.i = +1}},                     /* super + shift + k                 |  顺时针旋转窗口 */
    { MODKEY,              XK_h,            setmfact,         {.f = -0.05} },            /* super h            |  缩小主工作区 */
    { MODKEY,              XK_l,            setmfact,         {.f = +0.05} },            /* super l            |  放大主工作区 */

    { MODKEY,              XK_b,            hidewin,          {0} },                     /* super h            |  隐藏 窗口 */
    { MODKEY,              XK_f,            restorewin,       {0} },                     /* super shift h      |  取消隐藏 窗口 */

    { MODKEY|ShiftMask,    XK_Return,       zoom,             {0} },                     /* super shift enter  |  将当前聚焦窗口置为主窗口 */

    { MODKEY,              XK_t,            togglefloating,   {0} },                     /* super t            |  开启/关闭 聚焦目标的float模式 */
    { MODKEY|ShiftMask,    XK_t,            toggleallfloating,{0} },                     /* super shift t      |  开启/关闭 全部目标的float模式 */
    { MODKEY,              XK_a,            fullscreen,       {0} },                     /* super f            |  开启/关闭 全屏 */
    { MODKEY|ShiftMask,    XK_a,            togglebar,        {0} },                     /* super shift f      |  开启/关闭 状态栏 */
    { MODKEY,              XK_g,            toggleglobal,     {0} },                     /* super g            |  开启/关闭 全局 */
    { MODKEY,              XK_e,            incnmaster,       {.i = +1} },               /* super e            |  改变主工作区窗口数量 (1 2中切换) */

    { MODKEY,              XK_b,            focusmon,         {.i = +1} },               /* super b            |  光标移动到另一个显示器 */
    { MODKEY|ShiftMask,    XK_b,            tagmon,           {.i = +1} },               /* super shift b      |  将聚焦窗口移动到另一个显示器 */

    { MODKEY,              XK_q,            killclient,       {0} },                     /* super q            |  关闭窗口 */
    { MODKEY|ControlMask,  XK_q,            forcekillclient,  {0} },                     /* super ctrl q       |  强制关闭窗口(处理某些情况下无法销毁的窗口) */
    { MODKEY|ControlMask,  XK_F12,          quit,             {0} },                     /* super ctrl f12     |  退出dwm */

	{ MODKEY|ShiftMask,    XK_space,        selectlayout,     {.v = &layouts[1]} },      /* super shift space  |  切换到网格布局 */
	{ MODKEY,              XK_m,            selectlayout,     {.v = &layouts[2]} },      /* super shift space  |  切换到堆叠布局 */
	{ MODKEY,              XK_o,            showonlyorall,    {0} },                     /* super o            |  切换 只显示一个窗口 / 全部显示 */

    { MODKEY|ControlMask,  XK_equal,        setgap,           {.i = -6} },               /* super ctrl +       |  窗口增大 */
    { MODKEY|ControlMask,  XK_minus,        setgap,           {.i = +6} },               /* super ctrl -       |  窗口减小 */
    { MODKEY|ControlMask,  XK_space,        setgap,           {.i = 0} },                /* super ctrl space   |  窗口重置 */

    { MODKEY|ControlMask,  XK_Up,           movewin,          {.ui = UP} },              /* super ctrl up      |  移动窗口 */
    { MODKEY|ControlMask,  XK_Down,         movewin,          {.ui = DOWN} },            /* super ctrl down    |  移动窗口 */
    { MODKEY|ControlMask,  XK_Left,         movewin,          {.ui = LEFT} },            /* super ctrl left    |  移动窗口 */
    { MODKEY|ControlMask,  XK_Right,        movewin,          {.ui = RIGHT} },           /* super ctrl right   |  移动窗口 */

    { MODKEY|Mod1Mask,     XK_Up,           resizewin,        {.ui = V_REDUCE} },        /* super alt up       |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_Down,         resizewin,        {.ui = V_EXPAND} },        /* super alt down     |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_Left,         resizewin,        {.ui = H_REDUCE} },        /* super alt left     |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_Right,        resizewin,        {.ui = H_EXPAND} },        /* super alt right    |  调整窗口 */

    /* spawn + SHCMD 执行对应命令(已下部分建议完全自己重新定义) */
    { MODKEY,              XK_apostrophe,      togglescratch, SHCMD("st -t scratchpad -c float") },                      /* super s          | 打开scratch终端        */
    { MODKEY,              XK_Return,          spawn, SHCMD("st") },                                                     /* super enter      | 打开st终端             */
    { MODKEY,              XK_minus,           spawn, SHCMD("st -c global") },                                           /* super +          | 打开全局st终端          */
    { MODKEY,              XK_space,           spawn, SHCMD("st -c float") },                                            /* super space      | 打开浮动st终端          */
    { MODKEY,              XK_s,               spawn, SHCMD("~/scripts/call_rofi.sh run") },                             /* super d          | rofi: 执行run          */
    { MODKEY|ShiftMask,    XK_s,               spawn, SHCMD("~/scripts/call_rofi.sh drun") },                            /* super shift d    | rofi: 执行drun         */
    { MODKEY,              XK_d,               spawn, SHCMD("~/scripts/call_rofi.sh custom") },                          /* super p          | rofi: 执行自定义脚本    */
    { MODKEY|ShiftMask,    XK_d,               spawn, SHCMD("~/scripts/call_rofi.sh window") },                          /* super shift p    | rofi: 执行window       */
    { MODKEY,              XK_F1,              spawn, SHCMD("pcmanfm") },                                                /* super F1         | 文件管理器             */
    { MODKEY,              XK_k,               spawn, SHCMD("~/scripts/blurlock.sh") },                                  /* super k          | 锁定屏幕               */
    { MODKEY|ShiftMask,    XK_Up,              spawn, SHCMD("~/scripts/set_vol.sh up") },                                /* super shift up   | 音量加                 */
    { MODKEY|ShiftMask,    XK_Down,            spawn, SHCMD("~/scripts/set_vol.sh down") },                              /* super shift down | 音量减                 */
    { MODKEY|ControlMask,    XK_s,               spawn, SHCMD("flameshot gui -c -p ~/Pictures/screenshots") },             /* super control s    | 截图                   */
    { MODKEY|ShiftMask,    XK_k,               spawn, SHCMD("~/scripts/screenkey.sh") },                                 /* super shift k    | 打开键盘输入显示       */
    { MODKEY|ShiftMask,    XK_q,               spawn, SHCMD("kill -9 $(xprop | grep _NET_WM_PID | awk '{print $3}')") }, /* super shift q    | 选中某个窗口并强制kill */
    { ShiftMask|ControlMask, XK_c,             spawn, SHCMD("xclip -o | xclip -selection c") },                          /* super shift c    | 进阶复制               */

    /* super key : 跳转到对应tag (可附加一条命令 若目标目录无窗口，则执行该命令) */
    /* super shift key : 将聚焦窗口移动到对应tag */
    /* key tag cmd */
    TAGKEYS(XK_1, 0,  0)
    TAGKEYS(XK_2, 1,  0)
    TAGKEYS(XK_3, 2,  0)
    TAGKEYS(XK_4, 3,  0)
    TAGKEYS(XK_5, 4,  0)
    TAGKEYS(XK_6, 5,  0)
    TAGKEYS(XK_7, 6,  0)
    TAGKEYS(XK_8, 7,  0)
    TAGKEYS(XK_9, 8,  "simplescreenrecorder")
    TAGKEYS(XK_c, 9,  "google-chrome-stable")
    TAGKEYS(XK_p, 10,  "netease-cloud-music")
    TAGKEYS(XK_0, 11,  "linuxqq")
    // TAGKEYS(XK_w, 9,  "/opt/apps/com.qq.weixin.deepin/files/run.sh")
    // TAGKEYS(XK_l, 10, "/opt/apps/com.qq.weixin.work.deepin/files/run.sh")
};
static Button buttons[] = {
    /* click               event mask       button            function       argument  */
    /* 点击窗口标题栏操作 */
    { ClkWinTitle,         0,               Button1,          hideotherwins, {0} },                                   // 左键        |  点击标题     |  隐藏其他窗口仅保留该窗口
    { ClkWinTitle,         0,               Button3,          togglewin,     {0} },                                   // 右键        |  点击标题     |  切换窗口显示状态
    /* 点击窗口操作 */
    { ClkClientWin,        MODKEY,          Button1,          movemouse,     {0} },                                   // super+左键  |  拖拽窗口     |  拖拽窗口
    { ClkClientWin,        MODKEY,          Button3,          resizemouse,   {0} },                                   // super+右键  |  拖拽窗口     |  改变窗口大小
    /* 点击tag操作 */
    { ClkTagBar,           0,               Button1,          view,          {0} },                                   // 左键        |  点击tag      |  切换tag
	{ ClkTagBar,           0,               Button3,          toggleview,    {0} },                                   // 右键        |  点击tag      |  切换是否显示tag
    { ClkTagBar,           MODKEY,          Button1,          tag,           {0} },                                   // super+左键  |  点击tag      |  将窗口移动到对应tag
    { ClkTagBar,           0,               Button4,          viewtoleft,    {0} },                                   // 鼠标滚轮上  |  tag          |  向前切换tag
	{ ClkTagBar,           0,               Button5,          viewtoright,   {0} },                                   // 鼠标滚轮下  |  tag          |  向后切换tag
    /* 点击状态栏操作 */
    { ClkStatusText,       0,               Button1,          clickstatusbar,{0} },                                   // 左键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal L
    { ClkStatusText,       0,               Button2,          clickstatusbar,{0} },                                   // 中键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal M
    { ClkStatusText,       0,               Button3,          clickstatusbar,{0} },                                   // 右键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal R
    { ClkStatusText,       0,               Button4,          clickstatusbar,{0} },                                   // 鼠标滚轮上  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal U
    { ClkStatusText,       0,               Button5,          clickstatusbar,{0} },                                   // 鼠标滚轮下  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal D

    /* 点击桌面空白处 */
    // { ClkRootWin,          0,               Button1,          spawn, SHCMD("~/scripts/call_rofi.sh window") },        // 左键        |  桌面空白处   |  rofi 执行 window
    // { ClkRootWin,          0,               Button3,          spawn, SHCMD("~/scripts/call_rofi.sh drun") },          // 右键        |  桌面空白处   |  rofi 执行 drun
                                                                                                                      //
    /* 点击bar空白处 */
    { ClkBarEmpty,         0,               Button1,          spawn, SHCMD("~/scripts/call_rofi.sh window") },        // 左键        |  bar空白处    |  rofi 执行 window
    { ClkBarEmpty,         0,               Button3,          spawn, SHCMD("~/scripts/call_rofi.sh drun") },          // 右键        |  bar空白处    |  rofi 执行 drun
};
