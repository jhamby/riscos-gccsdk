diff --git a/build-aux/config.guess b/build-aux/config.guess
index 137bedf..099d316 100755
--- build-aux/config.guess
+++ build-aux/config.guess
@@ -306,7 +306,7 @@ case "${UNAME_MACHINE}:${UNAME_SYSTEM}:${UNAME_RELEASE}:${UNAME_VERSION}" in
     arm:RISC*:1.[012]*:*|arm:riscix:1.[012]*:*)
 	echo arm-acorn-riscix${UNAME_RELEASE}
 	exit ;;
-    arm:riscos:*:*|arm:RISCOS:*:*)
+    arm*:riscos:*:*|arm*:RISCOS:*:*)
 	echo arm-unknown-riscos
 	exit ;;
     SR2?01:HI-UX/MPP:*:* | SR8000:HI-UX/MPP:*:*)
