--- mpn/Makeasm.am.org	2014-04-21 20:56:21.120309763 +0200
+++ mpn/Makeasm.am	2014-04-21 20:59:08.718202772 +0200
@@ -107,7 +107,7 @@
 # -fPIC.  (Later versions of gas are happy to accept PIC stuff any time.)
 #
 .asm.o:
-	$(M4) -DOPERATION_$* `test -f '$<' || echo '$(srcdir)/'`$< >tmp-$*.s
+	$(M4) -DOPERATION_$* `test -f '$<' || echo '$(srcdir)/'`$< | sed s"/[[:space:]]bx[[:space:]]/ mov pc, /" >tmp-$*.s
 	$(CCAS) $(COMPILE_FLAGS) tmp-$*.s -o $@
 	$(RM_TMP) tmp-$*.s
 .asm.obj:
