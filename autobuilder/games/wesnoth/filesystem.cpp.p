--- src/filesystem.cpp.orig	2008-08-05 16:17:30.433000000 +0100
+++ src/filesystem.cpp	2008-10-01 15:01:26.904136800 +0100
@@ -265,6 +265,10 @@
 BPath be_path;
 #endif
 
+#ifdef __riscos__
+#include <unixlib/local.h>
+#endif
+
 // for getenv
 #include <cstdlib>
 #include <cerrno>
@@ -630,6 +634,10 @@
 	return be_path.Path();
 #else
 
+#ifdef __riscos__
+	static const std::string dir_path =  "/WesnothUser:/wesnoth";
+#else /* !defined(__riscos__) */
+
 #ifndef __AMIGAOS4__
 	static const char* const current_dir = ".";
 	const char* home_str = getenv("HOME");
@@ -651,6 +659,8 @@
 #else
 	const std::string dir_path = home + PREFERENCES_DIR;
 #endif
+#endif /* !defined(__riscos__) */
+
 	DIR* dir = opendir(dir_path.c_str());
 	if(dir == NULL) {
 		const int res = mkdir(dir_path.c_str(),AccessMode);
@@ -729,17 +739,24 @@
 		throw io_exception("Could not open file for writing: '" + fname + "'");
 	}
 
+#ifdef __riscos__
+	size_t bytes = data.size();
+	const char * buf = data.c_str();
+#else
 	const size_t block_size = 4096;
 	char buf[block_size];
 
 	for(size_t i = 0; i < data.size(); i += block_size) {
 		const size_t bytes = minimum<size_t>(block_size,data.size() - i);
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
 
 
@@ -1007,6 +1024,29 @@
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
@@ -1034,6 +1074,12 @@
 
 	init_binary_paths();
 
+#ifdef __riscos__
+	/*
+	 * RISC OS has well developed virtual path support. Using it speeds up the game significantly!
+	 */
+	res.push_back("/WesnothData:/" + type + "/");
+#else
 	for(std::set<std::string>::const_iterator i = binary_paths.begin(); i != binary_paths.end(); ++i) {
 		res.push_back(get_user_data_dir() + "/" + *i + type + "/");
 
@@ -1043,6 +1089,7 @@
 
 		res.push_back(*i + type + "/");
 	}
+#endif
 
 	return res;
 }
@@ -1050,6 +1097,20 @@
 std::string get_binary_file_location(const std::string& type, const std::string& filename)
 {
 	const std::vector<std::string>& paths = get_binary_paths(type);
+/*
+ * RISC OS has no concept of a current directory for an application. Since the current directory can change
+ * at any time, and there is only one for all applications, it makes no sense to look for anything there.
+ */
+#ifndef __riscos__
+	if(!filename.empty()) {
+		DBG_FS << "Looking for " << filename << " in  '.'\n";
+		if(file_exists(filename) || is_directory(filename)) {
+		  DBG_FS << "  Found at " << filename << "\n";
+			return filename;
+		}
+	}
+#endif
+
 	if(!filename.empty()) {
 		DBG_FS << "Looking for " << filename << " in  '.'\n";
 		if(file_exists(filename) || is_directory(filename)) {
