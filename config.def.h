/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */ 
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[256]         = "DejaVu Sans Mono:pixelsize=14:antialias=true:autohint=true:style=bold";
static char font2[256]        = "Noto Color Emoji:pixelsize=12:antialias=true:autohint=true:style=bold";
static const char *fonts[]          = { font, font2 };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*                fg           bg           border   */
       [SchemeNorm]  = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]   = { selfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeTitle] = { selbgcolor,  normbgcolor, selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },		/* Default, slaves on right */
	{ "TTT",      bstack },		/* slaves on bottom vertical stack */
	
	{ "[@]",      spiral },		/* Fibonacci spiral */
	{ "[\\]",     dwindle },	/* Fibonacci dwindle */
	
	{ "|M|",      centeredmaster }, 	/* Master in center */
	{ ">M>",      centeredfloatingmaster }, /* Master floats in center */
	
	{ "[M]",      monocle },	/* stack all windows in center */
	{ "H[]",      deck },		/* slaves as monocole */

	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

#include <X11/XF86keysym.h>

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack, {.i = 0 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "font2",              STRING,  &font2 },
		{ "color7",             STRING,  &normfgcolor },
		{ "color0",             STRING,  &normbgcolor },
		{ "color8",             STRING,  &normbordercolor },
		{ "color7",             STRING,  &selfgcolor },
		{ "color2",             STRING,  &selbgcolor },
		{ "color7",             STRING,  &selbordercolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",         	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",       	FLOAT,   &mfact },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          SHCMD("dmenu_run") },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	
	/* dwm view controls */
	{ MODKEY|ShiftMask,             XK_q,      spawn,          SHCMD("sysact") },
	{ MODKEY,			XK_F5,   reload_xresources,{0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } }, /* increase number of masters */
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } }, /* decrease number of masters */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} }, /* bstack */
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[4]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[5]} }, /* centeredfloatingmaster */
	{ MODKEY,                       XK_d,      setlayout,      {.v = &layouts[6]} }, /* deck */
	{ MODKEY|ShiftMask,             XK_d,      setlayout,      {.v = &layouts[7]} }, /* monocle */
	{ MODKEY,                       XK_space,  zoom,           {0} }, /* swap master and first slave */
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	/* Media controls */
	{ 0,                            XK_Print,  spawn,          SHCMD("maim | xclip -selection clipboard -t image/png") },
	{ ShiftMask,                    XK_Print,  spawn,          SHCMD("maim -s | xclip -selection clipboard -t image/png") },
	{ 0, XF86XK_AudioMute,                     spawn,          SHCMD("pamixer -t; kill -35 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,              spawn,          SHCMD("pamixer -i 2; kill -35 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,              spawn,          SHCMD("pamixer -d 2; kill -35 $(pidof dwmblocks)") },
	{ 0, XF86XK_MonBrightnessUp,               spawn,          SHCMD("xbacklight -inc 15; kill -36 $(pidof dwmblocks)") },
	{ 0, XF86XK_MonBrightnessUp,               spawn,          SHCMD("xbacklight -inc 15; kill -36 $(pidof dwmblocks)") },
	/* Media controls for X131e */
/*	{ MODKEY,                       XK_F6,     spawn,          SHCMD("xbacklight -dec 15; kill -36 $(pidof dwmblocks)") },	
	{ MODKEY,                       XK_F7,     spawn,          SHCMD("xbacklight -inc 15; kill -36 $(pidof dwmblocks)") },	
	{ MODKEY,                       XK_F8,     spawn,          SHCMD("pamixer -t; kill -35 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_F9,     spawn,          SHCMD("pamixer -d 2; kill -35 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_F10,    spawn,          SHCMD("pamixer -i 2; kill -35 $(pidof dwmblocks)") },
*/
	/* vanity gaps hotkeys */
	{ MODKEY,                       XK_z,      incrgaps,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_z,      incrgaps,       {.i = -1 } },
	{ MODKEY,                       XK_a,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_a,      defaultgaps,    {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

