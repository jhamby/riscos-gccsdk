--- src/global.h.orig	2004-08-19 17:24:20.000000000 +0100
+++ src/global.h	2014-02-28 11:18:45.000000000 +0000
@@ -58,7 +58,7 @@
 // screen options
 const int SCREEN_WIDTH = 640;
 const int SCREEN_HEIGHT = 480;
-const int BIT_DEPTH = 24;
+const int BIT_DEPTH = 0;
 
 const int MAX_PLAYER_CNT = 2;
 
@@ -100,30 +100,30 @@
 		   BANNER_ITEM_ENERGY_BEAM_COLLECTED,
 		   BANNER_ITEM_LASER_COLLECTED };
 const std::string FN_BANNER_TEXTS[] =
-{ "./images/bannerExcellent.bmp",
-  "./images/bannerYouRule.bmp",
-  "./images/bannerHeiho.bmp",
-  "./images/bannerHealth.bmp",
-  "./images/bannerEnemysKilled.bmp",
-  "./images/bannerItemHealthCollected.bmp",
-  "./images/bannerItemPrimaryUpgradeCollected.bmp",
-  "./images/bannerItemDumbfireDoubleCollected.bmp",
-  "./images/bannerItemKickAssRocketCollected.bmp",
-  "./images/bannerItemHellfireCollected.bmp",
-  "./images/bannerItemMachineGunCollected.bmp",
-  "./images/bannerItemHeatseekerCollected.bmp",
-  "./images/bannerItemNukeCollected.bmp",
-  "./images/bannerItemDeflectorCollected.bmp",
-  "./images/bannerItemEnergyBeamCollected.bmp",
-  "./images/bannerItemLaserCollected.bmp" };
+{ "/<AlienBlaster$Dir>/images/bannerExcellent.bmp",
+  "/<AlienBlaster$Dir>/images/bannerYouRule.bmp",
+  "/<AlienBlaster$Dir>/images/bannerHeiho.bmp",
+  "/<AlienBlaster$Dir>/images/bannerHealth.bmp",
+  "/<AlienBlaster$Dir>/images/bannerEnemysKilled.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemHealthCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemPrimaryUpgradeCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemDumbfireDoubleCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemKickAssRocketCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemHellfireCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemMachineGunCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemHeatseekerCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemNukeCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemDeflectorCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemEnergyBeamCollected.bmp",
+  "/<AlienBlaster$Dir>/images/bannerItemLaserCollected.bmp" };
 const int NR_BANNER_TEXTS = 3;
 
 enum BannerBoni { BANNER_BONUS_100=0, 
 		  BANNER_BONUS_200,
 		  BANNER_BONUS_NONE=1000 };
 const std::string FN_BANNER_BONUS[] = 
-{ "./images/bannerBonus100.bmp", 
-  "./images/bannerBonus200.bmp" };
+{ "/<AlienBlaster$Dir>/images/bannerBonus100.bmp", 
+  "/<AlienBlaster$Dir>/images/bannerBonus200.bmp" };
 const int NR_BANNER_BONI = 2;
 
 const float ARCADE_POINTS_FOR_FORMATION_DESTRUCTION = 100;
@@ -315,8 +315,8 @@
 enum SmokePuffTypes { SMOKE_PUFF_SMALL=0, SMOKE_PUFF_MEDIUM };
 const int NR_SMOKE_PUFF_TYPES = 2;
 const std::string FN_SMOKE_PUFF[ NR_SMOKE_PUFF_TYPES ] =
-{ "./images/smokePuffSmall.bmp", 
-  "./images/smokePuffMedium.bmp" };
+{ "/<AlienBlaster$Dir>/images/smokePuffSmall.bmp", 
+  "/<AlienBlaster$Dir>/images/smokePuffMedium.bmp" };
 const int LIFETIME_SMOKE_PUFF[ NR_SMOKE_PUFF_TYPES ] = { 500, 1000 };
 const int SMOKE_PUFF_DELAY_TO_NEXT_PUFF[ NR_SMOKE_PUFF_TYPES ] = { 100, 100 };
 const float SMOKE_PUFF_VELOCITY_FACTOR = 0.3;
@@ -498,171 +498,171 @@
 extern int LIGHT_FIGHTER_MAX_DAMAGE;
 extern int HEAVY_FIGHTER_MAX_DAMAGE;
 
