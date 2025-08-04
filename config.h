/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 0;//3       /* horiz inner gap between windows */
static const unsigned int gappiv    = 0;//3       /* vert inner gap between windows */
static const unsigned int gappoh    = 0;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 0;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
//                                          text font                           emoji font
static const char *fonts[]          = { "MesloLGM Nerd Font Mono:size=12", 
                                          "Noto Color Emoji:pixelsize=14:antialias=true:autohint=true"};
static const char dmenufont[]       =   "MesloLGM Nerd Font Mono:size=12";
static const int usealtbar          = 0;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *altbarcmd        = "$HOME/Documents/Source/Suckless-Software/dwm/bar.sh"; /* Alternate bar launch command */
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title                tags mask  isfloating  isterminal   noswallow    monitor   border width */
	{ "st",      NULL,     NULL,                    0,          0,          1,           0,        -1,        -1 },
	{ NULL,      NULL,     "st",                    0,          0,          1,           0,        -1,        -1},
	{ "St",      NULL,     NULL,                    0,          0,          1,           0,        -1,        -1 },
	{ "st-256color", NULL, NULL,                    0,          0,          1,           0,        -1,        -1 },
	{ "Alacritty", NULL, NULL,                          0,          0,          1,           0,        -1,        -1 },
	{ NULL, "Alacritty", NULL,                          0,          0,          1,           0,        -1,        -1 },
	{ "kitty", NULL, NULL,                          0,          0,          1,           0,        -1,        -1 },
	{ NULL, "kitty", NULL,                          0,          0,          1,           0,        -1,        -1 },
	{ "nautilus", NULL,    NULL,                    0,          0,          0,           0,       -1,        -1},
	{ "org.gnome.Nautilus", NULL, NULL,             0,          0,          0,           0,       -1,        -1},
	{ "Angry IP Scanner",   NULL,    NULL,          0,          0,          0,           0,       -1,        -1},
	{ "xclock",   NULL,    NULL,                    0,          0,          0,           0,       -1,        -1},
	{ NULL, "libreoffice",    NULL,                 0,          0,          0,           0,       -1,        -1},
	{ NULL, "libreoffice-calc",    NULL,                 0,          0,          0,           0,       -1,        -1},
	{ "Spotify", NULL,    NULL,                     1<<8,       0,          0,           0,       -1,        -1},
	{ "Gimp",    NULL,     NULL,                    0,          1,          0,           0,       -1,        -1 },
	{ "gnome-calendar", NULL, NULL,                 0,          0,          0,           0,       -1,        -1},
	{ "org.matplotlib.Matplotlib", NULL, NULL,      0,          0,          1,           1,       -1,        -1},
	{ "okular", NULL,     NULL,                     0,          0,          0,           0,        -1,        -1},
	//{ "Firefox", NULL,     NULL,                    0,          0,          0,           0,        -1,        -1},
	{ "org.mozilla.firefox", NULL,  NULL,           0,          0,          0,           0,        -1,        -1},
	//{ NULL, NULL,  "Mozilla Firefox",               0,          0,          0,           0,        -1,        -1},
	{ "bash",      NULL,     NULL,                  0,          0,          1,           0,        -1,        -1 },
	{ NULL,      NULL,     "Event Tester",          0,          0,          0,           1,        -1,        -1 }, /* xev */
	//{ "",                NULL,     NULL,            0,          0,          1,           1,        -1,        -1},
	/* class      instance    title       tags mask     isfloating   monitor   border width */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
//static const char *dmenucmd[] = { "dmenu_run",  NULL};
//static const char *termcmd[]  = { "st", NULL };
//static const char *termcmd[]  = { "kitty", NULL };
static const char *oldtermcmd[]  = { "kitty", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char * slockcmd[] = {"slock", NULL};

/* Control Media Players */
static const char *medplaypausecmd[] = { "playerctl", "play-pause", NULL };
static const char *mednextcmd[] = { "playerctl", "next", NULL };
static const char *medprevcmd[] = { "playerctl", "previous", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_t, spawn,          {.v = oldtermcmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("umonitor -a") },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,  togglescratch,      {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,      SHCMD("flameshot gui")},
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    //makes active tile master
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = slockcmd}},
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      spawn,         SHCMD("brightnessctl -d 'intel_backlight' set +5%")},
    { MODKEY|ShiftMask,             XK_d,      spawn,         SHCMD("brightnessctl -d 'intel_backlight' set 5%-")},
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    //{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("brightnessctl -d 'intel_backlight' set +5%")},
	//{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("brightnessctl -d 'intel_backlight' set +5%-")},
    { 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("brightnessctl  set +5%")},
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("brightnessctl  set +5%-")},
    { 0, XF86XK_AudioMute,		spawn,		SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 0%- && wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 0%+ && wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)") },

    /* Keybindings for Media play/pause/next/previous */
    {0, XF86XK_AudioPlay, spawn, {.v = medplaypausecmd}},
    { 0, XF86XK_AudioNext, spawn, {.v = mednextcmd } },
    { 0, XF86XK_AudioPrev, spawn, {.v = medprevcmd } },
    { ShiftMask, XF86XK_AudioMute, spawn, {.v = medplaypausecmd } },
    { ShiftMask, XF86XK_AudioRaiseVolume, spawn, {.v = mednextcmd } },
    { ShiftMask, XF86XK_AudioLowerVolume, spawn, {.v = medprevcmd } },
    { MODKEY, XF86XK_AudioMute, spawn, {.v = medplaypausecmd } },
    { MODKEY, XF86XK_AudioRaiseVolume, spawn, {.v = mednextcmd } },
    { MODKEY, XF86XK_AudioLowerVolume, spawn, {.v = medprevcmd } },


    // Vanity Gap Control
	{ MODKEY|XK_g,                  XK_h,      incrgaps,       {.i = +1 } },    //increase all gaps
	{ MODKEY|XK_g,                  XK_l,      incrgaps,       {.i = -1 } },    //decrease all gaps
	{ MODKEY|XK_g|ShiftMask,        XK_h,      incrogaps,      {.i = +1 } },    //increase outer gaps
	{ MODKEY|XK_g|ShiftMask,        XK_l,      incrogaps,      {.i = -1 } },    //decrease outer gaps
	{ MODKEY|XK_g|ControlMask,      XK_h,      incrigaps,      {.i = +1 } },    //increase inner gaps 
	{ MODKEY|XK_g|ControlMask,      XK_l,      incrigaps,      {.i = -1 } },    //decrease inner gaps 
	{ MODKEY|XK_g,                  XK_0,      togglegaps,     {0} },           //toggle gaps on and off
	{ MODKEY|XK_g|ShiftMask,        XK_0,      defaultgaps,    {0} },           //reset gaps back to default
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },    //increase inner horizontal gaps
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },    //decrease inner horizontal gaps
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },    //increase inner vertical gaps
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },    //decrease inner vertical gaps
	{ MODKEY|XK_g,                  XK_y,      incrohgaps,     {.i = +1 } },    //increase outer horizontal gaps
	{ MODKEY|XK_g,                  XK_o,      incrohgaps,     {.i = -1 } },    //decrease outer horizontal gaps
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },    //increase outer vertical gaps
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },    //decrease outer vertical gaps
};
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	//{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	//{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        ShiftMask,              Button1,        sigstatusbar,   {.i = 4} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

