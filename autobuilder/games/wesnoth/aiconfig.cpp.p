--- src/ai/configuration.cpp.oprig	2011-02-08 20:55:18.000000000 +0000
+++ src/ai/configuration.cpp	2011-06-24 18:22:24.866620700 +0100
@@ -305,7 +305,16 @@
 	if (cfg["ai_algorithm"]=="idle_ai") {
 		is_idle_ai = true;
 	} else {
+#ifdef __riscos__
+		// For some unknown reason I had to split out the for_each loop
+		// for RISC OS here
+		config::child_itors itrs = cfg.child_range("ai");
+		
+		for (config::child_iterator i = itrs.first; i != itrs.second; ++i) {
+			config &aiparam = *i;
+#else
 		foreach (config &aiparam, cfg.child_range("ai")) {
+#endif
 			if (aiparam["ai_algorithm"]=="idle_ai") {
 				is_idle_ai = true;
 				break;