-const std::string FN_SOUND_SHOT_PRIMARY = "./sound/shotPrimary.wav";
-const std::string FN_SOUND_SHOT_SECONDARY = "./sound/shotSecondary.wav";
-const std::string FN_SOUND_EXPLOSION_NORMAL = "./sound/explosion.wav";
-const std::string FN_SOUND_EXPLOSION_BOSS = "./sound/explosionBoss.wav";
-const std::string FN_SOUND_BOSS_ALARM = "./sound/alarm.wav";
-const std::string FN_SOUND_ARCADE_CONFIRM = "./sound/alarm.wav";
-const std::string FN_SOUND_ARCADE_CHOOSE = "./sound/choose.wav";
-const std::string FN_SOUND_INTRO_CONFIRM = "./sound/confirm.wav";
-const std::string FN_SOUND_INTRO_CHOOSE = "./sound/choose.wav";
-
-const std::string FN_ENEMY_FIGHTER = "./images/fighter.bmp";
-const std::string FN_ENEMY_FIGHTER_SHADOW = "./images/fighterShadow.bmp";
-const std::string FN_ENEMY_BOMBER = "./images/bomber.bmp";
-const std::string FN_ENEMY_BOMBER_SHADOW = "./images/bomberShadow.bmp";
-const std::string FN_ENEMY_TANK = "./images/tank.bmp";
-const std::string FN_ENEMY_BOSS_1_MAIN_GUN = "./images/boss1MainGun.bmp";
-const std::string FN_ENEMY_BOSS_1_ROCKET_LAUNCHER = "./images/boss1RocketLauncher.bmp";
-const std::string FN_ENEMY_BOSS_1_SHOT_BATTERY_LEFT = "./images/boss1ShotBatteryLeft.bmp";
-const std::string FN_ENEMY_BOSS_1_SHOT_BATTERY_RIGHT = "./images/boss1ShotBatteryRight.bmp";
-const std::string FN_ENEMY_BOSS_2 = "./images/boss2.bmp";
-const std::string FN_ENEMY_BOSS_2_SHADOW = "./images/boss2Shadow.bmp";
-
-const std::string FN_WRECK_FIGHTER = "./images/wreckFighter.bmp";
-const std::string FN_WRECK_BOMBER = "./images/wreckBomber.bmp";
-const std::string FN_WRECK_TANK = "./images/wreckTank.bmp";
-const std::string FN_WRECK_BOSS_1 = "./images/wreckBoss1.bmp";
-const std::string FN_WRECK_BOSS_1_BACKGROUND = "./images/wreckBossBackground.bmp";
-const std::string FN_WRECK_BOSS_1_DESTROYED = "./images/boss.bmp";
-const std::string FN_WRECK_BOSS_2_DESTROYED = "./images/wreckBoss2.bmp";
-
-const std::string FN_SHOT_NORMAL = "./images/normalShot.bmp";
-const std::string FN_SHOT_NORMAL_HEAVY = "./images/heavyShot.bmp";
-const std::string FN_SHOT_DOUBLE = "./images/normalShot.bmp";
-const std::string FN_SHOT_DOUBLE_HEAVY = "./images/heavyShot.bmp";
-const std::string FN_SHOT_TRIPLE = "./images/heavyShot.bmp";
-
-const std::string FN_SHOT_HF_NORMAL = "./images/normalShotHF.bmp";
-const std::string FN_SHOT_HF_DOUBLE = "./images/normalShotHF.bmp";
-const std::string FN_SHOT_HF_TRIPLE = "./images/normalShotHF.bmp";
-const std::string FN_SHOT_HF_QUATTRO = "./images/normalShotHF.bmp";
-const std::string FN_SHOT_HF_QUINTO = "./images/normalShotHF.bmp";
-
-const std::string FN_SHOT_DUMBFIRE = "./images/dumbfire.bmp";
-const std::string FN_SHOT_DUMBFIRE_DOUBLE = "./images/dumbfire.bmp";
-const std::string FN_SHOT_KICK_ASS_ROCKET = "./images/kickAssRocket.bmp";
-const std::string FN_SHOT_KICK_ASS_ROCKET_SHADOW = "./images/kickAssRocketShadow.bmp";
-const std::string FN_SHOT_HELLFIRE = "./images/hellfire.bmp";
-const std::string FN_SHOT_HELLFIRE_SHADOW = "./images/hellfireShadow.bmp";
-const std::string FN_SHOT_MACHINE_GUN = "./images/machineGun.bmp";
-const std::string FN_SHOT_ENERGY_BEAM = "./images/energyBeam.bmp";
-
-const std::string FN_SHOT_HF_DUMBFIRE = "./images/dumbfire.bmp";
-const std::string FN_SHOT_HF_DUMBFIRE_DOUBLE = "./images/dumbfire.bmp";
-const std::string FN_SHOT_HF_KICK_ASS_ROCKET = "./images/kickAssRocket.bmp";
-const std::string FN_SHOT_HF_KICK_ASS_ROCKET_SHADOW = "./images/kickAssRocketShadow.bmp";
-const std::string FN_SHOT_HF_LASER = "./images/laser.bmp";
-
-const std::string FN_ENEMY_SHOT_NORMAL = "./images/enemyShotNormal.bmp";
-const std::string FN_ENEMY_SHOT_TANK_ROCKET = "./images/tankRocket.bmp";
-const std::string FN_ENEMY_SHOT_TANK_ROCKET_SHADOW = "./images/tankRocketShadow.bmp";
-
-const std::string FN_SPECIAL_SHOT_HEATSEEKER = "./images/heatseeker.bmp";
-const std::string FN_SPECIAL_SHOT_NUKE = "./images/shotNuke.bmp";
-const std::string FN_SPECIAL_SHOT_NUKE_SHADOW = "./images/shotNukeShadow.bmp";
-const std::string FN_NUKE_EFFECT = "./images/nukeEffect.bmp";
-const std::string FN_SONIC_EFFECT = "./images/sonic.bmp";
-
-const std::string FN_ITEM_PRIMARY_UPGRADE = "./images/itemPrimaryUpgrade.bmp";
-const std::string FN_ITEM_DUMBFIRE_DOUBLE = "./images/itemDumbfireDouble.bmp";
-const std::string FN_ITEM_KICK_ASS_ROCKET = "./images/itemKickAssRocket.bmp";
-const std::string FN_ITEM_HELLFIRE = "./images/itemHellfire.bmp";
-const std::string FN_ITEM_MACHINE_GUN = "./images/itemMachineGun.bmp";
-const std::string FN_ITEM_HEALTH = "./images/itemHealth.bmp";
-const std::string FN_ITEM_HEATSEEKER = "./images/itemHeatseeker.bmp";
-const std::string FN_ITEM_NUKE = "./images/itemNuke.bmp";
-const std::string FN_ITEM_DEFLECTOR = "./images/itemDeflector.bmp";
-const std::string FN_ITEM_ENERGY_BEAM = "./images/itemEnergyBeam.bmp";
-const std::string FN_ITEM_LASER = "./images/itemLaser.bmp";
-
-const std::string FN_ALIENBLASTER_INTRO = "./images/alienblasterintro.bmp";
-const std::string FN_ALIENBLASTER_ICON = "./images/alienblastericon.bmp";
-const std::string FN_BACKGROUND = "./images/background.bmp";
-const std::string FN_PAUSED = "./images/paused.bmp";
-const std::string FN_YOU_LOSE = "./images/youLose.bmp";
-const std::string FN_YOU_WIN = "./images/youWin.bmp";
-const std::string FN_GAME_OVER = "./images/gameOver.bmp";
-const std::string FN_ARCADE_LOGO = "./images/arcadeLogo.bmp";
+const std::string FN_SOUND_SHOT_PRIMARY = "/<AlienBlaster$Dir>/sound/shotPrimary.wav";
+const std::string FN_SOUND_SHOT_SECONDARY = "/<AlienBlaster$Dir>/sound/shotSecondary.wav";
+const std::string FN_SOUND_EXPLOSION_NORMAL = "/<AlienBlaster$Dir>/sound/explosion.wav";
+const std::string FN_SOUND_EXPLOSION_BOSS = "/<AlienBlaster$Dir>/sound/explosionBoss.wav";
+const std::string FN_SOUND_BOSS_ALARM = "/<AlienBlaster$Dir>/sound/alarm.wav";
+const std::string FN_SOUND_ARCADE_CONFIRM = "/<AlienBlaster$Dir>/sound/alarm.wav";
+const std::string FN_SOUND_ARCADE_CHOOSE = "/<AlienBlaster$Dir>/sound/choose.wav";
+const std::string FN_SOUND_INTRO_CONFIRM = "/<AlienBlaster$Dir>/sound/confirm.wav";
+const std::string FN_SOUND_INTRO_CHOOSE = "/<AlienBlaster$Dir>/sound/choose.wav";
+
+const std::string FN_ENEMY_FIGHTER = "/<AlienBlaster$Dir>/images/fighter.bmp";
+const std::string FN_ENEMY_FIGHTER_SHADOW = "/<AlienBlaster$Dir>/images/fighterShadow.bmp";
+const std::string FN_ENEMY_BOMBER = "/<AlienBlaster$Dir>/images/bomber.bmp";
+const std::string FN_ENEMY_BOMBER_SHADOW = "/<AlienBlaster$Dir>/images/bomberShadow.bmp";
+const std::string FN_ENEMY_TANK = "/<AlienBlaster$Dir>/images/tank.bmp";
+const std::string FN_ENEMY_BOSS_1_MAIN_GUN = "/<AlienBlaster$Dir>/images/boss1MainGun.bmp";
+const std::string FN_ENEMY_BOSS_1_ROCKET_LAUNCHER = "/<AlienBlaster$Dir>/images/boss1RocketLauncher.bmp";
+const std::string FN_ENEMY_BOSS_1_SHOT_BATTERY_LEFT = "/<AlienBlaster$Dir>/images/boss1ShotBatteryLeft.bmp";
+const std::string FN_ENEMY_BOSS_1_SHOT_BATTERY_RIGHT = "/<AlienBlaster$Dir>/images/boss1ShotBatteryRight.bmp";
+const std::string FN_ENEMY_BOSS_2 = "/<AlienBlaster$Dir>/images/boss2.bmp";
+const std::string FN_ENEMY_BOSS_2_SHADOW = "/<AlienBlaster$Dir>/images/boss2Shadow.bmp";
+
+const std::string FN_WRECK_FIGHTER = "/<AlienBlaster$Dir>/images/wreckFighter.bmp";
+const std::string FN_WRECK_BOMBER = "/<AlienBlaster$Dir>/images/wreckBomber.bmp";
+const std::string FN_WRECK_TANK = "/<AlienBlaster$Dir>/images/wreckTank.bmp";
+const std::string FN_WRECK_BOSS_1 = "/<AlienBlaster$Dir>/images/wreckBoss1.bmp";
+const std::string FN_WRECK_BOSS_1_BACKGROUND = "/<AlienBlaster$Dir>/images/wreckBossBackground.bmp";
+const std::string FN_WRECK_BOSS_1_DESTROYED = "/<AlienBlaster$Dir>/images/boss.bmp";
+const std::string FN_WRECK_BOSS_2_DESTROYED = "/<AlienBlaster$Dir>/images/wreckBoss2.bmp";
+
+const std::string FN_SHOT_NORMAL = "/<AlienBlaster$Dir>/images/normalShot.bmp";
+const std::string FN_SHOT_NORMAL_HEAVY = "/<AlienBlaster$Dir>/images/heavyShot.bmp";
+const std::string FN_SHOT_DOUBLE = "/<AlienBlaster$Dir>/images/normalShot.bmp";
+const std::string FN_SHOT_DOUBLE_HEAVY = "/<AlienBlaster$Dir>/images/heavyShot.bmp";
+const std::string FN_SHOT_TRIPLE = "/<AlienBlaster$Dir>/images/heavyShot.bmp";
+
+const std::string FN_SHOT_HF_NORMAL = "/<AlienBlaster$Dir>/images/normalShotHF.bmp";
+const std::string FN_SHOT_HF_DOUBLE = "/<AlienBlaster$Dir>/images/normalShotHF.bmp";
+const std::string FN_SHOT_HF_TRIPLE = "/<AlienBlaster$Dir>/images/normalShotHF.bmp";
+const std::string FN_SHOT_HF_QUATTRO = "/<AlienBlaster$Dir>/images/normalShotHF.bmp";
+const std::string FN_SHOT_HF_QUINTO = "/<AlienBlaster$Dir>/images/normalShotHF.bmp";
+
+const std::string FN_SHOT_DUMBFIRE = "/<AlienBlaster$Dir>/images/dumbfire.bmp";
+const std::string FN_SHOT_DUMBFIRE_DOUBLE = "/<AlienBlaster$Dir>/images/dumbfire.bmp";
+const std::string FN_SHOT_KICK_ASS_ROCKET = "/<AlienBlaster$Dir>/images/kickAssRocket.bmp";
+const std::string FN_SHOT_KICK_ASS_ROCKET_SHADOW = "/<AlienBlaster$Dir>/images/kickAssRocketShadow.bmp";
+const std::string FN_SHOT_HELLFIRE = "/<AlienBlaster$Dir>/images/hellfire.bmp";
+const std::string FN_SHOT_HELLFIRE_SHADOW = "/<AlienBlaster$Dir>/images/hellfireShadow.bmp";
+const std::string FN_SHOT_MACHINE_GUN = "/<AlienBlaster$Dir>/images/machineGun.bmp";
+const std::string FN_SHOT_ENERGY_BEAM = "/<AlienBlaster$Dir>/images/energyBeam.bmp";
+
+const std::string FN_SHOT_HF_DUMBFIRE = "/<AlienBlaster$Dir>/images/dumbfire.bmp";
+const std::string FN_SHOT_HF_DUMBFIRE_DOUBLE = "/<AlienBlaster$Dir>/images/dumbfire.bmp";
+const std::string FN_SHOT_HF_KICK_ASS_ROCKET = "/<AlienBlaster$Dir>/images/kickAssRocket.bmp";
+const std::string FN_SHOT_HF_KICK_ASS_ROCKET_SHADOW = "/<AlienBlaster$Dir>/images/kickAssRocketShadow.bmp";
+const std::string FN_SHOT_HF_LASER = "/<AlienBlaster$Dir>/images/laser.bmp";
+
+const std::string FN_ENEMY_SHOT_NORMAL = "/<AlienBlaster$Dir>/images/enemyShotNormal.bmp";
+const std::string FN_ENEMY_SHOT_TANK_ROCKET = "/<AlienBlaster$Dir>/images/tankRocket.bmp";
+const std::string FN_ENEMY_SHOT_TANK_ROCKET_SHADOW = "/<AlienBlaster$Dir>/images/tankRocketShadow.bmp";
+
+const std::string FN_SPECIAL_SHOT_HEATSEEKER = "/<AlienBlaster$Dir>/images/heatseeker.bmp";
+const std::string FN_SPECIAL_SHOT_NUKE = "/<AlienBlaster$Dir>/images/shotNuke.bmp";
+const std::string FN_SPECIAL_SHOT_NUKE_SHADOW = "/<AlienBlaster$Dir>/images/shotNukeShadow.bmp";
+const std::string FN_NUKE_EFFECT = "/<AlienBlaster$Dir>/images/nukeEffect.bmp";
+const std::string FN_SONIC_EFFECT = "/<AlienBlaster$Dir>/images/sonic.bmp";
+
+const std::string FN_ITEM_PRIMARY_UPGRADE = "/<AlienBlaster$Dir>/images/itemPrimaryUpgrade.bmp";
+const std::string FN_ITEM_DUMBFIRE_DOUBLE = "/<AlienBlaster$Dir>/images/itemDumbfireDouble.bmp";
+const std::string FN_ITEM_KICK_ASS_ROCKET = "/<AlienBlaster$Dir>/images/itemKickAssRocket.bmp";
+const std::string FN_ITEM_HELLFIRE = "/<AlienBlaster$Dir>/images/itemHellfire.bmp";
+const std::string FN_ITEM_MACHINE_GUN = "/<AlienBlaster$Dir>/images/itemMachineGun.bmp";
+const std::string FN_ITEM_HEALTH = "/<AlienBlaster$Dir>/images/itemHealth.bmp";
+const std::string FN_ITEM_HEATSEEKER = "/<AlienBlaster$Dir>/images/itemHeatseeker.bmp";
+const std::string FN_ITEM_NUKE = "/<AlienBlaster$Dir>/images/itemNuke.bmp";
+const std::string FN_ITEM_DEFLECTOR = "/<AlienBlaster$Dir>/images/itemDeflector.bmp";
+const std::string FN_ITEM_ENERGY_BEAM = "/<AlienBlaster$Dir>/images/itemEnergyBeam.bmp";
+const std::string FN_ITEM_LASER = "/<AlienBlaster$Dir>/images/itemLaser.bmp";
+
+const std::string FN_ALIENBLASTER_INTRO = "/<AlienBlaster$Dir>/images/alienblasterintro.bmp";
+const std::string FN_ALIENBLASTER_ICON = "/<AlienBlaster$Dir>/images/alienblastericon.bmp";
+const std::string FN_BACKGROUND = "/<AlienBlaster$Dir>/images/background.bmp";
+const std::string FN_PAUSED = "/<AlienBlaster$Dir>/images/paused.bmp";
+const std::string FN_YOU_LOSE = "/<AlienBlaster$Dir>/images/youLose.bmp";
+const std::string FN_YOU_WIN = "/<AlienBlaster$Dir>/images/youWin.bmp";
+const std::string FN_GAME_OVER = "/<AlienBlaster$Dir>/images/gameOver.bmp";
+const std::string FN_ARCADE_LOGO = "/<AlienBlaster$Dir>/images/arcadeLogo.bmp";
 
 // numbers of images (animation-frames) per racer
 const int RACER_IMAGE_CNT = 9;
 
