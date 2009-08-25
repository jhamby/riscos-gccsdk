--- src/filesystem.cpp.orig	2009-08-24 17:29:51.000000000 -0700
+++ src/filesystem.cpp	2009-08-24 17:30:05.000000000 -0700
@@ -43,6 +43,10 @@
 BPath be_path;
 #endif
 
+#ifdef __riscos__
+#include <unixlib/local.h>
+#endif
+
 // for getenv
 #include <cerrno>
 #include <fstream>
@@ -511,6 +515,10 @@
 
 	const std::string home(home_str);
 
+#ifdef __riscos__
+	static const std::string dir_path =  "/WesnothUser:/wesnoth";
+#else /* !defined(__riscos__) */
+
 #ifndef __AMIGAOS4__
 	game_config::preferences_dir = home + std::string("/") + path;
 #else
@@ -519,6 +527,7 @@
 
 #endif /*_WIN32*/
 	user_data_dir = setup_user_data_dir();
+#endif
 }
 
 
@@ -664,17 +673,24 @@
 		throw io_exception("Could not open file for writing: '" + fname + "'");
 	}
 
+#ifdef __riscos__
+       size_t bytes = data.size();
+       const char * buf = data.c_str();
+#else
 	const size_t block_size = 4096;
 	char buf[block_size];
 
 	for(size_t i = 0; i < data.size(); i += block_size) {
 		const size_t bytes = std::min<size_t>(block_size,data.size() - i);
 		std::copy(data.begin() + i, data.begin() + i + bytes,buf);
+#endif
 		const size_t res = fwrite(buf,1,bytes,file.get());
 		if(res != bytes) {
 			throw io_exception("Error writing to file: '" + fname + "'");
 		}
+#ifndef __riscos__
 	}
+#endif
 }
 
 
@@ -939,6 +955,29 @@
 			paths_.push_back(path);
 		}
 	}
+#ifdef __riscos__
+#define RISCOSIFY_FLAGS (__RISCOSIFY_STRICT_UNIX_SPECS|__RISCOSIFY_NO_SUFFIX|__RISCOSIFY_DONT_CHECK_DIR|__RISCOSIFY_FILETYPE_NOT_SET)
+	{
+		/* Establishing a path variable costs now but pays on every call to get_binary_paths(const std::string&) */
+		int ftype;
+		char buf[PATH_MAX];
+		std::string path, ropathval;
+		path.reserve(PATH_MAX);
+		ropathval.reserve(PATH_MAX);
+		ropathval = "Wesnoth:";
+		for(std::set<std::string>::const_iterator i = binary_paths.begin(); i != binary_paths.end(); ++i) {
+			if((*i).length()) {
+				path = "/Wesnoth:/" + *i;
+				if(__riscosify(path.c_str(), 0, RISCOSIFY_FLAGS, buf, PATH_MAX, &ftype)) {
+					ropathval += ",";
+					ropathval += buf;
+					ropathval += ".";
+				}
+			}
+		}
+		setenv("WesnothData$Path", ropathval.c_str(), 1);
+	}
+#endif // __riscos__
 }
 
 void binary_paths_manager::cleanup()
@@ -973,6 +1012,12 @@
 
 	init_binary_paths();
 
+#ifdef __riscos__
+       /*
+        * RISC OS has well developed virtual path support. Using it speeds up the game significantly!
+        */
+       res.push_back("/WesnothData:/" + type + "/");
+#else
 	foreach (const std::string &path, binary_paths)
 	{
 		res.push_back(get_user_data_dir() + "/" + path + type + "/");
@@ -981,6 +1026,7 @@
 			res.push_back(game_config::path + "/" + path + type + "/");
 		}
 	}
+#endif
 
 	// not found in "/type" directory, try main directory
 	res.push_back(get_user_data_dir());
@@ -994,6 +1040,19 @@
 std::string get_binary_file_location(const std::string& type, const std::string& filename)
 {
 	DBG_FS << "Looking for '" << filename << "'.\n";
+/*
+ * RISC OS has no concept of a current directory for an application. Since the current directory can change
+ * at any time, and there is only one for all applications, it makes no sense to look for anything there.
+ */
+#ifndef __riscos__
+       if(!filename.empty()) {
+               DBG_FS << "Looking for " << filename << " in  '.'\n";
+               if(file_exists(filename) || is_directory(filename)) {
+                 DBG_FS << "  Found at " << filename << "\n";
+                       return filename;
+               }
+       }
+#endif
 
 	if (filename.empty()) {
 		LOG_FS << "  invalid filename (type: " << type <<")\n";
