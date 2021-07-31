//////////////////////////////////////////////////////////////////
// OpenTTD FrontEnd
//
// C program shell for rapid application development
// The Flying Pig!
// Started 17/7/2003
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <kernel.h>
#include <swis.h>

#include "oslib/osfile.h"
#include "oslib/osfscontrol.h"
#include "oslib/wimp.h"

#include "frontend.h"

typedef enum _SAVETYPE
{
  SAVETYPE_INVALID = -1,
  SAVETYPE_FILE,
  SAVETYPE_ICONTEXT,
  SAVETYPE_NUM
} SAVETYPE;

static char                   gpcTemp[512];
static char                   *pcMessages;
static int                    gwhMain;
//static int                    gwhSave;
static int                    gwhWarn;
//static int                    gihIconBarIcon;
static int                    gnMenuWins = 0;
static int                    gawhMenuWinHandle[10];
static int                    gnMenuXpos = 0;
static int                    gnMenuYpos = 0;
static char                   *gpcMenuCurrent = NULL;
//static char                   *gpcIconBarMenu;
static char                   *gpcMainMenu;
static int                    gnTaskHandle;
static BOOL                   gboDrag = FALSE;
static char                   gacFontRef[256];
static int                    gnMessageMyRef = 0;
static SAVETYPE               geSaveType = SAVETYPE_INVALID;
//static int                    gnSaveFileType;
static char                   gszSaveString[255];
//static int                    gnExampleSize;
static char                   *pcSprites;
static bool                   gboInstalled = FALSE;

bool FileNotEmpty (char * szFilename);
bool CheckFiles (void);
int cmpnstr (char * szString1, char * szString2, int nSize);

//////////////////////////////////////////////////////////////////
// Main application

// Respond to swi errors
inline void err (_kernel_oserror * sError)
{
  if (sError)
  {
    ShowError (sError);
  }
}

// Main program
int main(/*int argc, char * * argv*/)
{
  _kernel_swi_regs sRegs;
  char             cBlock[256];
  int              nVersion;
  int              anUserMessages[] = {1, 2, 3, 5, 6, 7, 0x502, 0x4af80, 0};
  int              nCount;

  // Load Messages file
  sRegs.r[0] = 6;
  sRegs.r[3] = 17 + sizeof(MESSAGES);
  _kernel_swi (OS_Module, & sRegs, & sRegs);
  pcMessages = (char *)sRegs.r[2];
  strcpy(pcMessages + 16,MESSAGES);
  sRegs.r[0] = (int)pcMessages;
  sRegs.r[1] = (int)(pcMessages + 16);
  sRegs.r[2] = 0;
  _kernel_swi (MessageTrans_OpenFile, & sRegs, & sRegs);

  // Initialise task
  sRegs.r[0] = 300;
  sRegs.r[1] = 0x4b534154;
  sRegs.r[2] = (int)(Tag ("Tsk"));
  sRegs.r[3] = (int)anUserMessages;
  _kernel_swi (Wimp_Initialise, & sRegs, & sRegs);
  nVersion = sRegs.r[0];
  gnTaskHandle = sRegs.r[1];
  gboInstalled = FALSE;

  // Load sprites
  pcSprites = LoadSprites (SPRITES);

  // Load templates
  for (nCount = 0; nCount < 256; nCount++)
  {
    gacFontRef[nCount] = 0;
  }
  sRegs.r[1] = (unsigned int)TEMPLATES;
  _kernel_swi (Wimp_OpenTemplate, & sRegs, & sRegs);
  gwhMain = LoadTemplateSprites ("Main", pcSprites);
//  gwhSave = LoadTemplate ("Save");
  gwhWarn = LoadTemplate ("Warning");

  // Create menus
  gpcMainMenu = CreateMenu (Tag ("Menu1"));

  // Close templates
  _kernel_swi (Wimp_CloseTemplate, & sRegs, & sRegs);

  // Create icon
//  gihIconBarIcon = CreateIconbarIcon (Tag ("Icn"), 76, 76);

  // Check for installed files
  gboInstalled = CheckFiles ();

  // Open main window
  OpenWindowInitCentre(gwhMain);

  // Main polling loop
  //////////////////////////////////////////////////////////////////
  do
  {
    sRegs.r[0] = FLAGS;
    sRegs.r[1] = (int)cBlock;

    _kernel_swi (Wimp_Poll, & sRegs, & sRegs);
    switch (sRegs.r[0])
    {
      case  0: // Null
        break;
      case  1: // Redraw window
        break;
      case  2: // Open window
        OpenWindow (cBlock);
        break;
      case  3: // Closewindow
        CloseWindow (cBlock);
        break;
      case  4: // Pointer leaving window
        break;
      case  5: // Pointer entering window
        break;
      case  6:
        MouseClick (cBlock);
        break;
      case  7: // User drag box
//        UserDragBox (cBlock);
        break;
      case  8: // Keys
        Keys (cBlock);
        break;
      case  9:
        MenuSelect (cBlock);
        break;
      case 10: // Scroll request
        break;
      case 11: // Lose caret
        break;
      case 12: // Gain caret
        break;
      case 17: // Receive message
      case 18:
        Receive (cBlock);
        break;
      case 19: // UserMessage Acknowledge
        Acknowledged (cBlock);
        break;
      default: // Default
        break;
    }
  } while TRUE;

  //////////////////////////////////////////////////////////////////

  return 0;
}

// Poll 16,17: Message received
void Receive (char * cBlock)
{
  switch (*(int*)(cBlock + 16))
  {
    case  0:
      Quit ();
      break;
    case  1:
//      DataSave (cBlock);
      break;
    case  2:
//      DataSaveAck (cBlock);
      break;
    case  3:
      DataLoad (cBlock);
      break;
    case  5:
//      DataOpen (cBlock);
      break;
    case  6:
//      RamFetch (cBlock);
      break;
    case  7:
//      RamTransmit (cBlock);
      break;
    case 0x502:
      Help (cBlock);
      break;
  }
}

// Poll 19: Message acknowledge received
void Acknowledged (char * cBlock)
{
  switch (*(int*)(cBlock + 16))
  {
    case 0x4af80:
      OpenURLReturned (cBlock);
      break;
  }
  gnMessageMyRef = 0;
}

// Quit task
void Quit (void)
{
  _kernel_swi_regs sRegs;
  int              nCount;
  int              nRemove;

  // Free the fonts
  for (nCount = 0; nCount < (int)sizeof(gacFontRef); nCount++)
  {
    if (gacFontRef[nCount])
    {
      for (nRemove = 0; nRemove < gacFontRef[nCount]; nRemove++)
      {
        sRegs.r[0] = nCount;
        _kernel_swi (Font_LoseFont, & sRegs, & sRegs);
      }
    }
  }

  // Close the program
  _kernel_swi (Wimp_CloseDown, & sRegs, & sRegs);
}

// Obtain string from Messages file
char * Tag (char * szTag)
{
  _kernel_swi_regs sRegs;

  sRegs.r[0] = (int)pcMessages;
  sRegs.r[1] = (int)szTag;
  sRegs.r[2] = (int)(gpcTemp + 256);
  sRegs.r[3] = 256;
  sRegs.r[4] = 0;
  sRegs.r[5] = 0;
  sRegs.r[6] = 0;
  sRegs.r[7] = 0;
  _kernel_swi (MessageTrans_Lookup, & sRegs, & sRegs);

  return gpcTemp + 256;
}

