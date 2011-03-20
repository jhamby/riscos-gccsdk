
OBJS = Avatar.o \
Enemy.o \
EnemyManager.o \
FontEngine.o \
GameEngine.o \
Hazard.o \
HazardManager.o \
InputState.o \
ItemDatabase.o \
LootManager.o \
main.o \
MapCollision.o \
MapIso.o \
MenuActionBar.o \
MenuCharacter.o \
MenuEnemy.o \
MenuExperience.o \
MenuHealthMana.o \
MenuInventory.o \
MenuLog.o \
MenuManager.o \
MenuMiniMap.o \
MenuPowers.o \
MenuTooltip.o \
MenuVendor.o \
NPC.o \
NPCManager.o \
PowerManager.o \
SaveLoad.o \
StatBlock.o \
TileSet.o \
Utils.o \
UtilsParsing.o

CXXFLAGS = -Wall -O3 -I. -I$(GCCSDK_INSTALL_ENV)/include/SDL

flare: $(OBJS)
	gcc $(CXXFLAGS) -static -o flare $(OBJS) -L$(GCCSDK_INSTALL_ENV) -lSDL -lSDL_image -lSDL_mixer -lpng -lz -ljpeg -lvorbisidec -logg -lstdc++  

clean:
	rm -f $(OBJS) flare

