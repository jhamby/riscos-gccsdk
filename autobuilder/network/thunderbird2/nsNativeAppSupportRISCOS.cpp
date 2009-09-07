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
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Bill Law       law@netscape.com
 *   Peter Naulls   peter@chocky.org
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

#include "nsIBaseWindow.h"
#include "nsIDOMWindowInternal.h"
#include "nsNativeAppSupportBase.h"
#include "nsIAppStartup.h"
#include "nsIServiceManager.h"
#include "nsCOMPtr.h"
#include "nsIObserver.h"
#include "nsIObserverService.h"
#include "nsIWindowWatcher.h"
#include "nsISupportsPrimitives.h"
#include "nsString.h"

#include "nsXPFEComponentsCID.h"

#include "DeskLib:WimpSWIs.h"
#include "DeskLib:Icon.h"
#include "DeskLib:Menu.h"
#include "DeskLib:Url.h"
#include "DeskLib:Template.h"
#include "DeskLib:Window.h"
#include "DeskLib:Resource.h"

const char * const __dynamic_da_name = "Mozilla Thunderbird";

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static icon_handle iconbar_icon;
static menu_ptr    iconbar_menu;

menu_ptr gCurrentROMenu;




class nsNativeAppSupportRISCOS : public nsNativeAppSupportBase,
                                 public nsIObserver
{
public:
    NS_DECL_NSIOBSERVER
    NS_DECL_ISUPPORTS_INHERITED

    nsNativeAppSupportRISCOS() { }

    // Overrides of base implementation.
    NS_IMETHOD Start( PRBool *aResult );
    NS_IMETHOD Stop( PRBool *aResult );
    NS_IMETHOD Quit();
    NS_IMETHOD Enable();
    NS_IMETHOD ReOpen();
    NS_IMETHOD OnLastWindowClosing();

    // Static methods
    static nsresult OpenWindow(const char *url);
    static BOOL IconbarHandler(event_pollblock *event, void *reference);
    static BOOL MenuHandler(event_pollblock *event, void *reference);
    static BOOL UrlHandler(BOOL check_only, const char *url);

};


NS_INTERFACE_MAP_BEGIN(nsNativeAppSupportRISCOS)
NS_INTERFACE_MAP_ENTRY(nsIObserver)
NS_INTERFACE_MAP_END_INHERITING(nsNativeAppSupportBase)

NS_IMPL_ADDREF_INHERITED(nsNativeAppSupportRISCOS, nsNativeAppSupportBase)
NS_IMPL_RELEASE_INHERITED(nsNativeAppSupportRISCOS, nsNativeAppSupportBase)



//nsNativeAppSupportRISCOS::~nsNativeAppSupportRISCOS()
//{
//}

//NS_IMPL_ISUPPORTS1(nsNativeAppSupportBase, nsINativeAppSupport)

// Start answer defaults to OK.
NS_IMETHODIMP
nsNativeAppSupportRISCOS::Start( PRBool *result )
{
//  fputs("Native App Start\n", stderr);

  *result = PR_TRUE;
  return NS_OK;
}


extern "C" {
void Chox11_CreateInternalEvent(void (*callback_func)(void *), void *arg);
}

static void OpenWindowFunc(void *arg)
{
  const char *args = (const char *)arg;
  const char *url = "chrome://browser/content/";

  /* NULL argument is an overload for quit */
  if (!args)
  {
    nsresult rv;
    nsCOMPtr<nsIAppStartup> appStartup(do_GetService(NS_APPSTARTUP_CONTRACTID, &rv));
    if (!NS_FAILED(rv)) appStartup->Quit(nsIAppStartup::eAttemptQuit);

  } else {
    nsCOMPtr<nsIWindowWatcher> wwatch(do_GetService(NS_WINDOWWATCHER_CONTRACTID));
    nsCOMPtr<nsISupportsCString> sarg(do_CreateInstance(NS_SUPPORTS_CSTRING_CONTRACTID));
    if (sarg)
      sarg->SetData(nsDependentCString(args));

    if (wwatch && sarg) {
      nsCOMPtr<nsIDOMWindow> newWindow;
      wwatch->OpenWindow(nsnull,
                         url,
                         "_blank",
                         "chrome,dialog=no,all",
                         sarg, getter_AddRefs(newWindow));
    }
  }
}



