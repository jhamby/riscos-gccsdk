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
#include "nsIDocumentViewer.h"
#include "nsIContent.h"

#include "nsMenuBar.h"         // for MenuHelper's namespace
#include "nsMenu.h"
#include "nsMenuItem.h"

#include "nsWidgetAtoms.h"

#include "nsIMenu.h"
#include "nsIMenuBar.h"
#include "nsIWidget.h"
#include "nsIMenuListener.h"
#include "nsINameSpaceManager.h"
#include "nsIServiceManager.h"
#include "nsIDocument.h"
#include "nsIDOMDocument.h"

#include "nsGUIEvent.h"


NS_IMPL_ISUPPORTS4(nsMenuItem, nsIMenuItem, nsIMenuListener, nsIChangeObserver, nsISupportsWeakReference)

//
// nsMenuItem constructor
//
nsMenuItem::nsMenuItem()
{
  mMenuParent         = nsnull;
  mIsSeparator        = PR_FALSE;
  mKeyEquivalent.AssignLiteral(" ");
  mEnabled            = PR_TRUE;
  mIsChecked          = PR_FALSE;
  mMenuType           = eRegular;

  puts("nsmenuitem::constructor");
}

//
// nsMenuItem destructor
//
nsMenuItem::~nsMenuItem()
{
  mManager->Unregister(mContent);
}


NS_METHOD nsMenuItem::Create ( nsIMenu* aParent, const nsString & aLabel, PRBool aIsSeparator,
                                EMenuItemType aItemType, PRBool aEnabled, 
                                nsIChangeManager* aManager, nsIDocShell* aShell, nsIContent* aNode )
{
  mContent = aNode;         // addref
  mMenuParent = aParent;    // weak
  mDocShellWeakRef = do_GetWeakReference(aShell);
  
  mEnabled = aEnabled;
  mMenuType = aItemType;
  
  // register for AttributeChanged messages
  mManager = aManager;
  nsCOMPtr<nsIChangeObserver> obs = do_QueryInterface(NS_STATIC_CAST(nsIChangeObserver*,this));
  mManager->Register(mContent, obs);   // does not addref this
  
  mIsSeparator = aIsSeparator;
  mLabel = aLabel;
  return NS_OK;
}

NS_METHOD
nsMenuItem::GetLabel(nsString &aText)
{
  aText = mLabel;
  return NS_OK;
}


NS_METHOD 
nsMenuItem::GetEnabled(PRBool *aIsEnabled)
{
  *aIsEnabled = mEnabled;
  return NS_OK;
}


