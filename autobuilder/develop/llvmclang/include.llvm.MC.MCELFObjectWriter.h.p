--- include/llvm/MC/MCELFObjectWriter.h.orig	2014-07-21 00:15:06.000000000 +0100
+++ include/llvm/MC/MCELFObjectWriter.h	2014-11-02 19:49:02.316230345 +0000
@@ -45,6 +45,8 @@
         return ELF::ELFOSABI_FREEBSD;
       case Triple::Linux:
         return ELF::ELFOSABI_LINUX;
+      case Triple::RISCOS:
+        return ELF::ELFOSABI_ARM;
       default:
         return ELF::ELFOSABI_NONE;
     }