// Create an iconbar icon
int CreateIconbarIcon (char * szSprite, int nWidth, int nHeight)
{
  _kernel_swi_regs sRegs;
  char             pcIconBlock[36];

  *(int*)(pcIconBlock +  0) = -1;
  *(int*)(pcIconBlock +  4) = 0;
  *(int*)(pcIconBlock +  8) = 0;
  *(int*)(pcIconBlock + 12) = nWidth;
  *(int*)(pcIconBlock + 16) = nHeight;
  *(int*)(pcIconBlock + 20) = 0x2002;
  strncpy(pcIconBlock + 24, szSprite, 12);
  sRegs.r[0] = 0;
  sRegs.r[1] = (int)pcIconBlock;
  _kernel_swi (Wimp_CreateIcon, & sRegs, & sRegs);

  return sRegs.r[0];
}

// Create window from Templates file
int LoadTemplate (char * szWindowTitle)
{
  _kernel_swi_regs sRegs;
  char *           pcWindow;
  char *           pcIndirected;
  int              nIndirectedSize;
  char             szTitle[12];

  sRegs.r[1] = 0;
  sRegs.r[5] = (int)szWindowTitle;
  sRegs.r[6] = 0;
  _kernel_swi (Wimp_LoadTemplate, & sRegs, & sRegs);
  pcWindow = malloc (sRegs.r[1]);
  nIndirectedSize = sRegs.r[2];
  pcIndirected = malloc (nIndirectedSize);
  strncpy (szTitle, szWindowTitle, 12);

  sRegs.r[1] = (int)pcWindow;
  sRegs.r[2] = (int)pcIndirected;
  sRegs.r[3] = (int)(pcIndirected + nIndirectedSize);
  sRegs.r[4] = (int)gacFontRef;
  sRegs.r[5] = (int)szTitle;
  sRegs.r[6] = 0;
  _kernel_swi (Wimp_LoadTemplate, & sRegs, & sRegs);
  _kernel_swi (Wimp_CreateWindow, & sRegs, & sRegs);

  free(pcWindow);

  return sRegs.r[0];
}

// Create window from Templates file with sprites
int LoadTemplateSprites (char * szWindowTitle, char * pcSpriteArea)
{
  _kernel_swi_regs sRegs;
  char *           pcWindow;
  char *           pcIndirected;
  int              nIndirectedSize;
  char             szTitle[12];
  int              nIcon;
  int              nIcons;

  sRegs.r[1] = 0;
  sRegs.r[5] = (int)szWindowTitle;
  sRegs.r[6] = 0;
  _kernel_swi (Wimp_LoadTemplate, & sRegs, & sRegs);
  pcWindow = malloc (sRegs.r[1]);
  nIndirectedSize = sRegs.r[2];
  pcIndirected = malloc (nIndirectedSize);
  strncpy (szTitle, szWindowTitle, 12);

  sRegs.r[1] = (int)pcWindow;
  sRegs.r[2] = (int)pcIndirected;
  sRegs.r[3] = (int)(pcIndirected + nIndirectedSize);
  sRegs.r[4] = (int)gacFontRef;
  sRegs.r[5] = (int)szTitle;
  sRegs.r[6] = 0;
  _kernel_swi (Wimp_LoadTemplate, & sRegs, & sRegs);
  if (pcSpriteArea)
  {
    *(int*)(pcWindow + 64) = (int)pcSpriteArea;
  }

  nIcons = *(int*)(pcWindow + 84);
  for (nIcon = 0; nIcon < nIcons; nIcon++)
  {
    if ((*(int*)(pcWindow + 88 + (nIcon * 32) + 16) & 0x103) == 0x102)
    {
      *(unsigned int*)(pcWindow + 88 + (nIcon * 32) + 24)
        = (unsigned int)pcSpriteArea;
    }
  }

  _kernel_swi (Wimp_CreateWindow, & sRegs, & sRegs);

  free(pcWindow);

  return sRegs.r[0];
}

// Poll 2: Open window given window block
void OpenWindow (char * cBlock)
{
  _kernel_swi_regs sRegs;

  sRegs.r[1] = (int)cBlock;
  _kernel_swi (Wimp_OpenWindow, & sRegs, & sRegs);
}

// Poll 6: Process mouse clicks
void MouseClick (char * cBlock)
{
  int              nXpos       = *(int*)(cBlock +  0);
  int              nYpos       = *(int*)(cBlock +  4);
  int              nButton     = *(int*)(cBlock +  8);
  int              whWindow    = *(int*)(cBlock + 12);
  int              ihIcon      = *(int*)(cBlock + 16);
//  char             *szFileIcon;

  if (whWindow == -2)
  {
    switch (nButton)
    {
      case  4:
        OpenWindowInit(gwhMain);
        break;
      case  2:
        OpenMenu (gpcMainMenu, nXpos - 64, 96 + 44 * 2);
        break;
    }
  }

  if (whWindow == gwhMain)
  {
    if (nButton == 2)
    {
      OpenMenu (gpcMainMenu, nXpos - 64, nYpos);
    }
    else
    {
      switch (ihIcon)
      {
        case 19: // OK / Cancel
          if (gboInstalled)
          {
            err ((_kernel_oserror *)xwimp_start_task (RUNGAME, NULL));
            Quit ();
          }
          else
          {
            Quit ();
          }
          break;
        default: // Save drag
          if (nButton & 0x50)
          {
            strncpy (gszSaveString, GetIconText (whWindow, ihIcon), sizeof(gszSaveString));
            DragBox (whWindow, ihIcon);
            geSaveType = SAVETYPE_ICONTEXT;
          }
          break;
      }
    }
  }

//  if (whWindow == gwhSave)
//  {
//    switch (ihIcon)
//    {
//      case  3: // Save drag
//        if (nButton & 0x50)
//        {
//          szFileIcon = GetIconText (gwhSave, 3);
//          gnSaveFileType = 0xfff;
//          DragSprite (whWindow, ihIcon, szFileIcon);
//          geSaveType = SAVETYPE_FILE;
//        }
//        break;
//      case  1: // Cancel
//        CloseMenu ();
//        break;
//      case  0: // Save
//        gnSaveFileType = 0xfff;
//        SaveSave ();
//        if (nButton != 1)
//        {
//          CloseMenu ();
//        }
//        break;
//    }
//  }

  if (whWindow == gwhWarn)
  {
    switch (ihIcon)
    {
      case  0: // OK
        CloseWindowHandle (gwhWarn);
        break;
    }
  }

  if (whWindow == gawhMenuWinHandle[0])
  {
    switch (ihIcon)
    {
      case  8: // Email
        OpenURL (EMAIL);
        if (nButton != 1)
        {
          CloseMenu ();
        }
        break;
      case  9: // Website
        OpenURL (WEBSITE);
        if (nButton != 1)
        {
          CloseMenu ();
        }
        break;
    }
  }
}

