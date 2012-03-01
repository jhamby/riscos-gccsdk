OBJS = src/Entity.o \
	src/Animation.o \
	src/Avatar.o \
	src/CampaignManager.o \
	src/Enemy.o \
	src/EnemyGroupManager.o \
	src/EnemyManager.o \
	src/FileParser.o \
	src/FontEngine.o \
	src/GameState.o \
	src/GameStateTitle.o \
	src/GameStateLoad.o \
	src/GameStatePlay.o \
	src/GameStateNew.o \
	src/GameSwitcher.o \
	src/GetText.o \
	src/Hazard.o \
	src/HazardManager.o \
	src/InputState.o \
	src/ItemManager.o \
	src/ItemStorage.o \
	src/LootManager.o \
	src/MapCollision.o \
	src/MapIso.o \
	src/Menu.o \
	src/MenuActionBar.o \
	src/MenuCharacter.o \
	src/MenuConfirm.o \
	src/MenuEnemy.o \
	src/MenuExit.o \
	src/MenuExperience.o \
	src/MenuHPMP.o \
	src/MenuHUDLog.o \
	src/MenuInventory.o \
	src/MenuItemStorage.o \
	src/MenuLog.o \
	src/MenuManager.o \
	src/MenuMiniMap.o \
	src/MenuPowers.o \
	src/MenuTalker.o \
	src/MenuVendor.o \
	src/MessageEngine.o \
	src/ModManager.o \
	src/NPC.o \
	src/NPCManager.o \
	src/PowerManager.o \
	src/QuestLog.o \
	src/SaveLoad.o \
	src/Settings.o \
	src/SharedResources.o \
	src/StatBlock.o \
	src/TileSet.o \
	src/Utils.o \
	src/UtilsFileSystem.o \
	src/UtilsParsing.o \
	src/WidgetButton.o \
	src/WidgetInput.o \
	src/WidgetLabel.o \
	src/WidgetTooltip.o \
	src/main.o
	
CXXFLAGS = -Wall -O3 -I. -I$(GCCSDK_INSTALL_ENV)/include/SDL

flare: $(OBJS)
	gcc $(CXXFLAGS) -static -o flare $(OBJS) -L$(GCCSDK_INSTALL_ENV) -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lfreetype  -lbz2 -lpng -lz -ljpeg -lvorbisidec -logg -lstdc++  

clean:
	rm -f $(OBJS) flare

