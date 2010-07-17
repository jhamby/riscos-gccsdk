
OBJS = FontRenderer.o \
Game.o \
GraphicsUtility.o \
Lemming.o \
lemmings_level.o \
LevelLoader.o \
Level.o \
MainForm.o \
main.o \
SamplePlayer.o \
SpriteStore.o \
StatusPanel.o \
Timer.o \
TwoPlayerTotals.o \
Vector2D.o

MENUOBJS = menuSystem/ClientGameScreen.o \
menuSystem/ClientLevelSelect.o \
menuSystem/ClientSetup.o \
menuSystem/MenuItem.o \
menuSystem/MultiplayerGameOverScreen.o \
menuSystem/OnePlayerGameOver.o \
menuSystem/OnePlayerGameScreen.o \
menuSystem/OnePlayerLevelSelect.o \
menuSystem/ServerGameScreen.o \
menuSystem/ServerLevelSelect.o \
menuSystem/TitleScreenMenu.o \
menuSystem/TwoPlayerSetupScreen.o \

LEVELDOBJS = LevelDesigner/DesignerScreen.o \
LevelDesigner/Tile.o

CXXFLAGS = -Wall -O3 

lemmings: $(OBJS) $(MENUOBJS) $(LEVELDOBJS)

	gcc $(CXXFLAGS) -static -o lemmings $(OBJS) $(MENUOBJS) $(LEVELDOBJS) -lstdc++ -lSDL -lSDL_mixer -lSDL_image -logg -lvorbis -lvorbisfile -ljpeg -lpng -lz -lSDL_net -lDesk -lCX11

clean:
	rm -f $(OBJS) $(MENUOBJS) $(LEVELOBJS)