// Open a window on screen that is currently closed
void OpenWindowInit(int whWindow)
{
  char             cBlock[36];
  _kernel_swi_regs sRegs;

  *(int*)(cBlock +  0) = whWindow;
  sRegs.r[1] = (int)cBlock;
  _kernel_swi (Wimp_GetWindowState, & sRegs, & sRegs);
  *(int*)(cBlock + 28) = -1;
  _kernel_swi (Wimp_OpenWindow, & sRegs, & sRegs);
}

// Open a window in the centre of the screen
void OpenWindowInitCentre(int whWindow)
{
  char             cBlock[36];
  _kernel_swi_regs sRegs;
  int              nHeight;
  int              nWidth;
  int              nEigFactor;
  int              nPosition;

  *(int*)(cBlock +  0) = whWindow;
  sRegs.r[1] = (int)cBlock;
  _kernel_swi (Wimp_GetWindowState, & sRegs, & sRegs);
  *(int*)(cBlock + 28) = -1;

  nWidth  = *(int*)(cBlock + 12) - *(int*)(cBlock +  4);
  nHeight = *(int*)(cBlock + 16) - *(int*)(cBlock +  8);

  sRegs.r[0] = -1;
  sRegs.r[1] = 4;
  _kernel_swi (OS_ReadModeVariable, & sRegs, & sRegs);
  nEigFactor = sRegs.r[2];
  sRegs.r[1] = 11;
  _kernel_swi (OS_ReadModeVariable, & sRegs, & sRegs);
  nPosition = ((sRegs.r[2] << nEigFactor) - nWidth) / 2;
  *(int*)(cBlock +  4) = nPosition;
  *(int*)(cBlock + 12) = nPosition + nWidth;

  sRegs.r[0] = -1;
  sRegs.r[1] = 5;
  _kernel_swi (OS_ReadModeVariable, & sRegs, & sRegs);
  nEigFactor = sRegs.r[2];
  sRegs.r[1] = 12;
  _kernel_swi (OS_ReadModeVariable, & sRegs, & sRegs);
  nPosition = ((sRegs.r[2] << nEigFactor) - nHeight) / 2;
  *(int*)(cBlock +  8) = nPosition;
  *(int*)(cBlock + 16) = nPosition + nHeight;

  sRegs.r[1] = (int)cBlock;
  _kernel_swi (Wimp_OpenWindow, & sRegs, & sRegs);
}

// Poll 3: Close a window
void CloseWindow (char * cBlock)
{
  int whWindow;
  _kernel_swi_regs sRegs;

  whWindow = *(int*)(cBlock);

  sRegs.r[1] = whWindow;
  _kernel_swi (Wimp_CloseWindow, & sRegs, & sRegs);

  if (whWindow == gwhMain)
  {
    Quit ();
  }
}

// Close window given handle
void CloseWindowHandle (int whWindow)
{
  char             cBlock[4];
  _kernel_swi_regs sRegs;

  *(int*)(cBlock +  0) = whWindow;
  sRegs.r[1] = (int)cBlock;
  _kernel_swi (Wimp_CloseWindow, & sRegs, & sRegs);

  if (whWindow == gwhMain)
  {
    Quit ();
  }
}

// Create a menu from a text string - usually kept in the messages file
char * CreateMenu (char * szMenu)
{
  char *           pcMenu;
  char *           pcBuffer;
  char *           nAt = 0;
  char *           nAt2 = 0;
  int              nWidth = 0;
  unsigned int     uFlags;
  char             szMenuString[512];
  char *           pcMenuString;
  char *           pcN;

  pcMenu = malloc (1024);
  pcBuffer = pcMenu + 512;

  nAt = strchr (szMenu, ',');
  if (!nAt)
  {
    nAt = (char *)strlen (szMenu);
  }
  else
  {
    nAt -= (int)szMenu;
  }
  strncpy (szMenuString, szMenu, (int)nAt);
  szMenuString[(int)nAt] = 0;

  if (strlen (szMenuString) >= 12)
  {
    strcpy (pcBuffer, szMenuString);
    *(unsigned int*)(pcMenu +  0) = (unsigned int)pcBuffer;
    pcBuffer += strlen (szMenuString) + 1;
    uFlags = 8u;
  }
  else
  {
    strncpy (pcMenu, szMenuString, 12);
    uFlags = 0u;
  }

  strncpy (pcMenu, szMenuString, 12);
  pcMenu[12] = 7;
  pcMenu[13] = 2;
  pcMenu[14] = 7;
  pcMenu[15] = 0;
  *(int*)(pcMenu + 20) = 44;
  *(int*)(pcMenu + 24) = 0;
  pcN = pcMenu + 28;
  do
  {
    *(int*)(pcN +  4) = -1;
    *(int*)(pcN +  8) = (7<<24)+(1<<5)+1;
    nAt2 = nAt + 1;

    nAt = strchr (szMenu + (int)nAt2, ',');
    if (!nAt)
    {
      nAt = (char *)strlen (szMenu) + 1;
    }
    else
    {
      nAt -= (int)szMenu;
    }
    strncpy (szMenuString, szMenu + (int)nAt2, (int)nAt - (int)nAt2);
    szMenuString[(int)nAt - (int)nAt2] = 0;

    pcMenuString = szMenuString;

    if (pcMenuString[0] == '+')
    {
      uFlags = 1u;
      pcMenuString++;
    }
    if (pcMenuString[0] == '-')
    {
      uFlags |= 2u;
      pcMenuString++;
    }
    if (pcMenuString[0] == '|')
    {
      uFlags |= 4u;
      pcMenuString++;
    }
    if (pcMenuString[0] == '>')
    {
      pcMenuString++;
      if (gnMenuWins < (int)sizeof(gawhMenuWinHandle))
      {
        gawhMenuWinHandle[gnMenuWins] = LoadTemplate (pcMenuString);
        *(int*)(pcN +  4) = gawhMenuWinHandle[gnMenuWins];
        gnMenuWins++;
      }
    }

    if (strlen (pcMenuString) >= 12)
    {
      *(int*)(pcN +  8) |= 256;
      *(int*)(pcN + 12) = (int)pcBuffer;
      *(int*)(pcN + 16) = 0;
      *(int*)(pcN + 20) = strlen (pcMenuString) + 1;
      strcpy (pcBuffer, pcMenuString);
      pcBuffer += strlen (pcMenuString) + 1;
    }
    else
    {
      strncpy (pcN + 12, pcMenuString, 12);
    }
    *(unsigned int*)(pcN +  0) = uFlags;
    if ((int)strlen (pcMenuString) > nWidth)
    {
      nWidth = strlen (pcMenuString);
    }
    pcN += 24;
    uFlags = 0u;
  } while ((int)nAt < (int)strlen (szMenu));

  *(int*)(pcMenu + 16) = (nWidth + 1) * 16;
  *(unsigned int*)(pcN - 24) |= 0x80;

  return pcMenu;
}

#if defined _DEBUG
// Report a debug message
void Report (char * szMessage)
{
  _kernel_swi_regs sRegs;

  sRegs.r[0] = (int)szMessage;
  _kernel_swi (0x054c80, & sRegs, & sRegs);
}

// Report a debug message with variable
void ReportVar (char * szFormat, int nVariable)
{
  _kernel_swi_regs sRegs;
  char             szReport[255];

  sprintf (szReport, szFormat, nVariable);
  sRegs.r[0] = (int)szReport;
  _kernel_swi (0x054c80, & sRegs, & sRegs);
}
#endif // _DEBUG

