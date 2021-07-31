--- src/emu/machine/nmc9306.c.orig	2012-04-09 17:23:16.000000000 +0100
+++ src/emu/machine/nmc9306.c	2012-04-09 17:23:35.000000000 +0100
@@ -43,7 +43,7 @@
 // states
 enum
 {
-	STATE_IDLE = 0,
+	_STATE_IDLE = 0,
 	STATE_COMMAND,
 	STATE_ADDRESS,
 	STATE_DATA_IN,
@@ -108,7 +108,7 @@
 nmc9306_device::nmc9306_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock)
 	: device_t(mconfig, NMC9306, "NMC9306", tag, owner, clock),
 	  device_nvram_interface(mconfig, *this),
-	  m_state(STATE_IDLE),
+	  m_state(_STATE_IDLE),
 	  m_ewen(false)
 {
 }
@@ -188,7 +188,7 @@
 
 	switch (m_state)
 	{
-	case STATE_IDLE:
+	case _STATE_IDLE:
 		if (LOG) logerror("NMC9306 '%s' Idle %u\n", tag(), m_di);
 
 		if (m_di)
@@ -230,7 +230,7 @@
 				case EWDS:
 					if (LOG) logerror("NMC9306 '%s' EWDS\n", tag());
 					m_ewen = false;
-					m_state = STATE_IDLE;
+					m_state = _STATE_IDLE;
 					break;
 
 				case WRAL:
@@ -244,7 +244,7 @@
 				case EWEN:
 					if (LOG) logerror("NMC9306 '%s' EWEN\n", tag());
 					m_ewen = true;
-					m_state = STATE_IDLE;
+					m_state = _STATE_IDLE;
 					break;
 				}
 				break;
@@ -282,7 +282,7 @@
 		{
 			write(m_address & 0x0f, m_data);
 
-			m_state = STATE_IDLE;
+			m_state = _STATE_IDLE;
 		}
 		break;
 
@@ -295,7 +295,7 @@
 
 		if (m_bits == 16)
 		{
-			m_state = STATE_IDLE;
+			m_state = _STATE_IDLE;
 		}
 		break;
 	}
