*** ltconfig.old        2009-07-08 14:05:42.000000000 +0100
--- ltconfig    2009-07-08 14:06:18.000000000 +0100
***************
*** 1200,1205 ****
--- 1200,1213 ----
    shlibpath_var=LD_LIBRARY_PATH
    ;;

+ *riscos)
+   version_type=linux
+   library_names_spec='${libname}$versuffix.so ${libname}.so.$major $libname.so'
+   soname_spec='${libname}.so.$major'
+   dynamic_linker='RISC OS ld-riscos.so'
+   shlibpath_var=LD_LIBRARY_PATH
+   ;;
+
  sco3.2v5*)
    version_type=osf
    soname_spec='${libname}${release}.so.$major'