// Open a given menu at the given position on screen
void OpenMenu (char * pcMenu, int nXpos, int nYpos)
{
  _kernel_swi_regs sRegs;

  sRegs.r[1] = (int)pcMenu;
  sRegs.r[2] = nXpos;
  sRegs.r[3] = nYpos;
  _kernel_swi (Wimp_CreateMenu, & sRegs, & sRegs);

  gnMenuXpos = nXpos;
  gnMenuYpos = nYpos;
  gpcMenuCurrent = pcMenu;
}

// Poll 9: Act on menu selections
void MenuSelect (char * cBlock)
{
  _kernel_swi_regs sRegs;

  sRegs.r[1] = (int)(cBlock + 20);
  _kernel_swi (Wimp_GetPointerInfo, & sRegs, & sRegs);
  switch (*(int*)(cBlock))
  {
    case 1:
      Quit ();
      break;
    default:
      break;
  }
  if ((cBlock[28] & 1))
  {
    sRegs.r[1] = (int)gpcMenuCurrent;
    sRegs.r[2] = gnMenuXpos;
    sRegs.r[3] = gnMenuYpos;
    _kernel_swi (Wimp_CreateMenu, & sRegs, & sRegs);
  }
}

// Poll 8: Act on user key presses
void Keys (char * cBlock)
{
  _kernel_swi_regs sRegs;
  int              whWindow;
  int              ihIcon;
  int              nKey;

  whWindow = *(int*)(cBlock +  0);
  ihIcon = *(int*)(cBlock +  4);
  nKey = *(int*)(cBlock + 24);

  switch (nKey)
  {
    case 0x0d:
//      if (whWindow == gwhSave)
//      {
//        gnSaveFileType = 0xfff;
//        SaveSave ();
//        CloseMenu ();
//      }
      if (whWindow == gwhWarn)
      {
        CloseWarning ();
      }
      break;
    case 0x1b:
      if (whWindow == gwhWarn)
      {
        CloseWarning ();
      }
      break;
    default:
      sRegs.r[0] = nKey;
      _kernel_swi (Wimp_ProcessKey, & sRegs, & sRegs);
      break;
  }
}

// Get the text string from an icon
char * GetIconText (int whWindow, int ihIcon)
{
  _kernel_swi_regs sRegs;
  char             *szString;
  int              nTerminate = 0;

  *(int*)(gpcTemp +  0) = whWindow;
  *(int*)(gpcTemp +  4) = ihIcon;
  sRegs.r[1] = (int)gpcTemp;
  _kernel_swi (Wimp_GetIconState, & sRegs, & sRegs);
  if ((*(int*)(gpcTemp + 24) & 0x100))
  {
    szString = (char *)(*(int*)((gpcTemp + 28)));
    while (szString[nTerminate] >= 32)
    {
      nTerminate++;
    }
    szString[nTerminate] = 0;
  }
  else
  {
    szString = (gpcTemp + 28);
    while ((szString[nTerminate] >= 32) && (nTerminate < 12))
    {
      nTerminate++;
    }
    szString[nTerminate] = 0;
  }
  return szString;
}

// Get the selection state from an icon
BOOL GetIconSelectionState (int whWindow, int ihIcon)
{
  _kernel_swi_regs sRegs;

  *(int*)(gpcTemp +  0) = whWindow;
  *(int*)(gpcTemp +  4) = ihIcon;
  sRegs.r[1] = (int)gpcTemp;
  _kernel_swi (Wimp_GetIconState, & sRegs, & sRegs);
  return ((*(int*)(gpcTemp + 24) & 0x200000) == 0x200000);
}

// Set the selection state from an icon
void SetIconSelectionState (BOOL boState, int whWindow, int ihIcon)
{
  _kernel_swi_regs sRegs;

  *(int*)(gpcTemp +  0) = whWindow;
  *(int*)(gpcTemp +  4) = ihIcon;
  if (boState)
  {
    *(int*)(gpcTemp +  8) = (1<<21);
  }
  else
  {
    *(int*)(gpcTemp +  8) = (0<<21);
  }
  *(int*)(gpcTemp + 12) = (1<<21);

  sRegs.r[1] = (int)gpcTemp;
  _kernel_swi (Wimp_SetIconState, & sRegs, & sRegs);
}

// Set the text string for an icon
void SetIconText (char * szText, int whWindow, int ihIcon)
{
  _kernel_swi_regs sRegs;

  *(int*)(gpcTemp +  0) = whWindow;
  *(int*)(gpcTemp +  4) = ihIcon;
  sRegs.r[1] = (int)gpcTemp;
  _kernel_swi (Wimp_GetIconState, & sRegs, & sRegs);

  if ((*(int*)(gpcTemp + 24) & 0x100))
  {
    strcpy ((char *)(*(int*)(gpcTemp + 28)), szText);
  }
  else
  {
    strcpy ((gpcTemp + 28), szText);
  }

  *(int*)(gpcTemp +  8) = 0;
  *(int*)(gpcTemp + 12) = 0;
  sRegs.r[1] = (int)gpcTemp;
  _kernel_swi (Wimp_SetIconState, & sRegs, & sRegs);
}

//// Poll 7: User has finished dragging an object
//void UserDragBox (char * cBlock)
//{
//  _kernel_swi_regs sRegs;
//  char             *szFilename;
//  int              whWindow;
//
//  _kernel_swi (DragASprite_Stop, & sRegs, & sRegs);
//  gboDrag = FALSE;
//
//  sRegs.r[1] = (int)cBlock;
//  _kernel_swi (Wimp_GetPointerInfo, & sRegs, & sRegs);
//
//  whWindow = *(int*)(cBlock + 12);
//
//  if ((whWindow != gwhMain)
////     && (whWindow != gwhSave)
//     && (whWindow != gwhWarn))
//  {
//    switch (geSaveType)
//    {
//      case SAVETYPE_FILE:
//        *(int*)(cBlock + 20) = *(int*)(cBlock + 12);
//        *(int*)(cBlock + 24) = *(int*)(cBlock + 16);
//        *(int*)(cBlock + 28) = *(int*)(cBlock +  0);
//        *(int*)(cBlock + 32) = *(int*)(cBlock +  4);
//        *(int*)(cBlock + 36) = gnExampleSize;
//        *(int*)(cBlock + 40) = gnSaveFileType; // filetype
//
//        szFilename = GetIconText (gwhSave, 2);
//
//        while (strchr (szFilename, '.'))
//        {
//          szFilename = strchr(szFilename, '.') + 1;
//        }
//
//        strcpy (cBlock + 44, szFilename);
//        *(int*)(cBlock +  0) = (44 + 4 + strlen (szFilename)) & ~3;
//        *(int*)(cBlock + 12) = 0;
//        *(int*)(cBlock + 16) = 1;
//
//        sRegs.r[0] = 17;
//        sRegs.r[1] = (int)(cBlock +  0);
//        sRegs.r[2] = *(int*)(cBlock + 20);
//        sRegs.r[3] = *(int*)(cBlock + 24);
//        _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//        break;
//      case SAVETYPE_ICONTEXT:
//        *(int*)(cBlock + 20) = *(int*)(cBlock + 12);
//        *(int*)(cBlock + 24) = *(int*)(cBlock + 16);
//        *(int*)(cBlock + 28) = *(int*)(cBlock +  0);
//        *(int*)(cBlock + 32) = *(int*)(cBlock +  4);
//        *(int*)(cBlock + 36) = strlen(gszSaveString) + 1;
//        *(int*)(cBlock + 40) = 0xfff; // filetype (text file)
//
//        strcpy (cBlock + 44, "IconText");
//        *(int*)(cBlock +  0) = 44 + 12;
//        *(int*)(cBlock + 12) = 0;
//        *(int*)(cBlock + 16) = 1;
//
//        sRegs.r[0] = 17;
//        sRegs.r[1] = (int)(cBlock +  0);
//        sRegs.r[2] = *(int*)(cBlock + 20);
//        sRegs.r[3] = *(int*)(cBlock + 24);
//        _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//        break;
//      default:
//        break;
//    }
//  }
//}

