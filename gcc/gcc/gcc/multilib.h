#if 1
static char *multilib_raw[] = {
"apcs26/soft/arch2/unixlib msoft-float !mlibscl !mapcs-32 !archv3 !archv4;",
"apcs26/hard/arch2/unixlib !msoft-float !mlibscl !mapcs-32 !archv3 !archv4;",
"apcs26/soft/arch3/unixlib msoft-float !mlibscl !mapcs-32 archv3 !archv4;",
"apcs26/hard/arch3/unixlib !msoft-float !mlibscl !mapcs-32 archv3 !archv4;",
"apcs26/soft/arch4/unixlib msoft-float !mlibscl !mapcs-32 !archv3 archv4;",
"apcs26/hard/arch4/unixlib !msoft-float !mlibscl !mapcs-32 !archv3 archv4;",

"apcs32/soft/arch2/unixlib msoft-float !mlibscl mapcs-32 !archv3 !archv4;",
"apcs32/hard/arch2/unixlib !msoft-float !mlibscl mapcs-32 !archv3 !archv4;",
"apcs32/soft/arch3/unixlib msoft-float !mlibscl mapcs-32 archv3 !archv4;",
"apcs32/hard/arch3/unixlib !msoft-float !mlibscl mapcs-32 archv3 !archv4;",
"apcs32/soft/arch4/unixlib msoft-float !mlibscl mapcs-32 !archv3 archv4;",
"apcs32/hard/arch4/unixlib !msoft-float !mlibscl mapcs-32 !archv3 archv4;",

"apcs26/hard/arch2/libscl mlibscl !mapcs-32 !archv3 !archv4;",
"apcs26/hard/arch3/libscl mlibscl !mapcs-32 archv3 !archv4;",
"apcs26/hard/arch4/libscl mlibscl !mapcs-32 !archv3 archv4;",

"apcs32/hard/arch2/libscl mlibscl mapcs-32 !archv3 !archv4;",
"apcs32/hard/arch3/libscl mlibscl mapcs-32 archv3 !archv4;",
"apcs32/hard/arch4/libscl mlibscl mapcs-32 !archv3 archv4;",
NULL
};

static char *multilib_matches_raw[] = {
"msoft-float msoft-float;",
"mlibscl mlibscl;",
"mcpu=arm6 archv3;",
"mcpu=arm600 archv3;",
"mcpu=arm610 archv3;",
"mcpu=arm7 archv3;",
"mcpu=arm700 archv3;",
"mcpu=arm710 archv3;",
"mcpu=arm7500 archv3;",
"mcpu=arm7500fe archv3;",
"mcpu=strongarm archv4;",
"mcpu=strongarm110 archv4;",
"mcpu=strongarm1100 archv4;",
NULL
};

static char *multilib_extra = "";
#else
static char *multilib_raw[] = {
". ;",
NULL
};

static char *multilib_matches_raw[] = {
NULL
};

static char *multilib_extra = "";
#endif
