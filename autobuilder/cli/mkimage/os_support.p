--- tools/os_support.c.orig	2010-09-29 20:11:37.000000000 +0100
+++ tools/os_support.c	2010-09-29 20:11:37.000000000 +0100
@@ -26,3 +26,60 @@
 #ifdef __APPLE__
 #include "getline.c"
 #endif
+#ifdef __riscos__
+#include <stdio.h>
+#include <stdint.h>
+#include <string.h>
+#include <errno.h>
+#include <unistd.h>
+#include <stddef.h>
+
+typedef struct {
+	int fd;
+	int prot;
+	size_t len;
+	int offset;
+	char buf[0];
+} mmap_struct;
+
+__caddr_t fake_mmap(__caddr_t addr, size_t len, int prot, int flags, int fd, off_t offset)
+{
+	if((prot & PROT_EXEC) || (prot == PROT_NONE))
+	{
+		errno = ENOSYS;
+		return MAP_FAILED;
+	}
+	mmap_struct *mm = (mmap_struct *) malloc(sizeof(mmap_struct)+len);
+	if(!mm)
+	{
+		errno = ENOMEM;
+		return MAP_FAILED;
+	}
+	mm->fd = fd;
+	mm->prot = prot;
+	mm->len = len;
+	mm->offset = offset;
+	if(prot & PROT_READ)
+	{
+		__off_t curpos = lseek(fd,0,SEEK_CUR);
+		lseek(fd,offset,SEEK_SET);
+		read(fd,mm->buf,len);
+		lseek(fd,curpos,SEEK_SET);
+	}
+	return mm->buf;
+}
+
+int fake_munmap(__caddr_t addr,size_t len)
+{
+	mmap_struct *mm = (mmap_struct *) (((int)addr)-offsetof(mmap_struct,buf));
+	if(mm->prot & PROT_WRITE)
+	{
+		__off_t curpos = lseek(mm->fd,0,SEEK_CUR);
+		lseek(mm->fd,mm->offset,SEEK_SET);
+		write(mm->fd,mm->buf,len);
+		lseek(mm->fd,curpos,SEEK_SET);
+	}
+	free(mm);
+	return 0;
+}
+#endif
