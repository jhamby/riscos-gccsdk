--- src/emu/machine/adc083x.c.orig	2012-04-09 17:19:07.000000000 +0100
+++ src/emu/machine/adc083x.c	2012-04-09 17:21:06.000000000 +0100
@@ -9,6 +9,8 @@
 #include "emu.h"
 #include "adc083x.h"
 
+#undef _STATE_IDLE
+
 #define VERBOSE_LEVEL ( 0 )
 
 INLINE void ATTR_PRINTF( 3, 4 ) verboselog( int n_level, running_machine &machine, const char *s_fmt, ... )
@@ -30,7 +32,7 @@
 
 enum
 {
-	STATE_IDLE,
+	_STATE_IDLE,
 	STATE_WAIT_FOR_START,
 	STATE_SHIFT_MUX,
 	STATE_MUX_SETTLE,
@@ -121,7 +123,7 @@
 
 	if( adc083x->cs == 0 && state != 0 )
 	{
-		adc083x->state = STATE_IDLE;
+		adc083x->state = _STATE_IDLE;
 		adc083x_clear_sars( device, adc083x );
 		adc083x->_do = 1;
 	}
@@ -364,7 +366,7 @@
 				break;
 
 			case STATE_FINISHED:
-				adc083x->state = STATE_IDLE;
+				adc083x->state = _STATE_IDLE;
 				adc083x->_do = 0;
 				break;
 			}
@@ -450,7 +452,7 @@
 	adc083x->odd = 0;
 	adc083x->sel1 = 0;
 	adc083x->sel0 = 0;
-	adc083x->state = STATE_IDLE;
+	adc083x->state = _STATE_IDLE;
 	adc083x->bit = 0;
 	adc083x->output = 0;
 
@@ -502,7 +504,7 @@
 
 	adc083x_clear_sars( device, adc083x );
 	adc083x->_do = 1;
-	adc083x->state = STATE_IDLE;
+	adc083x->state = _STATE_IDLE;
 }
 
 /*-------------------------------------------------
