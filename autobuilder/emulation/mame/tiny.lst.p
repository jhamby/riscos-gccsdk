--- src/mame/tiny.lst.orig	2012-04-11 10:56:50.000000000 +0100
+++ src/mame/tiny.lst	2012-04-09 11:15:02.000000000 +0100
@@ -1,68 +1,1894 @@
-/******************************************************************************
-
-    tiny.lst
-
-    List of all enabled drivers in the system. This file is parsed by
-    makelist.exe, sorted, and output as C code describing the drivers.
-
-****************************************************************************
-
-    Copyright Aaron Giles
-    All rights reserved.
-
-    Redistribution and use in source and binary forms, with or without
-    modification, are permitted provided that the following conditions are
-    met:
-
-        * Redistributions of source code must retain the above copyright
-          notice, this list of conditions and the following disclaimer.
-        * Redistributions in binary form must reproduce the above copyright
-          notice, this list of conditions and the following disclaimer in
-          the documentation and/or other materials provided with the
-          distribution.
-        * Neither the name 'MAME' nor the names of its contributors may be
-          used to endorse or promote products derived from this software
-          without specific prior written permission.
-
-    THIS SOFTWARE IS PROVIDED BY AARON GILES ''AS IS'' AND ANY EXPRESS OR
-    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-    DISCLAIMED. IN NO EVENT SHALL AARON GILES BE LIABLE FOR ANY DIRECT,
-    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
-    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
-    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
-    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
-    IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
-    POSSIBILITY OF SUCH DAMAGE.
-
-******************************************************************************/
-
-robby			// (c) 1981 Bally Midway
-gridlee			// [1983 Videa] prototype - no copyright notice
-alienar			// (c) 1985 Duncan Brown
-
-carpolo			// (c) 1977 Exidy
-sidetrac		// (c) 1979 Exidy
-targ			// (c) 1980 Exidy
-spectar			// (c) 1980 Exidy
-teetert			// (c) 1982 Exidy
-hardhat			// (c) 1982
-fax				// (c) 1983
-fax2			// (c) 1983
-circus			// (c) 1977 Exidy
-robotbwl		// (c) 197? Exidy
-crash			// (c) 1979 Exidy
-ripcord			// (c) 1979 Exidy
-starfire		// (c) 1979 Exidy
-starfirea		// (c) 1979 Exidy
-fireone			// (c) 1979 Exidy
-starfir2		// (c) 1979 Exidy
-victory			// (c) 1982
-victorba		// (c) 1982
-topgunnr		// (c) 1986
-
-looping			// (c) 1982 Video Games GMBH
-supertnk		// (c) 1981 VIDEO GAMES GmbH, W.-GERMANY
-
-wrally			// (c) 1993 - Ref 930705
+/******************************************************************************
+
+    tiny.lst
+
+    List of all enabled drivers in the system. This file is parsed by
+    makelist.exe, sorted, and output as C code describing the drivers.
+
+****************************************************************************
+
+    Copyright Aaron Giles
+    All rights reserved.
+
+    Redistribution and use in source and binary forms, with or without
+    modification, are permitted provided that the following conditions are
+    met:
+
+        * Redistributions of source code must retain the above copyright
+          notice, this list of conditions and the following disclaimer.
+        * Redistributions in binary form must reproduce the above copyright
+          notice, this list of conditions and the following disclaimer in
+          the documentation and/or other materials provided with the
+          distribution.
+        * Neither the name 'MAME' nor the names of its contributors may be
+          used to endorse or promote products derived from this software
+          without specific prior written permission.
+
+    THIS SOFTWARE IS PROVIDED BY AARON GILES ''AS IS'' AND ANY EXPRESS OR
+    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
+    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
+    DISCLAIMED. IN NO EVENT SHALL AARON GILES BE LIABLE FOR ANY DIRECT,
+    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
+    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
+    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
+    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
+    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
+    IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
+    POSSIBILITY OF SUCH DAMAGE.
+
+******************************************************************************/
+
+gridlee			// [1983 Videa] prototype - no copyright notice
+
+// Williams games
+defender		// (c) 1980
+defenderg		// (c) 1980
+defenderb		// (c) 1980
+defenderw		// (c) 1980
+defndjeu		// bootleg
+tornado1		// bootleg
+tornado2		// bootleg
+zero			// bootleg
+zero2			// bootleg
+defcmnd			// bootleg
+defence			// bootleg
+startrkd		// bootleg
+mayday			// bootleg
+maydaya			// bootleg
+maydayb			// bootleg
+colony7			// (c) 1981 Taito
+colony7a		// (c) 1981 Taito
+jin				// Falcon bootleg/hack
+stargate		// (c) 1981
+robotron		// (c) 1982
+robotronyo		// (c) 1982
+joust			// (c) 1982
+joustr			// (c) 1982
+joustwr			// (c) 1982
+bubbles			// (c) 1982
+bubblesr		// (c) 1982
+bubblesp		// (c) 1982
+splat			// (c) 1982
+sinistar		// (c) 1982
+sinistar1		// (c) 1982
+sinistar2		// (c) 1982
+playball		// (c) 1983
+blaster			// (c) 1983
+blastero		// (c) 1983
+blasterkit		// (c) 1983
+spdball			// (c) 1985
+alienar			// (c) 1985 Duncan Brown
+alienaru		// (c) 1985 Duncan Brown
+mysticm			// (c) 1983
+tshoot			// (c) 1984
+inferno			// (c) 1984
+joust2			// (c) 1986
+lottofun		// (c) 1987 H.A.R. Management
+
+// Exidy games
+carpolo			// (c) 1977
+sidetrac		// (c) 1979
+targ			// (c) 1980
+targc			// (c) 1980
+spectar			// (c) 1980
+spectar1		// (c) 1980
+rallys			// (c) 1980 Novar (bootleg)
+panzer			// (c) 1980 Proel (bootleg)
+phantoma		// (c) 1980 Jeutel (bootleg)
+phantom			// (c) 1980 Proel (bootleg)
+venture			// (c) 1981
+venture2		// (c) 1981
+venture4		// (c) 1981
+teetert			// (c) 1981
+mtrap			// (c) 1981
+mtrap3			// (c) 1981
+mtrap4			// (c) 1981
+pepper2			// (c) 1982
+pepper27		// (c) 1982
+hardhat			// (c) 1982
+fax				// (c) 1983
+fax2			// (c) 1983
+circus			// no copyright notice [1977?]
+circusse		// bootleg
+robotbwl		// no copyright notice
+crash			// Exidy [1979?]
+smash			// bootleg
+ripcord			// Exidy [1977?]
+starfire		// Exidy [1979?]
+starfirea		// Exidy [1979?]
+fireone			// (c) 1979 Exidy
+starfir2		// (c) 1979 Exidy
+victory			// (c) 1982
+victorba		// (c) 1982
+
+// Exidy 440 games
+crossbow		// (c) 1983
+cheyenne		// (c) 1984
+combat			// (c) 1985
+catch22			// (c) 1985
+cracksht		// (c) 1985
+claypign		// (c) 1986
+chiller			// (c) 1986
+topsecex		// (c) 1986
+hitnmiss		// (c) 1987
+hitnmiss2		// (c) 1987
+whodunit		// (c) 1988
+showdown		// (c) 1988
+yukon			// (c) 1989
+yukon1			// (c) 1989
+
+// Exidy Max-A-Flex
+maxaflex
+mf_achas		// (c) 1982 Exidy / First Star Software
+mf_brist		// (c) 1983 Exidy / First Star Software
+mf_flip			// (c) 1983 Exidy / First Star Software
+mf_bdash		// (c) 1984 Exidy / First Star Software
+
+// Venture Line games
+spcforce                // (c) 1980 Venture Line
+spcforc2                // bootleg
+meteor                  // (c) 1981 Venture Line
+looping                 // (c) 1982 Video Games GMBH
+loopingv                // (c) 1982 Video Games GMBH (Venture Line license)
+loopingva               // (c) 1982 Video Games GMBH (Venture Line license)
+skybump                 // (c) 1982 Venture Line
+suprridr                // (c) 1983 Venture Line + Taito license
+
+// Epos games
+megadon                 // (c) 1982
+catapult                // (c) 1982
+suprglob                // (c) 1983
+theglob                 // (c) 1983
+theglob2                // (c) 1983
+theglob3                // (c) 1983
+igmo                    // (c) 1984
+dealer                  // (c) 198?
+revenger                // (c) 1984
+
+// "Galaxian hardware" games
+galaxian		// (c) Namco
+galaxiana		// (c) Namco
+galaxianm		// (c) Midway
+galaxianmo		// (c) Midway
+galaxiant		// (c) Taito
+superg			// hack
+galapx			// hack
+moonaln			// [Nichibutsu] (Karateco license) or hack
+galap1			// hack
+galap4			// hack
+galturbo		// hack
+swarm			// hack
+zerotime		// hack
+starfght		// hack
+galaxbsf		// hack
+kamakazi3		// Video Games (UK) Ltd., hack or bootleg?
+tst_galx		// Galaxian Test ROM
+gmgalax			// bootleg
+pisces			// Subelectro
+piscesb			// bootleg
+omni			// bootleg
+uniwars			// (c) Irem
+gteikoku		// (c) Irem
+gteikokb		// bootleg
+gteikob2		// bootleg
+spacbatt		// bootleg
+spacbat2		// bootleg
+spacempr		// bootleg
+skyraidr		// bootleg
+batman2			// bootleg
+warofbug		// (c) 1981 Armenia
+warofbugg		// German Version
+redufo			// (c) Artic
+redufob			// bootleg
+exodus			// Subelectro - bootleg?
+streakng		// [1980] Shoei
+streaknga		// [1980] Shoei
+pacmanbl		// bootleg
+pacmanbla		// bootleg
+phoenxp2		// bootleg
+atlantisb		// bootleg
+tenspot			// (c) 1982 Thomas Automatics
+devilfsg		// (c) 1984 Vision / Artic (bootleg?)
+zigzag			// (c) 1982 LAX
+zigzag2			// (c) 1982 LAX
+jumpbug			// (c) 1981 Rock-ola
+jumpbugb		// (c) 1981 Sega
+levers			// (c) 1983 Rock-ola
+azurian			// (c) 1982 Rait Electronics Ltd
+orbitron		// (c) 1982 Signatron USA (Arcade Tv Game List - P.160, Left, 22 from top)
+mooncrgx		// bootleg
+mooncrst		// (c) 1980 Nichibutsu
+mooncrstu		// (c) 1980 Nichibutsu USA
+mooncrsto		// (c) 1980 Nichibutsu
+mooncrstg		// (c) 1980 Gremlin
+mooncrstuk		// (c) 1980 Nichibutsu UK
+smooncrs		// Gremlin
+sstarcrs		// Taito (Brazil)
+mooncmw			// bootleg
+mooncrsb		// bootleg
+mooncrs2		// bootleg
+spcdrag			// bootleg
+stera			// bootleg
+spcdraga		// bootleg
+mooncrs3		// bootleg
+fantazia		// bootleg
+moonqsr			// (c) 1980 Nichibutsu
+mshuttle		// (c) 1981 Nichibutsu
+mshuttle2		// (c) 1981 Nichibutsu
+mshuttlej		// (c) 1981 Nichibutsu
+mshuttlej2		// (c) 1981 Nichibutsu
+moonal2			// [1980] Nichibutsu
+moonal2b		// [1980] Nichibutsu
+supergx			// [1980] Nichibutsu
+eagle			// [1980] Centuri
+eagle2			// [1980] Centuri
+eagle3			// [1980] Centuri
+spctbird		// (c) Fortrek
+skybase			// (c) 1982 Omori Electric Co., Ltd.
+checkman		// (c) 1982 Zilec-Zenitone
+checkmanj		// (c) 1982 Jaleco (Zenitone/Zilec in ROM CM4, and the programmer names)
+dingo			// (c) 1983 Ashby Computers and Graphics LTD. + Jaleco license
+dingoe			// (c) 1983 Ashby Computers and Graphics LTD. + Jaleco license
+blkhole			// TDS (Tokyo Denshi Sekkei)
+kingball		// (c) 1980 Namco
+kingballj		// (c) 1980 Namco
+scorpionmc		// bootleg
+frogg			// bootleg
+4in1			// (c) 1981 Armenia / Food and Fun
+bagmanmc		// bootleg
+dkongjrm		// bootleg
+ozon1			// (c) 1983 Proma
+ladybugg		// bootleg
+vpool			// bootleg
+drivfrcg		// [1984] Shinkai (Magic Electronics USA license)
+drivfrcb		// bootleg
+bongo			// (c) 1983 Jetsoft
+hunchbkg		// (c) 1983 Century
+harem			// [1983] I.G.R.
+tazzmang		// bootleg
+racknrol		// (c) 1986 Status (Shinkia license) (Senko Kit)
+hexpool			// (c) 1986 Shinkia (Senko Kit)
+hexpoola		// (c) 1986 Shinkia (Senko Kit)
+trvchlng		// [1985] Joyland (Senko license)
+luctoday		// 1980 Sigma
+chewing			// ?? unknown
+catacomb		// 1982 MTM Games
+ckongg			// 1981 bootleg
+ckongmc			// 1981 bootleg
+porter			// 1982 bootleg (Arcade TV Game List - P.98, Left, 15 from bottom)
+vstars			// 19?? Competitive Video?
+tdpgal			// 1983 Design Labs / Thomas Automatics
+
+// Has some similarities with Moon Cresta but Board is very different
+rockclim                // (c)1981 Taito
+
+// "Scramble hardware" (and variations) games
+scramble		// GX387 (c) 1981 Konami
+scrambles		// GX387 (c) 1981 Stern
+scramblebf		// bootleg
+scramblebb		// bootleg?
+scramblb		// bootleg
+scramb2			// bootleg
+explorer		// bootleg
+strfbomb		// bootleg
+atlantis		// (c) 1981 Comsoft
+atlantis2		// (c) 1981 Comsoft
+theend			// (c) 1980 Konami
+theends			// (c) 1980 Stern
+theendb			// (c) 1980 Bootleg
+omega			// bootleg
+ckongs			// bootleg
+froggers		// bootleg
+frogf			// bootleg
+amidars			// GX337 (c) 1982 Konami
+triplep			// (c) 1982 KKI	/ made by Sanritsu?
+knockout		// (c) 1982 KKK
+mariner			// (c) 1981 Amenip
+800fath			// (c) 1981 Amenip + U.S. Billiards license
+mars			// (c) 1981 Artic
+devilfsh		// (c) 1982 Artic
+newsin7			// (c) 1983 ATW USA, Inc.
+mrkougar		// (c) 1984 ATW
+mrkougar2		// (c) 1984 ATW
+mrkougb			// bootleg
+mrkougb2		// bootleg
+hotshock		// (c) 1982 E.G. Felaco (Domino license)
+hotshockb		// bootleg
+conquer			// (c) 1982 ????
+hunchbks		// (c) 1983 Century
+hncholms		// (c) 1984 Century
+cavelon			// (c) 1983 Jetsoft
+sfx				// (c) 1983 Nichibutsu
+skelagon		// (c) 1983 Nichibutsu USA
+monsterz		// (c) 1982 Nihon (Arcade TV Game List - P.102, Left, 20 from top)
+mimonscr		// (c) 1982 Universal Video Games (US Copyright Office info - http://cocatalog.loc.gov)
+mimonkey		// (c) 1982 bootleg
+mimonsco		// (c) 1982 bootleg
+scobra			// GX316 (c) 1981 Konami
+scobras			// GX316 (c) 1981 Stern
+scobrase		// GX316 (c) 1981 Stern
+scobrab			// GX316 (c) 1981 Karateco (bootleg?)
+suprheli		// bootleg
+stratgyx		// GX306 (c) 1981 Konami
+stratgys		// GX306 (c) 1981 Stern
+strongx			// bootleg
+armorcar		// (c) 1981 Stern
+armorcar2		// (c) 1981 Stern
+moonwar			// (c) 1981 Stern
+moonwara		// (c) 1981 Stern
+spdcoin			// (c) 1984 Stern
+darkplnt		// (c) 1982 Stern
+tazmania		// (c) 1982 Stern
+tazmani2		// (c) 1982 Stern
+calipso			// (c) 1982 Tago
+anteater		// (c) 1982 Tago
+anteaterg		// (c) 1983 TV-Tuning (F.E.G. license)
+anteateruk		// (c) 1983 Free Enterprise Games
+rescue			// (c) 1982 Stern
+rescueb			// (c) Videl Games (Rescue bootleg)
+aponow			// bootleg
+minefld			// (c) 1983 Stern
+losttomb		// (c) 1982 Stern
+losttombh		// (c) 1982 Stern
+superbon		// bootleg
+hustler			// GX343 (c) 1981 Konami
+hustlerd		// GX343 (c) 1981 Konami + Dynamo Games license
+billiard		// bootleg
+hustlerb		// bootleg
+hustlerb2		// bootleg
+frogger			// GX392 (c) 1981 Konami
+froggers1		// (c) 1981 Sega
+froggers2		// 834-0068 (c) 1981 Sega
+froggermc		// 800-3110 (c) 1981 Sega
+amidar			// GX337 (c) 1982 Konami
+amidar1			// GX337 (c) 1981 Konami
+amidaru			// GX337 (c) 1982 Konami + Stern license
+amidaro			// GX337 (c) 1982 Konami + Olympia license
+amidarb			// bootleg
+amigo			// bootleg
+turtles			// (c) 1981 Stern
+turpin			// (c) 1981 Sega
+600				// GX353 (c) 1981 Konami
+flyboy			// (c) 1982 Kaneko
+flyboyb			// bootleg
+fastfred		// 136009   (c) 1982 Atari
+jumpcoas		// (c) 1983 Kaneko
+jumpcoast		// (c) 1983 Taito
+boggy84			// (c) 1984 Kaneko
+boggy84b		// bootleg
+redrobin		// (c) 1986 Elettronolo
+imago			// (c) 1984 Acom
+imagoa			// (c) 1983 Acom
+scorpion		// (c) 1982 Zaccaria
+scorpiona		// (c) 1982 Zaccaria
+scorpionb		// (c) 1982 Zaccaria
+aracnis			// bootleg
+ad2083			// (c) 1983 Midcoin
+turpins			// (c) 1981 bootleg
+
+// Namco games (plus some intruders on similar hardware)
+geebee			// [1978] Namco
+geebeeb			// [1978] Namco (F.lli Bertolino license)
+geebeeg			// [1978] Gremlin
+bombbee			// [1979] Namco
+cutieq			// (c) 1979 Namco
+navarone		// (c) 1980 Namco
+kaitein			// [1980] Namco
+kaitei			// [1980] K.K. Tokki
+sos				// [1980] Namco
+tankbatt		// (c) 1980 Namco
+warpwarp		// (c) 1981 Namco
+warpwarpr		// (c) 1981 [Namco] (Rock-ola license)
+warpwarpr2		// (c) 1981 [Namco] (Rock-ola license)
+rallyx			// (c) 1980 Namco
+rallyxa			// (c) 1980 Namco
+rallyxm			// (c) 1980 Midway
+nrallyx			// (c) 1981 Namco
+nrallyxb		// (c) 1981 Namco
+jungler			// GX327 (c) 1981 Konami
+junglers		// GX327 (c) 1981 Stern
+savanna			// Olympia (c) 1982 (Jungler bootleg)
+tactcian		// GX335 (c) 1982 Sega
+tactcian2		// GX335 (c) 1981 Sega
+locomotn		// GX359 (c) 1982 Konami + Centuri license
+gutangtn		// GX359 (c) 1982 Konami + Sega license
+cottong			// bootleg
+locoboot		// bootleg
+commsega		// (c) 1983 Sega
+// the following ones all have a custom I/O chip
+bosco			// (c) 1981
+boscoo			// (c) 1981
+boscoo2			// (c) 1981
+boscomd			// (c) 1981 Midway
+boscomdo		// (c) 1981 Midway
+galaga			// (c) 1981
+galagao			// (c) 1981
+galagamw		// (c) 1981 Midway
+galagamk		// (c) 1981 Midway
+galagamf		// (c) 1981 Midway / hack
+gallag			// bootleg
+gatsbee			// (c) 1984 Uchida / hack
+digdug			// (c) 1982
+digdug1			// (c) 1982
+digdugat		// 136007   (c) 1982 Atari
+digdugat1		// 136007   (c) 1982 Atari
+dzigzag			// bootleg
+digsid			// bootleg
+xevious			// (c) 1982
+xeviousa		// 136018   (c) 1982 + Atari license
+xeviousb		// 136018   (c) 1982 + Atari license
+xeviousc		// 136018   (c) 1982 + Atari license
+xevios			// bootleg
+battles			// bootleg
+sxevious		// (c) 1984
+sxeviousj		// (c) 1984
+superpac		// (c) 1982
+superpacm		// (c) 1982 Midway
+pacnpal			// (c) 1983
+pacnpal2		// (c) 1983
+pacnchmp		// (c) 1983
+phozon			// (c) 1983
+mappy			// (c) 1983
+mappyj			// (c) 1983
+digdug2			// (c) 1985
+digdug2o		// (c) 1985
+todruaga		// (c) 1984
+todruagao		// (c) 1984
+todruagas		// (c) 1984 pcb manufactured by Sidam
+motos			// (c) 1985
+grobda			// (c) 1984
+grobda2			// (c) 1984
+grobda3			// (c) 1984
+gaplus			// (c) 1984
+gaplusa			// (c) 1984
+gapluso			// (c) 1984
+galaga3			// (c) 1984
+galaga3m		// (c) 1984
+galaga3a		// (c) 1984
+// Z8000 games
+polepos			// (c) 1982
+poleposa		// 136014   (c) 1982 + Atari license
+polepos1		// 136014   (c) 1982 Atari
+topracer		// bootleg
+topracera		// bootleg
+topracern		// bootleg
+polepos2		// (c) 1983
+polepos2a		// 136014   (c) 1983 + Atari license
+polepos2b		// bootleg
+polepos2bi		// bootleg
+// no custom I/O in the following, HD63701 (or compatible) microcontroller instead
+pacland			// (c) 1984
+paclandj		// (c) 1984
+paclandjo		// (c) 1984
+paclandjo2		// (c) 1984
+paclandm		// (c) 1984 Midway
+drgnbstr		// (c) 1984
+skykid			// (c) 1985
+skykido			// (c) 1985
+skykidd			// (c) 1985
+skykids			// (c) 1985
+aliensec		// (c) 1985
+baraduke		// (c) 1985
+metrocrs		// (c) 1985
+metrocrsa		// (c) 1985
+
+// Atari vector games
+llander                 // 0345xx           no copyright notice
+llander1                // 0345xx           no copyright notice
+asteroid                // 035127-035145    (c) 1979
+asteroid2               // 035127-035145    (c) 1979
+asteroid1               // 035127-035145    no copyright notice
+asteroidb               // (bootleg)
+tempest                 // 136002           (c) 1980
+tempest3                // 136002           (c) 1980
+tempest2                // 136002           (c) 1980
+tempest1                // 136002           (c) 1980
+tempest1r               // 136002           (c) 1980
+temptube                // (hack)
+
+// "Phoenix hardware" (and variations) games
+safarir                 // Shin Nihon Kikaku (SNK)
+phoenix                 // (c) 1980 Amstar
+phoenixa                // (c) 1980 Amstar + Centuri license
+phoenixb                // (c) 1980 Amstar + Centuri license
+phoenixt                // (c) 1980 Taito
+phoenixj                // (c) 1980 Taito
+phoenix3                // bootleg
+phoenixc                // bootleg
+phoenixc2               // bootleg
+
+// Atari "Missile Command hardware" games
+missile                 // 035820-035825    (c) 1980, 1 ROM update to rev 2 board
+missile2                // 035820-035825    (c) 1980
+missile1                // 035820-035825    (c) 1980
+
+// Atari "Centipede hardware" games
+warlords                // 037153-037159    (c) 1980
+centiped                // 136001           (c) 1980
+centiped2               // 136001           (c) 1980
+centtime                // 136001           (c) 1980
+centipdb                // (bootleg)
+centipdd                // hack by Two Bit Score
+caterplr                // (bootleg)
+millpac                 // Valadon
+magworm                 // (bootleg)
+mazeinv                 // (proto)          (c) 1982
+milliped                // 136013           (c) 1982
+millipdd                // hack by Two Bit Score
+qwak                    // (proto)          (c) 1982
+runaway                 // (proto)          (c) 1982
+bullsdrt                // (c) 1985 Shinkai
+cmmb162
+
+// Gottlieb/Mylstar games (Gottlieb became Mylstar in 1983)
+reactor                 // GV-100 (c) 1982 Gottlieb
+mplanets                // GV-102 (c) 1983 Gottlieb
+mplanetsuk              // GV-102 (c) 1983 Gottlieb
+qbert                   // GV-103 (c) 1982 Gottlieb
+qberta                  // GV-103 (c) 1982 Gottlieb
+qbertj                  // GV-103 (c) 1982 Gottlieb + Konami license
+myqbert                 // GV-103?(c) 1982 Gottlieb
+qberttst                // GV-103 (c) 1982 Gottlieb
+qbtrktst                // GV-103 (c) 1982 Gottlieb
+insector                // GV-??? (c) 1982 Gottlieb - never released
+tylz                    // GV-??? (c) 1982 Gottlieb - never released
+argusg                  // GV-??? (c) 1982 Gottlieb - never released
+krull                   // GV-105 (c) 1983 Gottlieb
+kngtmare                // GV-??? (c) 1983 Gottlieb - never released
+sqbert                  // GV-??? (c) 1983 Mylstar - never released
+mach3                   // GV-109 (c) 1983 Mylstar
+cobram3                 // ...... (c) 1984 Data East
+usvsthem                // GV-??? (c) 198? Mylstar
+3stooges                // GV-113 (c) 1984 Mylstar
+qbertqub                // GV-119 (c) 1983 Mylstar
+screwloo                // GV-123 (c) 1983 Mylstar - never released
+curvebal                // GV-134 (c) 1984 Mylstar
+vidvince                // GV-??? (c) 1984 Mylstar - never released
+wizwarz                 // GV-??? (c) 1984 Mylstar - never released
+
+// Sega "Zaxxon hardware" games
+zaxxon                  // (c) 1982
+zaxxonj                 // (c) 1982
+zaxxon2                 // (c) 1982
+zaxxon3                 // (c) 1982
+zaxxonb                 // bootleg
+szaxxon                 // (c) 1982
+futspy                  // (c) 1984
+razmataz                // modified 834-0213, 834-0214 (c) 1983
+ixion                   // (c) 1983
+congo                   // 605-5167 (c) 1983
+tiptop                  // 605-5167 (c) 1983
+
+// Konami games
+pooyan			// GX320 (c) 1982
+pooyans			// GX320 (c) 1982 Stern
+pootan			// bootleg
+timeplt			// GX393 (c) 1982
+timepltc		// GX393 (c) 1982 + Centuri license
+timeplta		// GX393 (c) 1982 + Atari license
+spaceplt		// bootleg
+psurge			// (c) 1988 unknown (NOT Konami)
+megazone		// GX319 (c) 1983
+megazonea		// GX319 (c) 1983
+megazoneb		// GX319 (c) 1983 + Kosuka
+megazonec		// GX319 (c) 1983 + Kosuka
+megazonei		// GX319 (c) 1983 + Interlogic / Kosuka
+pandoras		// GX328 (c) 1984 + Interlogic
+gyruss			// GX347 (c) 1983
+gyrussce		// GX347 (c) 1983 + Centuri license
+gyrussb			// bootleg or actual Taito NZ license?
+venus			// bootleg
+trackfld		// GX361 (c) 1983
+trackfldc		// GX361 (c) 1983 + Centuri license
+trackfldnz		// (c) 1982 Goldberg Enterprizes Inc.
+hyprolym		// GX361 (c) 1983
+hyprolymb		// bootleg
+atlantol		// bootleg
+wizzquiz		// (c) 1985 Konami
+wizzquiza		// (c) 1985 Zilec-Zenitone
+reaktor			// (c) 1985 Zilec
+mastkin			// (c) 1988 Du Tech
+rocnrope		// GX364 (c) 1983
+rocnropek		// GX364 (c) 1983 + Kosuka
+ropeman			// (c) 1983 bootleg
+circusc			// GX380 (c) 1984
+circusc2		// GX380 (c) 1984
+circusc3		// GX380 (c) 1984
+circuscc		// GX380 (c) 1984 + Centuri license
+circusce		// GX380 (c) 1984 + Centuri license
+tp84                    // GX388 (c) 1984
+tp84a                   // GX388 (c) 1984
+tp84b                   // GX388 (c) 1984
+hyperspt                // GX330 (c) 1984 + Centuri
+hypersptb               // bootleg
+hpolym84                // GX330 (c) 1984
+sbasketb                // GX405 (c) 1984 (Version I, Encrypted)
+sbasketh                // GX405 (c) 1984 (Version H, Unprotected)
+sbasketg                // GX405 (c) 1984 (Version G, Encrypted)
+sbaskete                // GX405 (c) 1984 (Version E, Encrypted)
+mikie                   // GX469 (c) 1984
+mikiej                  // GX469 (c) 1984
+mikiehs                 // GX469 (c) 1984
+roadf                   // GX461 (c) 1984
+roadf2                  // GX461 (c) 1984
+yiear                   // GX407 (c) 1985
+yiear2                  // GX407 (c) 1985
+yieartf                 // GX361 (c) 1985
+kicker                  // GX477 (c) 1985
+shaolins                // GX477 (c) 1985
+shaolinb                // bootleg
+pingpong                // GX555 (c) 1985
+merlinmm                // (c) 1986 Zilec-Zenitone
+cashquiz                // (c) 1986 Zilec-Zenitone
+gberet                  // GX577 (c) 1985
+rushatck                // GX577 (c) 1985
+gberetb                 // bootleg on different hardware
+mrgoemon                // GX621 (c) 1986 (Japan)
+jailbrek                // GX507 (c) 1986
+manhatan                // GX507 (c) 1986 (Japan)
+jailbrekb               // bootleg
+scotrsht                // GX545 (c) 1985
+finalizr                // GX523 (c) 1985
+finalizrb               // bootleg
+ironhors                // GX560 (c) 1986
+dairesya                // GX560 (c) 1986 (Japan)
+farwest                 // bootleg
+jackal                  // GX631 (c) 1986 (World)
+topgunr                 // GX631 (c) 1986 (US)
+jackalj                 // GX631 (c) 1986 (Japan)
+topgunbl                // bootleg
+ddribble                // GX690 (c) 1986
+ddribblep               // GX690 (c) 1986
+
+// Stern "Berzerk hardware" games
+berzerk                 // (c) 1980
+berzerk1                // (c) 1980
+berzerkg                // (c) 1980
+frenzy                  // (c) 1982
+moonwarp                // (c) 1982 - prototype
+
+// "Crazy Climber hardware" games
+cclimber		// (c) 1980 Nichibutsu
+cclimberj		// (c) 1980 Nichibutsu
+ccboot			// bootleg
+ccboot2			// bootleg
+
+ckong			// (c) 1981 (Falcon?)
+ckongo			// bootleg (Orca)
+ckongalc		// bootleg (Alca)
+bigkong			// bootleg
+monkeyd			// bootleg
+
+ckongpt2		// (c) 1981 Falcon
+ckongpt2a		// (c) 1981 Falcon
+ckongpt2j		// (c) 1981 Falcon
+ckongpt2jeu		// bootleg (Jeutel)
+ckongpt2b		// bootleg
+
+rpatrol			// Orca, from a suspicious set
+rpatrolb		// bootleg
+silvland		// Falcon
+yamato			// (c) 1983 Sega
+yamato2			// (c) 1983 Sega
+toprollr		// (c) 1983 Jaleco
+swimmer			// (c) 1982 Tehkan
+swimmera		// (c) 1982 Tehkan
+swimmerb		// (c) 1982 Tehkan
+guzzler			// (c) 1983 Tehkan
+guzzlers		// (c) 1983 Tehkan (Swimmer Conversion)
+cannonb			// (c) 1985 Soft
+cannonb2		// (c) 1985 TV Game Gruenberg
+cannonb3		// (c) 1985 Soft
+bagmanf			// bootleg on Falcon FCK-00 (Falcon Crazy Kong) PCB
+
+// Other Games
+spacefb                 // (c) [1980?] Nintendo
+supertnk		// (c) 1981 VIDEO GAMES GmbH, W.-GERMANY
+
+// Gremlin/Sega "VIC dual game board" games
+// the numbers listed are the range of ROM part numbers
+carnival                // 651-666 (c) 1980 Sega
+
+// SNK / Rock-ola games
+vanguard                // (c) 1981 SNK
+
+// "Pacman hardware" games
+puckman			// (c) 1980 Namco
+puckmana		// (c) 1980 Namco
+puckmanf		// hack
+puckmanh		// hack
+pacman			// (c) 1980 Midway
+pacmanf			// hack
+puckmod			// (c) 1981 Namco
+pacmod			// (c) 1981 Midway
+newpuc2			// hack
+newpuc2b		// hack
+newpuckx		// hack
+pacheart		// hack
+bucaner			// hack
+hangly			// hack
+hangly2			// hack
+hangly3			// hack
+popeyeman		// hack
+crockman		// 1980 bootleg (RENE PIERRE)
+piranhah		// hack
+abscam			// hack
+crush			// (c) 1981 Kural Samno Electric Ltd
+crushbl			// bootleg
+crushbl2		// bootleg 2
+crush2			// (c) 1981 Kural Esco Electric Ltd - bootleg?
+crush3			// Kural Electric Ltd - bootleg?
+crush4			// Kural TWT
+maketrax		// (c) 1981 Williams, high score table says KRL (fur Kural)
+maketrxb		// (c) 1981 [Kural] (Williams license)
+korosuke		// (c) 1981 Kural Electric
+mbrush			// 1981 bootleg
+paintrlr		// 1981 bootleg
+crushs			// 1981 bootleg
+pacplus			// (c) 1982 Namco (Midway license)
+joyman			// hack
+ctrpllrp		// hack
+eyes			// (c) 1982 Digitrex Techstar + "Rockola presents"
+eyes2			// (c) 1982 Techstar + "Rockola presents"
+eyeszac			// (c) 1982 Techstar / Zaccaria
+eyeszacb		// bootleg
+birdiy			// (c) 1983 Mama Top
+mrtnt			// (c) 1983 Telko
+gorkans			// (c) 1984 Techstar
+eggor			// (c) 1983 Telko
+jumpshot		// (c) 1985 Bally Midway
+jumpshotp		// (c) 1985 Bally Midway
+shootbul		// (c) 1985 Bally Midway
+piranha			// GL
+piranhao		// GL
+nmouse			// (c) 1981 Amenip (Palcom Queen River)
+nmouseb			// (c) 1981 Amenip Nova Games Ltd.
+mspacman		// (c) 1981 Midway	// made by General Computer
+mspacmnf		// hack
+mspacmat		// hack
+woodpeck		// (c) 1981 Amenip (Palcom Queen River)
+woodpeca		// (c) 1981 Amenip Nova Games Ltd.
+mspacmab		// bootleg
+mspacmbe		// bootleg
+pacgal			// hack
+mspacpls		// hack
+ponpoko			// (c) 1982 Sigma Ent. Inc.
+ponpokov		// (c) 1982 Sigma Ent. Inc. + Venture Line license
+lizwiz			// (c) 1985 Techstar + "Sunn presents"
+alibaba			// (c) 1982 Sega
+alibabab		// Mustafa" and 40 Thieves (Ali Baba bootleg)
+dremshpr		// (c) 1982 Sanritsu
+vanvan			// (c) 1983 Sanritsu
+vanvank			// (c) 1983 Karateco (bootleg?)
+vanvanb			// (c) 1983 Karateco (bootleg?)
+bwcasino		// (c) 1983 Epos Coropration
+acitya			// (c) 1983 Epos Corporation
+theglobp		// (c) 1983 Epos Corporation
+sprglobp		// (c) 1983 Epos Corporation
+sprglbpg		// German bootleg
+beastf			// (c) 1984 Epos Corporation
+rocktrv2		// (c) 1986 Triumph Software Inc
+bigbucks		// (c) 1986 Dynasoft Inc.
+mschamp			// hack by RAYGLO MFG "Zola-Puc" (c) 1992/1993
+mschamps		// hack (hack of the Rayglo hack?)
+cannonbp		// hack
+
+// S2650 Pacman Kits
+drivfrcp                // (c) 1984 Shinkai Inc. (Magic Eletronics Inc. license)
+8bpm                    // (c) 1985 Seatongrove Ltd (Magic Eletronics USA license)
+porky                   // (c) 1985 Shinkai Inc. (Magic Eletronics Inc. license)
+
+pengo                   // 834-0386 (c) 1982 Sega
+pengo2                  // 834-0386 (c) 1982 Sega
+pengo2u                 // 834-0386 (c) 1982 Sega
+pengo3u                 // 834-0386 (c) 1982 Sega
+pengo4                  // 834-0386 (c) 1982 Sega
+pengob                  // bootleg
+penta                   // bootleg
+jrpacmbl                // bootleg on Pengo HW
+
+jrpacman                // (c) 1983 Midway
+
+// other Taito games
+fgoal                   // TF  (c) 1979 Taito Corporation
+fgoala                  // MF  (c) 1979 Taito Corporation
+crbaloon                // CL  (c) 1980 Taito Corporation
+crbaloon2               // CL  (c) 1980 Taito Corporation
+sbowling                // KB  (c) 1982 Taito Corporation
+grchamp                 // GM  (c) 1981 Taito Corporation
+marinedt                // MG  (c) 1981 Taito Corporation
+changela                // ??? (c) 1983 Taito Corporation
+fspiderb                // bootleg
+jollyjgr                // KD  (c) 1982 Taito Corporation
+bking                   // DM  (c) 1982 Taito Corporation
+bking2                  // AD6 (c) 1983 Taito Corporation
+bking3                  // A24 (c) 1984 Taito Corporation
+chaknpop                // AO4 (c) 1983 Taito Corporation
+josvolly                // AA  (c) 1983 Taito Corporation
+gsword                  // AC  (c) 1984 Taito Corporation
+gsword2                 // AC  (c) 1984 Taito Corporation
+pitnrun                 // PR  (c) 1984 Taito Corporation
+pitnruna                // PR  (c) 1984 Taito Corporation
+lkage                   // A54 (c) 1984 Taito Corporation
+lkageo                  // A54 (c) 1984 Taito Corporation
+lkageoo                 // A54 (c) 1984 Taito Corporation
+lkageb                  // bootleg
+lkageb2                 // bootleg
+bygone                  // prototype ? A53 (c) 1985 Taito Corporation
+msisaac                 // A34 (c) 1985 Taito Corporation
+arkanoid                // A75 (c) 1986 Taito Corporation Japan (World
+arkanoidu               // A75 (c) 1986 Taito America Corporation + Romstar license (US)
+arkanoiduo              // A75 (c) 1986 Taito America Corporation + Romstar license (US)
+arkanoidj               // A75 (c) 1986 Taito Corporation (Japan)
+arkanoidjo              // A75 (c) 1986 Taito Corporation (Japan)
+arkanoidjb              // bootleg
+ark1ball                // bootleg
+arkangc                 // bootleg
+arkangc2                // bootleg
+arkblock                // bootleg
+arkbloc2                // bootleg
+arkbloc3                // bootleg
+arkgcbl                 // bootleg
+arkgcbla                // bootleg
+paddle2                 // bootleg
+block2                  // bootleg
+arkatayt                // bootleg
+arktayt2                // bootleg
+
+// Capcom games
+// The following is a COMPLETE list of the Capcom games up to 1997, as shown on
+// their web site. The list is sorted by production date.
+// A comprehensive list of Capcom games with board info can be found here:
+// http://www.arcadeflyers.com/strider/capcom_list.html
+vulgus			//  5/1984 (c) 1984
+vulgusa			//  5/1984 (c) 1984
+vulgusj			//  5/1984 (c) 1984
+sonson			//  7/1984 (c) 1984
+sonsonj			//  7/1984 (c) 1984 (Japan)
+higemaru		//  9/1984 (c) 1984
+1942			// 12/1984 (c) 1984
+1942a			// 12/1984 (c) 1984
+1942abl			// bootleg
+1942b			// 12/1984 (c) 1984
+1942w			// 12/1984 (c) 1984 + Williams Electronics license (c) 1985
+exedexes		//  2/1985 (c) 1985
+savgbees		//  2/1985 (c) 1985 + Memetron license
+commando		//  5/1985 (c) 1985 (World)
+commandou		//  5/1985 (c) 1985 + Data East license (US)
+commandoj		//  5/1985 (c) 1985 (Japan)
+commandob		// bootleg
+sinvasn			// Europe original?
+sinvasnb		// bootleg
+gng				//  9/1985 (c) 1985
+gnga			//  9/1985 (c) 1985
+gngbl			// bootleg
+gngblita		// bootleg
+gngc			//  9/1985 (c) 1985
+gngt			//  9/1985 (c) 1985
+makaimur		//  9/1985 (c) 1985
+makaimurc		//  9/1985 (c) 1985
+makaimurg		//  9/1985 (c) 1985
+diamond			// (c) 1989 KH Video (NOT A CAPCOM GAME but runs on GnG hardware)
+gunsmoke		// 11/1985 (c) 1985 (World)
+gunsmokeu		// 11/1985 (c) 1985 + Romstar (US)
+gunsmokeua		// 11/1985 (c) 1985 (US)
+gunsmokej		// 11/1985 (c) 1985 (Japan)
+sectionz		// 12/1985 (c) 1985
+sectionza		// 12/1985 (c) 1985
+trojan			//  4/1986 (c) 1986 (US)
+trojanr			//  4/1986 (c) 1986 + Romstar
+trojanj			//  4/1986 (c) 1986 (Japan)
+srumbler		//  9/1986 (c) 1986
+srumbler2		//  9/1986 (c) 1986
+rushcrsh		//  9/1986 (c) 1986
+lwings			// 11/1986 (c) 1986
+lwings2			// 11/1986 (c) 1986
+lwingsj			// 11/1986 (c) 1986
+lwingsb			// 11/1986 (c) 1986 (bootleg)
+sidearms		// 12/1986 (c) 1986 (World)
+sidearmsr		// 12/1986 (c) 1986 + Romstar license (US)
+sidearmsj		// 12/1986 (c) 1986 (Japan)
+turtship		// (c) 1988 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware)
+turtshipj		// (c) 1988 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware)
+turtshipk		// (c) 1988 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware)
+dyger			// (c) 1989 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware)
+dygera			// (c) 1989 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware)
+twinfalc		// (c) 1989 Philko (Poara Enterprises license) (NOT A CAPCOM GAME but runs on modified Sidearms hardware)
+whizz			// (c) 1989 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware)
+
+// Namco System 1 games
+shadowld                // (c) 1987
+youkaidk                // (c) 1987 (Japan new version)
+youkaidko               // (c) 1987 (Japan old version)
+dspirit                 // (c) 1987 (Japan new version)
+dspirito                // (c) 1987 (Japan old version)
+dspirita                // (c) 1987 (Atari license)
+blazer                  // (c) 1987 (Japan)
+quester                 // (c) 1987 (Japan)
+questers                // (c) 1987 (Japan)
+pacmania                // (c) 1987
+pacmaniaj               // (c) 1987 (Japan)
+galaga88                // (c) 1987
+galaga88j               // (c) 1987 (Japan)
+ws                              // (c) 1988 (Japan)
+berabohm                // (c) 1988 (Japan)
+berabohmo               // (c) 1988 (Japan)
+                                // 1988 Alice in Wonderland (English version of Marchen maze)
+mmaze                   // (c) 1988 (Japan)
+bakutotu                // (c) 1988 (Japan)
+wldcourt                // (c) 1988 (Japan)
+splatter                // (c) 1988 (World)
+splattero               // (c) 1988 (World)
+splatterj               // (c) 1988 (Japan)
+faceoff                 // (c) 1988 (Japan)
+rompers                 // (c) 1989 (Japan)
+romperso                // (c) 1989 (Japan)
+blastoff                // (c) 1989 (Japan)
+ws89                    // (c) 1989 (Japan)
+dangseed                // (c) 1989 (Japan)
+ws90                    // (c) 1990 (Japan)
+pistoldm                // (c) 1990 (Japan)
+boxyboy                 // (c) 1990 (US)
+soukobdx                // (c) 1990 (Japan)
+puzlclub                // (c) 1990 (Japan)
+tankfrce                // (c) 1991 (US, 2 Player)
+tankfrce4               // (c) 1991 (US, 4 Player)
+tankfrcej               // (c) 1991 (Japan)
+
+// M72 (and derivatives)
+rtype                   // (c) 1987 (Japan)
+rtypej                  // (c) 1987 (Japan)
+rtypejp                 // (c) 1987 (Japan)
+rtypeu                  // (c) 1987 + Nintendo USA license (US)
+rtypeb                  // bootleg
+bchopper                // (c) 1987
+mrheli                  // (c) 1987 (Japan)
+nspirit                 // (c) 1988
+nspiritj                // (c) 1988 (Japan)
+imgfight                // (c) 1988 (Japan)
+imgfighto               // (c) 1988 (Japan)
+loht                    // (c) 1989
+lohtj                   // (c) 1989 (Japan)
+lohtb                   // (c) 1989 (bootleg)
+lohtb2                  // (c) 1989 (bootleg)
+xmultipl                // (c) 1989 (Japan)
+xmultiplm72             // (c) 1989 (Japan)
+dbreed                  // (c) 1989
+dbreedm72               // (c) 1989
+rtype2                  // (c) 1989
+rtype2j                 // (c) 1989 (Japan)
+rtype2jc                // (c) 1989 (Japan)
+majtitle                // (c) 1990 (World)
+majtitlej               // (c) 1990 (Japan)
+hharry                  // (c) 1990 (World)
+hharryu                 // (c) 1990 Irem America (US)
+dkgensan                // (c) 1990 (Japan)
+dkgensanm72             // (c) 1990 (Japan)
+poundfor                // (c) 1990 (World)
+poundforj               // (c) 1990 (Japan)
+poundforu               // (c) 1990 Irem America (US)
+airduel                 // (c) 1990 (Japan)
+cosmccop                // (c) 1991 (World)
+gallop                  // (c) 1991 (Japan)
+kengo                   // (c) 1991
+// M90
+hasamu			// (c) 1991 Irem (Japan)
+dynablst		// (c) 1991 Irem (World)
+dynablstb		// bootleg
+bombrman		// (c) 1991 Irem (Japan)
+atompunk		// (c) 1991 Irem America (licensed from Hudson Soft)
+// M97
+bbmanw			// (c) 1992 Irem (World)
+bbmanwj			// (c) 1992 Irem (Japan)
+bomblord		// bootleg
+newapunk		// (c) 1992 Irem America (US)
+quizf1			// (c) 1992 Irem (Japan)
+riskchal		// (c) 1993 Irem
+gussun			// (c) 1993 Irem (Japan)
+matchit2		// (c) 1993 Tamtex
+shisen2			// (c) 1993 Tamtex
+// M92
+gunforce		// (c) 1991 Irem (World)
+gunforceu		// (c) 1991 Irem America (US)
+gunforcej		// (c) 1991 Irem (Japan)
+bmaster			// (c) 1991 Irem
+crossbld		// (c) 1991 Irem (Japan)
+lethalth		// (c) 1991 Irem (World)
+thndblst		// (c) 1991 Irem (Japan)
+uccops			// (c) 1992 Irem (World)
+uccopsu			// (c) 1992 Irem (US)
+uccopsar		// (c) 1992 Irem (World)
+uccopsj			// (c) 1992 Irem (Japan)
+mysticri		// (c) 1992 Irem (World)
+mysticrib		// bootleg?
+gunhohki		// (c) 1992 Irem (Japan)
+majtitl2		// (c) 1992 Irem (World)
+majtitl2j		// (c) 1992 Irem (Japan)
+skingame		// (c) 1992 Irem America (US)
+skingame2		// (c) 1992 Irem America (US)
+hook			// (c) 1992 Irem (World)
+hooku			// (c) 1992 Irem America (US)
+hookj			// (c) 1992 Irem (Japan)
+ppan			// bootleg
+rtypeleo		// (c) 1992 Irem (World)
+rtypeleoj		// (c) 1992 Irem (Japan)
+inthunt			// (c) 1993 Irem (World)
+inthuntu		// (c) 1993 Irem (US)
+kaiteids		// (c) 1993 Irem (Japan)
+nbbatman		// (c) 1993 Irem America (US)
+leaguemn		// (c) 1993 Irem (Japan)
+ssoldier		// (c) 1993 Irem America (US)
+psoldier		// (c) 1993 Irem (Japan)
+dsoccr94j		// (c) 1994 Irem (Japan)
+gunforc2		// (c) 1994 Irem
+geostorm		// (c) 1994 Irem (Japan)
+// M107
+firebarr		// (c) 1993 Irem (Japan)
+dsoccr94		// (c) 1994 Irem (Data East Corporation license)
+kftgoal			// (c) 1994 Jaleco
+wpksoc			// (c) 1995 Jaleco
+
+// Universal games
+cosmicg			// 7907 (c) 1979
+cosmica			// 7910-AII (c) [1979]
+cosmica1		// 7910-A (c) [1979]
+cosmica2		// 7910 (c) 1979
+nomnlnd			// 8003 (c) [1980?]
+nomnlndg		// 8003 (c) [1980?] + Gottlieb
+magspot			// ???? (c) [1980]
+magspot2		// 8013 (c) [1980]
+panic			// 8015 (c) 1980
+panic2			// 8015 (c) 1980
+panic3			// 8015 (c) 1980
+panich			// 8015 (c) 1980
+panicger		// 8015 (c) 1980
+devzone			// 8022 (c) [1980]
+devzone2		// 8022 (c) [1980]
+cheekyms		// 8004 (c) [1980?]
+zerohour		// 8011 (c) Universal
+zerohoura		// 8011 (c) Universal
+redclash		// (c) 1981 Tehkan
+redclasha		// (c) 1981 Tehkan
+redclashk		// (c) Kaneko (bootleg?)
+cavenger		// 8106 (c) 1981
+ladybug			// 8110 (c) 1981
+ladybugb		// bootleg
+ladybgb2		// bootleg
+snapjack		// ???? (c)
+sraider			// ???? (c) 1982 Universal
+dorodon			// Falcon
+dorodon2		// Falcon
+mrdo			// 8201 (c) 1982
+mrdoy			// 8201 (c) 1982
+mrdot			// 8201 (c) 1982 + Taito license
+mrdofix			// 8201 (c) 1982 + Taito license
+mrlo			// bootleg
+mrdu			// bootleg
+yankeedo		// bootleg
+docastle		// 8302 (c) 1983
+docastle2		// 8302 (c) 1983
+docastleo		// 8302 (c) 1983
+douni			// 8302 (c) 1983
+dorunrun		// ???? (c) 1984
+dorunrun2		// ???? (c) 1984
+dorunrunc		// ???? (c) 1984
+dorunrunca		// ???? (c) 1984
+spiero			// ???? (c) 1987
+dowild			// ???? (c) 1984
+jjack			// ???? (c) 1984
+kickridr		// ???? (c) 1984
+idsoccer		// ???? (c) 1985
+idsoccera		// ???? (c) 1985
+asoccer			// 8461 (c) 1987
+superdq			// ???? (c) 1984
+superdqs		// ???? (c) 1984
+superdqa		// ???? (c) 1984
+
+// Nintendo games
+spacefev		// (c) 1979 Nintendo
+spacefevo		// (c) 1979 Nintendo
+spacefevo2		// (c) 1979 Nintendo
+highsplt		// (c) 1979 Nintendo
+highsplta		// (c) 1979 Nintendo
+highspltb		// (c) 1979 Nintendo
+spacelnc		// (c) 1979 Nintendo
+sheriff			// (c) 1979 Nintendo
+bandido			// (c) 1980 Exidy
+westgun2		// (c) 1980 Taito Corporation
+helifire		// (c) 1980 Nintendo
+helifirea		// (c) 1980 Nintendo
+radarscp		// (c) 1980 Nintendo
+radarscp1		// (c) 1980 Nintendo
+dkong			// (c) 1981 Nintendo of America
+dkongo			// (c) 1981 Nintendo
+dkongj			// (c) 1981 Nintendo
+dkongjo			// (c) 1981 Nintendo
+dkongjo1		// (c) 1981 Nintendo
+dkongf			// hack from Jeff's Romhack
+dkongx			// hack
+dkongx11		// hack
+dkongjr			// (c) 1982 Nintendo of America
+dkongjrj		// (c) 1982 Nintendo
+dkongjnrj		// (c) 1982 Nintendo
+dkongjrb		// bootleg
+jrking			// bootleg
+dkingjr			// bootleg
+dkongjre		// (c) 1982 Nintendo of America
+dkong3			// (c) 1983 Nintendo of America
+dkong3j			// (c) 1983 Nintendo
+dkong3b			// bootleg
+mario			// (c) 1983 Nintendo of America
+marioe			// (c) 1983 Nintendo of America
+marioo			// (c) 1983 Nintendo of America
+marioj			// (c) 1983 Nintendo
+masao			// bootleg
+pestplce		// bootleg on donkey kong hw
+spclforc		// (c) 1985 Senko Industries (Magic Eletronics Inc. license)
+spcfrcii		// (c) 1985 Senko Industries (Magic Eletronics Inc. license)
+8ballact		// (c) 1984 Seatongrove Ltd (Magic Eletronics USA license)
+8ballact2		// (c) 1984 Seatongrove Ltd (Magic Eletronics USA license)
+shootgal		// (c) 1984 Seatongrove Ltd (Zaccaria license)
+drakton			// (c) 1984 Epos Corporation
+drktnjr			// (c) 1984 Epos Corporation
+strtheat		// (c) 1985 Epos Corporation
+hunchbkd		// (c) 1983 Century
+sbdk			// (c) 1984 Century
+herbiedk		// (c) 1984 CVS
+herodk			// (c) 1984 Seatongrove + Crown license
+herodku			// (c) 1984 Seatongrove + Crown license
+skyskipr		// (c) 1981
+popeye			// (c) 1982
+popeyeu			// (c) 1982
+popeyef			// (c) 1982
+popeyebl		// bootleg
+punchout		// (c) 1984
+punchita		// bootleg
+spnchout		// (c) 1984
+spnchoutj		// (c) 1984 (Japan)
+armwrest		// (c) 1985
+
+// Nintendo Playchoice 10 games
+playch10
+pc_tenis		// (c) 1983 Nintendo
+pc_mario		// (c) 1983 Nintendo
+pc_bball		// (c) 1984 Nintendo of America
+pc_bfght		// (c) 1984 Nintendo
+pc_ebike		// (c) 1984 Nintendo
+pc_golf			// (c) 1984 Nintendo
+pc_kngfu		// (c) 1984 Irem (Nintendo license)
+pc_1942			// (c) 1985 Capcom
+pc_smb			// (c) 1985 Nintendo
+pc_vball		// (c) 1986 Nintendo
+pc_duckh		// (c) 1984 Nintendo
+pc_hgaly		// (c) 1984 Nintendo
+pc_wgnmn		// (c) 1984 Nintendo
+pc_grdus		// (c) 1986 Konami
+pc_grdue		// (c) 1986 Konami
+pc_tkfld		// (c) 1987 Konami (Nintendo of America license)
+pc_pwrst		// (c) 1986 Nintendo
+pc_trjan		// (c) 1986 Capcom USA (Nintendo of America license)
+pc_cvnia		// (c) 1987 Konami (Nintendo of America license)
+pc_dbldr		// (c) 1987 Konami (Nintendo of America license)
+pc_rnatk		// (c) 1987 Konami (Nintendo of America license)
+pc_rygar		// (c) 1987 Tecmo (Nintendo of America license)
+pc_cntra		// (c) 1988 Konami (Nintendo of America license)
+pc_goons		// (c) 1986 Konami
+pc_mtoid		// (c) 1986 Nintendo
+pc_radrc		// (c) 1987 Square
+pc_miket		// (c) 1987 Nintendo
+pc_rcpam		// (c) 1987 Rare
+pc_ngaid		// (c) 1989 Tecmo (Nintendo of America license)
+pc_tmnt			// (c) 1989 Konami (Nintendo of America license)
+pc_ftqst		// (c) 1989 Sunsoft (Nintendo of America license)
+pc_bstar		// (c) 1989 SNK (Nintendo of America license)
+pc_tbowl		// (c) 1989 Tecmo (Nintendo of America license)
+pc_drmro		// (c) 1990 Nintendo
+pc_ynoid		// (c) 1990 Capcom USA (Nintendo of America license)
+pc_rrngr		// (c) Capcom USA (Nintendo of America license)
+pc_ddrgn		// (c) 1988 Technos
+pc_gntlt		// (c) 1985 Atari/Tengen (Nintendo of America license)
+pc_smb2			// (c) 1988 Nintendo
+pc_smb3			// (c) 1988 Nintendo
+pc_mman3		// (c) 1990 Capcom USA (Nintendo of America license)
+pc_radr2		// (c) 1990 Square (Nintendo of America license)
+pc_suprc		// (c) 1990 Konami (Nintendo of America license)
+pc_tmnt2		// (c) 1990 Konami (Nintendo of America license)
+pc_wcup			// (c) 1990 Technos (Nintendo license)
+pc_ngai2		// (c) 1990 Tecmo (Nintendo of America license)
+pc_ngai3		// (c) 1991 Tecmo (Nintendo of America license)
+pc_pwbld		// (c) 1991 Taito (Nintendo of America license)
+pc_rkats		// (c) 1991 Atlus (Nintendo of America license)
+pc_pinbt		// (c) 1988 Rare (Nintendo of America license)
+pc_cshwk		// (c) 1989 Rare (Nintendo of America license)
+pc_sjetm		// (c) 1990 Rare
+pc_moglf		// (c) 1991 Nintendo
+
+// Nintendo VS games
+btlecity		// (c) 1985 Namco
+starlstr		// (c) 1985 Namco
+cstlevna		// (c) 1987 Konami
+cluclu			// (c) 1984 Nintendo
+drmario			// (c) 1990 Nintendo
+duckhunt		// (c) 1985 Nintendo
+excitebk		// (c) 1984 Nintendo
+excitebkj		// (c) 1984 Nintendo
+goonies			// (c) 1986 Konami
+hogalley		// (c) 1985 Nintendo
+iceclimb		// (c) 1984 Nintendo
+iceclimbj		// (c) 1984 Nintendo
+ladygolf		// (c) 1984 Nintendo
+ladygolfe		// (c) 1984 Nintendo
+machridr		// (c) 1985 Nintendo
+machridrj		// (c) 1985 Nintendo
+rbibb			// (c) 1987 Namco
+rbibba			// (c) 1987 Namco
+suprmrio		// (c) 1986 Nintendo
+suprmrioa		// (c) 1986 Nintendo
+suprmriobl		// bootleg
+suprmriobl2		// bootleg
+skatekds		// (c) 1988 (hack)
+vsskykid		// (c) 1986 Namco
+tkoboxng		// (c) 1987 Data East
+smgolf			// (c) 1984 Nintendo
+smgolfj			// (c) 1984 Nintendo
+vspinbal		// (c) 1984 Nintendo
+vspinbalj		// (c) 1984 Nintendo
+vsslalom		// (c) 1986 Nintendo
+vssoccer		// (c) 1985 Nintendo
+vssoccerj		// (c) 1985 Nintendo
+vsgradus		// (c) 1986 Konami
+platoon			// (c) 1987 Ocean
+vstetris		// (c) 1988 Atari
+mightybj		// (c) 1986 Tecmo
+jajamaru		// (c) 1985 Jaleco
+topgun			// (c) 1987 Konami
+bnglngby		// (c) 1985 Nintendo / Broderbund Software Inc.
+vstennis		// (c) 1984 Nintendo
+vstennisj		// (c) 1984 Nintendo
+wrecking		// (c) 1984 Nintendo
+balonfgt		// (c) 1984 Nintendo
+vsmahjng		// (c) 1984 Nintendo
+vsbball			// (c) 1984 Nintendo
+vsbballj		// (c) 1984 Nintendo
+vsbballja		// (c) 1984 Nintendo
+vsbballjb		// (c) 1984 Nintendo
+iceclmrj		// (c) 1984 Nintendo
+vsgshoe			// (c) 1986 Nintendo
+supxevs			// (c) 1986 Nintendo
+vsfdf			// (c) 1986 Sunsoft
+smgolfb			// (c) 1985 Nintendo
+
+// NES-based hacks
+multigam		// 1992
+multigmb		// 1992
+multigm2		// 1992
+multigm3		// 19??
+multigmt
+supergm3		// 1996
+cham24			// 2002
+
+// Famicom Box
+famibox			// 1986
+
+// Nintendo Super System Games
+nss
+nss_ssoc		// 1992 Human Inc.
+nss_actr		// 1992 Enix
+nss_con3		// 1992 Konami
+nss_adam		// 1992 Ocean
+nss_aten		// 1992 Absolute Entertainment Inc.
+nss_rob3		// 1992 Ocean
+nss_ncaa		// 1992 Sculptured Software Inc.
+nss_skin		// 1992 Irem
+nss_lwep		// 1992 Nintendo
+nss_smw			// 1991 Nintendo
+nss_fzer		// 1991 Nintendo
+nss_sten		// 199? Nintendo
+
+// Super Famicom Box Games
+sfcbox
+pss61			// 199? Nintendo
+pss62			// 199? T & E Soft / I'Max
+pss63			// 199? Nintendo / BPS
+pss64			// 199? Nintendo / Hudson Soft
+
+// SNES-based hacks
+kinstb			// bootleg
+ffight2b		// bootleg
+sblast2b		// bootleg
+iron			// bootleg
+endless			// bootleg
+denseib			// bootleg
+
+// Midway 8080 b/w games
+seawolf			// 596 [1976]
+seawolfo		// 596 [1976]
+gunfight		// 597 [1975]
+gunfighto		// 597 [1975]
+				// 603 - Top Gun [1976]
+tornbase		// 605 [1976]
+280zzzap		// 610 [1976]
+maze			// 611 [1976]
+boothill		// 612 [1977]
+checkmat		// 615 [1977]
+desertgu		// 618 [1977]
+roadrunm		// 618 [1977]
+dplay			// 619 [1977]
+lagunar			// 622 [1977]
+gmissile		// 623 [1977]
+m4				// 626 [1977]
+clowns			// 630 [1978]
+clowns1			// 630 [1978]
+spacwalk		// 640 [1978]
+einning			// 642 [1978] Midway
+shuffle			// 643 [1978]
+dogpatch		// 644 [1977]
+spcenctr		// 645 (c) 1980 Midway
+phantom2		// 652 [1979]
+bowler			// 730 [1978] Midway
+invaders		// 739 [1979]
+blueshrk		// 742 [1978]
+invad2ct		// 851 (c) 1980 Midway
+invadpt2		// 852 [1980] Taito
+invaddlx		// 852 [1980] Midway
+vortex			// 852 [1980] Zilec Electronics Ltd.
+moonbase		// Taito
+moonbasea		// Zeta - Nichibutsu
+				// 870 - Space Invaders Deluxe cocktail
+searthin		// bootleg
+searthina		// bootleg
+invadrmr		// (c) 1978 Model Racing
+modelr			// (c) 19?? Model Racing
+spaceatt		// (c) 1978 Video Games GMBH
+spaceat2		// (c) 1980 Zenitone-Microsec Ltd
+sinvzen			// Zenitone-Microsec Ltd
+superinv		// bootleg
+invasion		// Sidam
+invasiona		// bootleg
+invasionb		// bootleg
+invasionrz		// bootleg
+invasionrza		// bootleg
+sstrangr		// (c) 1978 Yachiyo Electronics, Ltd.
+sstrangr2		// (c) 1979 Yachiyo Electronics, Ltd.
+sinvemag		// Zenitone-Microsec Ltd.
+jspecter		// (c) 1979 Jatre
+jspecter2		// (c) 1979 Jatre
+invrvnge		// Zenitone-Microsec Ltd.
+invrvngea		// Zenitone-Microsec Ltd. (Dutchford license)
+galxwars		// (c) 1979 Universal
+galxwars2		// (c) 1979 Universal
+galxwarst		// (c) 1979 Taito?
+starw			// bootleg
+lrescue			// LR  (c) 1979 Taito
+mlander			// (c) 1980 Leisure Time Electronics
+lrescuem		// Model Racing bootleg
+grescue			// bootleg?
+desterth		// bootleg
+cosmicmo		// Universal
+cosmicm2		// Universal
+rollingc		// Nichibutsu
+ozmawars		// Shin Nihon Kikaku (SNK)
+ozmawars2		// Shin Nihon Kikaku (SNK)
+solfight		// bootleg
+spaceph			// Zilec Games
+galactic		// (c) 19?? Taito
+spacmiss		// bootleg?
+schaser			// RT  Taito
+schasercv		// RT  Taito
+lupin3			// LP  (c) 1980 Taito
+lupin3a			// LP  (c) 1980 Taito
+intruder		// (c) 1980 GamePlan (Taito)
+spclaser		// (c) 1980 GamePlan (Taito)
+laser			// (c) 1980 Leisure Time Electronics Inc.
+spcewarl		// (c) 1979 Leijac Corporation (Konami)
+polaris			// PS  (c) 1980 Taito
+polarisa		// PS  (c) 1980 Taito
+polariso		// PS  (c) 1980 Taito
+ballbomb		// TN  (c) 1980 Taito
+indianbt		// (c) 1980 Taito
+steelwkr		// (c) 1980 Taito
+m79amb			// (c) 1977 RamTek
+alieninv		// Margamatics
+alieninvp2		// bootleg
+tst_invd		// Test ROM
+sitv			// (c) 1979 Taito
+sicv			// (c) 1979 Taito
+sisv			// (c) 1978 Taito
+sisv2			// (c) 1978 Taito
+spacewr3		// bootleg
+invaderl		// bootleg
+invader4		// bootleg
+yosakdon		// (c) 1979 Yosaku To Donbei
+yosakdona		// (c) 1979 Yosaku To Donbei
+spceking		// (c) 1978 Leijac Corporation (Konami)
+spcewars		// (c) 1978 Sanritsu
+astropal		// (c) 19?? Sidam
+cosmo			// TDS+Mints
+darthvdr		// bootleg
+
+18w				// 653 (c) 1979 Midway
+18w2			// 653 (c) 1979 Midway
+sspeedr			// 1979 Midway
+
+// Similar but with a M6800 instead of an 8080
+sflush			// (c)1979 Taito
+
+// Meadows S2650 games
+lazercmd		// [1976?]
+bbonk			// [1976?]
+deadeye			// [1978?]
+bowl3d			// [1978?]
+gypsyjug		// [1978?]
+minferno		// [1978?]
+medlanes		// [1977?]
+
+// Meadows Z80 game
+warpsped		// 197?
+
+// CVS games
+cosmos			// (c) 1981 Century
+darkwar			// (c) 1981 Century
+spacefrt		// (c) 1981 Century
+8ball			// (c) 1982 Century
+8ball1			// (c) 1982 Century
+logger			// (c) 1982 Century
+dazzler			// (c) 1982 Century
+diggerc			// (c) 1982 Century
+wallst			// (c) 1982 Century
+radarzon		// (c) 1982 Century
+radarzon1		// (c) 1982 Century
+radarzont		// (c) 1982 Tuni Electro Service
+outline			// (c) 1982 Century
+goldbug			// (c) 1982 Century
+heartatk		// (c) 1983 Century Electronics
+hunchbak		// (c) 1983 Century
+hunchbaka		// (c) 1983 Century
+superbik		// (c) 1983 Century
+raiders			// (c) 1983 Century Electronics UK
+raidersr3		// (c) 1983 Century Electronics UK
+hero			// (c) 1983 Seatongrove (c) 1984 CVS
+huncholy		// (c) 1984 Seatongrove (c) CVS
+
+// Zaccaria Hardware (similar to CVS)
+quasar			// (c) 1980 Zelco Games Italy
+quasara			// (c) 1980 Zelco Games Italy
+
+// Midway "Astrocade" games
+seawolf2		// (c) 1978
+spacezap		// (c) 1980
+ebases			// (c) 1980
+wow				// (c) 1980
+wowg			// (c) 1980
+gorf			// (c) 1981
+gorfpgm1		// (c) 1981
+gorfpgm1g		// (c) 1981
+robby			// (c) 1981 Bally Midway
+demndrgn		// (c) 1982 Bally Midway
+profpac			// (c) 1983 Bally Midway
+tenpindx		// (c) 1983 Bally Midway
+
+// Seibu Denshi / Seibu Kaihatsu games
+stinger			// (c) 1983 Seibu Denshi
+stinger2		// (c) 1983 Seibu Denshi
+scion			// (c) 1984 Seibu Denshi
+scionc			// (c) 1984 Seibu Denshi + Cinematronics license
+kungfut			// (c) 1984 Seibu Kaihatsu
+kungfuta		// (c) 1984 Seibu Kaihatsu
+wiz				// (c) 1985 Seibu Kaihatsu
+wizt			// (c) 1985 Taito Corporation
+wizta			// (c) 1985 Taito Corporation
+kncljoe			// (c) 1985 Taito Corporation
+kncljoea		// (c) 1985 Taito Corporation
+bcrusher		// bootleg
+empcity			// (c) 1986 Seibu Kaihatsu (bootleg?)
+empcityu		// (c) 1986 Taito (Romstar License)
+empcityj		// (c) 1986 Taito Corporation (Japan)
+empcityi		// (c) 1986 Seibu Kaihatsu (Eurobed license)
+stfight			// (c) 1986 Seibu Kaihatsu (Germany) (bootleg?)
+stfighta		// (c) 1986 Seibu Kaihatsu (bootleg?)
+mustache		// (c) 1987 March
+cshooter		// (c) 1987 Taito
+cshootere		// (c) 1987 JKH (bootleg)
+airraid			// (c) 1987 Seibu Kaihatsu
+panicr			// Seibu / Taito
+deadang			// (c) 1988 Seibu Kaihatsu
+leadang			// (c) 1988 Seibu Kaihatsu
+ghunter			// (c) 1988 Seibu Kaihatsu + Segasa/Sonic license
+dynduke			// (c) 1989 Seibu Kaihatsu + Fabtek license
+dyndukef		// (c) 1989 Seibu Kaihatsu + Fabtek license
+dbldyn			// (c) 1989 Seibu Kaihatsu + Fabtek license
+dbldynf			// (c) 1989 Seibu Kaihatsu + Fabtek license
+raiden			// (c) 1990 Seibu Kaihatsu
+raidena			// (c) 1990 Seibu Kaihatsu
+raidenk			// (c) 1990 Seibu Kaihatsu + IBL Corporation license
+raident			// (c) 1990 Seibu Kaihatsu + Liang HWA Electronics license
+raidenu			// (c) 1990 Seibu Kaihatsu + Fabtek license
+raidenua		// (c) 1990 Seibu Kaihatsu + Fabtek license
+sengokmj		// (c) 1991 Sigma
+totmejan		// (c) 1991 Seibu/Tecmo
+goodejan		// (c) 1991 Seibu/Tecmo
+goodejana		// (c) 1991 Seibu/Tecmo
+
+
+raiden2			// (c) 1993 Seibu Kaihatsu + Fabtek license
+raiden2a		// (c) 1993 Seibu Kaihatsu + Metrotainment license
+raiden2b		// (c) 1993 Seibu Kaihatsu
+raiden2c		// (c) 1993 Seibu Kaihatsu
+raiden2d		// (c) 1993 Seibu Kaihatsu
+raiden2e		// (c) 1993 Seibu Kaihatsu
+raiden2f		// (c) 1993 Seibu Kaihatsu + Fabtek license
+raiden2g		// (c) 1993 Seibu Kaihatsu
+raidndx			// (c) 1994 Seibu Kaihatsu
+raidndxj		// (c) 1994 Seibu Kaihatsu
+raidndxu		// (c) 1994 Seibu Kaihatsu + Fabtek license
+raidndxa1		// (c) 1994 Seibu Kaihatsu + Metrotainment license
+raidndxa2		// (c) 1994 Seibu Kaihatsu + Metrotainment license
+raidndxg		// (c) 1994 Seibu Kaihatsu + Tuning license
+zeroteam		// (c) 1993 Seibu Kaihatsu + Fabtek license
+zeroteama		// (c) 1993 Seibu Kaihatsu
+zeroteamb		// (c) 1993 Seibu Kaihatsu
+zeroteamc		// (c) 1993 Seibu Kaihatsu + Liang Hwa license
+zeroteamd		// (c) 1993 Seibu Kaihatsu + Dreamsoft license
+zeroteams		// (c) 1993 Seibu Kaihatsu
+zeroteamsr		// (c) 1993 Seibu Kaihatsu
+xsedae			// (c) 1995 Dream Island
+
+// Jaleco games
+exerion			// (c) 1983 Jaleco
+exeriont		// (c) 1983 Jaleco + Taito America license
+exerionb		// bootleg
+fcombat			// (c) 1985 Jaleco
+pturn			// (c) 1984 Jaleco
+ddayjlc			// (c) 1984 Jaleco
+ddayjlca		// (c) 1984 Jaleco
+formatz			// (c) 1984 Jaleco
+aeroboto		// (c) 1984 Williams
+citycon			// (c) 1985 Jaleco
+citycona		// (c) 1985 Jaleco
+cruisin			// (c) 1985 Jaleco/Kitkorp
+momoko			// (c) 1986 Jaleco
+argus			// (c) 1986 Jaleco
+valtric			// (c) 1986 Jaleco
+butasan			// (c) 1987 Jaleco - english subtitled
+butasanj		// (c) 1987 Jaleco
+psychic5		// (c) 1987 Jaleco
+psychic5a		// (c) 1987 Jaleco
+bombsa			// (c) 1988 Jaleco
+skyfox			// (c) 1987 Jaleco + Nichibutsu USA license
+exerizer		// (c) 1987? Jaleco
+exerizerb		// bootleg
+homerun			// (c) 1988 Jaleco
+dynashot		// (c) 1988 Jaleco
+
+// Sega System 1 / System 2 games
+starjack		// 834-5191 (c) 1983 (S1)
+starjacks		// (c) 1983 Stern (S1)
+regulus			// 834-5328?(c) 1983 (S1)
+reguluso		// 834-5328?(c) 1983 (S1)
+regulusu		// 834-5328?(c) 1983 (S1)
+upndown			// (c) 1983 (S1)
+upndownu		// (c) 1983 (S1)
+mrviking		// 834-5383 (c) 1984 (S1)
+mrvikingj		// 834-5383 (c) 1984 (S1)
+swat			// 834-5388 (c) 1984 Coreland / Sega (S1)
+flicky			// (c) 1984 (S1)
+flickys2		// (c) 1984 (S1)
+flickyo			// (c) 1984 (S1)
+flickys1		// (c) 1984 (S1)
+wmatch			// (c) 1984 (S1)
+bullfgt			// 834-5478 (c) 1984 Sega / Coreland (S1)
+thetogyu		// 834-5478 (c) 1984 Sega / Coreland (S1)
+spatter			// 834-5583 (c) 1984 (S1)
+ssanchan		// 834-5583 (c) 1984 (S1)
+pitfall2		// 834-5627 [1985?] reprogrammed, (c) 1984 Activision (S1)
+pitfall2a		// 834-5627 [1985?] reprogrammed, (c) 1984 Activision (S1)
+pitfall2u		// 834-5627 [1985?] reprogrammed, (c) 1984 Activision (S1)
+seganinj		// 834-5677 (c) 1985 (S1)
+seganinju		// 834-5677 (c) 1985 (S1)
+ninja			// 834-5677 (c) 1985 (S1)
+nprinces		// 834-5677 (c) 1985 (S1)
+nprinceso		// 834-5677 (c) 1985 (S1)
+nprincesu		// 834-5677 (c) 1985 (S1)
+nprincesb		// bootleg? (S1)
+imsorry			// 834-5707 (c) 1985 Coreland / Sega (S1)
+imsorryj		// 834-5707 (c) 1985 Coreland / Sega (S1)
+teddybb			// 834-5712 (c) 1985 (S1)
+teddybbo		// 834-5712 (c) 1985 (S1)
+hvymetal		// 834-5745 (c) 1985 (S2?)
+myhero			// 834-5755 (c) 1985 (S1)
+sscandal		// 834-5755 (c) 1985 Coreland / Sega (S1)
+myherok			// 834-5755 (c) 1985 Coreland / Sega (S1)
+shtngmst		// 834-5719/5720 (c) 1985 (S2)
+shtngmste		// 834-5719/5720 (c) 1985 (S2)
+choplift		// 834-5795 (c) 1985, (c) 1982 Dan Gorlin (S2)
+chopliftu		// 834-5795 (c) 1985, (c) 1982 Dan Gorlin (S2)
+chopliftbl		// bootleg (S2)
+4dwarrio		// 834-5918 (c) 1985 Coreland / Sega (S1)
+brain			// (c) 1986 Coreland / Sega (S2?)
+raflesia		// 834-5753 (c) 1985 Coreland / Sega (S1)
+wboy			// 834-5984 (c) 1986 + Escape license (S1)
+wboyo			// 834-5984 (c) 1986 + Escape license (S1)
+wboy2			// 834-5984 (c) 1986 + Escape license (S1)
+wboy2u			// 834-5984 (c) 1986 + Escape license (S1)
+wboy3			// 834-5984 (c) 1986 + Escape license (S1)
+wboy4			// 834-5984 (c) 1986 + Escape license (S1)
+wboyu			// 834-5753 (? maybe a conversion) (c) 1986 + Escape license (S1)
+wbdeluxe		// (c) 1986 + Escape license (S1)
+wboysys2		// 834-5984  (c) 1986 + Escape license (S1)
+gardia			// 834-6119 (S2?)
+gardiab			// bootleg
+nob				// (c) Data East
+nobb			// bootleg
+blockgal		// 834-6303 (S1)
+blockgalb		// bootleg
+tokisens		// (c) 1987 (from a bootleg board) (S2)
+wbml			// bootleg (S2)
+wbmljo			// (c) 1987 Sega/Westone (S2)
+wbmljb			// (c) 1987 Sega/Westone (S2)
+wbmlb			// bootleg? (S2)
+wbmlbg			// bootleg (Galaxy Electronics)
+dakkochn		// 836-6483? (S2)
+ufosensi		// 834-6659 (S2)
+ufosensib		// bootleg
+/*
+other System 1 / System 2 games:
+
+WarBall
+DokiDoki Penguin Land *not confirmed
+*/
+
+// later SNK games, each game can be identified by PCB code and ROM
+// code, the ROM code is the same between versions, and usually based
+// upon the Japanese title.
+lasso                   //       'WM' (c) 1982
+chameleo                // (c) 1983 Jaleco
+wwjgtin                 // (c) 1984 Jaleco / Casio
+photof                  // (c) 1984 Jaleco / Casio
+pinbo                   // (c) 1984 Jaleco
+pinboa                  // (c) 1984 Jaleco
+pinbos                  // (c) 1985 Strike
+joyfulr                 // A2001      (c) 1983
+mnchmobl                // A2001      (c) 1983 + Centuri license
+marvins                 // A2003      (c) 1983
+madcrash                // A2005      (c) 1984
+madcrush                // A2005      (c) 1984
+vangrd2                 // A2005      (c) 1984
+mainsnk                 //            (c) 1984
+canvas                  // A3008      (c) 1985
+jcross                  // A3006      (c) 1984
+sgladiat                // A3006      (c) 1984
+hal21                   // A4031      (c) 1985
+hal21j                  //            (c) 1985 (Japan)
+aso                             //            (c) 1985
+alphamis                //            (c) 1985
+arian                   //            (c) 1985
+tnk3                    // A5001      (c) 1985
+tnk3j                   // A5001      (c) 1985
+athena                  //       'UP' (c) 1986
+fitegolf                //       'GU' (c) 1988
+fitegolfu               //       'GU' (c) 1988
+countryc                // A7004 'CC' (c) 1988
+ikari                   // A5004 'IW' (c) 1986
+ikaria                  // A5004 'IW' (c) 1986
+ikarinc                 // A5004 'IW' (c) 1986
+ikarijp                 // A5004 'IW' (c) 1986 (Japan)
+ikarijpb                // bootleg
+victroad                // A6002      (c) 1986
+dogosoke                // A6002      (c) 1986
+dogosokb                // bootleg
+gwar                    // A7003 'GV' (c) 1987
+gwarj                   // A7003 'GV' (c) 1987 (Japan)
+gwara                   // A7003 'GV' (c) 1987
+gwarb                   // bootleg
+bermudat                //            (c) 1987
+bermudatj               //            (c) 1987
+bermudata               // A6004 'WW' (c) 1987
+worldwar                // A6004 'WW' (c) 1987
+psychos                 //       'PS' (c) 1987
+psychosj                //       'PS' (c) 1987 (Japan)
+chopper                 // A7003 'KK' (c) 1988
+choppera                // A7003 'KK' (c) 1988
+chopperb                // A7003 'KK' (c) 1988
+legofair                // A7003 'KK' (c) 1988
+fsoccer                 // A6006 'FS' (c) 1988
+fsoccerj                // A6006 'FS' (c) 1988
+fsoccerb                //            (c) 1988
+tdfever                 // A6006 'TD' (c) 1987
+tdfeverj                // A6006 'TD' (c) 1987
+tdfever2                // A6006 'TD'?(c) 1988
+
+// Kyugo games
+// Kyugo only made four games: Repulse, Flash Gal, SRD Mission and Air Wolf.
+// Gyrodine was made by Crux. Crux was antecedent of Toa Plan, and spin-off from Orca.
+gyrodine                // (c) 1984 Crux
+gyrodinet               // (c) 1984 Crux (Taito Corporation license)
+buzzard                 // (c) 1984 Crux
+repulse                 // (c) 1985 Sega
+99lstwar                // (c) 1985 Proma
+99lstwara               // (c) 1985 Proma
+99lstwark               // (c) 1985 Kyugo
+sonofphx                // bootleg
+flashgal                // (c) 1985 Sega
+flashgala               // (c) 1985 Sega
+srdmissn                // (c) 1986 Taito Corporation
+fx                              // bootleg
+legend                  // no copyright notice [1986 Sega/Coreland] (Arcade TV Game List - P.104, Left, 14 from top)
+legendb                 // no copyright notice [1986 Sega/Coreland] (Arcade TV Game List - P.104, Left, 14 from top)
+airwolf                 // (c) 1987 Kyugo
+airwolfa                // (c) 1987 Kyugo
+skywolf                 // bootleg
+skywolf2                // bootleg
+skywolf3                // bootleg
+
+// Tehkan / Tecmo games (Tehkan became Tecmo in 1986)
+senjyo                  // (c) 1983 Tehkan
+starforc                // (c) 1984 Tehkan
+starforce               // (c) 1984 Tehkan
+starforcb               // (c) 1984 Tehkan
+starforca               // (c) 1984 Tehkan
+megaforc                // (c) 1985 Tehkan + Video Ware license
+baluba                  // (c) 1986 Able Corp.
+bombjack                // (c) 1984 Tehkan
+bombjack2               // (c) 1984 Tehkan
+pbaction                // (c) 1985 Tehkan
+pbaction2               // (c) 1985 Tehkan
+pbaction3               // (c) 1985 Tehkan
+pbaction4               // (c) 1985 Tehkan
+pbaction5               // (c) 1985 Tehkan
+                                // 6009 Tank Busters
+ponttehk                // 6011 - (c) 1985 Tehkan
+lvpoker                 // ???? - (c) 1985 Tehkan
+lvcards                 // (c) 1985 Tehkan
+tehkanwc                // (c) 1985 Tehkan
+tehkanwcb               // bootleg
+tehkanwcc               // bootleg
+gridiron                // (c) 1985 Tehkan
+teedoff                 // 6102 - (c) 1986 Tecmo
+solomon                 // (c) 1986 Tecmo
+solomonj                // (c) 1986 Tecmo
+rygar                   // 6002 - (c) 1986 Tecmo
+rygar2                  // 6002 - (c) 1986 Tecmo
+rygar3                  // 6002 - (c) 1986 Tecmo
+rygarj                  // 6002 - (c) 1986 Tecmo
+gemini                  // (c) 1987 Tecmo
+silkworm                // 6217 - (c) 1988 Tecmo
+silkwormj               // 6217 - (c) 1988 Tecmo
+backfirt                // bootleg [(c) 1988 Tecmo]
+tbowl                   // 6206 - (c) 1987 Tecmo
+tbowlj                  // 6206 - (c) 1987 Tecmo
+wc90                    // (c) 1989 Tecmo
+wc90a                   // (c) 1989 Tecmo
+wc90b                   // (c) 1989 Tecmo
+wc90t                   // (c) 1989 Tecmo
+wc90b1                  // bootleg
+wc90b2                  // bootleg
+
+// Data East LD games
+begas			// (c) 1983
+begas1			// (c) 1983
+cobra			// (c) 1984
+rblaster		// (c) 1985
+
+// other Data East games
+madalien		// (c) 1980
+madaliena		// (c) 1980
+astrof			// (c) [1980?]
+astrof2			// (c) [1980?]
+astrof3			// (c) [1980?]
+abattle			// Sidam
+abattle2		// Sidam
+afire			// Rene Pierre
+acombat			// bootleg
+acombato		// bootleg
+sstarbtl		// bootleg
+spfghmk2		// (c) [1979] Data East Corporation
+spfghmk22		// (c) [1979] Data East Corporation
+tomahawk		// (c) 1980 Data East, Inc.
+tomahawk1		// (c) 1980 Data East, Inc.
+progolf			// (c) 1981
+progolfa		// (c) 1981
+deshoros		// (c) 1983
+prosoccr		// (c) 1983
+prosport		// (c) 1983
+prosporta		// (c) 1983
+boomrang		// (c) 1983
+boomranga		// (c) 1983
+kamikcab		// (c) 1984
+yellowcbj		// (c) 1984
+yellowcbb		// bootleg
+liberate		// (c) 1984  Data East Corporation
+dualaslt		// (c) 1984  Data East USA (US)
+liberateb		// bootleg
+bwings			// (c) 1984
+bwingso			// (c) 1984
+bwingsa			// (c) 1984
+zaviga			// (c)
+zavigaj			// (c)
+kchamp			// (c) 1984 Data East USA (US)
+karatedo		// (c) 1984 Data East Corporation (Japan)
+kchampvs		// (c) 1984 Data East USA (US)
+kchampvs2		// (c) 1984 Data East USA (US)
+karatevs		// (c) 1984 Data East Corporation (Japan)
+compgolf		// (c) 1985 Data East Corporation (Japan)
+compgolfo		// (c) 1985 Data East Corporation (Japan)
+cntsteer		// (c) 1985 Data East Corporation
+zerotrgt		// (c) 1985 Data East Corporation
+zerotrgta		// (c) 1985 Data East Corporation
+gekitsui		// (c) 1985 Data East Corporation (Japan)
+tryout			// (c) 1985 Data East Corporation (Japan)
+firetrap		// (c) 1986 Data East USA (US)
+firetrapj		// (c) 1986 Wood Place Inc. (Japan)
+firetrapbl		// bootleg
+metlclsh		// (c) 1985 Data East
+chanbara		// (c) 1985 Data East
+brkthru			// (c) 1986 Data East USA (US)
+brkthruj		// (c) 1986 Data East Corporation (Japan)
+forcebrk		// (c) 1986 Data East Corporation (Japan)
+darwin			// (c) 1986 Data East Corporation (Japan)
+shootout		// (c) 1985 Data East USA (US)
+shootoutj		// (c) 1985 Data East USA (Japan)
+shootoutb		// bootleg
+sidepckt		// (c) 1986 Data East Corporation
+sidepcktj		// (c) 1986 Data East Corporation
+sidepcktb		// bootleg
+exprraid		// (c) 1986 Data East USA (US)
+exprraida		// (c) 1986 Data East USA (US)
+wexpress		// (c) 1986 Data East Corporation (World?)
+wexpressb		// bootleg
+wexpressb2		// bootleg
+pcktgal			// (c) 1987 Data East Corporation (Japan)
+pcktgalb		// bootleg
+pcktgal2		// (c) 1989 Data East Corporation (World?)
+pcktgal2j		// (c) 1989 Data East Corporation (World?)
+spool3			// (c) 1989 Data East Corporation (World?)
+spool3i			// (c) 1990 Data East Corporation + I-Vics license
+actfancr		// (c) 1989 Data East Corporation (World)
+actfancr1		// (c) 1989 Data East Corporation (World)
+actfancrj		// (c) 1989 Data East Corporation (Japan)
+triothep		// (c) 1989 Data East Corporation (World)
+triothepj		// (c) 1989 Data East Corporation (Japan)
+
+// Data East 8-bit games
+lastmisn		// (c) 1986 Data East USA (US)
+lastmisno		// (c) 1986 Data East USA (US)
+lastmisnj		// (c) 1986 Data East Corporation (Japan)
+shackled		// (c) 1986 Data East USA (US)
+breywood		// (c) 1986 Data East Corporation (Japan)
+csilver			// (c) 1987 Data East Corporation (Japan)
+csilverj		// (c) 1987 Data East Corporation (Japan)
+ghostb			// (c) 1987 Data East USA (US)
+ghostb2a		// (c) 1987 Data East USA (US)
+ghostb3			// (c) 1987 Data East USA (US)
+meikyuh			// (c) 1987 Data East Corporation (Japan)
+meikyuha		// (c) 1987 Data East Corporation (Japan)
+srdarwin		// (c) 1987 Data East Corporation (World)
+srdarwinj		// (c) 1987 Data East Corporation (Japan)
+gondo			// (c) 1987 Data East USA (US)
+makyosen		// (c) 1987 Data East Corporation (Japan)
+garyoret		// (c) 1987 Data East Corporation (Japan)
+cobracom		// (c) 1988 Data East Corporation (World)
+cobracomj		// (c) 1988 Data East Corporation (Japan)
+oscar			// (c) 1988 Data East Corporation (World)
+oscaru			// (c) 1987 Data East USA (US)
+oscarj1			// (c) 1987 Data East Corporation (Japan)
+oscarj2			// (c) 1987 Data East Corporation (Japan)
