/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "nsCOMPtr.h"
#include "nsIServiceManager.h"
#include "nsIComponentManager.h"
#include "nsINameSpaceManager.h"
#include "nsIMenu.h"
#include "nsIMenuItem.h"
#include "nsIContent.h"

#include "nsMenuBar.h"
#include "nsMenu.h"

#include "nsISupports.h"
#include "nsIWidget.h"
#include "nsString.h"
//#include "nsIStringBundle.h"
#include "nsIDocument.h"
#include "nsIDocShell.h"
#include "nsIDocumentViewer.h"
#include "nsIDocumentObserver.h"

#include "nsIDOMDocument.h"
#include "nsWidgetAtoms.h"

#include "nsGUIEvent.h"

#include "DeskLib:Event.h"
#include "DeskLib:WimpSWIs.h"


#include <stdio.h>

// CIDs
#include "nsWidgetsCID.h"
static NS_DEFINE_CID(kMenuCID, NS_MENU_CID);

NS_IMPL_ISUPPORTS5(nsMenuBar, nsIMenuBar, nsIMenuListener, nsIDocumentObserver,
                    nsIChangeManager, /*nsIMenuCommandDispatcher,*/ nsISupportsWeakReference)

extern menu_ptr gCurrentROMenu;  

//
// nsMenuBar constructor
//
nsMenuBar::nsMenuBar() : mDocument(nsnull), mNumMenus(0)
{
  puts("menubar::new menubar");
}


//
// nsMenuBar destructor
//
nsMenuBar::~nsMenuBar()
{
  puts("menubar::kill menubar");

  mMenusArray.Clear();    // release all menus

  // make sure we unregister ourselves as a document observer
  if ( mDocument ) {
    nsCOMPtr<nsIDocumentObserver> observer ( do_QueryInterface(NS_STATIC_CAST(nsIMenuBar*,this)) );
    mDocument->RemoveObserver(observer);
  }

  if (mROMenu)
    Menu_FullDispose(mROMenu);

  Event_Release(event_CLICK, event_ANY, event_ANY, MenuOpen, this);
  Event_Release(event_MENU, event_ANY, event_ANY, MenuHandler, this);
}


