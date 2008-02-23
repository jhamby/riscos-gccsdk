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
#include "nsIDocument.h"
#include "nsIContent.h"
#include "nsIDOMDocument.h"
#include "nsIDocumentViewer.h"
#include "nsIDocumentObserver.h"
#include "nsIComponentManager.h"
#include "nsIDocShell.h"
#include "prinrval.h"

#include "nsMenu.h"
#include "nsMenuBar.h"
#include "nsIMenu.h"
#include "nsIMenuBar.h"
#include "nsIMenuItem.h"
#include "nsIMenuListener.h"

#include "nsString.h"
#include "nsReadableUtils.h"
#include "plstr.h"

#include "nsINameSpaceManager.h"
#include "nsWidgetAtoms.h"
#include "nsIXBLService.h"
#include "nsIServiceManager.h"

#include "nsGUIEvent.h"

#include "nsCRT.h"

static PRBool gConstructingMenu = PR_FALSE;


// CIDs
#include "nsWidgetsCID.h"
static NS_DEFINE_CID(kMenuCID,     NS_MENU_CID);
static NS_DEFINE_CID(kMenuItemCID, NS_MENUITEM_CID);

// Refcounted class for dummy menu items, like separators and help menu items.
class nsDummyMenuItem : public nsISupports {
public:
    NS_DECL_ISUPPORTS

    nsDummyMenuItem()
    {
    }
};

NS_IMETHODIMP_(nsrefcnt) nsDummyMenuItem::AddRef() { return ++mRefCnt; }
NS_METHOD nsDummyMenuItem::Release() { return --mRefCnt; }
NS_IMPL_QUERY_INTERFACE0(nsDummyMenuItem)
static nsDummyMenuItem gDummyMenuItem;

//-------------------------------------------------------------------------
NS_IMPL_ISUPPORTS4(nsMenu, nsIMenu, nsIMenuListener, nsIChangeObserver, nsISupportsWeakReference)


//
// nsMenu constructor
//
nsMenu::nsMenu()
    :   mNumMenuItems(0), mParent(nsnull), mManager(nsnull), mROMenu(nsnull),
        mNeedsRebuild(PR_TRUE)
{
//  puts("menu::constructor");
//#if DEBUG
//  ++gMenuCounterX;
//#endif
}


//
// nsMenu destructor
//
nsMenu::~nsMenu()
{
  RemoveAll();

  printf("menu::destructor\n");// %s\n", mROMenu->title);
//  abort();

//  if (mROMenu)
//    Menu_FullDispose(mROMenu);
}


void
nsMenu::GetMenuPopupContent(nsIContent** aResult)
{
  if (!aResult )
    return;
  *aResult = nsnull;
  
  nsresult rv;
  nsCOMPtr<nsIXBLService> xblService = 
           do_GetService("@mozilla.org/xbl;1", &rv);
  if ( !xblService )
    return;
  
  PRUint32 count = mMenuContent->GetChildCount();

  for (PRUint32 i = 0; i < count; i++) {
    PRInt32 dummy;
    nsIContent *child = mMenuContent->GetChildAt(i);
    nsCOMPtr<nsIAtom> tag;
    xblService->ResolveTag(child, &dummy, getter_AddRefs(tag));
    if (tag == nsWidgetAtoms::menupopup) {
      *aResult = child;
      NS_ADDREF(*aResult);
      return;
    }
  }

} // GetMenuPopupContent


//
// Create
//
NS_METHOD 
nsMenu::Create(nsISupports * aParent, const nsAString &aLabel, const nsAString &aAccessKey, 
                nsIChangeManager* aManager, nsIDocShell* aShell, nsIContent* aNode )
{
  mDocShellWeakRef = do_GetWeakReference(aShell);
  mMenuContent = aNode;

  printf("nsMenu::MenuCreate called for %s\n", NS_LossyConvertUCS2toASCII(aLabel).get());

  // register this menu to be notified when changes are made to our content object
  mManager = aManager;	// weak ref
  nsCOMPtr<nsIChangeObserver> changeObs ( do_QueryInterface(NS_STATIC_CAST(nsIChangeObserver*, this)) );
  mManager->Register(mMenuContent, changeObs);

  NS_ASSERTION ( mMenuContent, "Menu not given a dom node at creation time" );
  NS_ASSERTION ( mManager, "No change manager given, can't tell content model updates" );

  mParent = aParent;
  // our parent could be either a menu bar (if we're toplevel) or a menu (if we're a submenu)
  nsCOMPtr<nsIMenuBar> menubar = do_QueryInterface(aParent);
  nsCOMPtr<nsIMenu> menu = do_QueryInterface(aParent);
  NS_ASSERTION(menubar || menu, "Menu parent not a menu bar or menu!" );

  SetLabel(aLabel);
  SetAccessKey(aAccessKey);

  nsAutoString hiddenValue, collapsedValue;
  mMenuContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::hidden, hiddenValue);
  mMenuContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::collapsed, collapsedValue);
//  if ( hiddenValue.EqualsLiteral("true") || collapsedValue.EqualsLiteral("true") )
//    mVisible = PR_FALSE;

//  if (menubar && mMenuContent->GetChildCount() == 0)
//    mVisible = PR_FALSE;

  nsCOMPtr<nsIContent> popupContent;
  GetMenuPopupContent(getter_AddRefs(popupContent));

  PRUint32 count = popupContent->GetChildCount();
  mROMenu = Menu_NewEmpty(NS_LossyConvertUCS2toASCII(aLabel).get(), count);

  printf("count: %d\n", count);

  PRUint32 ROPosition = 0, item;
  menu_item *lastItem = nsnull;

  for ( item = 0; item < count; item++ ) {
    nsIContent *child = popupContent->GetChildAt(item);
    if ( child ) {

      printf("item: %d of %d\n", item, count);

      nsIAtom *tag = child->Tag();

      if (lastItem != nsnull && tag == nsWidgetAtoms::menuseparator &&
          !lastItem->menuflags.data.dotted) {
        lastItem->menuflags.data.dotted = 1;

      } else {

        nsAutoString hiddenValue;
        child->GetAttr(kNameSpaceID_None, nsWidgetAtoms::hidden, hiddenValue);
        if ( hiddenValue.EqualsLiteral("true") ) continue;

        nsAutoString itemName;
        child->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, itemName);
        if ( itemName.EqualsLiteral("") ) continue;

        lastItem = Menu_FirstItem(mROMenu) + ROPosition;

        const char *text = NS_LossyConvertUCS2toASCII(itemName).get();

        char *hs = (char *)alloca(strlen(text) + 1);
        int pos = 0;

        do {
          if (text[pos] != ' ')
            hs[pos] = text[pos];
          else
            hs[pos] = 160;
        } while (text[pos++] != '\0');

        Menu_SetText(mROMenu, ROPosition, hs);

        if ( tag == nsWidgetAtoms::menu ) {
          mMenuPosition = ROPosition;
          LoadSubMenu(child);
        } else {
          LoadMenuItem(child, ROPosition);
        }

        if (tag == nsWidgetAtoms::menuseparator)
          lastItem->menuflags.data.dotted = lastItem->menuflags.data.shaded = 1;

//        printf("text: %s %p\n", Menu_GetText(mROMenu, ROPosition), Menu_GetText(mROMenu, ROPosition));

        ROPosition++;
      }
      puts("done item");
    }
  } // for each menu item

  if (lastItem) lastItem->menuflags.data.dotted = 0;
  // Get rid of empty entries at the end
