Index: config/config.alias
===================================================================
RCS file: /cvs/kaffe/kaffe/config/config.alias,v
retrieving revision 1.14
diff -u -r1.14 config.alias
--- config/config.alias	3 Aug 2001 17:39:43 -0000	1.14
+++ config/config.alias	12 Jan 2003 17:29:48 -0000
@@ -18,6 +18,7 @@
 case "$Khost_os" in
 
 aix*)		Khost_os=aix ;;
+aof*)		Khost_os=riscos ;;
 aux*)		Khost_os=a-ux ;;
 bsdi3*)		Khost_os=bsdi3 ;;
 bsdi*)		Khost_os=bsdi ;;