NS_METHOD nsMenuItem::SetChecked(PRBool aIsEnabled)
{
  mIsChecked = aIsEnabled;
  
  // update the content model. This will also handle unchecking our siblings
  // if we are a radiomenu
  if (mIsChecked)
      mContent->SetAttr(kNameSpaceID_None, nsWidgetAtoms::checked,
                        NS_LITERAL_STRING("true"), PR_TRUE);
  else
      mContent->UnsetAttr(kNameSpaceID_None, nsWidgetAtoms::checked, PR_TRUE);

  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::GetChecked(PRBool *aIsEnabled)
{
  *aIsEnabled = mIsChecked;
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::GetMenuItemType(EMenuItemType *aType)
{
  *aType = mMenuType;
  return NS_OK;
}


//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::GetTarget(nsIWidget *& aTarget)
{
  NS_IF_ADDREF(aTarget = mTarget);
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::GetNativeData(void *& aData)
{
  //aData = (void *)mMenu;
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::AddMenuListener(nsIMenuListener * aMenuListener)
{
  mXULCommandListener = aMenuListener;    // addref
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::RemoveMenuListener(nsIMenuListener * aMenuListener)
{
  if (mXULCommandListener.get() == aMenuListener)
    mXULCommandListener = nsnull;
  return NS_OK;
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::IsSeparator(PRBool & aIsSep)
{
  aIsSep = mIsSeparator;
  return NS_OK;
}


//-------------------------------------------------------------------------
// nsIMenuListener interface
//-------------------------------------------------------------------------
nsEventStatus nsMenuItem::MenuItemSelected(const nsMenuEvent & aMenuEvent)
{
  // this is all handled by Carbon Events
  puts("menuitem:menuitemselected");
  return nsEventStatus_eConsumeNoDefault;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenuItem::MenuSelected(const nsMenuEvent & aMenuEvent)
{
  return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
// nsIMenuListener interface
//-------------------------------------------------------------------------
nsEventStatus nsMenuItem::MenuDeselected(const nsMenuEvent & aMenuEvent)
{
    return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenuItem::MenuConstruct(
    const nsMenuEvent & aMenuEvent,
    nsIWidget         * aParentWindow, 
    void              * menuNode,
    void              * aDocShell)
{
    return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenuItem::MenuDestruct(const nsMenuEvent & aMenuEvent)
{
    return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenuItem::CheckRebuild(PRBool & aNeedsRebuild)
{
  aNeedsRebuild = PR_TRUE; 
  return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
nsEventStatus nsMenuItem::SetRebuild(PRBool aNeedsRebuild)

{
  puts("menuitme: set rebuild");
//  mNeedsRebuild = aNeedsRebuild;
  return nsEventStatus_eIgnore;
}

//-------------------------------------------------------------------------
/**
* Executes the "cached" JavaScript Command 
* @return NS_OK if the command was executed properly, otherwise an error code
*/
NS_METHOD nsMenuItem::DoCommand()
{

    nsAutoString label;
    mContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, label);
    printf("DoCommand: %s\n", NS_LossyConvertUCS2toASCII(label).get());

    // flip "checked" state if we're a checkbox menu, or an un-checked radio menu
    if (mMenuType == nsIMenuItem::eCheckbox || (mMenuType == nsIMenuItem::eRadio && !mIsChecked)) {
puts("checkbox");
        nsAutoString value;
        mContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::autocheck, value);
        if (!value.EqualsLiteral("false"))
            SetChecked(!mIsChecked);
            /* the AttributeChanged code will update all the internal state */
    }

  return MenuHelpers::DispatchCommandTo(mDocShellWeakRef, mContent);
//  return nsEventStatus_eIgnore;
}
    
   
   //-------------------------------------------------------------------------
NS_METHOD nsMenuItem::GetModifiers(PRUint8 * aModifiers)
{
    nsresult res = NS_OK;
    *aModifiers = mModifiers; 
    return res; 
}

//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::SetModifiers(PRUint8 aModifiers)
{
    nsresult res = NS_OK;
    
    mModifiers = aModifiers;
    return res;
}
 
//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::SetShortcutChar(const nsString &aText)
{
    nsresult res = NS_OK;
    mKeyEquivalent = aText;
    return res;
} 

//-------------------------------------------------------------------------
NS_METHOD nsMenuItem::GetShortcutChar(nsString &aText)
{
    nsresult res = NS_OK;
    aText = mKeyEquivalent;
    return res;
} 

//
// UncheckRadioSiblings
//
// walk the sibling list looking for nodes with the same name and
// uncheck them all.
//
void
nsMenuItem :: UncheckRadioSiblings(nsIContent* inCheckedContent)
{
  nsAutoString myGroupName;
  inCheckedContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::name, myGroupName);
  if ( ! myGroupName.Length() )        // no groupname, nothing to do
    return;
  
  nsCOMPtr<nsIContent> parent = inCheckedContent->GetParent();
  if ( !parent )
    return;

  // loop over siblings
  PRUint32 count = parent->GetChildCount();
  for ( PRUint32 i = 0; i < count; ++i ) {
    nsIContent *sibling = parent->GetChildAt(i);
    if ( sibling ) {      
      if ( sibling != inCheckedContent ) {                    // skip this node
        // if the current sibling is in the same group, clear it
        nsAutoString currGroupName;
        sibling->GetAttr(kNameSpaceID_None, nsWidgetAtoms::name, currGroupName);
        if ( currGroupName == myGroupName )
          sibling->SetAttr(kNameSpaceID_None, nsWidgetAtoms::checked, NS_LITERAL_STRING("false"), PR_TRUE);
      }
    }    
  } // for each sibling

} // UncheckRadioSiblings


//
// nsIChangeObserver
//


NS_IMETHODIMP
nsMenuItem :: AttributeChanged ( nsIDocument *aDocument, PRInt32 aNameSpaceID, nsIContent *aContent, nsIAtom *aAttribute )
{
  nsAutoString menuitemName;
  mContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::label, menuitemName);
  printf("attribute changed: \"%s\"\n", NS_LossyConvertUCS2toASCII(menuitemName).get());

  if (aAttribute == nsWidgetAtoms::checked) {
    // if we're a radio menu, uncheck our sibling radio items. No need to
    // do any of this if we're just a normal check menu.
    if ( mMenuType == eRadio ) {
      nsAutoString checked;
      mContent->GetAttr(kNameSpaceID_None, nsWidgetAtoms::checked, checked);
      if (checked.EqualsLiteral("true") ) 
        UncheckRadioSiblings(mContent);
    }


    nsCOMPtr<nsIMenuListener> listener = do_QueryInterface(mMenuParent);
    listener->SetRebuild(PR_TRUE);
    
  } 
  else if (aAttribute == nsWidgetAtoms::disabled || aAttribute == nsWidgetAtoms::hidden ||
             aAttribute == nsWidgetAtoms::collapsed || aAttribute == nsWidgetAtoms::label )  {
    nsCOMPtr<nsIMenuListener> listener = do_QueryInterface(mMenuParent);
    listener->SetRebuild(PR_TRUE);
  }
  
  return NS_OK;

} // AttributeChanged


NS_IMETHODIMP
nsMenuItem :: ContentRemoved(nsIDocument *aDocument, nsIContent *aChild, PRInt32 aIndexInContainer)
{
  
  nsCOMPtr<nsIMenuListener> listener = do_QueryInterface(mMenuParent);
  listener->SetRebuild(PR_TRUE);
  return NS_OK;
  
} // ContentRemoved

NS_IMETHODIMP
nsMenuItem :: ContentInserted(nsIDocument *aDocument, nsIContent *aChild, PRInt32 aIndexInContainer)
{
  
  nsCOMPtr<nsIMenuListener> listener = do_QueryInterface(mMenuParent);
  listener->SetRebuild(PR_TRUE);
  return NS_OK;
  
} // ContentInserted