//// Sender wants to send data to the receiver
//// Normal use: user has terminated a drag, so this message is sent
//// Response:   DataSaveAck
////             RamFetch
//void DataSave (char * cBlock)
//{
//  _kernel_swi_regs sRegs;
//
//  // Check filetype
//  if (*(int*)(cBlock + 40) == 0xfff)
//  {
//    *(int*)(cBlock +  0) = 28;
//    *(int*)(cBlock + 12) = *(int*)(cBlock +  8);
//    *(int*)(cBlock + 16) = 6;
//    *(int*)(cBlock + 20) = (int)malloc (1024 * 10);
//    *(int*)(cBlock + 24) = 1024 * 10;
//
//    sRegs.r[0] = 17;
//    sRegs.r[1] = (int)cBlock;
//    sRegs.r[2] = (*(int*)(cBlock +  4));
//    _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//
///*
//    *(int*)(cBlock +  0) = 60;
//    *(int*)(cBlock + 12) = *(int*)(cBlock +  8);
//    *(int*)(cBlock + 16) = 2;
//    *(int*)(cBlock + 36) = -1;
//    strcpy ((cBlock + 44), "<Wimp$Scrap>");
//
//    sRegs.r[0] = 17;
//    sRegs.r[1] = (int)cBlock;
//    sRegs.r[2] = (*(int*)(cBlock +  4));
//    _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//*/
//  }
//}

//// Acknowledge that you received a DataSave message
//// Usually followed by a DataLoad message
//void DataSaveAck (char * cBlock)
//{
//  _kernel_swi_regs sRegs;
//  char             *szFilename;
//  int              nStringLen;
//
//  CloseMenu ();
//
//  switch (geSaveType)
//  {
//      case SAVETYPE_FILE:
//        szFilename = cBlock + 44;
//        // Save the file to the given filename
//        SetIconText (szFilename, gwhSave, 2);
//
//        // Save the file
//        SaveFile (szFilename);
//
//        *(int*)(cBlock + 12) = *(int*)(cBlock +  8);
//        *(int*)(cBlock + 16) = 3;
//        sRegs.r[0] = 17;
//        sRegs.r[1] = (int)cBlock;
//        sRegs.r[2] = (*(int*)(cBlock +  4));
//        _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//
//        break;
//      case SAVETYPE_ICONTEXT:
//        szFilename = cBlock + 44;
//        // Save the file to the given filename
//        nStringLen = strlen (gszSaveString);
//        sRegs.r[0] = 10;
//        sRegs.r[1] = (int)szFilename;
//        sRegs.r[2] = 0xfff;
//        sRegs.r[4] = (int)gszSaveString;
//        sRegs.r[5] = (int)(gszSaveString + nStringLen);
//        err (_kernel_swi (OS_File, & sRegs, & sRegs));
//
//        *(int*)(cBlock + 12) = *(int*)(cBlock +  8);
//        *(int*)(cBlock + 16) = 3;
//        sRegs.r[0] = 17;
//        sRegs.r[1] = (int)cBlock;
//        sRegs.r[2] = (*(int*)(cBlock +  4));
//
//        _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//        break;
//      default:
//        break;
//  }
//}

// Receiver of this message should load the file
// and answer with DataSaveAck if successful
void DataLoad (char * pcBlock)
{
//  _kernel_swi_regs sRegs;
  char             *szFilename;
//  char             *pcLoadedFile;
//  int              nSize;
  int              nPos;
  char             *szLeafname;
  char             szFileTo[1024];
  os_error         *psError;

  // Check filetype
  szFilename = pcBlock + 44;

  // Check the filename
  for (nPos = 0; *(szFilename + nPos) >= 32; nPos++)
  {
    // Do nothing
  }

  while ((nPos > 0) && (*(szFilename + nPos - 1) != '.')
    && (*(szFilename + nPos - 1) != ':'))
  {
    nPos--;
  }
  szLeafname = szFilename + nPos;

  if ((cmpnstr (szLeafname, "trg1r/grf", 10) == 0)
    || (cmpnstr (szLeafname, "trg1/grf", 9) == 0)
    || (cmpnstr (szLeafname, "trgcr/grf", 10) == 0)
    || (cmpnstr (szLeafname, "trgc/grf", 9) == 0)
    || (cmpnstr (szLeafname, "trghr/grf", 10) == 0)
    || (cmpnstr (szLeafname, "trgh/grf", 9) == 0)
    || (cmpnstr (szLeafname, "trgir/grf", 10) == 0)
    || (cmpnstr (szLeafname, "trgi/grf", 9) == 0)
    || (cmpnstr (szLeafname, "trgtr/grf", 10) == 0)
    || (cmpnstr (szLeafname, "trgt/grf", 9) == 0)
    || (cmpnstr (szLeafname, "sample/cat", 11) == 0))
  {
    sprintf (szFileTo, "<OpenTTD$Dir>.data.%s", szLeafname);
    psError = xosfscontrol_copy (szFilename, szFileTo, 0, 0, 0, 0, 0, NULL);
    if (psError)
    {
      err ((_kernel_oserror *)psError);
    }
    else
    {
      ((wimp_block*)pcBlock)->message.your_ref
        = ((wimp_block*)pcBlock)->message.my_ref;
      ((wimp_block*)pcBlock)->message.action = message_DATA_LOAD_ACK;

      xwimp_send_message (wimp_USER_MESSAGE,
        & ((wimp_block*)pcBlock)->message,
        ((wimp_block*)pcBlock)->message.sender);
    }
  }

  gboInstalled = CheckFiles ();
}

//// User has double clicked on a file
//void DataOpen (char * cBlock)
//{
//  _kernel_swi_regs sRegs;
//  char             *szFilename;
//  char             *pcLoadedFile;
//  int              nSize;
//
//  // Check filetype
//  if (*(int*)(cBlock + 40) == 0xffd)
//  {
//    szFilename = cBlock + 44;
//
//    // Load the file from the given filename
//    pcLoadedFile = LoadFile (szFilename, & nSize);
//
//    if (pcLoadedFile)
//    {
//      *(int*)(cBlock + 12) = *(int*)(cBlock +  8);
//      *(int*)(cBlock + 16) = 4;
//
//      sRegs.r[0] = 17;
//      sRegs.r[1] = (int)cBlock;
//      sRegs.r[2] = (*(int*)(cBlock +  4));
//      _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//
//      FileLoaded (pcLoadedFile, nSize);
//    }
//  }
//}

