/* Multilib support for ARM/RISC OS.   */

#if 1
static const char *const multilib_raw[] = {
  "apcs32/abs/unixlib !mlibscl mapcs-32 !mmodule;",
  "apcs32/rma/unixlib !mlibscl mapcs-32 mmodule;",

  "apcs32/abs/libscl mlibscl mapcs-32 !mmodule;",
  "apcs32/rma/libscl mlibscl mapcs-32 mmodule;",

  "apcs26/abs/unixlib !mlibscl !mapcs-32 !mmodule;",
  "apcs26/rma/unixlib !mlibscl !mapcs-32 mmodule;",

  "apcs26/abs/libscl mlibscl !mapcs-32 !mmodule;",
  "apcs26/rma/libscl mlibscl !mapcs-32 mmodule;",

  NULL
};
#endif

#if 0
static const char *const multilib_raw[] = {
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
#endif

static const char *const multilib_matches_raw[] = {
"msoft-float msoft-float;",
"mlibscl mlibscl;",
"mmodule mmodule;",
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

static const char *multilib_extra = "";

static const char *const multilib_exclusions_raw[] = {
NULL
};

static const char *multilib_options = "";

