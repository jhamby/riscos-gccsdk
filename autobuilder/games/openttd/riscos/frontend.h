//////////////////////////////////////////////////////////////////
// OpenTTD FrontEnd
//
// C program shell for rapid application development
// The Flying Pig!
// Started 17/7/2003
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// Includes

#include "oslib/wimp.h"

//////////////////////////////////////////////////////////////////
// Defines

//#define _DEBUG

#define FLAGS                 ((wimp_poll_flags)0x3c33)

#define BOOL                  int

#ifndef TRUE
#define TRUE                  (1)
#endif
#ifndef FALSE
#define FALSE                 (0)
#endif

#define MESSAGES              "<OpenTTD$Dir>.Resources.Installer.Messages"
#define TEMPLATES             "<OpenTTD$Dir>.Resources.Installer.Templates"
#define SPRITES               "<OpenTTD$Dir>.Resources.Installer.Sprites"
#define RUNGAME               "<OpenTTD$Dir>.Resources.RunGame"
#define EMAIL                 "mailto:david@flypig.co.uk"
#define WEBSITE               "http://www.flypig.co.uk/"

#if defined _DEBUG
#define REPORT                Report
#define REPORTVAR             ReportVar
void    Report                (char * szMessage);
void    ReportVar             (char * szFormat, int nVariable);
#else
#define REPORT                //
#define REPORTVAR             //
#endif

//////////////////////////////////////////////////////////////////
// Structures

//////////////////////////////////////////////////////////////////
// Function prototypes

inline void err               (_kernel_oserror * sError);
void    Receive               (char * cBlock);
void    Acknowledged          (char * cBlock);
void    Quit                  (void);
char    *Tag                  (char * szTag);
int     CreateIconbarIcon     (char * szSprite, int nWidth, int nHeight);
int     LoadTemplate          (char * szWindowTitle);
int     LoadTemplateSprites   (char * szWindowTitle, char * pcSpriteArea);
void    OpenWindow            (char * cBlock);
void    MouseClick            (char * cBlock);
void    OpenWindowInit        (int whWindow);
void    OpenWindowInitCentre  (int whWindow);
void    CloseWindow           (char * cBlock);
void    CloseWindowHandle     (int whWindow);
char    *CreateMenu           (char * szMenu);
void    OpenMenu              (char * pcMenu, int nXpos, int nYpos);
void    MenuSelect            (char * cBlock);
void    Keys                  (char * cBlock);
char    *GetIconText          (int whWindow, int ihIcon);
BOOL    GetIconSelectionState (int whWindow, int ihIcon);
void    SetIconSelectionState (BOOL boState, int whWindow, int ihIcon);
void    SetIconText           (char * szText, int whWindow, int ihIcon);
void    UserDragBox           (char * cBlock);
void    DataSave              (char * cBlock);
void    DataSaveAck           (char * cBlock);
void    DataLoad              (char * cBlock);
void    DataOpen              (char * cBlock);
void    RamFetch              (char * cBlock);
void    RamTransmit           (char * cBlock);
void    Help                  (char * cBlock);
char    *TagCheck             (char * szTag);
void    SetIconGreyness       (BOOL boState, int whWindow, int ihIcon);
char    *LoadFile             (char * szFilename, int * pnSize);
char    *LoadSprites          (char * szFilename);
void    ShowErrorTag          (char * szTag);
void    ShowError             (_kernel_oserror * sError);
void    DragSprite            (int whWindow, int ihIcon, char * szIconName);
void    DragBox               (int whWindow, int ihIcon);
void    OpenURL               (char * szURL);
void    OpenURLReturned       (char * cBlock);
void    CloseMenu             (void);
void    SaveSave              (void);
void    CloseWarning          (void);
void    FileLoaded            (char * pcMemory, int nSize);
void    SaveFile              (char * szFilename);