//// Copy the data from here to the external program
//// Reply with RamTransmit
//void RamFetch (char * cBlock)
//{
//  _kernel_swi_regs sRegs;
//  int              nStringLen;
//  int              nDestHandle;
//
//  CloseMenu ();
//
//  switch (geSaveType)
//  {
//    case SAVETYPE_FILE:
//      nDestHandle = *(int*)(cBlock +  4);
//
//      sRegs.r[0] = gnTaskHandle;
//      sRegs.r[1] = (int)gpcExample;
//                                 // Address of source data to transfer
//      sRegs.r[2] = nDestHandle;
//      sRegs.r[3] = *(int*)(cBlock + 20);
//      sRegs.r[4] = gnExampleSize;
//                                 // Length of buffer (byte aligned)
//      _kernel_swi (Wimp_TransferBlock, & sRegs, & sRegs);
//
//      *(int*)(cBlock +  0) = 28;
//      *(int*)(cBlock + 12) = *(int*)(cBlock +  8);
//      *(int*)(cBlock + 16) = 7;
//      *(int*)(cBlock + 24) = gnExampleSize;
//      sRegs.r[0] = 17;
//      sRegs.r[1] = (int)cBlock;
//      sRegs.r[2] = nDestHandle;
//      _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//      break;
//    case SAVETYPE_ICONTEXT:
//      nDestHandle = *(int*)(cBlock +  4);
//
//      nStringLen = (int)strlen (gszSaveString);
//
//      if (nStringLen < *(int*)(cBlock + 24))
//      {
//        sRegs.r[0] = gnTaskHandle;
//        sRegs.r[1] = (int)gszSaveString;
//                                   // Address of source data to transfer
//        sRegs.r[2] = nDestHandle;
//        sRegs.r[3] = *(int*)(cBlock + 20);
//        sRegs.r[4] = nStringLen;
//                                   // Length of buffer (byte aligned)
//        _kernel_swi (Wimp_TransferBlock, & sRegs, & sRegs);
//
//        *(int*)(cBlock +  0) = 28;
//        *(int*)(cBlock + 12) = *(int*)(cBlock +  8);
//        *(int*)(cBlock + 16) = 7;
//        *(int*)(cBlock + 24) = nStringLen;
//        sRegs.r[0] = 17;
//        sRegs.r[1] = (int)cBlock;
//        sRegs.r[2] = nDestHandle;
//        _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
//      }
//      break;
//    default:
//      break;
//  }
//}
//
//// Response to a RamFetch
//void RamTransmit (char * cBlock)
//{
//  char *           pcLoadedFile;
//  int              nSize;
//
//  pcLoadedFile = (char*)*(int*)(cBlock + 20);
//  nSize = *(int*)(cBlock + 24);
//
//  // Save the file out, for the sake of the example
//  FileLoaded (pcLoadedFile, nSize);
//}

// Response to a Help message
void Help (char * cBlock)
{
  _kernel_swi_regs sRegs;
  int              whWindow;
  int              ihIcon;
  char             szHelpText[236];
  int              nStrLen;
  char             szTag[16];
  int              nMenu[2];
  int              nWinNum = 0;

  whWindow = *(int*)(cBlock + 32);
  ihIcon = *(int*)(cBlock + 36);

   // Icon bar
  if (whWindow == -2)
  {
    nWinNum = 1;
  }

  // Main window
  if (whWindow == gwhMain)
  {
    nWinNum = 2;
  }

  // Warning window
  if (whWindow == gwhWarn)
  {
    nWinNum = 3;
  }

  // Information window
  if (whWindow == gawhMenuWinHandle[0])
  {
    nWinNum = 4;
  }

//  // Save window
//  if (whWindow == gwhSave)
//  {
//    nWinNum = 5;
//  }

  if (nWinNum)
  {
    // Window
    sprintf (szTag, "H%d", nWinNum);
    strncpy (szHelpText, TagCheck (szTag), sizeof(szHelpText));
    szHelpText[sizeof(szHelpText) - 1] = 0;
    nStrLen = strlen(szHelpText);
    sprintf (szTag, "H%dI%d", nWinNum, ihIcon);
    strncpy (szHelpText + nStrLen, TagCheck (szTag), sizeof(szHelpText) - nStrLen);
    nWinNum = -1;
  }
  else
  {
    // Menu
    sRegs.r[0] = 1;
    sRegs.r[1] = (int)gpcTemp;
    sRegs.r[2] = whWindow;
    sRegs.r[3] = ihIcon;
    _kernel_swi (Wimp_GetMenuState, & sRegs, & sRegs);

    nMenu[0] = *(int*)(gpcTemp +  0);
    nMenu[1] = *(int*)(gpcTemp +  4);

    if (nMenu[1] == -1)
    {
      if (gpcMenuCurrent == gpcMainMenu)
      {
        nWinNum = 6;
      }
    }
    if (nWinNum)
    {
      sprintf (szTag, "H%dI%d", nWinNum, nMenu[0]);
      strncpy (szHelpText, TagCheck (szTag), sizeof(szHelpText));
      nWinNum = -1;
    }
  }

  if (nWinNum == -1)
  {
    *(int*)(cBlock +  0) = (24 + strlen(szHelpText)) & (~3);
    *(int*)(cBlock + 12) = *(int*)(cBlock +  8);
    *(int*)(cBlock + 16) = 0x503;
    strncpy (cBlock + 20, szHelpText, sizeof(szHelpText));

    sRegs.r[0] = 17;
    sRegs.r[1] = (int)cBlock;
    sRegs.r[2] = (*(int*)(cBlock +  4));

    _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);
  }
}

// Obtain string from Messages file if it exists
char * TagCheck (char * szTag)
{
  _kernel_swi_regs sRegs;

  sRegs.r[0] = (int)pcMessages;
  sRegs.r[1] = (int)szTag;
  sRegs.r[2] = (int)gpcTemp;
  sRegs.r[3] = 256;
  sRegs.r[4] = 0;
  _kernel_swi (MessageTrans_EnumerateTokens, & sRegs, & sRegs);

  if (sRegs.r[2] == 0)
  {
    strcpy (gpcTemp, "");
  }
  else
  {
    sRegs.r[0] = (int)pcMessages;
    sRegs.r[1] = (int)szTag;
    sRegs.r[2] = (int)gpcTemp;
    sRegs.r[3] = 256;
    sRegs.r[4] = 0;
    sRegs.r[5] = 0;
    sRegs.r[6] = 0;
    sRegs.r[7] = 0;
    _kernel_swi (MessageTrans_Lookup, & sRegs, & sRegs);
    gpcTemp[sRegs.r[2]] = 13;
    gpcTemp[sRegs.r[2] + 1] = 13;
  }

  return gpcTemp;
}

// Grey out or ungrey an icon
void SetIconGreyness (BOOL boState, int whWindow, int ihIcon)
{
  _kernel_swi_regs sRegs;

  *(int*)(gpcTemp +  0) = whWindow;
  *(int*)(gpcTemp +  4) = ihIcon;
  if (boState)
  {
    *(int*)(gpcTemp +  8) = (1<<22);
  }
  else
  {
    *(int*)(gpcTemp +  8) = (0<<22);
  }
  *(int*)(gpcTemp + 12) = (1<<22);

  sRegs.r[1] = (int)gpcTemp;
  _kernel_swi (Wimp_SetIconState, & sRegs, & sRegs);
}

