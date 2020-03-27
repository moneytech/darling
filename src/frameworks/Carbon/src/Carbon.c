/*
 This file is part of Darling.

 Copyright (C) 2017 Lubos Dolezel

 Darling is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Darling is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Darling.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <Carbon/Carbon.h>

#include <stdlib.h>
#include <stdio.h>

static int verbose = 0;

__attribute__((constructor))
static void initme(void) {
    verbose = getenv("STUB_VERBOSE") != NULL;
}


// These stubs should prob be moved elsewhere

OSErr ActivateTSMDocument(TSMDocumentID a)
{
    if (verbose) puts("STUB: ActivateTSMDocument called");
	return 0;
}

OSErr DeactivateTSMDocument(TSMDocumentID a)
{
    if (verbose) puts("STUB: DeactivateTSMDocument called");
	return 0;
}

OSStatus CallNextEventHandler(EventHandlerCallRef a, EventRef b)
{
    if (verbose) puts("STUB: CallNextEventHandler called");
	return 0;
}

OSStatus CreateEvent(CFAllocatorRef a, UInt32 b, UInt32 c, EventTime d, EventAttributes e, EventRef * f)
{
    if (verbose) puts("STUB: CreateEvent called");
	return 0;
}

OSStatus CreateStandardAlert(AlertType a, CFStringRef b, CFStringRef c, const AlertStdCFStringAlertParamRec * d, DialogRef * e)
{
    if (verbose) puts("STUB: CreateStandardAlert called");
	return 0;
}

OSErr UseInputWindow(TSMDocumentID a, Boolean b)
{
    if (verbose) puts("STUB: UseInputWindow called");
	return 0;
}

void FlushEvents(EventMask a, EventMask b)
{
    if (verbose) puts("STUB: FlushEvents called");
}

EventTargetRef GetApplicationEventTarget(void)
{
    if (verbose) puts("STUB: GetApplicationEventTarget called");
	return (EventTargetRef)0;
}

UInt32 GetEventClass(EventRef a)
{
    if (verbose) puts("STUB: GetEventClass called");
	return 0;
}

OSStatus GetEventDispatcherTarget()
{
    if (verbose) puts("STUB: GetEventDispatcherTarget called");
	return 0;
}

UInt32 GetEventKind(EventRef a)
{
    if (verbose) puts("STUB: GetEventKind called");
	return 0;
}

OSStatus GetEventParameter(EventRef a, EventParamName b, EventParamType c, EventParamType * d, UInt32 e, UInt32 * f, void * g)
{
    if (verbose) puts("STUB: GetEventParameter called");
	return 0;
}

OSStatus GetMainEventQueue()
{
    if (verbose) puts("STUB: GetMainEventQueue called");
	return 0;
}

OSStatus GetScrapByName(CFStringRef a, OptionBits b, ScrapRef * c)
{
    if (verbose) puts("STUB: GetScrapByName called");
	return 0;
}

OSStatus GetScrapFlavorData(ScrapRef a, ScrapFlavorType b, Size * c, void * d)
{
    if (verbose) puts("STUB: GetScrapFlavorData called");
	return 0;
}

OSStatus GetScrapFlavorSize(ScrapRef a, ScrapFlavorType b, Size * c)
{
    if (verbose) puts("STUB: GetScrapFlavorSize called");
	return 0;
}

OSStatus GetStandardAlertDefaultParams(AlertStdCFStringAlertParamPtr a, UInt32 b)
{
    if (verbose) puts("STUB: GetStandardAlertDefaultParams called");
	return 0;
}

void HideMenuBar(void)
{
    if (verbose) puts("STUB: HideMenuBar called");
	
}

OSStatus InstallEventHandler(EventTargetRef a, EventHandlerUPP b, UInt32 c, const EventTypeSpec * d, void * e, EventHandlerRef * f)
{
    if (verbose) puts("STUB: InstallEventHandler called");
	return 0;
}

OSErr NMInstall(NMRecPtr a)
{
    if (verbose) puts("STUB: NMInstall called");
	return 0;
}

EventHandlerUPP NewEventHandlerUPP(EventHandlerProcPtr a)
{
    if (verbose) puts("STUB: NewEventHandlerUPP called");
	return (EventHandlerUPP)0;
}

OSErr NewTSMDocument(short a, InterfaceTypeList b, TSMDocumentID * c, long d)
{
    if (verbose) puts("STUB: NewTSMDocument called");
	return 0;
}

OSStatus PostEventToQueue(EventQueueRef a, EventRef b, EventPriority c)
{
    if (verbose) puts("STUB: PostEventToQueue called");
	return 0;
}

OSStatus ProcessHICommand(const HICommand * a)
{
    if (verbose) puts("STUB: ProcessHICommand called");
	return 0;
}

OSStatus PutScrapFlavor(ScrapRef a, ScrapFlavorType b, ScrapFlavorFlags c, Size d, const void * e)
{
    if (verbose) puts("STUB: PutScrapFlavor called");
	return 0;
}

OSStatus ReceiveNextEvent(UInt32 a, const EventTypeSpec * b, EventTimeout c, Boolean d, EventRef * e)
{
    if (verbose) puts("STUB: ReceiveNextEvent called");
	return 0;
}

OSStatus ReleaseEvent(EventRef a)
{
    if (verbose) puts("STUB: ReleaseEvent called");
	return 0;
}

OSStatus RemoveEventHandler(EventHandlerRef a)
{
    if (verbose) puts("STUB: RemoveEventHandler called");
	return 0;
}

void RunApplicationEventLoop(void)
{
    if (verbose) puts("STUB: RunApplicationEventLoop called");
	
}

OSStatus RunStandardAlert(DialogRef a, ModalFilterUPP b, DialogItemIndex * c)
{
    if (verbose) puts("STUB: RunStandardAlert called");
	return 0;
}

OSStatus SendEventToEventTarget(EventRef a, EventTargetRef b)
{
    if (verbose) puts("STUB: SendEventToEventTarget called");
	return 0;
}

OSStatus SetEventMask(EventMask a)
{
    if (verbose) puts("STUB: SetEventMask called");
	return 0;
}

OSStatus SetEventParameter(EventRef a, EventParamName b, EventParamType c, UInt32 d, const void * e)
{
    if (verbose) puts("STUB: SetEventParameter called");
	return 0;
}

void ShowMenuBar(void)
{
    if (verbose) puts("STUB: ShowMenuBar called");
}


void GetKeys (KeyMap theKeys)
{

}