-const std::string FN_LIGHT_FIGHTER_1 = "./images/lightFighter1.bmp";
-const std::string FN_LIGHT_FIGHTER_2 = "./images/lightFighter2.bmp";
-const std::string FN_LIGHT_FIGHTER_SHADOW = "./images/lightFighterShadow.bmp";
-const std::string FN_LIGHT_FIGHTER_SHIELD_DAMAGED = "./images/lightFighterShieldDamaged.bmp";
-const std::string FN_LIGHT_FIGHTER_1_ICON = "./images/lightFighter1Icon.bmp";
-const std::string FN_LIGHT_FIGHTER_2_ICON = "./images/lightFighter2Icon.bmp";
-const std::string FN_LIGHT_FIGHTER_1_SMALL = "./images/lightFighter1Small.bmp";
-const std::string FN_LIGHT_FIGHTER_2_SMALL = "./images/lightFighter2Small.bmp";
-
-const std::string FN_HEAVY_FIGHTER_1 = "./images/heavyFighter1.bmp";
-const std::string FN_HEAVY_FIGHTER_2 = "./images/heavyFighter2.bmp";
-const std::string FN_HEAVY_FIGHTER_SHADOW = "./images/heavyFighterShadow.bmp";
-const std::string FN_HEAVY_FIGHTER_SHIELD_DAMAGED = "./images/heavyFighterShieldDamaged.bmp";
-const std::string FN_HEAVY_FIGHTER_DEFLECTOR = "./images/heavyFighterDeflector.bmp";
-const std::string FN_HEAVY_FIGHTER_1_ICON = "./images/heavyFighter1Icon.bmp";
-const std::string FN_HEAVY_FIGHTER_2_ICON = "./images/heavyFighter2Icon.bmp";
-const std::string FN_HEAVY_FIGHTER_1_SMALL = "./images/heavyFighter1Small.bmp";
-const std::string FN_HEAVY_FIGHTER_2_SMALL = "./images/heavyFighter2Small.bmp";
+const std::string FN_LIGHT_FIGHTER_1 = "/<AlienBlaster$Dir>/images/lightFighter1.bmp";
+const std::string FN_LIGHT_FIGHTER_2 = "/<AlienBlaster$Dir>/images/lightFighter2.bmp";
+const std::string FN_LIGHT_FIGHTER_SHADOW = "/<AlienBlaster$Dir>/images/lightFighterShadow.bmp";
+const std::string FN_LIGHT_FIGHTER_SHIELD_DAMAGED = "/<AlienBlaster$Dir>/images/lightFighterShieldDamaged.bmp";
+const std::string FN_LIGHT_FIGHTER_1_ICON = "/<AlienBlaster$Dir>/images/lightFighter1Icon.bmp";
+const std::string FN_LIGHT_FIGHTER_2_ICON = "/<AlienBlaster$Dir>/images/lightFighter2Icon.bmp";
+const std::string FN_LIGHT_FIGHTER_1_SMALL = "/<AlienBlaster$Dir>/images/lightFighter1Small.bmp";
+const std::string FN_LIGHT_FIGHTER_2_SMALL = "/<AlienBlaster$Dir>/images/lightFighter2Small.bmp";
+
+const std::string FN_HEAVY_FIGHTER_1 = "/<AlienBlaster$Dir>/images/heavyFighter1.bmp";
+const std::string FN_HEAVY_FIGHTER_2 = "/<AlienBlaster$Dir>/images/heavyFighter2.bmp";
+const std::string FN_HEAVY_FIGHTER_SHADOW = "/<AlienBlaster$Dir>/images/heavyFighterShadow.bmp";
+const std::string FN_HEAVY_FIGHTER_SHIELD_DAMAGED = "/<AlienBlaster$Dir>/images/heavyFighterShieldDamaged.bmp";
+const std::string FN_HEAVY_FIGHTER_DEFLECTOR = "/<AlienBlaster$Dir>/images/heavyFighterDeflector.bmp";
+const std::string FN_HEAVY_FIGHTER_1_ICON = "/<AlienBlaster$Dir>/images/heavyFighter1Icon.bmp";
+const std::string FN_HEAVY_FIGHTER_2_ICON = "/<AlienBlaster$Dir>/images/heavyFighter2Icon.bmp";
+const std::string FN_HEAVY_FIGHTER_1_SMALL = "/<AlienBlaster$Dir>/images/heavyFighter1Small.bmp";
+const std::string FN_HEAVY_FIGHTER_2_SMALL = "/<AlienBlaster$Dir>/images/heavyFighter2Small.bmp";
 
