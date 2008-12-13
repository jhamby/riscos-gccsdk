--- opcodes/arm-dis.c.orig	2008-12-09 23:50:47.000000000 +0100
+++ opcodes/arm-dis.c	2008-12-09 23:50:33.000000000 +0100
@@ -2200,14 +2200,14 @@ print_insn_thumb16 (bfd_vma pc, struct d
 		      if (started)
 			func (stream, ", ");
 		      started = 1;
-		      func (stream, arm_regnames[14] /* "lr" */);
+		      func (stream, "%s", arm_regnames[14] /* "lr" */);
 		    }
 
 		  if (domaskpc)
 		    {
 		      if (started)
 			func (stream, ", ");
-		      func (stream, arm_regnames[15] /* "pc" */);
+		      func (stream, "%s", arm_regnames[15] /* "pc" */);
 		    }
 
 		  func (stream, "}");
@@ -2738,7 +2738,7 @@ print_insn_thumb32 (bfd_vma pc, struct d
 		  }
 		else
 		  {
-		    func (stream, psr_name (given & 0xff));
+		    func (stream, "%s", psr_name (given & 0xff));
 		  }
 		break;
 
@@ -2746,7 +2746,7 @@ print_insn_thumb32 (bfd_vma pc, struct d
 		if ((given & 0xff) == 0)
 		  func (stream, "%cPSR", (given & 0x100000) ? 'S' : 'C');
 		else
-		  func (stream, psr_name (given & 0xff));
+		  func (stream, "%s", psr_name (given & 0xff));
 		break;
 
 	      case '0': case '1': case '2': case '3': case '4':