// Load a file into a block of memory
char * LoadFile (char * szFilename, int * pnSize)
{
  _kernel_swi_regs sRegs;
  char             *pcMemory;

  sRegs.r[0] = 17;
  sRegs.r[1] = (int)szFilename;
  err (_kernel_swi (OS_File, & sRegs, & sRegs));

  if (sRegs.r[4] > 0)
  {
    *pnSize = (sRegs.r[4] + 3) &~3;
    pcMemory = malloc (*pnSize);

    if (pcMemory)
    {
      sRegs.r[0] = 0x10;
      sRegs.r[1] = (int)szFilename;
      sRegs.r[2] = (int)pcMemory;
      sRegs.r[3] = 0;
      err (_kernel_swi (OS_File, & sRegs, & sRegs));
    }
  }
  else
  {
    pcMemory = NULL;
  }

  return pcMemory;
}

// Load a sprite file into a block of memory
char * LoadSprites (char * szFilename)
{
  _kernel_swi_regs sRegs;
  char             *pcMemory;
  int              nSize;

  sRegs.r[0] = 17;
  sRegs.r[1] = (int)szFilename;
  err (_kernel_swi (OS_File, & sRegs, & sRegs));

  if (sRegs.r[4] > 0)
  {
    nSize = (sRegs.r[4] + 9) &~3;
    pcMemory = malloc (nSize);

    if (pcMemory)
    {
      *(int*)(pcMemory +  0) = nSize;
      *(int*)(pcMemory +  4) = 0;
      *(int*)(pcMemory +  8) = 16;
      *(int*)(pcMemory + 12) = 16;

      sRegs.r[0] = 0x10a;
      sRegs.r[1] = (int)pcMemory;
      sRegs.r[2] = (int)szFilename;
      err (_kernel_swi (OS_SpriteOp, & sRegs, & sRegs));
    }
  }
  else
  {
    pcMemory = NULL;
  }

  return pcMemory;
}

// Display an error from the Messages filein an error box
void ShowErrorTag (char * szTag)
{
    _kernel_swi_regs sRegs;

    _kernel_swi (OS_WriteI + 0x07, & sRegs, & sRegs);
    SetIconText (Tag(szTag), gwhWarn, 1);
    OpenWindowInitCentre (gwhWarn);
    CloseMenu ();

    sRegs.r[0] = gwhWarn;
    sRegs.r[1] = -1;
    sRegs.r[2] = 0;
    sRegs.r[3] = 0;
    sRegs.r[4] = -1;
    sRegs.r[5] = -1;
    _kernel_swi (Wimp_SetCaretPosition, & sRegs, & sRegs);
}

// Display an error in an error box
void ShowError (_kernel_oserror * sError)
{
    _kernel_swi_regs sRegs;

    _kernel_swi (OS_WriteI + 0x07, & sRegs, & sRegs);
    SetIconText (sError->errmess, gwhWarn, 1);
    OpenWindowInitCentre (gwhWarn);
    CloseMenu ();

    sRegs.r[0] = gwhWarn;
    sRegs.r[1] = -1;
    sRegs.r[2] = 0;
    sRegs.r[3] = 0;
    sRegs.r[4] = -1;
    sRegs.r[5] = -1;
    _kernel_swi (Wimp_SetCaretPosition, & sRegs, & sRegs);
}

// Initiate a sprite drag from the contents of the given icon
void DragSprite (int whWindow, int ihIcon, char * szIconName)
{
  _kernel_swi_regs sRegs;
  char             *pcSpriteArea;
  int              nXinc;
  int              nYinc;

  if (!gboDrag)
  {
    *(int*)(gpcTemp +  0) = whWindow;
    *(int*)(gpcTemp +  4) = ihIcon;

    sRegs.r[1] = (int)gpcTemp;
    _kernel_swi (Wimp_GetIconState, & sRegs, & sRegs);

    if ((*(int*)(gpcTemp + 24) & 0x103) == 0x102)
    {
      pcSpriteArea = (char *)*(int*)(gpcTemp + 32);
    }
    else
    {
      pcSpriteArea = 0;
    }

    *(int*)(gpcTemp + 24) = whWindow;

    sRegs.r[1] = (int)(gpcTemp + 24);
    _kernel_swi (Wimp_GetWindowInfo, & sRegs, & sRegs);

    if (pcSpriteArea == 0)
    {
      pcSpriteArea = (char *)*(int*)(gpcTemp + 24 + 68);
    }

    nXinc = *(int*)(gpcTemp + 28) - *(int*)(gpcTemp + 44);
    nYinc = *(int*)(gpcTemp + 40) - *(int*)(gpcTemp + 48);

    *(int*)(gpcTemp +  8) += nXinc;
    *(int*)(gpcTemp + 12) += nYinc;
    *(int*)(gpcTemp + 16) += nXinc;
    *(int*)(gpcTemp + 20) += nYinc;

    sRegs.r[0] = 0xc5;
    sRegs.r[1] = (int)pcSpriteArea;
    sRegs.r[2] = (int)szIconName;
    sRegs.r[3] = (int)(gpcTemp +  8);
    sRegs.r[4] = 0;

    _kernel_swi (DragASprite_Start, & sRegs, & sRegs);

    gboDrag = TRUE;
  }
}

// Initiate a outline drag from the dimensions of the given icon
void DragBox (int whWindow, int ihIcon)
{
  _kernel_swi_regs sRegs;
  int              nXinc;
  int              nYinc;

  if (!gboDrag)
  {
    *(int*)(gpcTemp +  0) = whWindow;
    *(int*)(gpcTemp +  4) = ihIcon;

    sRegs.r[1] = (int)gpcTemp;
    _kernel_swi (Wimp_GetIconState, & sRegs, & sRegs);

    *(int*)(gpcTemp + 24) = whWindow;

    sRegs.r[1] = (int)(gpcTemp + 24);
    _kernel_swi (Wimp_GetWindowState, & sRegs, & sRegs);

    *(int*)(gpcTemp +  0) = 0;
    *(int*)(gpcTemp +  4) = 5;

    nXinc = *(int*)(gpcTemp + 28) - *(int*)(gpcTemp + 44);
    nYinc = *(int*)(gpcTemp + 40) - *(int*)(gpcTemp + 48);

    *(int*)(gpcTemp +  8) += nXinc;
    *(int*)(gpcTemp + 12) += nYinc;
    *(int*)(gpcTemp + 16) += nXinc;
    *(int*)(gpcTemp + 20) += nYinc;
    *(int*)(gpcTemp + 24) = 0;
    *(int*)(gpcTemp + 28) = 0;
    *(int*)(gpcTemp + 32) = 0;
    *(int*)(gpcTemp + 36) = 0;

    sRegs.r[1] = (int)gpcTemp;
    sRegs.r[2] = 0x4b534154;
    sRegs.r[3] = 0;

    _kernel_swi (Wimp_DragBox, & sRegs, & sRegs);

    gboDrag = TRUE;
  }
}

