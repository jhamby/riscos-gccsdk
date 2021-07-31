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
 *   Peter Hartshorn <peter@igelaus.com.au>
 *   Ken Faulkner <faulkner@igelaus.com.au>
 *   Dan Rosen <dr@netscape.com>
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

#include "nsIGenericFactory.h"
#include "nsIModule.h"
#include "nsCOMPtr.h"
#include "nsWidgetsCID.h"

#include "nsWindow.h"
#include "nsButton.h"
#include "nsCheckButton.h"
#include "nsTextWidget.h"
#include "nsAppShell.h"
#include "nsToolkit.h"
#include "nsLookAndFeel.h"
#include "nsMenuBar.h"
#include "nsMenu.h"
#include "nsMenuItem.h"
#include "nsLabel.h"
#include "nsTransferable.h"
#include "nsClipboard.h"
#include "nsClipboardHelper.h"
#include "nsHTMLFormatConverter.h"
#include "nsDragService.h"
#include "nsSound.h"
#include "nsNativeScrollbar.h"

#include "nsBidiKeyboard.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(nsWindow)
NS_GENERIC_FACTORY_CONSTRUCTOR(ChildWindow)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsButton)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsCheckButton)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsTextWidget)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsAppShell)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsToolkit)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsLookAndFeel)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsMenuBar)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsMenu)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsMenuItem)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSound)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsLabel)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsTransferable)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsClipboard)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsClipboardHelper)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsHTMLFormatConverter)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsDragService)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsNativeScrollbar)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsBidiKeyboard)

static const nsModuleComponentInfo components[] =
{
  { "RISC OS nsWindow",
    NS_WINDOW_CID,
    "@mozilla.org/widgets/window/riscos;1",
    nsWindowConstructor },
  { "RISC OS Child nsWindow",
    NS_CHILD_CID,
    "@mozilla.org/widgets/child_window/riscos;1",
    ChildWindowConstructor },
  { "RISC OS Button",
    NS_BUTTON_CID,
    "@mozilla.org/widgets/button/riscos;1",
    nsButtonConstructor },
  { "RISC OS Check Button",
    NS_CHECKBUTTON_CID,
    "@mozilla.org/widgets/checkbutton/riscos;1",
    nsCheckButtonConstructor },
  { "RISC OS Text Widget",
    NS_TEXTFIELD_CID,
    "@mozilla.org/widgets/textwidget/riscos;1",
    nsTextWidgetConstructor },
  { "RISC OS AppShell",
    NS_APPSHELL_CID,
    "@mozilla.org/widget/appshell/riscos;1",
    nsAppShellConstructor },
  { "RISC OS Toolkit",
    NS_TOOLKIT_CID,
    "@mozilla.org/widget/toolkit/riscos;1",
    nsToolkitConstructor },
  { "RISC OS Look And Feel",
    NS_LOOKANDFEEL_CID,
    "@mozilla.org/widget/lookandfeel;1",
    nsLookAndFeelConstructor },
  { "Menubar",
    NS_MENUBAR_CID,
    "@mozilla.org/widget/menubar/riscos;1",
    nsMenuBarConstructor },
  { "Menu",
   NS_MENU_CID,
   "@mozilla.org/widget/menu/riscos;1",
    nsMenuConstructor },
  { "Menu",
   NS_MENUITEM_CID,
   "@mozilla.org/widget/menuitem/riscos;1",
    nsMenuItemConstructor },
  { "RISC OS Sound",
    NS_SOUND_CID,
      "@mozilla.org/sound;1",
    nsSoundConstructor },
  { "RISC OS Label",
    NS_LABEL_CID,
    "@mozilla.org/widget/label/riscos;1",
    nsLabelConstructor },
  { "Transferrable",
    NS_TRANSFERABLE_CID,
    "@mozilla.org/widget/transferable;1",
    nsTransferableConstructor },
  { "RISC OS Clipboard",
    NS_CLIPBOARD_CID,
    "@mozilla.org/widget/clipboard;1",
    nsClipboardConstructor },
  { "Clipboard Helper",
    NS_CLIPBOARDHELPER_CID,
    "@mozilla.org/widget/clipboardhelper;1",
    nsClipboardHelperConstructor },
  { "HTML Format Converter",
    NS_HTMLFORMATCONVERTER_CID,
    "@mozilla.org/widget/htmlformatconverter;1",
    nsHTMLFormatConverterConstructor },
  { "RISC OS Drag Service",
    NS_DRAGSERVICE_CID,
    "@mozilla.org/widget/dragservice;1",
    nsDragServiceConstructor },
  { "RISC OS Native Scrollbar",
    NS_NATIVESCROLLBAR_CID,
    "@mozilla.org/widget/nativescrollbar;1",
    nsNativeScrollbarConstructor },
  { "RISC OS Bidi Keyboard",
    NS_BIDIKEYBOARD_CID,
    "@mozilla.org/widget/bidikeyboard;1",
    nsBidiKeyboardConstructor }
};

PR_STATIC_CALLBACK(void)
nsWidgetRISCOSModuleDtor(nsIModule *self)
{
  nsClipboard::Shutdown();
}

NS_IMPL_NSGETMODULE_WITH_DTOR(nsWidgetRISCOSModule,
                              components,
                              nsWidgetRISCOSModuleDtor)