-const std::string FN_ICONS_SPECIALS = "./images/iconsSpecials.bmp";
-const std::string FN_ICONS_SECONDARY_WEAPONS = "./images/iconsSecondaryWeapons.bmp";
+const std::string FN_ICONS_SPECIALS = "/<AlienBlaster$Dir>/images/iconsSpecials.bmp";
+const std::string FN_ICONS_SECONDARY_WEAPONS = "/<AlienBlaster$Dir>/images/iconsSecondaryWeapons.bmp";
 
-const std::string FN_HITPOINTS_STAT = "./images/hpStat.bmp";
+const std::string FN_HITPOINTS_STAT = "/<AlienBlaster$Dir>/images/hpStat.bmp";
 
-const std::string FN_INTRO_SHOW_CHOICE      = "./images/menuIcon.bmp";
+const std::string FN_INTRO_SHOW_CHOICE      = "/<AlienBlaster$Dir>/images/menuIcon.bmp";
 
-const std::string FN_FONT_PATH = "./images/";
+const std::string FN_FONT_PATH = "/<AlienBlaster$Dir>/images/";
 const std::string FN_FONT_SUFFIX_SURFACE = ".bmp";
-const std::string FN_FONT_INTRO = "./images/font-20white.bmp";
-const std::string FN_FONT_INTRO_HIGHLIGHTED = "./images/font-20lightblue.bmp";
-const std::string FN_FONT_NUMBERS_TIME = "./images/font-20red.bmp";
-const std::string FN_FONT_NUMBERS_LEFT = "./images/font-20red.bmp";
-const std::string FN_FONT_NUMBERS_RIGHT = "./images/font-20blue.bmp";
-const std::string FN_FONT_SETTINGS = "./images/font-20white.bmp";
-const std::string FN_FONT_SETTINGS_HIGHLIGHTED = "./images/font-20lightblue.bmp";
-const std::string FN_FONT_SETTINGS_SMALL = "./images/font-14white.bmp";
-const std::string FN_FONT_SETTINGS_SMALL_BLUE = "./images/font-14lightblue.bmp";
-const std::string FN_FONT_SETTINGS_SMALL_HIGHLIGHTED = "./images/font-14red.bmp";
-
-const std::string FN_SETTINGS_BLUE = "./images/bluePlain.bmp";
-const std::string FN_SETTINGS_WHITE = "./images/whitePlain.bmp";
+const std::string FN_FONT_INTRO = "/<AlienBlaster$Dir>/images/font-20white.bmp";
+const std::string FN_FONT_INTRO_HIGHLIGHTED = "/<AlienBlaster$Dir>/images/font-20lightblue.bmp";
+const std::string FN_FONT_NUMBERS_TIME = "/<AlienBlaster$Dir>/images/font-20red.bmp";
+const std::string FN_FONT_NUMBERS_LEFT = "/<AlienBlaster$Dir>/images/font-20red.bmp";
+const std::string FN_FONT_NUMBERS_RIGHT = "/<AlienBlaster$Dir>/images/font-20blue.bmp";
+const std::string FN_FONT_SETTINGS = "/<AlienBlaster$Dir>/images/font-20white.bmp";
+const std::string FN_FONT_SETTINGS_HIGHLIGHTED = "/<AlienBlaster$Dir>/images/font-20lightblue.bmp";
+const std::string FN_FONT_SETTINGS_SMALL = "/<AlienBlaster$Dir>/images/font-14white.bmp";
+const std::string FN_FONT_SETTINGS_SMALL_BLUE = "/<AlienBlaster$Dir>/images/font-14lightblue.bmp";
+const std::string FN_FONT_SETTINGS_SMALL_HIGHLIGHTED = "/<AlienBlaster$Dir>/images/font-14red.bmp";
+
+const std::string FN_SETTINGS_BLUE = "/<AlienBlaster$Dir>/images/bluePlain.bmp";
+const std::string FN_SETTINGS_WHITE = "/<AlienBlaster$Dir>/images/whitePlain.bmp";
 
