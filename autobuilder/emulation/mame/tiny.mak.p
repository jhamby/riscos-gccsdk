--- src/mame/tiny.mak.orig	2012-04-11 10:56:45.000000000 +0100
+++ src/mame/tiny.mak	2012-04-09 13:10:09.000000000 +0100
@@ -1,103 +1,535 @@
-###########################################################################
-#
-#   tiny.mak
-#
-#   Small driver-specific example makefile
-#	Use make SUBTARGET=tiny to build
-#
-#   Copyright Nicola Salmoria and the MAME Team.
-#   Visit  http://mamedev.org for licensing and usage restrictions.
-#
-###########################################################################
-
-MAMESRC = $(SRC)/mame
-MAMEOBJ = $(OBJ)/mame
-
-AUDIO = $(MAMEOBJ)/audio
-DRIVERS = $(MAMEOBJ)/drivers
-LAYOUT = $(MAMEOBJ)/layout
-MACHINE = $(MAMEOBJ)/machine
-VIDEO = $(MAMEOBJ)/video
-
-OBJDIRS += \
-	$(AUDIO) \
-	$(DRIVERS) \
-	$(LAYOUT) \
-	$(MACHINE) \
-	$(VIDEO) \
-
-
-
-#-------------------------------------------------
-# Specify all the CPU cores necessary for the
-# drivers referenced in tiny.c.
-#-------------------------------------------------
-
-CPUS += Z80
-CPUS += M6502
-CPUS += MCS48
-CPUS += MCS51
-CPUS += M6800
-CPUS += M6809
-CPUS += M680X0
-CPUS += TMS9900
-CPUS += COP400
-
-
-
-#-------------------------------------------------
-# Specify all the sound cores necessary for the
-# drivers referenced in tiny.c.
-#-------------------------------------------------
-
-SOUNDS += SAMPLES
-SOUNDS += DAC
-SOUNDS += DISCRETE
-SOUNDS += AY8910
-SOUNDS += YM2151
-SOUNDS += ASTROCADE
-SOUNDS += TMS5220
-SOUNDS += OKIM6295
-SOUNDS += HC55516
-SOUNDS += YM3812
-SOUNDS += CEM3394
-
-
-
-#-------------------------------------------------
-# This is the list of files that are necessary
-# for building all of the drivers referenced
-# in tiny.c
-#-------------------------------------------------
-
-DRVLIBS = \
-	$(EMUDRIVERS)/emudummy.o \
-	$(MACHINE)/ticket.o \
-	$(DRIVERS)/carpolo.o $(MACHINE)/carpolo.o $(VIDEO)/carpolo.o \
-	$(DRIVERS)/circus.o $(AUDIO)/circus.o $(VIDEO)/circus.o \
-	$(DRIVERS)/exidy.o $(AUDIO)/exidy.o $(VIDEO)/exidy.o \
-	$(AUDIO)/exidy440.o \
-	$(DRIVERS)/starfire.o $(VIDEO)/starfire.o \
-	$(DRIVERS)/vertigo.o $(MACHINE)/vertigo.o $(VIDEO)/vertigo.o \
-	$(DRIVERS)/victory.o $(VIDEO)/victory.o \
-	$(AUDIO)/targ.o \
-	$(DRIVERS)/astrocde.o $(VIDEO)/astrocde.o \
-	$(DRIVERS)/gridlee.o $(AUDIO)/gridlee.o $(VIDEO)/gridlee.o \
-	$(DRIVERS)/williams.o $(MACHINE)/williams.o $(AUDIO)/williams.o $(VIDEO)/williams.o \
-	$(AUDIO)/gorf.o \
-	$(AUDIO)/wow.o \
-	$(DRIVERS)/gaelco.o $(VIDEO)/gaelco.o $(MACHINE)/gaelcrpt.o \
-	$(DRIVERS)/wrally.o $(MACHINE)/wrally.o $(VIDEO)/wrally.o \
-	$(DRIVERS)/looping.o \
-	$(DRIVERS)/supertnk.o \
-
-
-
-#-------------------------------------------------
-# layout dependencies
-#-------------------------------------------------
-
-$(DRIVERS)/astrocde.o:	$(LAYOUT)/gorf.lh \
-						$(LAYOUT)/tenpindx.lh
-$(DRIVERS)/circus.o:	$(LAYOUT)/circus.lh \
-						$(LAYOUT)/crash.lh
+###########################################################################
+#
+#   tiny.mak
+#
+#   Small driver-specific example makefile
+#	Use make SUBTARGET=tiny to build
+#
+#   Copyright Nicola Salmoria and the MAME Team.
+#   Visit  http://mamedev.org for licensing and usage restrictions.
+#
+###########################################################################
+
+MAMESRC = $(SRC)/mame
+MAMEOBJ = $(OBJ)/mame
+
+AUDIO = $(MAMEOBJ)/audio
+DRIVERS = $(MAMEOBJ)/drivers
+LAYOUT = $(MAMEOBJ)/layout
+MACHINE = $(MAMEOBJ)/machine
+VIDEO = $(MAMEOBJ)/video
+
+OBJDIRS += \
+	$(AUDIO) \
+	$(DRIVERS) \
+	$(LAYOUT) \
+	$(MACHINE) \
+	$(VIDEO) \
+
+
+
+#-------------------------------------------------
+# Specify all the CPU cores necessary for the
+# drivers referenced in tiny.c.
+#-------------------------------------------------
+
+CPUS += Z80
+CPUS += M6502
+CPUS += MCS48
+CPUS += MCS51
+CPUS += M6800
+CPUS += M6805
+CPUS += M6809
+CPUS += TMS9900
+CPUS += TMS32031
+CPUS += COP400
+CPUS += I8085
+CPUS += I86
+CPUS += S2650
+CPUS += MB88XX
+CPUS += T11
+CPUS += HD6309
+CPUS += V30MZ
+CPUS += NEC
+CPUS += UPD7725
+CPUS += SPC700
+CPUS += G65816
+CPUS += SUPERFX
+CPUS += PIC16C5X
+CPUS += TMS340X0
+CPUS += I386
+CPUS += KONAMI
+CPUS += ARM7
+CPUS += ARM
+CPUS += Z8000
+CPUS += Z180
+CPUS += ADSP21XX
+CPUS += ADSP21062
+CPUS += H6280
+#CPUS += M680X0
+
+#-------------------------------------------------
+# Specify all the sound cores necessary for the
+# drivers referenced in tiny.c.
+#-------------------------------------------------
+
+SOUNDS += SAMPLES
+SOUNDS += DAC
+SOUNDS += DISCRETE
+SOUNDS += AY8910
+SOUNDS += YM2151
+SOUNDS += YM2203
+SOUNDS += ASTROCADE
+SOUNDS += TMS5220
+SOUNDS += TMS5110
+SOUNDS += OKIM6295
+SOUNDS += HC55516
+SOUNDS += K005289
+SOUNDS += YM3812
+SOUNDS += YM2608
+SOUNDS += YM2610
+SOUNDS += CEM3394
+SOUNDS += SN76477
+SOUNDS += SN76496
+SOUNDS += TMS36XX
+SOUNDS += POKEY
+SOUNDS += VLM5030
+SOUNDS += MSM5205
+SOUNDS += VLM5030
+SOUNDS += SP0250
+SOUNDS += DIGITALKER
+SOUNDS += NES
+SOUNDS += S14001A
+SOUNDS += Y8950
+SOUNDS += YM3526
+SOUNDS += SNKWAVE
+SOUNDS += NAMCO
+SOUNDS += NAMCO_15XX
+SOUNDS += NAMCO_CUS30
+SOUNDS += NAMCO_52XX
+SOUNDS += NAMCO_63701X
+SOUNDS += YM2413
+SOUNDS += IREMGA20
+SOUNDS += SPEAKER
+SOUNDS += DMADAC
+SOUNDS += S2636
+SOUNDS += CDDA
+SOUNDS += UPD7759
+SOUNDS += K007232
+SOUNDS += K051649
+SOUNDS += K053260
+SOUNDS += K054539
+SOUNDS += C140
+SOUNDS += YMF262
+SOUNDS += YMZ280B
+SOUNDS += YM3438
+SOUNDS += RF5C68
+SOUNDS += K056800
+SOUNDS += MSM5232
+SOUNDS += ES5505
+
+#-------------------------------------------------
+# This is the list of files that are necessary
+# for building all of the drivers referenced
+# in tiny.c
+#-------------------------------------------------
+
+
+DRVLIBS = \
+	$(EMUDRIVERS)/emudummy.o \
+        $(DRIVERS)/epos.o $(VIDEO)/epos.o \
+        $(DRIVERS)/circusc.o $(VIDEO)/circusc.o \
+        $(DRIVERS)/ddribble.o $(VIDEO)/ddribble.o \
+        $(DRIVERS)/combatsc.o $(VIDEO)/combatsc.o \
+        $(DRIVERS)/contra.o $(VIDEO)/contra.o \
+        $(DRIVERS)/fastfred.o $(VIDEO)/fastfred.o \
+        $(DRIVERS)/fastlane.o $(VIDEO)/fastlane.o \
+        $(DRIVERS)/finalizr.o $(VIDEO)/finalizr.o \
+        $(DRIVERS)/flkatck.o $(VIDEO)/flkatck.o \
+        $(DRIVERS)/gberet.o $(VIDEO)/gberet.o \
+        $(DRIVERS)/gyruss.o $(VIDEO)/gyruss.o \
+        $(DRIVERS)/hexion.o $(VIDEO)/hexion.o \
+        $(DRIVERS)/hyperspt.o $(AUDIO)/hyprolyb.o $(VIDEO)/hyperspt.o \
+        $(DRIVERS)/ironhors.o $(VIDEO)/ironhors.o \
+        $(DRIVERS)/jackal.o $(VIDEO)/jackal.o \
+        $(DRIVERS)/jailbrek.o $(VIDEO)/jailbrek.o \
+        $(DRIVERS)/junofrst.o \
+        $(DRIVERS)/konendev.o \
+        $(DRIVERS)/labyrunr.o $(VIDEO)/labyrunr.o \
+        $(DRIVERS)/megazone.o $(VIDEO)/megazone.o \
+        $(DRIVERS)/mikie.o $(VIDEO)/mikie.o \
+        $(DRIVERS)/mogura.o \
+        $(DRIVERS)/pandoras.o $(VIDEO)/pandoras.o \
+        $(DRIVERS)/pingpong.o $(VIDEO)/pingpong.o \
+        $(DRIVERS)/pooyan.o $(VIDEO)/pooyan.o \
+        $(DRIVERS)/rocnrope.o $(VIDEO)/rocnrope.o \
+        $(DRIVERS)/rockrage.o $(VIDEO)/rockrage.o \
+        $(DRIVERS)/sbasketb.o $(VIDEO)/sbasketb.o \
+        $(DRIVERS)/scobra.o \
+        $(DRIVERS)/scotrsht.o $(VIDEO)/scotrsht.o \
+        $(DRIVERS)/scramble.o $(MACHINE)/scramble.o $(AUDIO)/scramble.o \
+        $(DRIVERS)/shaolins.o $(VIDEO)/shaolins.o \
+        $(DRIVERS)/tgtpanic.o \
+        $(DRIVERS)/timeplt.o $(AUDIO)/timeplt.o $(VIDEO)/timeplt.o \
+        $(DRIVERS)/tp84.o $(VIDEO)/tp84.o \
+        $(DRIVERS)/trackfld.o $(MACHINE)/konami1.o $(AUDIO)/trackfld.o $(VIDEO)/trackfld.o \
+        $(DRIVERS)/tutankhm.o $(VIDEO)/tutankhm.o \
+        $(DRIVERS)/yiear.o $(VIDEO)/yiear.o \
+        $(VIDEO)/konamiic.o $(VIDEO)/konicdev.o \
+        $(DRIVERS)/calchase.o \
+        $(DRIVERS)/pangofun.o \
+        $(DRIVERS)/pcat_dyn.o \
+        $(DRIVERS)/pcat_nit.o \
+        $(DRIVERS)/pcxt.o \
+        $(DRIVERS)/quakeat.o \
+        $(DRIVERS)/queen.o \
+        $(DRIVERS)/igspc.o \
+        $(DRIVERS)/carpolo.o $(MACHINE)/carpolo.o $(VIDEO)/carpolo.o \
+        $(DRIVERS)/circus.o $(AUDIO)/circus.o $(VIDEO)/circus.o \
+        $(DRIVERS)/exidy.o $(AUDIO)/exidy.o $(VIDEO)/exidy.o \
+        $(DRIVERS)/exidy440.o $(AUDIO)/exidy440.o $(VIDEO)/exidy440.o \
+        $(DRIVERS)/maxaflex.o $(MACHINE)/atari.o $(VIDEO)/atari.o $(VIDEO)/antic.o $(VIDEO)/gtia.o \
+        $(DRIVERS)/starfire.o $(VIDEO)/starfire.o \
+        $(DRIVERS)/victory.o $(VIDEO)/victory.o \
+        $(AUDIO)/targ.o \
+	$(MACHINE)/ticket.o \
+	$(DRIVERS)/williams.o $(MACHINE)/williams.o $(AUDIO)/williams.o $(VIDEO)/williams.o \
+        $(DRIVERS)/astrocde.o $(VIDEO)/astrocde.o \
+        $(DRIVERS)/gridlee.o $(AUDIO)/gridlee.o $(VIDEO)/gridlee.o \
+        $(DRIVERS)/midtunit.o $(MACHINE)/midtunit.o $(VIDEO)/midtunit.o \
+        $(DRIVERS)/midvunit.o $(VIDEO)/midvunit.o \
+        $(DRIVERS)/midwunit.o $(MACHINE)/midwunit.o \
+        $(DRIVERS)/midxunit.o $(MACHINE)/midxunit.o \
+        $(DRIVERS)/midyunit.o $(MACHINE)/midyunit.o $(VIDEO)/midyunit.o \
+        $(DRIVERS)/midzeus.o $(VIDEO)/midzeus.o $(VIDEO)/midzeus2.o \
+        $(DRIVERS)/omegrace.o \
+        $(DRIVERS)/williams.o $(MACHINE)/williams.o $(AUDIO)/williams.o $(VIDEO)/williams.o \
+        $(MACHINE)/midwayic.o \
+        $(AUDIO)/dcs.o \
+        $(AUDIO)/gorf.o \
+        $(AUDIO)/wow.o \
+	$(DRIVERS)/looping.o \
+        $(DRIVERS)/spcforce.o $(VIDEO)/spcforce.o \
+        $(DRIVERS)/suprridr.o $(VIDEO)/suprridr.o \
+	$(DRIVERS)/supertnk.o \
+        $(DRIVERS)/cheekyms.o $(VIDEO)/cheekyms.o \
+        $(DRIVERS)/cosmic.o $(VIDEO)/cosmic.o \
+        $(DRIVERS)/docastle.o $(MACHINE)/docastle.o $(VIDEO)/docastle.o \
+        $(DRIVERS)/ladybug.o $(VIDEO)/ladybug.o \
+        $(DRIVERS)/mrdo.o $(VIDEO)/mrdo.o \
+        $(DRIVERS)/redclash.o $(VIDEO)/redclash.o \
+        $(DRIVERS)/superdq.o \
+        $(DRIVERS)/20pacgal.o $(VIDEO)/20pacgal.o \
+        $(DRIVERS)/galaga.o $(AUDIO)/galaga.o $(VIDEO)/galaga.o \
+        $(DRIVERS)/galaxian.o $(AUDIO)/galaxian.o $(VIDEO)/galaxian.o \
+        $(DRIVERS)/galaxold.o $(MACHINE)/galaxold.o $(VIDEO)/galaxold.o \
+        $(DRIVERS)/gaplus.o $(MACHINE)/gaplus.o $(VIDEO)/gaplus.o \
+        $(DRIVERS)/mappy.o $(VIDEO)/mappy.o \
+        $(DRIVERS)/pacland.o $(VIDEO)/pacland.o \
+        $(DRIVERS)/polepos.o $(AUDIO)/polepos.o $(VIDEO)/polepos.o \
+        $(DRIVERS)/rallyx.o $(VIDEO)/rallyx.o \
+        $(DRIVERS)/skykid.o $(VIDEO)/skykid.o \
+        $(DRIVERS)/tankbatt.o $(VIDEO)/tankbatt.o \
+        $(DRIVERS)/warpwarp.o $(AUDIO)/geebee.o $(AUDIO)/warpwarp.o $(VIDEO)/warpwarp.o \
+        $(DRIVERS)/asteroid.o $(MACHINE)/asteroid.o $(AUDIO)/asteroid.o $(AUDIO)/llander.o \
+        $(DRIVERS)/phoenix.o $(AUDIO)/phoenix.o $(VIDEO)/phoenix.o \
+        $(AUDIO)/pleiads.o \
+        $(DRIVERS)/safarir.o \
+        $(DRIVERS)/missile.o \
+        $(DRIVERS)/cham24.o \
+        $(DRIVERS)/dkong.o $(AUDIO)/dkong.o $(VIDEO)/dkong.o \
+        $(DRIVERS)/mario.o $(AUDIO)/mario.o $(VIDEO)/mario.o \
+        $(DRIVERS)/multigam.o \
+        $(DRIVERS)/n8080.o $(AUDIO)/n8080.o $(VIDEO)/n8080.o \
+        $(DRIVERS)/nss.o $(MACHINE)/snes.o $(AUDIO)/snes_snd.o $(VIDEO)/snes.o \
+        $(DRIVERS)/playch10.o $(MACHINE)/playch10.o $(VIDEO)/playch10.o \
+        $(DRIVERS)/popeye.o $(VIDEO)/popeye.o \
+        $(DRIVERS)/punchout.o $(VIDEO)/punchout.o \
+        $(DRIVERS)/famibox.o \
+        $(DRIVERS)/sfcbox.o \
+        $(DRIVERS)/snesb.o \
+        $(DRIVERS)/spacefb.o $(AUDIO)/spacefb.o  $(VIDEO)/spacefb.o \
+        $(DRIVERS)/vsnes.o $(MACHINE)/vsnes.o $(VIDEO)/vsnes.o \
+        $(VIDEO)/ppu2c0x.o \
+        $(DRIVERS)/scramble.o $(MACHINE)/scramble.o $(AUDIO)/scramble.o \
+        $(DRIVERS)/centiped.o $(VIDEO)/centiped.o \
+        $(DRIVERS)/gottlieb.o $(AUDIO)/gottlieb.o $(VIDEO)/gottlieb.o \
+        $(DRIVERS)/zaxxon.o $(AUDIO)/zaxxon.o $(VIDEO)/zaxxon.o \
+        $(MACHINE)/nmk112.o \
+        $(MACHINE)/pckeybrd.o \
+        $(MACHINE)/pcshare.o \
+        $(MACHINE)/segacrpt.o \
+        $(MACHINE)/segacrp2.o \
+        $(MACHINE)/ticket.o \
+        $(VIDEO)/avgdvg.o \
+        $(DRIVERS)/mazerbla.o \
+        $(MACHINE)/atari_vg.o \
+        $(DRIVERS)/cmmb.o \
+        $(DRIVERS)/runaway.o $(VIDEO)/runaway.o \
+        $(DRIVERS)/segag80r.o $(MACHINE)/segag80.o $(AUDIO)/segag80r.o $(VIDEO)/segag80r.o \
+        $(AUDIO)/segasnd.o \
+        $(AUDIO)/taitosnd.o \
+        $(DRIVERS)/bking.o $(VIDEO)/bking.o \
+        $(DRIVERS)/buggychl.o $(MACHINE)/buggychl.o $(VIDEO)/buggychl.o \
+        $(DRIVERS)/chaknpop.o $(MACHINE)/chaknpop.o $(VIDEO)/chaknpop.o \
+        $(DRIVERS)/changela.o $(VIDEO)/changela.o \
+        $(DRIVERS)/crbaloon.o $(VIDEO)/crbaloon.o $(AUDIO)/crbaloon.o \
+        $(DRIVERS)/fgoal.o $(VIDEO)/fgoal.o \
+        $(DRIVERS)/grchamp.o $(AUDIO)/grchamp.o $(VIDEO)/grchamp.o \
+        $(DRIVERS)/gsword.o $(MACHINE)/tait8741.o $(VIDEO)/gsword.o \
+        $(DRIVERS)/jollyjgr.o \
+        $(DRIVERS)/lkage.o $(MACHINE)/lkage.o $(VIDEO)/lkage.o \
+        $(DRIVERS)/marinedt.o \
+        $(DRIVERS)/msisaac.o $(VIDEO)/msisaac.o \
+        $(DRIVERS)/pitnrun.o $(MACHINE)/pitnrun.o $(VIDEO)/pitnrun.o \
+        $(DRIVERS)/sbowling.o \
+        $(AUDIO)/t5182.o \
+        $(MACHINE)/taitoio.o \
+        $(DRIVERS)/tempest.o \
+        $(DRIVERS)/astinvad.o \
+        $(DRIVERS)/berzerk.o \
+        $(DRIVERS)/cliffhgr.o $(AUDIO)/cliffhgr.o \
+        $(DRIVERS)/mazerbla.o \
+        $(DRIVERS)/supdrapo.o \
+        $(MACHINE)/mathbox.o \
+        $(AUDIO)/carnival.o \
+        $(DRIVERS)/slotcarn.o \
+        $(AUDIO)/depthch.o \
+        $(AUDIO)/invinco.o \
+        $(AUDIO)/pulsar.o \
+        $(DRIVERS)/dmndrby.o \
+        $(DRIVERS)/lasso.o $(VIDEO)/lasso.o \
+        $(DRIVERS)/mainsnk.o $(VIDEO)/mainsnk.o \
+        $(DRIVERS)/munchmo.o $(VIDEO)/munchmo.o \
+        $(DRIVERS)/snk6502.o $(AUDIO)/snk6502.o $(VIDEO)/snk6502.o \
+        $(DRIVERS)/snk.o $(VIDEO)/snk.o \
+        $(DRIVERS)/jrpacman.o \
+        $(DRIVERS)/pacman.o $(VIDEO)/pacman.o \
+        $(DRIVERS)/pengo.o \
+        $(MACHINE)/acitya.o \
+        $(MACHINE)/jumpshot.o \
+        $(MACHINE)/pacplus.o \
+        $(MACHINE)/theglobp.o \
+        $(VIDEO)/digdug.o \
+        $(MACHINE)/xevious.o $(VIDEO)/xevious.o \
+        $(DRIVERS)/namcos1.o $(MACHINE)/namcos1.o $(VIDEO)/namcos1.o \
+        $(DRIVERS)/namcos86.o $(VIDEO)/namcos86.o \
+        $(MACHINE)/namcoio.o \
+        $(MACHINE)/namco06.o \
+        $(MACHINE)/namco50.o \
+        $(MACHINE)/namco51.o \
+        $(MACHINE)/namco53.o \
+        $(MACHINE)/namco62.o \
+        $(AUDIO)/namco52.o \
+        $(AUDIO)/namco54.o \
+        $(VIDEO)/bosco.o \
+        $(AUDIO)/cage.o \
+        $(VIDEO)/atarimo.o \
+        $(DRIVERS)/1942.o $(VIDEO)/1942.o \
+        $(DRIVERS)/1943.o $(VIDEO)/1943.o \
+        $(DRIVERS)/blktiger.o $(VIDEO)/blktiger.o \
+        $(DRIVERS)/cbasebal.o $(VIDEO)/cbasebal.o \
+        $(DRIVERS)/commando.o $(VIDEO)/commando.o \
+        $(DRIVERS)/egghunt.o \
+        $(DRIVERS)/exedexes.o $(VIDEO)/exedexes.o \
+        $(DRIVERS)/gng.o $(VIDEO)/gng.o \
+        $(DRIVERS)/gunsmoke.o $(VIDEO)/gunsmoke.o \
+        $(DRIVERS)/higemaru.o $(VIDEO)/higemaru.o \
+        $(DRIVERS)/lwings.o $(VIDEO)/lwings.o \
+        $(DRIVERS)/mitchell.o $(VIDEO)/mitchell.o \
+        $(DRIVERS)/sidearms.o $(VIDEO)/sidearms.o \
+        $(DRIVERS)/sonson.o $(VIDEO)/sonson.o \
+        $(DRIVERS)/srumbler.o $(VIDEO)/srumbler.o \
+        $(DRIVERS)/vulgus.o $(VIDEO)/vulgus.o \
+        $(MACHINE)/kabuki.o \
+        $(DRIVERS)/m10.o $(VIDEO)/m10.o \
+        $(DRIVERS)/m14.o \
+        $(DRIVERS)/m52.o $(VIDEO)/m52.o \
+        $(DRIVERS)/m57.o $(VIDEO)/m57.o \
+        $(DRIVERS)/m58.o $(VIDEO)/m58.o \
+        $(DRIVERS)/m62.o $(VIDEO)/m62.o \
+        $(DRIVERS)/m63.o \
+        $(DRIVERS)/m72.o $(AUDIO)/m72.o $(VIDEO)/m72.o \
+        $(DRIVERS)/m90.o $(VIDEO)/m90.o \
+        $(DRIVERS)/m92.o $(VIDEO)/m92.o \
+        $(DRIVERS)/m107.o $(VIDEO)/m107.o \
+        $(DRIVERS)/olibochu.o \
+        $(DRIVERS)/redalert.o $(AUDIO)/redalert.o $(VIDEO)/redalert.o \
+        $(DRIVERS)/shisen.o $(VIDEO)/shisen.o \
+        $(DRIVERS)/travrusa.o $(VIDEO)/travrusa.o \
+        $(DRIVERS)/vigilant.o $(VIDEO)/vigilant.o \
+        $(MACHINE)/irem_cpu.o \
+        $(AUDIO)/irem.o \
+        $(DRIVERS)/sstrangr.o \
+        $(DRIVERS)/8080bw.o $(AUDIO)/8080bw.o $(VIDEO)/8080bw.o \
+        $(DRIVERS)/m79amb.o $(AUDIO)/m79amb.o \
+        $(DRIVERS)/mw18w.o \
+        $(DRIVERS)/mw8080bw.o $(MACHINE)/mw8080bw.o $(AUDIO)/mw8080bw.o $(VIDEO)/mw8080bw.o \
+        $(DRIVERS)/rotaryf.o \
+        $(DRIVERS)/sspeedr.o $(VIDEO)/sspeedr.o \
+        $(DRIVERS)/mainsnk.o $(VIDEO)/mainsnk.o \
+        $(DRIVERS)/snk6502.o $(AUDIO)/snk6502.o $(VIDEO)/snk6502.o \
+        $(DRIVERS)/vicdual.o $(AUDIO)/vicdual.o $(VIDEO)/vicdual.o \
+        $(DRIVERS)/cclimber.o $(MACHINE)/cclimber.o $(AUDIO)/cclimber.o $(VIDEO)/cclimber.o \
+        $(DRIVERS)/arkanoid.o $(MACHINE)/arkanoid.o $(VIDEO)/arkanoid.o \
+        $(DRIVERS)/cvs.o $(VIDEO)/cvs.o \
+        $(DRIVERS)/quasar.o $(VIDEO)/quasar.o \
+        $(DRIVERS)/lazercmd.o $(VIDEO)/lazercmd.o \
+        $(DRIVERS)/meadows.o $(AUDIO)/meadows.o $(VIDEO)/meadows.o \
+        $(DRIVERS)/warpsped.o \
+        $(VIDEO)/sknsspr.o \
+        $(DRIVERS)/baraduke.o $(VIDEO)/baraduke.o \
+        $(DRIVERS)/cshooter.o \
+        $(DRIVERS)/deadang.o $(VIDEO)/deadang.o \
+        $(DRIVERS)/dynduke.o $(VIDEO)/dynduke.o \
+        $(DRIVERS)/goodejan.o \
+        $(DRIVERS)/kncljoe.o $(VIDEO)/kncljoe.o \
+        $(DRIVERS)/mustache.o $(VIDEO)/mustache.o \
+        $(DRIVERS)/panicr.o \
+        $(DRIVERS)/raiden.o $(VIDEO)/raiden.o \
+        $(DRIVERS)/raiden2.o $(MACHINE)/r2crypt.o \
+        $(DRIVERS)/r2dx_v33.o \
+        $(DRIVERS)/sengokmj.o \
+        $(DRIVERS)/stfight.o $(MACHINE)/stfight.o $(VIDEO)/stfight.o \
+        $(DRIVERS)/wiz.o $(VIDEO)/wiz.o \
+        $(MACHINE)/spisprit.o \
+        $(AUDIO)/seibu.o \
+        $(VIDEO)/sei_crtc.o \
+     $(DRIVERS)/aeroboto.o $(VIDEO)/aeroboto.o \
+        $(DRIVERS)/argus.o $(VIDEO)/argus.o \
+        $(DRIVERS)/blueprnt.o $(VIDEO)/blueprnt.o \
+        $(DRIVERS)/citycon.o $(VIDEO)/citycon.o \
+        $(DRIVERS)/ddayjlc.o \
+        $(DRIVERS)/exerion.o $(VIDEO)/exerion.o \
+        $(DRIVERS)/fcombat.o $(VIDEO)/fcombat.o \
+        $(DRIVERS)/homerun.o $(VIDEO)/homerun.o \
+        $(DRIVERS)/momoko.o $(VIDEO)/momoko.o \
+        $(DRIVERS)/psychic5.o $(VIDEO)/psychic5.o \
+        $(DRIVERS)/pturn.o \
+        $(DRIVERS)/skyfox.o $(VIDEO)/skyfox.o \
+        $(MACHINE)/jalcrpt.o \
+        $(VIDEO)/jalblend.o \
+        $(VIDEO)/taitoic.o \
+        $(DRIVERS)/system1.o $(VIDEO)/system1.o \
+        $(MACHINE)/fd1089.o \
+        $(MACHINE)/fd1094.o \
+        $(MACHINE)/mc8123.o \
+        $(DRIVERS)/kyugo.o $(VIDEO)/kyugo.o \
+        $(DRIVERS)/bombjack.o $(VIDEO)/bombjack.o \
+        $(DRIVERS)/lvcards.o $(VIDEO)/lvcards.o \
+        $(DRIVERS)/pbaction.o $(VIDEO)/pbaction.o \
+        $(DRIVERS)/senjyo.o $(AUDIO)/senjyo.o $(VIDEO)/senjyo.o \
+        $(DRIVERS)/solomon.o $(VIDEO)/solomon.o \
+        $(DRIVERS)/tbowl.o $(VIDEO)/tbowl.o \
+        $(DRIVERS)/tecmo.o $(VIDEO)/tecmo.o \
+        $(DRIVERS)/tehkanwc.o $(VIDEO)/tehkanwc.o \
+        $(DRIVERS)/wc90.o $(VIDEO)/wc90.o \
+        $(DRIVERS)/wc90b.o $(VIDEO)/wc90b.o \
+        $(DRIVERS)/actfancr.o $(VIDEO)/actfancr.o \
+        $(DRIVERS)/astrof.o $(AUDIO)/astrof.o \
+        $(DRIVERS)/backfire.o \
+        $(DRIVERS)/brkthru.o $(VIDEO)/brkthru.o \
+        $(DRIVERS)/btime.o $(MACHINE)/btime.o $(VIDEO)/btime.o \
+        $(DRIVERS)/bwing.o $(VIDEO)/bwing.o \
+        $(DRIVERS)/chanbara.o \
+        $(DRIVERS)/cntsteer.o \
+        $(DRIVERS)/compgolf.o $(VIDEO)/compgolf.o \
+        $(DRIVERS)/dec8.o $(VIDEO)/dec8.o \
+        $(DRIVERS)/deco_ld.o \
+        $(DRIVERS)/deco156.o $(MACHINE)/deco156.o \
+        $(DRIVERS)/decocass.o $(MACHINE)/decocass.o $(VIDEO)/decocass.o \
+        $(DRIVERS)/deshoros.o \
+        $(DRIVERS)/exprraid.o $(VIDEO)/exprraid.o \
+        $(DRIVERS)/firetrap.o $(VIDEO)/firetrap.o \
+        $(DRIVERS)/kchamp.o $(VIDEO)/kchamp.o \
+        $(DRIVERS)/kingobox.o $(VIDEO)/kingobox.o \
+        $(DRIVERS)/liberate.o $(VIDEO)/liberate.o \
+        $(DRIVERS)/madalien.o $(AUDIO)/madalien.o $(VIDEO)/madalien.o \
+        $(DRIVERS)/metlclsh.o $(VIDEO)/metlclsh.o \
+        $(DRIVERS)/pcktgal.o $(VIDEO)/pcktgal.o \
+        $(DRIVERS)/progolf.o \
+        $(DRIVERS)/shootout.o $(VIDEO)/shootout.o \
+        $(DRIVERS)/sidepckt.o $(VIDEO)/sidepckt.o \
+        $(DRIVERS)/simpl156.o $(VIDEO)/simpl156.o \
+        $(DRIVERS)/tryout.o $(VIDEO)/tryout.o \
+        $(MACHINE)/decocrpt.o \
+        $(MACHINE)/decoprot.o \
+        $(VIDEO)/decbac06.o \
+        $(VIDEO)/deco16ic.o \
+        $(VIDEO)/decocomn.o \
+        $(VIDEO)/decospr.o \
+        $(VIDEO)/decmxc06.o \
+        $(VIDEO)/deckarn.o \
+
+
+#-------------------------------------------------
+# layout dependencies
+#-------------------------------------------------
+
+$(DRIVERS)/astrocde.o:	$(LAYOUT)/gorf.lh \
+						$(LAYOUT)/tenpindx.lh
+$(DRIVERS)/circus.o:	$(LAYOUT)/circus.lh \
+						$(LAYOUT)/crash.lh
+$(DRIVERS)/galaxian.o:  $(MAMESRC)/drivers/galdrvr.c
+
+$(DRIVERS)/vicdual.o:   $(LAYOUT)/depthch.lh
+
+$(DRIVERS)/namcofl.o:   $(LAYOUT)/namcofl.lh
+
+$(DRIVERS)/namcos2.o:   $(LAYOUT)/finallap.lh
+
+$(DRIVERS)/mw18w.o:     $(LAYOUT)/18w.lh
+
+$(DRIVERS)/8080bw.o:    $(LAYOUT)/invrvnge.lh \
+                        $(LAYOUT)/shuttlei.lh \
+                        $(LAYOUT)/cosmicm.lh
+
+$(DRIVERS)/mw8080bw.o:  $(LAYOUT)/280zzzap.lh \
+                        $(LAYOUT)/clowns.lh \
+                        $(LAYOUT)/invaders.lh \
+                        $(LAYOUT)/invad2ct.lh \
+                        $(LAYOUT)/lagunar.lh \
+                        $(LAYOUT)/spacwalk.lh
+
+$(DRIVERS)/sspeedr.o:   $(LAYOUT)/sspeedr.lh
+
+$(DRIVERS)/sstrangr.o:  $(LAYOUT)/sstrangr.lh
+
+$(DRIVERS)/lazercmd.o:  $(LAYOUT)/lazercmd.lh
+
+$(DRIVERS)/meadows.o:   $(LAYOUT)/deadeye.lh \
+                        $(LAYOUT)/gypsyjug.lh
+
+$(DRIVERS)/balsente.o:  $(LAYOUT)/stocker.lh
+
+$(DRIVERS)/midzeus.o:   $(LAYOUT)/crusnexo.lh
+
+$(DRIVERS)/maxaflex.o:  $(LAYOUT)/maxaflex.lh
+
+$(DRIVERS)/chqflag.o:   $(LAYOUT)/chqflag.lh
+
+$(DRIVERS)/firebeat.o:  $(LAYOUT)/firebeat.lh
+
+$(DRIVERS)/warpwarp.o:  $(LAYOUT)/geebee.lh \
+                        $(LAYOUT)/navarone.lh \
+                        $(LAYOUT)/sos.lh
+
+$(DRIVERS)/polepos.o:   $(LAYOUT)/polepos.lh \
+                        $(LAYOUT)/topracer.lh
+
+$(DRIVERS)/cischeat.o:  $(LAYOUT)/cischeat.lh \
+                        $(LAYOUT)/f1gpstar.lh
+
+$(DRIVERS)/stepstag.o:  $(LAYOUT)/stepstag.lh
+
+$(DRIVERS)/tetrisp2.o:  $(LAYOUT)/rocknms.lh
+
+$(DRIVERS)/tehkanwc.o:  $(LAYOUT)/gridiron.lh
+
+$(DRIVERS)/buggychl.o:  $(LAYOUT)/buggychl.lh
+
+$(DRIVERS)/changela.o:  $(LAYOUT)/changela.lh
+
+$(DRIVERS)/grchamp.o:   $(LAYOUT)/grchamp.lh
+
+
