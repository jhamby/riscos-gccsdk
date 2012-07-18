Index: !LibPkg/libpkg/control.cc
===================================================================
--- !LibPkg/libpkg/control.cc	(revision 128)
+++ !LibPkg/libpkg/control.cc	(working copy)
@@ -151,21 +151,21 @@
 	{
 		out << (*i).first << ':';
 		std::string value((*i).second);
-		unsigned int i=0;
-		unsigned int f=value.find('\n',i);
+		unsigned int j=0;
+		unsigned int f=value.find('\n',j);
 		while (f!=std::string::npos)
 		{
-			if ((f==i)&&(i!=0))
+			if ((f==j)&&(j!=0))
 				out << " ." << std::endl;
 			else
-				out << ' ' << value.substr(i,f-i) << std::endl;
-			i=f+1;
-			f=value.find('\n',i);
+				out << ' ' << value.substr(j,f-j) << std::endl;
+			j=f+1;
+			f=value.find('\n',j);
 		}
-		if ((value.length()==i)&&(i!=0))
+		if ((value.length()==j)&&(j!=0))
 			out << " ." << std::endl;
 		else
-			out << ' ' << value.substr(i) << std::endl;
+			out << ' ' << value.substr(j) << std::endl;
 	}
 	return out;
 }