printf("%d %d\n", item, ROPosition);
  while (item > ROPosition)
    Menu_RemoveItem(mROMenu, --item);

//  puts("done create");

  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::GetParent(nsISupports*& aParent)
{
  puts("menu::getparent");
  aParent = mParent;
  NS_IF_ADDREF(aParent);
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::GetLabel(nsString &aText)
{
  puts("menu::getlabel");
  aText = mLabel;
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsMenu::SetLabel(const nsAString &aText)
{
  puts("menu::setlabel");
  mLabel = aText;

  // first time? create the menu handle, attach event handler to it.
//  if (mMacMenuHandle == nsnull) {
//    mMacMenuID = gMacMenuIDCountX++;
//    mMacMenuHandle = NSStringNewMenu(mMacMenuID, mLabel);
//  }
  
  return NS_OK;
}


//-------------------------------------------------------------------------
void nsMenu::SetROPosition(PRUint32 position)
{
  puts("menu::setposition");
  mMenuPosition = position;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::GetAccessKey(nsString &aText)
{
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::SetAccessKey(const nsAString &aText)
{
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::AddItem(nsISupports* aItem)
{
//  puts("menu::additem");
    nsresult rv = NS_ERROR_FAILURE;
    if (aItem) {
        // Figure out what we're adding
        nsCOMPtr<nsIMenuItem> menuItem(do_QueryInterface(aItem));
        if (menuItem) {
            rv = AddMenuItem(menuItem);
        } else {
            nsCOMPtr<nsIMenu> menu(do_QueryInterface(aItem));
            if (menu)
                rv = AddMenu(menu);
        }
    }
    return rv;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::AddMenuItem(nsIMenuItem * aMenuItem)
{
//  puts("menu::addmenuitem");
  if(!aMenuItem) return NS_ERROR_NULL_POINTER;

  mMenuItemsArray.AppendElement(aMenuItem);    // owning ref
  PRUint32 currItemIndex;
  mMenuItemsArray.Count(&currItemIndex);

  mNumMenuItems++;

  nsAutoString label;
  aMenuItem->GetLabel(label);
//  InsertMenuItemWithTruncation ( label, currItemIndex );
  
  // I want to be internationalized too!
  nsAutoString keyEquivalent(NS_LITERAL_STRING(" "));
  aMenuItem->GetShortcutChar(keyEquivalent);
  if (!keyEquivalent.EqualsLiteral(" ")) {
//    ToUpperCase(keyEquivalent);
    char keyStr[2];
    keyEquivalent.ToCString(keyStr, sizeof(keyStr));
//    short inKey = keyStr[0];
//    ::SetItemCmd(mMacMenuHandle, currItemIndex, inKey);
    //::SetMenuItemKeyGlyph(mMacMenuHandle, mNumMenuItems, 0x61);
  }

  PRUint8 modifiers;
  aMenuItem->GetModifiers(&modifiers);
//  PRUint8 macModifiers = kMenuNoModifiers;
//  if (knsMenuItemShiftModifier & modifiers)
//    macModifiers |= kMenuShiftModifier;
//
//  if (knsMenuItemAltModifier & modifiers)
//    macModifiers |= kMenuOptionModifier;
//
//  if (knsMenuItemControlModifier & modifiers)
//    macModifiers |= kMenuControlModifier;
//
//  if (!(knsMenuItemCommandModifier & modifiers))
//    macModifiers |= kMenuNoCommandModifier;
//
//  ::SetMenuItemModifiers(mMacMenuHandle, currItemIndex, macModifiers);

  // set its command. we get the unique command id from the menubar
//  nsCOMPtr<nsIMenuCommandDispatcher> dispatcher ( do_QueryInterface(mManager) );
//  if ( dispatcher ) {
//    PRUint32 commandID = 0L;
//    dispatcher->Register(aMenuItem, &commandID);
//    if ( commandID )
//      ::SetMenuItemCommandID(mMacMenuHandle, currItemIndex, commandID);
//  }
  
  PRBool isEnabled;
  aMenuItem->GetEnabled(&isEnabled);
//  if(isEnabled)
//    ::EnableMenuItem(mMacMenuHandle, currItemIndex);
//  else
//    ::DisableMenuItem(mMacMenuHandle, currItemIndex);

  PRBool isChecked;
  aMenuItem->GetChecked(&isChecked);
//  if(isChecked)
//    ::CheckMenuItem(mMacMenuHandle, currItemIndex, true);
//  else
//    ::CheckMenuItem(mMacMenuHandle, currItemIndex, false);

  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::AddMenu(nsIMenu * aMenu)
{
  puts("menu::addmenu");
  // Add a submenu
  if (!aMenu) return NS_ERROR_NULL_POINTER;

  nsCOMPtr<nsISupports>  supports = do_QueryInterface(aMenu);
  if (!supports) return NS_ERROR_NO_INTERFACE;

  mMenuItemsArray.AppendElement(supports);   // owning ref
  PRUint32 currItemIndex;
  mMenuItemsArray.Count(&currItemIndex);

  mNumMenuItems++;

  // We have to add it as a menu item and then associate it with the item
  nsAutoString label;
  aMenu->GetLabel(label);
//  InsertMenuItemWithTruncation ( label, currItemIndex );

  PRBool isEnabled;
  aMenu->GetEnabled(&isEnabled);
//  if (isEnabled)
//    ::EnableMenuItem(mMacMenuHandle, currItemIndex);
//  else
//    ::DisableMenuItem(mMacMenuHandle, currItemIndex);	    

 // MenuHandle childMenu;
 // if (aMenu->GetNativeData((void**)&childMenu) == NS_OK)
 //   ::SetMenuItemHierarchicalMenu((MenuHandle) mMacMenuHandle, currItemIndex, childMenu);
  
  return NS_OK;
}


//
// InsertMenuItemWithTruncation
//
// Insert a new item in this menu with index |inItemIndex| with the text |inItemLabel|,
// middle-truncated to a certain pixel width with an elipsis.
//
//void
//nsMenu :: InsertMenuItemWithTruncation ( nsAutoString & inItemLabel, PRUint32 inItemIndex )
//{
//  // ::TruncateThemeText() doesn't take the number of characters to truncate to, it takes a pixel with
//  // to fit the string in. Ugh. I talked it over with sfraser and we couldn't come up with an 
//  // easy way to compute what this should be given the system font, etc, so we're just going
//  // to hard code it to something reasonable and bigger fonts will just have to deal.
//  const short kMaxItemPixelWidth = 300;
//
////  CFMutableStringRef labelRef = ::CFStringCreateMutable ( kCFAllocatorDefault, inItemLabel.Length() );
////  ::CFStringAppendCharacters ( labelRef, (UniChar*)inItemLabel.get(), inItemLabel.Length() );
////  ::TruncateThemeText(labelRef, kThemeMenuItemFont, kThemeStateActive, kMaxItemPixelWidth, truncMiddle, NULL);
////  ::InsertMenuItemTextWithCFString(mMacMenuHandle, labelRef, inItemIndex, 0, 0);
////  ::CFRelease(labelRef);
//
//} // InsertMenuItemWithTruncation


//-------------------------------------------------------------------------
NS_METHOD nsMenu::AddSeparator()
{
  NS_ASSERTION(0, "Not implemented");
#if 0
  // HACK - We're not really appending an nsMenuItem but it
  // needs to be here to make sure that event dispatching isn't off by one.
  mMenuItemsArray.AppendElement(&gDummyMenuItem);   // owning ref
  PRUint32  numItems;
  mMenuItemsArray.Count(&numItems);
//  ::InsertMenuItem(mMacMenuHandle, "\p(-", numItems);
  mNumMenuItems++;
#endif
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::GetItemCount(PRUint32 &aCount)
{
  return mMenuItemsArray.Count(&aCount);
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::GetItemAt(const PRUint32 aPos, nsISupports *& aMenuItem)
{
  mMenuItemsArray.GetElementAt(aPos, &aMenuItem);
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::InsertItemAt(const PRUint32 aPos, nsISupports * aMenuItem)
{
  NS_ASSERTION(0, "Not implemented");
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::RemoveItem(const PRUint32 aPos)
{
  NS_ASSERTION(0, "Not implemented");
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::RemoveAll()
{
//  if (mMacMenuHandle != NULL) {    
//    // clear command id's
//    nsCOMPtr<nsIMenuCommandDispatcher> dispatcher ( do_QueryInterface(mManager) );
//    if ( dispatcher ) {
//      for ( unsigned int i = 1; i <= mNumMenuItems; ++i ) {
//        PRUint32 commandID = 0L;
////        OSErr err = ::GetMenuItemCommandID(mMacMenuHandle, i, (unsigned long*)&commandID);
////        if ( !err )
////          dispatcher->Unregister(commandID);
//      }
//    }                                                             3
////    ::DeleteMenuItems(mMacMenuHandle, 1, ::CountMenuItems(mMacMenuHandle));
//  }
//  
  mMenuItemsArray.Clear();    // remove all items
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::GetNativeData(void ** aData)
{
  *aData = mROMenu;
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::SetNativeData(void * aData)
{
  mROMenu = (menu_ptr) aData;
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::AddMenuListener(nsIMenuListener * aMenuListener)
{
  mListener = aMenuListener;    // strong ref
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenu::RemoveMenuListener(nsIMenuListener * aMenuListener)
{
  if (aMenuListener == mListener)
    mListener = nsnull;
  return NS_OK;                                                    
}


//-------------------------------------------------------------------------
//
// nsIMenuListener interface
//
//-------------------------------------------------------------------------
nsEventStatus nsMenu::MenuItemSelected(const nsMenuEvent & aMenuEvent)
{
  puts("menu::menuitemselected");
  // all this is now handled by Carbon Events.
  return nsEventStatus_eConsumeNoDefault;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenu::MenuSelected(const nsMenuEvent & aMenuEvent)
{
  printf("MenuSelected called for %s \n", NS_LossyConvertUCS2toASCII(mLabel).get());
  nsEventStatus eventStatus = nsEventStatus_eIgnore;

printf("needs rebuild: %d\n", mNeedsRebuild);

  // Determine if this is the correct menu to handle the event

  int *selectionData = (int *)aMenuEvent.mCommand;

  if (selectionData[0] == -1) {
//    puts("menu selected");
    eventStatus = nsEventStatus_eConsumeNoDefault;

  } else {
    extern menu_ptr gCurrentROMenu;

    // Make sure none of our submenus are the ones that should be handling this
//    puts("check submenus");

    // Determine actual position, taking into account RISC OS
    //   menu separators
    menu_item *items = Menu_FirstItem(gCurrentROMenu);
    PRUint32 count = 0; //, item = 0;

#if 0
    while (!items[item].menuflags.data.last && item < selectionData[0]) {
      printf("%d %d %s\n", count, item, Menu_GetText(gCurrentROMenu, item));
      if (/*items[item].menuflags.data.dotted &&*/
          count && strcmp(Menu_GetText(gCurrentROMenu, item), "") != 0)
        count++;
      count++;
      item++;
    }
#endif

//    printf("RO item: %d  Real item: %d\n", item, count);
   count = selectionData[0];

    nsCOMPtr<nsISupports>     menuSupports = getter_AddRefs(mMenuItemsArray.ElementAt(count));
    nsCOMPtr<nsIMenu>         subMenu = do_QueryInterface(menuSupports);

    GetItemCount(count);
    printf("length: %d %d\n", count, selectionData[0]);

    if (subMenu) {

     puts("sub menu selected");

      nsMenuEvent *menuEvent = (nsMenuEvent *)&aMenuEvent;
      menuEvent->mCommand = (PRUint32)(selectionData + 1);
      gCurrentROMenu = items[selectionData[0]].submenu.menu;

      nsCOMPtr<nsIMenuListener> menuListener = do_QueryInterface(subMenu);
      return menuListener->MenuSelected(aMenuEvent);

    } else {

      printf("item selected: %s\n", "");

      nsCOMPtr<nsIMenuItem> menuItem(do_QueryInterface(menuSupports));
      if (menuItem) {
        puts("do command");
        menuItem->DoCommand();
        return nsEventStatus_eConsumeNoDefault;
      }
    }
  }

  return eventStatus;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenu::MenuDeselected(const nsMenuEvent & aMenuEvent)
{
  // Destroy the menu
//  if (mConstructed) {
//    MenuDestruct(aMenuEvent);
//    mConstructed = false;
//  }
  return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenu::MenuConstruct(
    const nsMenuEvent & aMenuEvent,
    nsIWidget         * aParentWindow, 
    void              * /* menuNode */,
	  void              * aDocShell)
{
//  mConstructed = false;
  gConstructingMenu = PR_TRUE;
  
  // reset destroy handler flag so that we'll know to fire it next time this menu goes away.
//  mDestroyHandlerCalled = PR_FALSE;
  
  printf("nsMenu::MenuConstruct called for %s\n", NS_LossyConvertUCS2toASCII(mLabel).get());
  // Begin menuitem inner loop
  
  // Retrieve our menupopup.
  nsCOMPtr<nsIContent> menuPopup;
//  GetMenuPopupContent(getter_AddRefs(menuPopup));
  if (!menuPopup)
    return nsEventStatus_eIgnore;
      
  // Iterate over the kids
  PRUint32 count = menuPopup->GetChildCount();
  for ( PRUint32 i = 0; i < count; ++i ) {
    nsIContent *child = menuPopup->GetChildAt(i);
    if ( child ) {
      // depending on the type, create a menu item, separator, or submenu
      //nsIAtom *tag = child->Tag();
 //     if ( tag == nsWidgetAtoms::menuitem )
 //       LoadMenuItem(this, child);
 //     else if ( tag == nsWidgetAtoms::menuseparator )
 //       LoadSeparator(child);
 //     else if ( tag == nsWidgetAtoms::menu )
 //       LoadSubMenu(this, child);
    }
  } // for each menu item
  
  gConstructingMenu = PR_FALSE;
puts("menu::menuconstruct: no longer needs rebuild");
  mNeedsRebuild = PR_FALSE;
  //printf("  Done building, mMenuItemVoidArray.Count() = %d \n", mMenuItemVoidArray.Count());
  
  return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
//nsEventStatus nsMenu::HelpMenuConstruct(
//    const nsMenuEvent & aMenuEvent,
//    nsIWidget         * aParentWindow, 
//    void              * /* menuNode */,
//    void              * aDocShell)
//{
//  printf("nsMenu::MenuConstruct called for %s = %d \n", NS_LossyConvertUCS2toASCII(mLabel).get(), 0);
// 
////  int numHelpItems = ::CountMenuItems(mMacMenuHandle);
////  for (int i=0; i < numHelpItems; ++i) {
////    mMenuItemsArray.AppendElement(&gDummyMenuItem);
////  }
////     
////  // Retrieve our menupopup.
////  nsCOMPtr<nsIContent> menuPopup;
////  GetMenuPopupContent(getter_AddRefs(menuPopup));
////  if (!menuPopup)
////    return nsEventStatus_eIgnore;
////      
////  // Iterate over the kids
////  PRUint32 count = menuPopup->GetChildCount();
////  for ( PRUint32 i = 0; i < count; ++i ) {
////    nsIContent *child = menuPopup->GetChildAt(i);
////    if ( child ) {      
////      // depending on the type, create a menu item, separator, or submenu
////      nsIAtom *tag = child->Tag();
////      if ( tag == nsWidgetAtoms::menuitem )
////        LoadMenuItem(this, child);
////      else if ( tag == nsWidgetAtoms::menuseparator )
////        LoadSeparator(child);
////      else if ( tag == nsWidgetAtoms::menu )
////        LoadSubMenu(this, child);
////    }   
////  } // for each menu item
////  
////  //printf("  Done building, mMenuItemVoidArray.Count() = %d \n", mMenuItemVoidArray.Count());
//             
//  return nsEventStatus_eIgnore;
//}

//-------------------------------------------------------------------------
nsEventStatus nsMenu::MenuDestruct(const nsMenuEvent & aMenuEvent)
{
  //printf("nsMenu::MenuDestruct() called for %s \n", NS_LossyConvertUCS2toASCII(mLabel).get());
  
  // Fire our ondestroy handler. If we're told to stop, don't destroy the menu
  PRBool keepProcessing = nsnull; //OnDestroy();
  if ( keepProcessing ) {
//    if(mNeedsRebuild) {
//        mConstructed = false;
        //printf("  mMenuItemVoidArray.Count() = %d \n", mMenuItemVoidArray.Count());
//    }
    // Close the node.
    mMenuContent->UnsetAttr(kNameSpaceID_None, nsWidgetAtoms::open, PR_TRUE);

//    OnDestroyed();
  }
  
  return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenu::CheckRebuild(PRBool & aNeedsRebuild)
{
  aNeedsRebuild = mNeedsRebuild;
  return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenu::SetRebuild(PRBool aNeedsRebuild)
{
puts("set rebuild 1");
  if(!gConstructingMenu) {
    mNeedsRebuild = aNeedsRebuild;
 //   if(mNeedsRebuild)
 //     RemoveAll();
 //
//puts("set rebuild 2");
 //   // our parent could be either a menu bar (if we're toplevel) or a menu (if we're a submenu)
 //   nsCOMPtr<nsIMenuBar> menubar = do_QueryInterface(mParent);
 //   nsCOMPtr<nsIMenu> menu = do_QueryInterface(mParent);
 //   if (menubar) {
 //     printf("set rebuild 4: %p\n", mParent);
 //     menubar.SetRebuild(PR_TRUE);
////      ((nsMenuBar *)mParent)->mNeedsRebuild = PR_TRUE;
 //   } else {
 //     menu.SetRebuild(PR_TRUE);
 //     puts("set rebuild 3");
 ////     ((nsMenu *)mParent)->mNeedsRebuild = PR_TRUE;
//    }
  }
  return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
/**
* Set enabled state
*
*/
NS_METHOD nsMenu::SetEnabled(PRBool aIsEnabled)
{
//  mIsEnabled = aIsEnabled;

//  if ( aIsEnabled )
//    ::EnableMenuItem(mMacMenuHandle, 0);
//  else
//    ::DisableMenuItem(mMacMenuHandle, 0);

  return NS_OK;
}

//-------------------------------------------------------------------------
/**
* Get enabled state
*
*/
NS_METHOD nsMenu::GetEnabled(PRBool* aIsEnabled)
{
  NS_ENSURE_ARG_POINTER(aIsEnabled);
  *aIsEnabled = PR_TRUE; //mIsEnabled;
  return NS_OK;
}

//-------------------------------------------------------------------------
/**
* Query if this is the help menu
*
*/
NS_METHOD nsMenu::IsHelpMenu(PRBool* aIsHelpMenu)
{
  NS_ENSURE_ARG_POINTER(aIsHelpMenu);
  *aIsHelpMenu = PR_FALSE; //mIsHelpMenu;
  return NS_OK;
}


//-------------------------------------------------------------------------
/**
* Get GetMenuContent
*
*/
NS_METHOD nsMenu::GetMenuContent(nsIContent ** aMenuContent)
{
  NS_ENSURE_ARG_POINTER(aMenuContent);
  NS_IF_ADDREF(*aMenuContent = mMenuContent);
	return NS_OK;
}


//----------------------------------------
void nsMenu::LoadMenuItem(nsIContent* inMenuItemContent, PRUint32 ROPosition )
{
  if ( !inMenuItemContent )
    return;

  // if menu should be hidden, bail
  nsAutoString hidden;
  nsAutoString menuitemName;
  inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::hidden, hidden);

  if ( hidden.EqualsLiteral("true") )
    return;

  inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, menuitemName);

  if (!menuitemName.EqualsLiteral(""))
    printf("menuitem \"%s\"\n", NS_LossyConvertUCS2toASCII(menuitemName).get());
  else {
    puts("no name");
    return;
  }

  // Create nsMenuItem
  nsCOMPtr<nsIMenuItem> pnsMenuItem = do_CreateInstance ( kMenuItemCID ) ;
  if ( pnsMenuItem ) {
    nsAutoString disabled;
    nsAutoString checked;
    nsAutoString type;
    nsAutoString menuitemCmd;
    
    inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::disabled, disabled);
    inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::checked, checked);
    inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::type, type);
    inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::command, menuitemCmd);

    nsIMenuItem::EMenuItemType itemType = nsIMenuItem::eRegular;
    if ( type.EqualsLiteral("checkbox") )
      itemType = nsIMenuItem::eCheckbox;
    else if ( type.EqualsLiteral("radio") )
      itemType = nsIMenuItem::eRadio;

    nsCOMPtr<nsIDocShell>  docShell = do_QueryReferent(mDocShellWeakRef);
    if (!docShell)
      return;

    // Create the item.
    pnsMenuItem->Create(this, menuitemName, PR_FALSE, itemType, 
                          PR_TRUE, mManager, docShell, inMenuItemContent);

    Menu_SetFlags(mROMenu, ROPosition, checked.EqualsLiteral("true"),
                           disabled.EqualsLiteral("true"));

//printf("check: %d disabled: %d\n", checked.EqualsLiteral("true"), disabled.EqualsLiteral("true"));

    //
    // Set key shortcut and modifiers
    //
    
    nsAutoString keyValue;
    inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::key, keyValue);

    // Try to find the key node. Get the document so we can do |GetElementByID|
    nsCOMPtr<nsIDOMDocument> domDocument =
      do_QueryInterface(inMenuItemContent->GetDocument());
    if ( !domDocument )
      return;

    nsCOMPtr<nsIDOMElement> keyElement;
    if (!keyValue.IsEmpty())
      domDocument->GetElementById(keyValue, getter_AddRefs(keyElement));
    if ( keyElement ) {
      nsCOMPtr<nsIContent> keyContent ( do_QueryInterface(keyElement) );
      nsAutoString keyChar(NS_LITERAL_STRING(" "));
      keyContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::key, keyChar);
	    if(!keyChar.EqualsLiteral(" ")) 
        pnsMenuItem->SetShortcutChar(keyChar);
        
//      PRUint8 modifiers = knsMenuItemNoModifier;
//	    nsAutoString modifiersStr;
//      keyContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::modifiers, modifiersStr);
//		  char* str = ToNewCString(modifiersStr);
//		  char* newStr;
//		  char* token = nsCRT::strtok( str, ", ", &newStr );
//		  while( token != NULL ) {
//		    if (PL_strcmp(token, "shift") == 0)
//		      modifiers |= knsMenuItemShiftModifier;
//		    else if (PL_strcmp(token, "alt") == 0) 
//		      modifiers |= knsMenuItemAltModifier;
//		    else if (PL_strcmp(token, "control") == 0) 
//		      modifiers |= knsMenuItemControlModifier;
//		    else if ((PL_strcmp(token, "accel") == 0) ||
//		             (PL_strcmp(token, "meta") == 0)) {
//          modifiers |= knsMenuItemCommandModifier;
//		    }
//		    
//		    token = nsCRT::strtok( newStr, ", ", &newStr );
//		  }
//		  nsMemory::Free(str);
//
//	    pnsMenuItem->SetModifiers ( modifiers );
    }

    pnsMenuItem->SetChecked(checked.EqualsLiteral("true"));
      
    nsCOMPtr<nsISupports> supports ( do_QueryInterface(pnsMenuItem) );
    AddItem(supports);         // Parent now owns menu item
  }
}


void 
nsMenu::LoadSubMenu(/* nsIMenu * pParentMenu,*/ nsIContent* inMenuItemContent)
{
  // if menu should be hidden, bail
  nsAutoString hidden; 
  inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::hidden, hidden);
  if ( hidden.EqualsLiteral("true") )
    return;
  
  nsAutoString menuName; 
  inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, menuName);
//  printf("Creating Menu [%s] \n", NS_LossyConvertUCS2toASCII(menuName).get());

  // Create nsMenu
  nsCOMPtr<nsIMenu> pnsMenu ( do_CreateInstance(kMenuCID) );
  if (pnsMenu) {
    // Call Create
    nsCOMPtr<nsIDocShell> docShell = do_QueryReferent(mDocShellWeakRef);
    if (!docShell)
        return;
    nsCOMPtr<nsISupports> supports(do_QueryInterface((nsIMenu *)this));
    pnsMenu->Create(supports, menuName, EmptyString(), mManager, docShell, inMenuItemContent);

    // set if it's enabled or disabled
    nsAutoString disabled;
    inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::disabled, disabled);
    if ( disabled.EqualsLiteral("true") )
      pnsMenu->SetEnabled ( PR_FALSE );
    else
      pnsMenu->SetEnabled ( PR_TRUE );

    menu_ptr menu;
    pnsMenu->GetNativeData((void**)&menu);
    Menu_AddSubMenu(mROMenu, mMenuPosition, menu);

    // Make pnsMenu a child of parent nsMenu. The parent takes ownership
    nsCOMPtr<nsISupports> supports2 ( do_QueryInterface(pnsMenu) );
    AddItem(supports2);
  }
}
//
//
//void
//nsMenu::LoadSeparator ( nsIContent* inMenuItemContent ) 
//{
//  // if item should be hidden, bail
//  nsAutoString hidden;
//  inMenuItemContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::hidden, hidden);
//  if ( hidden.EqualsLiteral("true") )
//    return;
//
//  AddSeparator();
//}



//
// OnCreate
//
// Fire our oncreate handler. Returns TRUE if we should keep processing the event,
// FALSE if the handler wants to stop the creation of the menu
//
PRBool
nsMenu::OnCreate()
{
   puts("nsMenu::OnCreate");
   return PR_TRUE;
}

//  nsEventStatus status = nsEventStatus_eIgnore;
//  nsMouseEvent event(PR_TRUE, NS_XUL_POPUP_SHOWING, nsnull,
//                     nsMouseEvent::eReal);
//  
//  nsCOMPtr<nsIContent> popupContent;
////  GetMenuPopupContent(getter_AddRefs(popupContent));
//
//  nsCOMPtr<nsIDocShell> docShell = do_QueryReferent(mDocShellWeakRef);
//  if (!docShell) {
//    NS_ERROR("No doc shell");
//    return PR_FALSE;
//  }
//  nsCOMPtr<nsPresContext> presContext;
////  MenuHelpersX::DocShellToPresContext(docShell, getter_AddRefs(presContext) );
//  if ( presContext ) {
//    nsresult rv = NS_OK;
//    nsIContent* dispatchTo = popupContent ? popupContent : mMenuContent;
//    rv = dispatchTo->HandleDOMEvent(presContext, &event, nsnull, NS_EVENT_FLAG_INIT, &status);
//    if ( NS_FAILED(rv) || status == nsEventStatus_eConsumeNoDefault )
//      return PR_FALSE;
// }
//
//  // the menu is going to show and the oncreate handler has executed. We
//  // now need to walk our menu items, checking to see if any of them have
//  // a command attribute. If so, several apptributes must potentially
//  // be updated.
//  if (popupContent) {
//    nsCOMPtr<nsIDOMDocument> domDoc(do_QueryInterface(popupContent->GetDocument()));
//
//    PRUint32 count = popupContent->GetChildCount();
//    for (PRUint32 i = 0; i < count; i++) {
//      nsIContent *grandChild = popupContent->GetChildAt(i);
//      if (grandChild->Tag() == nsWidgetAtoms::menuitem) {
//        // See if we have a command attribute.
//        nsAutoString command;
//        grandChild->GetAttr(kNameSpaceID_None, nsWidgetAtoms::command, command);
//        if (!command.IsEmpty()) {
//          // We do! Look it up in our document
//          nsCOMPtr<nsIDOMElement> commandElt;
//          domDoc->GetElementById(command, getter_AddRefs(commandElt));
//          nsCOMPtr<nsIContent> commandContent(do_QueryInterface(commandElt));
//
//          if ( commandContent ) {
//            nsAutoString commandDisabled, menuDisabled;
//            commandContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::disabled, commandDisabled);
//            grandChild->GetAttr(kNameSpaceID_None, nsWidgetAtoms::disabled, menuDisabled);
//            if (!commandDisabled.Equals(menuDisabled)) {
//              // The menu's disabled state needs to be updated to match the command.
//              if (commandDisabled.IsEmpty()) 
//                grandChild->UnsetAttr(kNameSpaceID_None, nsWidgetAtoms::disabled, PR_TRUE);
//              else grandChild->SetAttr(kNameSpaceID_None, nsWidgetAtoms::disabled, commandDisabled, PR_TRUE);
//            }
//
//            // The menu's value and checked states need to be updated to match the command.
//            // Note that (unlike the disabled state) if the command has *no* value for either, we
//            // assume the menu is supplying its own.
//            nsAutoString commandChecked, menuChecked;
//            commandContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::checked, commandChecked);
//            grandChild->GetAttr(kNameSpaceID_None, nsWidgetAtoms::checked, menuChecked);
//            if (!commandChecked.Equals(menuChecked)) {
//              if (!commandChecked.IsEmpty()) 
//                grandChild->SetAttr(kNameSpaceID_None, nsWidgetAtoms::checked, commandChecked, PR_TRUE);
//            }
//
//            nsAutoString commandValue, menuValue;
//            commandContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, commandValue);
//            grandChild->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, menuValue);
//            if (!commandValue.Equals(menuValue)) {
//              if (!commandValue.IsEmpty()) 
//                grandChild->SetAttr(kNameSpaceID_None, nsWidgetAtoms::label, commandValue, PR_TRUE);
//            }
//          }
//        }
//      }
//    }
//  }
//  
//  return PR_TRUE;
//}

//PRBool
//nsMenu::OnCreated()
//{
//  nsEventStatus status = nsEventStatus_eIgnore;
//  nsMouseEvent event(PR_TRUE, NS_XUL_POPUP_SHOWN, nsnull, nsMouseEvent::eReal);
//  
//  nsCOMPtr<nsIContent> popupContent;
//  GetMenuPopupContent(getter_AddRefs(popupContent));
//
//  nsCOMPtr<nsIDocShell> docShell = do_QueryReferent(mDocShellWeakRef);
//  if (!docShell) {
//    NS_ERROR("No doc shell");
//    return PR_FALSE;
//  }
//  nsCOMPtr<nsPresContext> presContext;
//  MenuHelpersX::DocShellToPresContext(docShell, getter_AddRefs(presContext) );
//  if ( presContext ) {
//    nsresult rv = NS_OK;
//    nsIContent* dispatchTo = popupContent ? popupContent : mMenuContent;
//    rv = dispatchTo->HandleDOMEvent(presContext, &event, nsnull, NS_EVENT_FLAG_INIT, &status);
//    if ( NS_FAILED(rv) || status == nsEventStatus_eConsumeNoDefault )
//      return PR_FALSE;
// }
//  
//  return PR_TRUE;
//}

//
// OnDestroy
//
// Fire our ondestroy handler. Returns TRUE if we should keep processing the event,
// FALSE if the handler wants to stop the destruction of the menu
//
/*
PRBool
nsMenu::OnDestroy()
{
  if ( mDestroyHandlerCalled )
    return PR_TRUE;

  nsEventStatus status = nsEventStatus_eIgnore;
  nsMouseEvent event(PR_TRUE, NS_XUL_POPUP_HIDING, nsnull,
                     nsMouseEvent::eReal);
  
  nsCOMPtr<nsIDocShell>  docShell = do_QueryReferent(mDocShellWeakRef);
  if (!docShell) {
    NS_WARNING("No doc shell so can't run the OnDestroy");
    return PR_FALSE;
  }

  nsCOMPtr<nsIContent> popupContent;
  GetMenuPopupContent(getter_AddRefs(popupContent));

  nsCOMPtr<nsPresContext> presContext;
  MenuHelpersX::DocShellToPresContext (docShell, getter_AddRefs(presContext) );
  if (presContext )  {
    nsresult rv = NS_OK;
    nsIContent* dispatchTo = popupContent ? popupContent : mMenuContent;
    rv = dispatchTo->HandleDOMEvent(presContext, &event, nsnull, NS_EVENT_FLAG_INIT, &status);

    mDestroyHandlerCalled = PR_TRUE;
    
    if ( NS_FAILED(rv) || status == nsEventStatus_eConsumeNoDefault )
      return PR_FALSE;
  }
  return PR_TRUE;
}

PRBool
nsMenu::OnDestroyed()
{
  nsEventStatus status = nsEventStatus_eIgnore;
  nsMouseEvent event(PR_TRUE, NS_XUL_POPUP_HIDDEN, nsnull,
                     nsMouseEvent::eReal);
  
  nsCOMPtr<nsIDocShell>  docShell = do_QueryReferent(mDocShellWeakRef);
  if (!docShell) {
    NS_WARNING("No doc shell so can't run the OnDestroy");
    return PR_FALSE;
  }

  nsCOMPtr<nsIContent> popupContent;
  GetMenuPopupContent(getter_AddRefs(popupContent));

  nsCOMPtr<nsPresContext> presContext;
  MenuHelpersX::DocShellToPresContext (docShell, getter_AddRefs(presContext) );
  if (presContext )  {
    nsresult rv = NS_OK;
    nsIContent* dispatchTo = popupContent ? popupContent : mMenuContent;
    rv = dispatchTo->HandleDOMEvent(presContext, &event, nsnull, NS_EVENT_FLAG_INIT, &status);

    mDestroyHandlerCalled = PR_TRUE;
    
    if ( NS_FAILED(rv) || status == nsEventStatus_eConsumeNoDefault )
      return PR_FALSE;
  }
  return PR_TRUE;
} */
//
// GetMenuPopupContent
//
// Find the |menupopup| child in the |popup| representing this menu. It should be one
// of a very few children so we won't be iterating over a bazillion menu items to find
// it (so the strcmp won't kill us).
//
/*void
nsMenu::GetMenuPopupContent(nsIContent** aResult)
{
  if (!aResult )
    return;
  *aResult = nsnull;
  
  nsresult rv;
  nsCOMPtr<nsIXBLService> xblService = 
           do_GetService("@mozilla.org/xbl;1", &rv);
  if ( !xblService )
    return;
  
  PRUint32 count = mMenuContent->GetChildCount();

  for (PRUint32 i = 0; i < count; i++) {
    PRInt32 dummy;
    nsIContent *child = mMenuContent->GetChildAt(i);
    nsCOMPtr<nsIAtom> tag;
    xblService->ResolveTag(child, &dummy, getter_AddRefs(tag));
    if (tag == nsWidgetAtoms::menupopup) {
      *aResult = child;
      NS_ADDREF(*aResult);
      return;
    }
  }

} // GetMenuPopupContent
*/

//
// CountVisibleBefore
//
// Determines how many menus are visible among the siblings that are before me.
// It doesn't matter if I am visible. Note that this will always count the Apple
// menu, since we always put it in there.
//
/*
nsresult
nsMenu :: CountVisibleBefore ( PRUint32* outVisibleBefore )
{
  NS_ASSERTION ( outVisibleBefore, "bad index param" );
  
  nsCOMPtr<nsIMenuBar> menubarParent = do_QueryInterface(mParent);
  if (!menubarParent) return NS_ERROR_FAILURE;

  PRUint32 numMenus = 0;
  menubarParent->GetMenuCount(numMenus);
  
  // Find this menu among the children of my parent menubar
  PRBool gotThisMenu = PR_FALSE;
  *outVisibleBefore = 1;                            // start at 1, the apple menu will always be there
  for ( PRUint32 i = 0; i < numMenus; ++i ) {
    nsCOMPtr<nsIMenu> currMenu;
    menubarParent->GetMenuAt(i, *getter_AddRefs(currMenu));
    
    // we found ourselves, break out
    if ( currMenu == NS_STATIC_CAST(nsIMenu*, this) ) {
      gotThisMenu = PR_TRUE;
      break;
    }
      
    // check the current menu to see if it is visible (not hidden, not collapsed). If
    // it is, count it.
    if (currMenu) {
      nsCOMPtr<nsIContent> menuContent;
      currMenu->GetMenuContent(getter_AddRefs(menuContent));
      if ( menuContent ) {
        nsAutoString hiddenValue, collapsedValue;
        menuContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::hidden, hiddenValue);
        menuContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::collapsed, collapsedValue);
        if ( menuContent->GetChildCount() > 0 ||
             !hiddenValue.EqualsLiteral("true") &&
             !collapsedValue.EqualsLiteral("true"))
          ++(*outVisibleBefore);
      }
    }
    
  } // for each menu

  return gotThisMenu ? NS_OK : NS_ERROR_FAILURE;

} // CountVisibleBefore


#pragma mark -
*/
//
// nsIChangeObserver
//


NS_IMETHODIMP
nsMenu::AttributeChanged(nsIDocument *aDocument, PRInt32 aNameSpaceID, nsIContent *aContent, nsIAtom *aAttribute)
{
  if (gConstructingMenu)
    return NS_OK;

  // ignore the |open| attribute, which is by far the most common
  if ( aAttribute == nsWidgetAtoms::open )
    return NS_OK;

puts("nsMenu:AttributeChanged");

  nsCOMPtr<nsIMenuBar> menubarParent = do_QueryInterface(mParent);

  if ( !menubarParent ) {
    puts("parent not menubar");
    nsCOMPtr<nsIMenuListener> parentListener ( do_QueryInterface(mParent) );
    parentListener->SetRebuild(PR_TRUE);
  } else {
    puts("parent menubar");
    nsCOMPtr<nsIMenuListener> parentListener ( do_QueryInterface(mParent) );
    parentListener->SetRebuild(PR_TRUE);
 }

#if 0

  if(aAttribute == nsWidgetAtoms::disabled) {
puts("disabled");
    SetRebuild(PR_TRUE);
   
    nsAutoString valueString;
    mMenuContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::disabled, valueString);
    if(valueString.EqualsLiteral("true"))
      SetEnabled(PR_FALSE);
    else
      SetEnabled(PR_TRUE);
      
//    ::DrawMenuBar();
  } 
  else if(aAttribute == nsWidgetAtoms::label) {
puts("label");
    SetRebuild(PR_TRUE);
    
    mMenuContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, mLabel);

    // invalidate my parent. If we're a submenu parent, we have to rebuild
    // the parent menu in order for the changes to be picked up. If we're
    // a regular menu, just change the title and redraw the menubar.
    if ( !menubarParent ) {
      nsCOMPtr<nsIMenuListener> parentListener ( do_QueryInterface(mParent) );
      parentListener->SetRebuild(PR_TRUE);
    }
    else {
      // reuse the existing menu, to avoid rebuilding the root menu bar.
//      NS_ASSERTION(mMacMenuHandle != NULL, "nsMenu::AttributeChanged: invalid menu handle.");
      RemoveAll();
//      CFStringRef titleRef = ::CFStringCreateWithCharacters(kCFAllocatorDefault, (UniChar*)mLabel.get(), mLabel.Length());
//      NS_ASSERTION(titleRef, "nsMenu::AttributeChanged: CFStringCreateWithCharacters failed.");
//      ::SetMenuTitleWithCFString(mMacMenuHandle, titleRef);
//      ::CFRelease(titleRef);
      
//      ::DrawMenuBar();
    }
  }
  else if(aAttribute == nsWidgetAtoms::hidden || aAttribute == nsWidgetAtoms::collapsed) {
puts("other");

    SetRebuild(PR_TRUE);
      
      nsAutoString hiddenValue, collapsedValue;
      mMenuContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::hidden, hiddenValue);
      mMenuContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::collapsed, collapsedValue);
        
//      if (hiddenValue.EqualsLiteral("true") || collapsedValue.EqualsLiteral("true")) {
//        if ( mVisible ) {
//          if ( menubarParent ) {
//            PRUint32 indexToRemove = 0;
//            if ( NS_SUCCEEDED(CountVisibleBefore(&indexToRemove)) ) {
//              ++indexToRemove;                // if there are N siblings before me, my index is N+1
//              void *clientData = nsnull;
//              menubarParent->GetNativeData ( clientData );
//              if ( clientData ) {
//                MenuRef menubar = reinterpret_cast<MenuRef>(clientData);
//                ::SetMenuItemHierarchicalMenu(menubar, indexToRemove, nsnull);
//                ::DeleteMenuItem(menubar, indexToRemove);
//                mVisible = PR_FALSE;
//              }
//            }
//          } // if on the menubar
//          else {
//            // hide this submenu
//            NS_ASSERTION(PR_FALSE, "nsMenu::AttributeChanged: WRITE HIDE CODE FOR SUBMENU.");
//          }
//        } // if visible
//        else
//          NS_WARNING("You're hiding the menu twice, please stop");
//      } // if told to hide menu
//      else {
//        if ( !mVisible ) {
//          if ( menubarParent ) {
//            PRUint32 insertAfter = 0;
//            if ( NS_SUCCEEDED(CountVisibleBefore(&insertAfter)) ) {
//              void *clientData = nsnull;
//              menubarParent->GetNativeData ( clientData );
//              if ( clientData ) {
//                MenuRef menubar = reinterpret_cast<MenuRef>(clientData);
//                // Shove this menu into its rightful place in the menubar. It doesn't matter
//                // what title we pass to InsertMenuItem() because when we stuff the actual menu
//                // handle in, the correct title goes with it.
//                ::InsertMenuItem(menubar, "\pPlaceholder", insertAfter);
//                ::SetMenuItemHierarchicalMenu(menubar, insertAfter + 1, mMacMenuHandle);  // add 1 to get index of inserted item
//                mVisible = PR_TRUE;
//              }
//            }
//          } // if on menubar
//          else {
//            // show this submenu
//            NS_ASSERTION(PR_FALSE, "nsMenu::AttributeChanged: WRITE SHOW CODE FOR SUBMENU.");
//          }
//        } // if not visible
//      } // if told to show menu

      if (menubarParent) {
//        ::DrawMenuBar();
      }
  }
#endif

  return NS_OK;
  
} // AttributeChanged


NS_IMETHODIMP
nsMenu :: ContentRemoved(nsIDocument *aDocument, nsIContent *aChild, PRInt32 aIndexInContainer)
{  
  if (gConstructingMenu)
    return NS_OK;

    SetRebuild(PR_TRUE);

  RemoveItem(aIndexInContainer);
  mManager->Unregister(aChild);

  return NS_OK;
  
} // ContentRemoved


NS_IMETHODIMP
nsMenu :: ContentInserted(nsIDocument *aDocument, nsIContent *aChild, PRInt32 aIndexInContainer)
{  
  if(gConstructingMenu)
    return NS_OK;

    SetRebuild(PR_TRUE);
  
  return NS_OK;
  
} // ContentInserted