//-------------------------------------------------------------------------
//
// Create the proper widget
//
//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::Create(nsIWidget *aParent)
{
  puts("menubar::Create menubar");

  SetParent(aParent);
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::GetParent(nsIWidget *&aParent)
{
  puts("menubar::GetParent");
  NS_IF_ADDREF(aParent = mParent);
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::SetParent(nsIWidget *aParent)
{
  puts("menubar::Set Parent");
  mParent = aParent;    // weak ref
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::AddMenu(nsIMenu * aMenu)
{
  // keep track of all added menus.
  mMenusArray.AppendElement(aMenu);    // owner

  menu_ptr menu = nsnull;
  aMenu->GetNativeData((void**)&menu);
  ((nsMenu *)aMenu)->SetROPosition(mNumMenus);

  Menu_AddSubMenu(mROMenu, mNumMenus, menu);
  mNumMenus++;

  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::GetMenuCount(PRUint32 &aCount)
{
  puts("menubar::Get menu count");
  aCount = mNumMenus;
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::GetMenuAt(const PRUint32 aCount, nsIMenu *& aMenu)
{ 
  aMenu = NULL;
  nsCOMPtr<nsISupports> supports = getter_AddRefs(mMenusArray.ElementAt(aCount));
  if (!supports) return NS_OK;

  return CallQueryInterface(supports, &aMenu); // addref
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::InsertMenuAt(const PRUint32 aCount, nsIMenu *& aMenu)
{
  puts("menubar::InsertMenuAt");
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::RemoveMenu(const PRUint32 aCount)
{
  puts("menubar::RemoveMenu");
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::RemoveAll()
{
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::GetNativeData(void *& aData)
{
    puts("menubar::GetNativeData");
    aData = nsnull;
    return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::SetNativeData(void* aData)
{
    puts("menubar::SetNativeData");
    return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuBar::Paint()
{
    puts("menubar::Paint");
    return NS_OK;
}


nsEventStatus 
nsMenuBar::MenuItemSelected(const nsMenuEvent & aMenuEvent)
{
  // Dispatch menu event


  puts("menubar::menuitemselected");
  // Dispatch menu event
  nsEventStatus eventStatus = nsEventStatus_eIgnore;

  PRUint32  numItems;
  mMenusArray.Count(&numItems);

  while (numItems != 0) {
    nsCOMPtr<nsISupports>     menuSupports = getter_AddRefs(mMenusArray.ElementAt(--numItems));
    nsCOMPtr<nsIMenuListener> menuListener = do_QueryInterface(menuSupports);

    if (menuListener)
    {
      eventStatus = menuListener->MenuItemSelected(aMenuEvent);
      if (nsEventStatus_eIgnore != eventStatus)
        return eventStatus;
    }
  }
  return eventStatus;
}


nsEventStatus 
nsMenuBar::MenuSelected(const nsMenuEvent & aMenuEvent)
{
  // Dispatch event
  puts("menubar::menuselected");
  return nsEventStatus_eIgnore;
}


nsEventStatus 
nsMenuBar::MenuDeselected(const nsMenuEvent & aMenuEvent)
{
  puts("menubar::menudeselected");
  return nsEventStatus_eIgnore;
}


BOOL
nsMenuBar::MenuOpen(event_pollblock *event, void *reference)
{
  nsMenuBar *menubar = (nsMenuBar *)reference;

  printf("MenuBar MenuOpen: %p\n", (void *)menubar);

  if (!event->data.mouse.button.data.menu ||
      event->data.mouse.window < 0)
    return FALSE;

  if (menubar->mNeedsRebuild) {
    puts("menubar needs rebuild");

      nsCOMPtr<nsIDocShell> docShell = do_QueryReferent(menubar->mDocShellWeakRef);
      const nsMenuEvent aMenuEvent(PR_FALSE, 0, nsnull);
      menubar->MenuConstruct(aMenuEvent, nsnull, nsnull, docShell);
   } else {
     puts("no rebuild");
   }

  Menu_Show(menubar->mROMenu, event->data.mouse.pos.x, event->data.mouse.pos.y); 
  gCurrentROMenu = menubar->mROMenu;

//  dump_menu(menubar->mROMenu);

  return TRUE;
}



extern "C" {
void Chox11_CreateInternalEvent(void (*callback_func)(void *), void *arg);
}

void nsMenuBar::MenuHandlerFunc(void *arg)
{
  event_pollblock *event = (event_pollblock *)arg;
  nsMenuBar *menubar = (nsMenuBar *)event->type;
  const int *selectionData = event->data.selection;

  nsCOMPtr<nsIMenu> menu;
  menubar->GetMenuAt((PRUint32)selectionData[0], *getter_AddRefs(menu));
  nsCOMPtr<nsIMenuListener> menuListener = do_QueryInterface(menu);

  if (menuListener) {
    nsMenuEvent menuEvent(PR_TRUE, NS_MENU_SELECTED, nsnull);
    menuEvent.time = PR_IntervalNow();
    menuEvent.mCommand = (PRUint32)(selectionData + 1);

    gCurrentROMenu = (Menu_FirstItem(gCurrentROMenu) +
                     selectionData[0])->submenu.menu;

    menuListener->MenuSelected(menuEvent);
  }

  mouse_block ptr;
  Wimp_GetPointerInfo(&ptr);
  if (ptr.button.data.adjust) {
    gCurrentROMenu = menubar->mROMenu;
    Menu_ShowLast();
  }
}


BOOL
nsMenuBar::MenuHandler(event_pollblock *event, void *reference)
{
  nsMenuBar *menubar = (nsMenuBar *)reference;

  if (gCurrentROMenu == menubar->mROMenu) {
    /* Cause callback in main thread */
    event_pollblock *data = (event_pollblock *)malloc(sizeof(event_pollblock));

    if (data) {
      memcpy(data, event, sizeof(event_pollblock));
      data->type = (event_type)(int)reference;
      Chox11_CreateInternalEvent(MenuHandlerFunc, data);
    }
    return TRUE;
  }
  return FALSE;
}





nsEventStatus
nsMenuBar::MenuConstruct( const nsMenuEvent & aMenuEvent, nsIWidget* aParentWindow,
                            void * menubarNode, void * aDocShell )
{
  puts("menubar::construct");

  mDocShellWeakRef = do_GetWeakReference(NS_STATIC_CAST(nsIDocShell*, aDocShell));
  nsIDOMNode* aDOMNode  = NS_STATIC_CAST(nsIDOMNode*, menubarNode);
  mMenuBarContent = do_QueryInterface(aDOMNode);           // strong ref

  if ( !mMenuBarContent ) {
    puts("no content");
    return nsEventStatus_eIgnore;
  }

  Create(aParentWindow);

  nsCOMPtr<nsIDocShell> docShell = do_QueryReferent(mDocShellWeakRef);
  if (docShell) RegisterAsDocumentObserver(docShell);

  // set this as a nsMenuListener on aParentWindow
  aParentWindow->AddMenuListener((nsIMenuListener *)this);

  PRUint32 count = mMenuBarContent->GetChildCount();
printf("menu count: %d\n", count);

  mROMenu = Menu_NewEmpty("Mozilla Firefox", count);
  if (!mROMenu) return nsEventStatus_eIgnore;

  for ( PRUint32 i = 0; i < count; ++i ) {
    nsIContent *menu = mMenuBarContent->GetChildAt(i);
    if ( menu ) {
//printf("menu: %x %d %d\n", menu->Tag(), menu->Tag() == nsWidgetAtoms::menu,
//          menu->IsContentOfType(nsIContent::eXUL));
      if (menu->Tag() == nsWidgetAtoms::menu &&
          menu->IsContentOfType(nsIContent::eXUL)) {

        nsAutoString menuName;
        nsAutoString menuAccessKey(NS_LITERAL_STRING(" "));
        menu->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, menuName);
        menu->GetAttr(kNameSpaceID_None, nsWidgetAtoms::accesskey, menuAccessKey);

//printf("name: %s\n", NS_LossyConvertUCS2toASCII(menuName).get());
        const char *text = NS_LossyConvertUCS2toASCII(menuName).get();
/*        char *hs = (char *)alloca(strlen(text) + 1);
        int pos = 0;

        do {
          if (text[pos] != ' ')
            hs[pos] = text[pos];
          else
            hs[pos] = 160;
        } while (text[pos++] != '\0');*/

        Menu_SetText(mROMenu, i, text);
        // Don't create the whole menu yet, just add in the top level names

        // Create nsMenu, the menubar will own it
        nsCOMPtr<nsIMenu> pnsMenu ( do_CreateInstance(kMenuCID) );
        if ( pnsMenu ) {
//printf("create menu\n");
          pnsMenu->Create(NS_STATIC_CAST(nsIMenuBar*, this), menuName, menuAccessKey,
                          NS_STATIC_CAST(nsIChangeManager *, this), 
                          NS_REINTERPRET_CAST(nsIDocShell*, aDocShell), menu);

          // Make nsMenu a child of nsMenuBar. nsMenuBar takes ownership
          AddMenu(pnsMenu);
//puts("done add menu");
                  
//          nsAutoString menuIDstring;
//          menu->GetAttr(kNameSpaceID_None, nsWidgetAtoms::id, menuIDstring);
//          if ( menuIDstring.EqualsLiteral("menu_Help") ) {
//            nsMenuEvent event(PR_TRUE, 0, nsnull);
//            MenuHandle handle = nsnull;
//            event.mCommand = (unsigned int) handle;

//            nsCOMPtr<nsIMenuListener> listener(do_QueryInterface(pnsMenu));
//            listener->MenuSelected(event);
// //          }          
        }
      } 
    }
  } // for each menu

  // Give the aParentWindow this nsMenuBar to hold onto.
  // The parent takes ownership
//puts("call set menubar");
  aParentWindow->SetMenuBar(this);

//puts("call event claim");
  Event_Claim(event_CLICK, event_ANY, event_ANY, MenuOpen, this);
  Event_Claim(event_MENU, event_ANY, event_ANY, MenuHandler, this);

  //dump_menu(mROMenu);
puts("menubar::menuconstruct: no longer needs rebuild");
  mNeedsRebuild = PR_FALSE;

  return nsEventStatus_eIgnore;
}

nsEventStatus 
nsMenuBar::MenuDestruct(const nsMenuEvent & aMenuEvent)
{
  puts("menubar::menudestruct");
  return nsEventStatus_eIgnore;
}


nsEventStatus 
nsMenuBar::CheckRebuild(PRBool & aNeedsRebuild)
{
  puts("menubar::checkrebuild");
  aNeedsRebuild = PR_TRUE;
  return nsEventStatus_eIgnore;
}

nsEventStatus
nsMenuBar::SetRebuild(PRBool aNeedsRebuild)
{
  puts("menubar::setrebuild");
  return nsEventStatus_eIgnore;
}
void
nsMenuBar :: GetDocument ( nsIDocShell* inDocShell, nsIDocument** outDocument )
{
  *outDocument = nsnull;
  
  if ( inDocShell ) {
    nsCOMPtr<nsIContentViewer> cv;
    inDocShell->GetContentViewer(getter_AddRefs(cv));
    if (cv) {
      // get the document
      nsCOMPtr<nsIDocumentViewer> docv(do_QueryInterface(cv));
      if (!docv)
        return;
      docv->GetDocument(outDocument);    // addrefs
    }
  }
}

//
// RegisterAsDocumentObserver
//
// Name says it all.
//
void
nsMenuBar::RegisterAsDocumentObserver ( nsIDocShell* inDocShell )
{
  nsCOMPtr<nsIDocument> doc;
  GetDocument(inDocShell, getter_AddRefs(doc));
  if (!doc)
    return;

  // register ourselves
  nsCOMPtr<nsIDocumentObserver> observer ( do_QueryInterface(NS_STATIC_CAST(nsIMenuBar*,this)) );
  doc->AddObserver(observer);
  // also get pointer to doc, just in case docshell goes away
  // we can still remove ourself as doc observer directly from doc
  mDocument = doc;
} // RegisterAsDocumentObesrver


//
// nsIDocumentObserver
// this is needed for menubar changes
//

NS_IMPL_NSIDOCUMENTOBSERVER_LOAD_STUB(nsMenuBar)
NS_IMPL_NSIDOCUMENTOBSERVER_REFLOW_STUB(nsMenuBar)
NS_IMPL_NSIDOCUMENTOBSERVER_STATE_STUB(nsMenuBar)
NS_IMPL_NSIDOCUMENTOBSERVER_STYLE_STUB(nsMenuBar)

void
nsMenuBar::BeginUpdate( nsIDocument * aDocument, nsUpdateType aUpdateType )
{
}

void
nsMenuBar::EndUpdate( nsIDocument * aDocument, nsUpdateType aUpdateType )
{
}

void
nsMenuBar::CharacterDataChanged( nsIDocument * aDocument,
                                  nsIContent * aContent, PRBool aAppend)
{
}

void
nsMenuBar::ContentAppended( nsIDocument * aDocument, nsIContent  * aContainer,
                              PRInt32 aNewIndexInContainer)
{
  if ( aContainer == mMenuBarContent ) {
    //Register(aContainer, );
    //InsertMenu ( aNewIndexInContainer );
  }
  else {
    nsCOMPtr<nsIChangeObserver> obs;
    Lookup ( aContainer, getter_AddRefs(obs) );
    if ( obs )
      obs->ContentInserted ( aDocument, aContainer, aNewIndexInContainer );
    else {
      nsCOMPtr<nsIContent> parent = aContainer->GetParent();
      if(parent) {
        Lookup ( parent, getter_AddRefs(obs) );
        if ( obs )
          obs->ContentInserted ( aDocument, aContainer, aNewIndexInContainer );
      }
    }
  }
}

void
nsMenuBar::DocumentWillBeDestroyed( nsIDocument * aDocument )
{
  mDocument = nsnull;
}


void
nsMenuBar::AttributeChanged( nsIDocument * aDocument, nsIContent * aContent,
                              PRInt32 aNameSpaceID, nsIAtom * aAttribute,
                              PRInt32 aModType )
{
//puts("attribute changed menubar listener");
  // lookup and dispatch to registered thang.
  nsCOMPtr<nsIChangeObserver> obs;
  Lookup ( aContent, getter_AddRefs(obs) );
  if ( obs )
    obs->AttributeChanged ( aDocument, aNameSpaceID, aContent, aAttribute );
  mNeedsRebuild = PR_TRUE;
}

void
nsMenuBar::ContentRemoved( nsIDocument * aDocument, nsIContent * aContainer,
                            nsIContent * aChild, PRInt32 aIndexInContainer )
{  
  if ( aContainer == mMenuBarContent ) {
    Unregister(aChild);
    RemoveMenu ( aIndexInContainer );
  }
  else {
    nsCOMPtr<nsIChangeObserver> obs;
    Lookup ( aContainer, getter_AddRefs(obs) );
    if ( obs )
      obs->ContentRemoved ( aDocument, aChild, aIndexInContainer );
    else {
      nsCOMPtr<nsIContent> parent = aContainer->GetParent();
      if(parent) {
        Lookup ( parent, getter_AddRefs(obs) );
        if ( obs )
          obs->ContentRemoved ( aDocument, aChild, aIndexInContainer );
      }
    }
  }
}

void
nsMenuBar::ContentInserted( nsIDocument * aDocument, nsIContent * aContainer,
                             nsIContent * aChild, PRInt32 aIndexInContainer )
{  
  if ( aContainer == mMenuBarContent ) {
    //Register(aChild, );
    //InsertMenu ( aIndexInContainer );
  }
  else {
    nsCOMPtr<nsIChangeObserver> obs;
    Lookup ( aContainer, getter_AddRefs(obs) );
    if ( obs )
      obs->ContentInserted ( aDocument, aChild, aIndexInContainer );
    else {
      nsCOMPtr<nsIContent> parent = aContainer->GetParent();
      if(parent) {
        Lookup ( parent, getter_AddRefs(obs) );
        if ( obs )
          obs->ContentInserted ( aDocument, aChild, aIndexInContainer );
      }
    }
  }
}


//
// nsIChangeManager
//
// We don't use a |nsSupportsHashtable| because we know that the lifetime of all these items
// is bouded by the lifetime of the menubar. No need to add any more strong refs to the
// picture because the containment hierarchy already uses strong refs.
//

NS_IMETHODIMP 
nsMenuBar :: Register ( nsIContent *aContent, nsIChangeObserver *aMenuObject )
{
  nsVoidKey key ( aContent );
  mObserverTable.Put ( &key, aMenuObject );
  
  return NS_OK;
}


NS_IMETHODIMP 
nsMenuBar :: Unregister ( nsIContent *aContent )
{
  nsVoidKey key ( aContent );
  mObserverTable.Remove ( &key );
  
  return NS_OK;
}


NS_IMETHODIMP 
nsMenuBar :: Lookup ( nsIContent *aContent, nsIChangeObserver **_retval )
{
  *_retval = nsnull;
  
  nsVoidKey key ( aContent );
  *_retval = NS_REINTERPRET_CAST(nsIChangeObserver*, mObserverTable.Get(&key));
  NS_IF_ADDREF ( *_retval );
  
  return NS_OK;
}


//
// ExecuteCommand
//
// Execute the menu item by sending a command message to the 
// DOM node specified in |inDispatchTo|.
//
//nsEventStatus
//nsMenuBar::ExecuteCommand ( nsIContent* inDispatchTo )
//{
//  if (!inDispatchTo)
//    return nsEventStatus_eIgnore;
//
//  return MenuHelpers::DispatchCommandTo(mDocShellWeakRef, inDispatchTo);
//} // ExecuteCommand
nsEventStatus
nsMenuBar :: ExecuteCommand ( nsIContent* inDispatchTo )
{
  nsEventStatus status = nsEventStatus_eIgnore;
  if ( inDispatchTo ) {
    nsCOMPtr<nsIDocShell> docShell = do_QueryReferent(mDocShellWeakRef);
    if (!docShell)
      return nsEventStatus_eConsumeNoDefault;
    nsCOMPtr<nsPresContext> presContext;
    MenuHelpers::DocShellToPresContext(docShell, getter_AddRefs(presContext));

    nsMouseEvent event(PR_TRUE, NS_XUL_COMMAND, nsnull, nsMouseEvent::eReal);

    inDispatchTo->HandleDOMEvent(presContext, &event, nsnull, NS_EVENT_FLAG_INIT, &status);
  }

  return status;
} // ExecuteCommand


//
// DocShellToPresContext
//
// Helper to dig out a pres context from a docshell. A common thing to do before
// sending an event into the DOM.
//
nsresult
MenuHelpers::DocShellToPresContext (nsIDocShell* inDocShell, nsPresContext** outContext )
{
  NS_ENSURE_ARG_POINTER(outContext);
  *outContext = nsnull;
  if (!inDocShell)
    return NS_ERROR_INVALID_ARG;
  
  nsresult retval = NS_OK;
  
  nsCOMPtr<nsIContentViewer> contentViewer;
  inDocShell->GetContentViewer(getter_AddRefs(contentViewer));
  if ( contentViewer ) {
    nsCOMPtr<nsIDocumentViewer> docViewer ( do_QueryInterface(contentViewer) );
    if ( docViewer )
      docViewer->GetPresContext(outContext);     // AddRefs for us
    else
      retval = NS_ERROR_FAILURE;
  }
  else
    retval = NS_ERROR_FAILURE;
  
  return retval;
  
} // DocShellToPresContext

nsEventStatus
MenuHelpers::DispatchCommandTo(nsIWeakReference* aDocShellWeakRef,
                               nsIContent* aTargetContent)
{
puts("helpers::dispatchcommandto");
  NS_PRECONDITION(aTargetContent, "null ptr");

  nsCOMPtr<nsIDocShell> docShell = do_QueryReferent(aDocShellWeakRef);
  if (!docShell)
    return nsEventStatus_eConsumeNoDefault;
  nsCOMPtr<nsPresContext> presContext;
  MenuHelpers::DocShellToPresContext(docShell, getter_AddRefs(presContext));

  nsEventStatus status = nsEventStatus_eConsumeNoDefault;
  nsMouseEvent event(PR_TRUE, NS_XUL_COMMAND, nsnull, nsMouseEvent::eReal);

  // FIXME: Should probably figure out how to init this with the actual
  // pressed keys, but this is a big old edge case anyway. -dwh

  // See if we have a command element.  If so, we execute on the
  // command instead of on our content element.
  nsAutoString command;
  aTargetContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::command, command);
  if (!command.IsEmpty()) {
    puts("not empty");
    nsCOMPtr<nsIDOMDocument> domDoc(do_QueryInterface(aTargetContent->GetDocument()));
    nsCOMPtr<nsIDOMElement> commandElt;
    domDoc->GetElementById(command, getter_AddRefs(commandElt));
    nsCOMPtr<nsIContent> commandContent(do_QueryInterface(commandElt));
    if (commandContent) {
      commandContent->HandleDOMEvent(presContext, &event, nsnull,
                                     NS_EVENT_FLAG_INIT, &status);
      puts("has content");
    }
  }
  else {
    puts("handle dom event");
    aTargetContent->HandleDOMEvent(presContext, &event, nsnull,
                                   NS_EVENT_FLAG_INIT, &status);
  }

  return status;
}



