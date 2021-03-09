OBJS = src/main.o \
	src/Animation.o \
	src/AnimationManager.o \
	src/AnimationSet.o \
	src/AStarNode.o \
	src/Avatar.o \
	src/BehaviorStandard.o \
	src/CampaignManager.o \
	src/CombatText.o \
	src/EnemyBehavior.o \
	src/Enemy.o \
	src/EnemyGroupManager.o \
	src/EnemyManager.o \
	src/Entity.o \
	src/FileParser.o \
	src/FontEngine.o \
	src/GameStateConfig.o \
	src/GameState.o \
	src/GameStateLoad.o \
	src/GameStateNew.o \
	src/GameStatePlay.o \
	src/GameStateTitle.o \
	src/GameSwitcher.o \
	src/GetText.o \
	src/Hazard.o \
	src/HazardManager.o \
	src/InputState.o \
	src/ItemManager.o \
	src/ItemStorage.o \
	src/LootManager.o \
	src/MapCollision.o \
	src/MapRenderer.o \
	src/MenuActionBar.o \
	src/MenuActiveEffects.o \
	src/MenuCharacter.o \
	src/MenuConfirm.o \
	src/Menu.o \
	src/MenuEnemy.o \
	src/MenuExit.o \
	src/MenuHUDLog.o \
	src/MenuInventory.o \
	src/MenuItemStorage.o \
	src/MenuLog.o \
	src/MenuManager.o \
	src/MenuMiniMap.o \
	src/MenuPowers.o \
	src/MenuStash.o \
	src/MenuStatBar.o \
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
	src/UtilsDebug.o \
	src/UtilsFileSystem.o \
	src/UtilsParsing.o \
	src/WidgetButton.o \
	src/WidgetCheckBox.o \
	src/WidgetInput.o \
	src/WidgetLabel.o \
	src/WidgetListBox.o \
	src/WidgetScrollBar.o \
	src/WidgetScrollBox.o \
	src/WidgetSlider.o \
	src/WidgetTabControl.o \
	src/WidgetTooltip.o

CXXFLAGS = -Wall -O3 -I. -I$(GCCSDK_INSTALL_ENV)/include/SDL

flare: $(OBJS)
	gcc $(CXXFLAGS) -static -o flare $(OBJS) -L$(GCCSDK_INSTALL_ENV) -lSDL -lSDL_gfx -lSDL_image -lSDL_mixer -lSDL_ttf -lfreetype  -lbz2 -lpng -lz -ljpeg -lvorbisidec -logg -lstdc++ -lFLAC -lmikmod -lwebp -ltiff -llzma

clean:
	rm -f $(OBJS) flare