nsresult
nsNativeAppSupportRISCOS::OpenWindow(const char *url)
{
  Chox11_CreateInternalEvent(OpenWindowFunc, strdup(url));

  return NS_OK;
}


BOOL
nsNativeAppSupportRISCOS::IconbarHandler(event_pollblock *event, void *reference)
{

  if (event->data.mouse.button.data.select)
  {
    return OpenWindow("") == NS_OK;
  }
  else if (event->data.mouse.button.data.menu)
  {
    Menu_Show(iconbar_menu, event->data.mouse.pos.x, -1);
    gCurrentROMenu = iconbar_menu;
    return TRUE;
  }

  return FALSE;
}


BOOL
nsNativeAppSupportRISCOS::MenuHandler(event_pollblock *event, void *reference)
{
  if (gCurrentROMenu == iconbar_menu)
  {
    switch (event->data.selection[0])
    {
      case 1: // Quit
        OpenWindow(NULL);
        break;
    }

    mouse_block ptr;
    Wimp_GetPointerInfo(&ptr);
    if (ptr.button.data.adjust) Menu_ShowLast();
  }

  return FALSE;
}


BOOL
nsNativeAppSupportRISCOS::UrlHandler(BOOL check_only, const char *url)
{
  if (!(strncmp(url, "http://", 7) == 0))
    return FALSE;

  if (!check_only)
  {
    return OpenWindow(url) == NS_OK;
  }

  return TRUE;
}


NS_IMETHODIMP
nsNativeAppSupportRISCOS::Enable()
{
//  fputs("Native App Enable\n", stderr);

  Resource_Initialise("Thunderbird");

  Template_Initialise();
  Template_LoadFile("Templates");

  iconbar_icon = Icon_BarIcon("!thunderbird", iconbar_RIGHT);

  iconbar_menu = Menu_New("Mozilla Thunderbird", "Info|Quit");

  Menu_AddSubMenu(iconbar_menu, 0, (menu_ptr)Window_Create("Info", 0));

  Event_Claim(event_CLICK, window_ICONBAR, iconbar_icon, IconbarHandler, NULL);
  Event_Claim(event_MENU, event_ANY, event_ANY, MenuHandler, this);
  Url_AddHandler(UrlHandler);

  nsCOMPtr<nsIObserverService> obs
      (do_GetService("@mozilla.org/observer-service;1"));
  if (obs) {
      obs->AddObserver(this, "quit-application", PR_FALSE);
  } else {
      NS_ERROR("No observer service?");
  }

  return NS_OK;
}


NS_IMETHODIMP
nsNativeAppSupportRISCOS::Stop( PRBool *result )
{
//  puts("Native App Stop");
  *result = PR_TRUE;
  return NS_OK;
}

NS_IMETHODIMP
nsNativeAppSupportRISCOS::Quit()
{
//  fputs("Native App Quit\n", stderr);
  return NS_OK;
}

NS_IMETHODIMP
nsNativeAppSupportRISCOS::ReOpen()
{
  return NS_OK;
}

NS_IMETHODIMP
nsNativeAppSupportRISCOS::OnLastWindowClosing() {
//  puts("Native App On Last Window Closing");
  return NS_OK;
}


NS_IMETHODIMP
nsNativeAppSupportRISCOS::Observe(nsISupports* aSubject, const char *aTopic,
                                  const PRUnichar* aData)
{
//  printf("Native App Observe: %s\n", aTopic);
  return NS_OK;
}

nsresult
NS_CreateNativeAppSupport( nsINativeAppSupport **aResult )
{
  nsNativeAppSupportRISCOS* native = new nsNativeAppSupportRISCOS();
  if (!native) return NS_ERROR_OUT_OF_MEMORY;

  *aResult = native;
  NS_ADDREF( *aResult );

  return NS_OK;
}

