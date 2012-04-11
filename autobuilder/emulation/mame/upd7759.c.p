--- src/emu/sound/upd7759.c.orig	2012-04-11 11:33:51.000000000 +0100
+++ src/emu/sound/upd7759.c	2012-04-11 11:34:16.000000000 +0100
@@ -120,7 +120,7 @@
 /* chip states */
 enum
 {
-	STATE_IDLE,
+	STATE_IDLE_,
 	STATE_DROP_DRQ,
 	STATE_START,
 	STATE_FIRST_REQ,
@@ -260,7 +260,7 @@
 	switch (chip->state)
 	{
 		/* Idle state: we stick around here while there's nothing to do */
-		case STATE_IDLE:
+		case STATE_IDLE_:
 			chip->clocks_left = 4;
 			break;

@@ -308,7 +308,7 @@

 			/* 28 cycles later, we will latch this value and request another byte */
 			chip->clocks_left = 28;	/* 28 - breaks cotton */
-			chip->state = (chip->req_sample > chip->last_sample) ? STATE_IDLE : STATE_DUMMY1;
+			chip->state = (chip->req_sample > chip->last_sample) ? STATE_IDLE_ : STATE_DUMMY1;
 			break;

 		/* First dummy state: ignore any data here and issue a request for the third byte */
@@ -377,7 +377,7 @@
 			{
 				case 0x00:	/* silence */
 					chip->clocks_left = 1024 * ((chip->block_header & 0x3f) + 1);
-					chip->state = (chip->block_header == 0 && chip->first_valid_header) ? STATE_IDLE : STATE_BLOCK_HEADER;
+					chip->state = (chip->block_header == 0 && chip->first_valid_header) ? STATE_IDLE_ : STATE_BLOCK_HEADER;
 					chip->sample = 0;
 					chip->adpcm_state = 0;
 					break;
@@ -476,7 +476,7 @@
 	stream_sample_t *buffer = outputs[0];

 	/* loop until done */
-	if (chip->state != STATE_IDLE)
+	if (chip->state != STATE_IDLE_)
 		while (samples != 0)
 		{
 			/* store the current sample */
@@ -502,7 +502,7 @@
 				{
 					/* advance one state; if we hit idle, bail */
 					advance_state(chip);
-					if (chip->state == STATE_IDLE)
+					if (chip->state == STATE_IDLE_)
 						break;

 					/* reimport the variables that we cached */
@@ -546,7 +546,7 @@
 		(*chip->drqcallback)(chip->device, chip->drq);

 	/* set a timer to go off when that is done */
-	if (chip->state != STATE_IDLE)
+	if (chip->state != STATE_IDLE_)
 		chip->timer->adjust(chip->clock_period * chip->clocks_left);
 }

@@ -563,11 +563,11 @@
 	chip->pos                = 0;
 	chip->fifo_in            = 0;
 	chip->drq                = 0;
-	chip->state              = STATE_IDLE;
+	chip->state              = STATE_IDLE_;
 	chip->clocks_left        = 0;
 	chip->nibbles_left       = 0;
 	chip->repeat_count       = 0;
-	chip->post_drq_state     = STATE_IDLE;
+	chip->post_drq_state     = STATE_IDLE_;
 	chip->post_drq_clocks    = 0;
 	chip->req_sample         = 0;
 	chip->last_sample        = 0;
@@ -649,7 +649,7 @@
 	chip->clock_period = attotime::from_hz(device->clock());

 	/* set the intial state */
-	chip->state = STATE_IDLE;
+	chip->state = STATE_IDLE_;

 	/* compute the ROM base or allocate a timer */
 	chip->rom = chip->rombase = *device->region();
@@ -705,7 +705,7 @@
 	chip->channel->update();

 	/* on the rising edge, if we're idle, start going, but not if we're held in reset */
-	if (chip->state == STATE_IDLE && !oldstart && chip->start && chip->reset)
+	if (chip->state == STATE_IDLE_ && !oldstart && chip->start && chip->reset)
 	{
 		chip->state = STATE_START;

@@ -728,7 +728,7 @@
 {
 	/* return /BUSY */
 	upd7759_state *chip = get_safe_token(device);
-	return (chip->state == STATE_IDLE);
+	return (chip->state == STATE_IDLE_);
 }