-const std::string FN_EXPLOSION_NORMAL = "./images/explosion.bmp";
-const std::string FN_EXPLOSION_ENEMY = "./images/explosionEnemy.bmp";
+const std::string FN_EXPLOSION_NORMAL = "/<AlienBlaster$Dir>/images/explosion.bmp";
+const std::string FN_EXPLOSION_ENEMY = "/<AlienBlaster$Dir>/images/explosionEnemy.bmp";
 
-const std::string FN_LOADING = "./images/loading.bmp";
+const std::string FN_LOADING = "/<AlienBlaster$Dir>/images/loading.bmp";
 
-const std::string FN_SETTINGS = "./cfg/alienBlaster.cfg";
+const std::string FN_SETTINGS = "/<AlienBlaster$Dir>/cfg/alienBlaster.cfg";
 
-const std::string FN_DIFFICULTY_CONFIG = "./cfg/alienBlasterDifficulty";
+const std::string FN_DIFFICULTY_CONFIG = "/<AlienBlaster$Dir>/cfg/alienBlasterDifficulty";
 const std::string FN_DIFFICULTY_CONFIG_SUFFIX = ".cfg";
 
-const std::string FN_HIGHSCORE = "./cfg/highscore.dat";
+const std::string FN_HIGHSCORE = "/<AlienBlaster$Dir>/cfg/highscore.dat";
 
 enum MusicTracks { MUSIC_INTRO=0, MUSIC_PLAYON, MUSIC_BOSS1, MUSIC_NONE };
 const int NR_MUSIC_TRACKS = 3;