// Initiate a URL link
void OpenURL (char * szURL)
{
  _kernel_swi_regs sRegs;

  *(int*)(gpcTemp + 12) = 0;
  *(int*)(gpcTemp + 16) = 0x4af80;

  strcpy (gpcTemp + 20, szURL);

  *(int*)(gpcTemp +  0) = (strlen(szURL) + 24) &~3;

  sRegs.r[0] = 18;
  sRegs.r[1] = (int)(gpcTemp +  0);
  sRegs.r[2] = 0;
  sRegs.r[3] = 0;
  _kernel_swi (Wimp_SendMessage, & sRegs, & sRegs);

  gnMessageMyRef = *(int*)(gpcTemp +  8);
}

// No URL lancher responded, so we have to run one ourselves
void OpenURLReturned (char * cBlock)
{
  _kernel_swi_regs sRegs;
  char             szAlias[255];

  if (gnMessageMyRef == *(int*)(cBlock +  8))
  {
    if (strncmp (cBlock + 20, "mailto:", 7) == 0)
    {
      strcpy (szAlias, "URLOpen_MailTo");
    }
    else
    {
      strcpy (szAlias, "URLOpen_HTTP");
    }

    sprintf (gpcTemp, "Alias$%s", szAlias);

    sRegs.r[0] = (int)gpcTemp;
    sRegs.r[1] = 0;
    sRegs.r[2] = -1;
    sRegs.r[3] = 0;
    sRegs.r[4] = 0;
    _kernel_swi (OS_ReadVarVal, & sRegs, & sRegs);

    if (sRegs.r[2])
    {
      sprintf (gpcTemp, "%s %s", szAlias, cBlock + 20);
      sRegs.r[0] = (int)gpcTemp;
      err (_kernel_swi (Wimp_StartTask, & sRegs, & sRegs));
    }
  }
}

// Close the currently open menu
void CloseMenu (void)
{
  _kernel_swi_regs sRegs;

  sRegs.r[1] = -1;
  _kernel_swi (Wimp_CreateMenu, & sRegs, & sRegs);

  gnMenuXpos = 0;
  gnMenuYpos = 0;
  gpcMenuCurrent = NULL;
}

//// Save box save button clicked
//void SaveSave (void)
//{
//  char             *szFilename;
//
//  szFilename = GetIconText (gwhSave, 2);
//
//  if (strchr(szFilename, '.'))
//  {
//    // Save the file
//    SaveFile (szFilename);
//  }
//  else
//  {
//    ShowErrorTag ("Er1");
//  }
//}

// Close the warning message box
void CloseWarning (void)
{
  CloseWindowHandle (gwhWarn);
}

//// File loaded routine
//void FileLoaded (char * pcMemory, int nSize)
//{
//  _kernel_swi_regs sRegs;
//
//  // Save the file out, for the sake of the example
//  sRegs.r[0] = 10;
//  sRegs.r[1] = (int)GetIconText (gwhSave, 2);
//  sRegs.r[2] = 0xffd;
//  sRegs.r[4] = (int)(pcMemory);
//  sRegs.r[5] = (int)(pcMemory + nSize);
//  err (_kernel_swi (OS_File, & sRegs, & sRegs));
//
//  // Free up the memory
//  free (pcMemory);
//}

//// Save file routine - saves the example file out
//void SaveFile (char * szFilename)
//{
//  _kernel_swi_regs sRegs;
//
//  sRegs.r[0] = 10;
//  sRegs.r[1] = (int)szFilename;
//  sRegs.r[2] = gnSaveFileType;
//  sRegs.r[4] = (int)(gpcExample);
//  sRegs.r[5] = (int)(gpcExample + gnExampleSize);
//  err (_kernel_swi (OS_File, & sRegs, & sRegs));
//}

// Check if a file exists and has non-zero size
bool FileNotEmpty (char * szFilename)
{
  fileswitch_object_type nObjectType;
  int nSize;
  bool boReturn;

  xosfile_read_stamped_no_path (szFilename, & nObjectType, NULL, NULL,
    & nSize, NULL, NULL);

  if ((nObjectType == fileswitch_IS_FILE) && (nSize > 0))
  {
    boReturn = TRUE;
  }
  else
  {
    boReturn = FALSE;
  }

  return boReturn;
}

// Set up the scribble for the particular file
bool SetScribble (char * szLeaf, int nIcon)
{
  char szFileR[1024];
  char szFile[1024];
  bool boReturn;

  sprintf (szFileR, "<OpenTTD$Dir>.data.%sr/grf", szLeaf);
  sprintf (szFile, "<OpenTTD$Dir>.data.%s/grf", szLeaf);

  if (FileNotEmpty (szFileR)
    || FileNotEmpty (szFile))
  {
    SetIconText ("scribble", gwhMain, nIcon);
    boReturn = TRUE;
  }
  else
  {
    SetIconText ("", gwhMain, nIcon);
    boReturn = FALSE;
  }

  return boReturn;
}

// Check to see if the installation files exist
bool CheckFiles (void)
{
  bool boInstalled;
  wimp_icon_state sIconState;

  boInstalled = SetScribble ("trg1", 20);
  boInstalled &= SetScribble ("trgc", 21);
  boInstalled &= SetScribble ("trgh", 22);
  boInstalled &= SetScribble ("trgi", 23);
  boInstalled &= SetScribble ("trgt", 24);

  if (FileNotEmpty ("<OpenTTD$Dir>.data.sample/cat"))
  {
    SetIconText ("scribble", gwhMain, 25);
    boInstalled &= TRUE;
  }
  else
  {
    SetIconText ("", gwhMain, 25);
    boInstalled &= FALSE;
  }

  if (boInstalled)
  {
    SetIconText (Tag ("OK"), gwhMain, 19);
    SetIconText (Tag ("Text2"), gwhMain, 18);
  }
  else
  {
    SetIconText (Tag ("Cancel"), gwhMain, 19);
    SetIconText (Tag ("Text1"), gwhMain, 18);
  }

  // Redraw things
  sIconState.w = (wimp_w)gwhMain;
  sIconState.i = (wimp_i)0;
  wimp_get_icon_state (& sIconState);
  wimp_force_redraw ((wimp_w)gwhMain, sIconState.icon.extent.x0,
    sIconState.icon.extent.y0, sIconState.icon.extent.x1,
    sIconState.icon.extent.y1);

  return boInstalled;
}

//////////////////////////////////////////////////////////////////
// Compare two strings
int cmpnstr (char * szString1, char * szString2, int nSize)
{
  int                   nPos;
  int                   nReturn = 0;
  char                  cChar1;
  char                  cChar2;

  nPos = -1;
  do
  {
    nPos++;

    cChar1 = szString1[nPos];
    cChar2 = szString2[nPos];

    if ((cChar1 >= 'A') && (cChar1 <= 'Z'))
    {
      cChar1 += ('a' - 'A');
    }
    if ((cChar2 >= 'A') && (cChar2 <= 'Z'))
    {
      cChar2 += ('a' - 'A');
    }

    if (cChar1 < cChar2)
    {
      nReturn = +1;
    }
    if (cChar1 > cChar2)
    {
      nReturn = -1;
    }
  } while ((nReturn == 0) && (cChar1 != 0)  && (cChar2 != 0)
    && (nPos < (nSize - 1)));

  return nReturn;
}