-const std::string FN_MUSIC[] = { "./sound/intro.wav",
-				 "./sound/playon.wav",
-				 "./sound/intro.wav" };
-
-const std::string FN_LEVEL_ONE_PLAYER = "./cfg/level1.cfg";
-const std::string FN_LEVEL_TWO_PLAYER = "./cfg/level2.cfg";
-const std::string FN_LEVEL_ARCADEMODE = "./cfg/levelArcade.cfg";
-
-const std::string FN_SCREENSHOT0 =  "./intro/HellShot0.bmp";
-const std::string FN_SCREENSHOT1 =  "./intro/HellShot1.bmp";
-const std::string FN_SCREENSHOT2 =  "./intro/HellShot2.bmp";
-const std::string FN_SCREENSHOT3 =  "./intro/HellShot3.bmp";
-const std::string FN_SCREENSHOT4 =  "./intro/HellShot5.bmp";
-const std::string FN_SCREENSHOT5 =  "./intro/HellShot4.bmp";
-const std::string FN_SCREENSHOT6 =  "./intro/HellShot6.bmp";
-const std::string FN_SCREENSHOT7 =  "./intro/HellShot7.bmp";
-const std::string FN_SCREENSHOT8 =  "./intro/HellShot8.bmp";
-const std::string FN_SCREENSHOT9 =  "./intro/HellShot9.bmp";
+const std::string FN_MUSIC[] = { "/<AlienBlaster$Dir>/sound/intro.wav",
+				 "/<AlienBlaster$Dir>/sound/playon.wav",
+				 "/<AlienBlaster$Dir>/sound/intro.wav" };
+
+const std::string FN_LEVEL_ONE_PLAYER = "/<AlienBlaster$Dir>/cfg/level1.cfg";
+const std::string FN_LEVEL_TWO_PLAYER = "/<AlienBlaster$Dir>/cfg/level2.cfg";
+const std::string FN_LEVEL_ARCADEMODE = "/<AlienBlaster$Dir>/cfg/levelArcade.cfg";
+
+const std::string FN_SCREENSHOT0 =  "/<AlienBlaster$Dir>/intro/HellShot0.bmp";
+const std::string FN_SCREENSHOT1 =  "/<AlienBlaster$Dir>/intro/HellShot1.bmp";
+const std::string FN_SCREENSHOT2 =  "/<AlienBlaster$Dir>/intro/HellShot2.bmp";
+const std::string FN_SCREENSHOT3 =  "/<AlienBlaster$Dir>/intro/HellShot3.bmp";
+const std::string FN_SCREENSHOT4 =  "/<AlienBlaster$Dir>/intro/HellShot5.bmp";
+const std::string FN_SCREENSHOT5 =  "/<AlienBlaster$Dir>/intro/HellShot4.bmp";
+const std::string FN_SCREENSHOT6 =  "/<AlienBlaster$Dir>/intro/HellShot6.bmp";
+const std::string FN_SCREENSHOT7 =  "/<AlienBlaster$Dir>/intro/HellShot7.bmp";
+const std::string FN_SCREENSHOT8 =  "/<AlienBlaster$Dir>/intro/HellShot8.bmp";
+const std::string FN_SCREENSHOT9 =  "/<AlienBlaster$Dir>/intro/HellShot9.bmp";
 
 const std::string LVL_BACKG_TILE_CNT = "BACKG_TILES";
 const std::string LVL_BACKG_TILE     = "BACKG_TILE";
