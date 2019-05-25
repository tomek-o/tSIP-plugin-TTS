//---------------------------------------------------------------------------

#define _EXPORTING
#include "..\tSIP\tSIP\phone\Phone.h"
#include "..\tSIP\tSIP\phone\PhoneSettings.h"
#include "..\tSIP\tSIP\phone\PhoneCapabilities.h"
#include "Log.h"
#include <assert.h>
#include <algorithm>	// needed by Utils::in_group
#include "Utils.h"
#include "FormMain.h"
#include "Settings.h"
#include "PhoneState.h"
#include "Paths.h"
#include "T2S.h"
#include <string>


//---------------------------------------------------------------------------

static const struct S_PHONE_DLL_INTERFACE dll_interface =
{DLL_INTERFACE_MAJOR_VERSION, DLL_INTERFACE_MINOR_VERSION};

// callback ptrs
CALLBACK_LOG lpLogFn = NULL;
CALLBACK_CONNECT lpConnectFn = NULL;
CALLBACK_KEY lpKeyFn = NULL;
CALLBACK_PAGING_TX lpPagingTxFn = NULL;
CALLBACK_CLEAR_DIAL lpClearDialFn = NULL;
CALLBACK_RUN_SCRIPT_ASYNC lpRunScriptAsyncFn = NULL;

void *callbackCookie;	///< used by upper class to distinguish library instances when receiving callbacks

namespace {
	std::string configPath = Utils::ReplaceFileExtension(Utils::GetDllPath(), ".cfg");
}

void __stdcall GetPhoneInterfaceDescription(struct S_PHONE_DLL_INTERFACE* interf) {
	interf->majorVersion = dll_interface.majorVersion;
	interf->minorVersion = dll_interface.minorVersion;
}

void Log(char* txt) {
	if (lpLogFn)
		lpLogFn(callbackCookie, txt);
}

void Connect(int state, char *szMsgText) {
	if (lpConnectFn)
		lpConnectFn(callbackCookie, state, szMsgText);
}

void Key(int keyCode, int state) {
	if (lpKeyFn)
		lpKeyFn(callbackCookie, keyCode, state);
}

int PagingTx(const char* target, const char* filename, const char* codecname) {
	if (lpPagingTxFn) {
		return lpPagingTxFn(callbackCookie, target, filename, codecname);
	}
	return -1;
}

int ClearDial(void) {
	if (lpClearDialFn) {
		lpClearDialFn(callbackCookie);
		return 0;
	}
	return -1;
}

int RunScriptAsync(const char* script) {
	if (lpRunScriptAsyncFn) {
		return lpRunScriptAsyncFn(callbackCookie, script);
	}
	return -1;
}

void __stdcall SetCallbacks(void *cookie, CALLBACK_LOG lpLog, CALLBACK_CONNECT lpConnect, CALLBACK_KEY lpKey) {
	assert(cookie && lpLog && lpConnect && lpKey);
	lpLogFn = lpLog;
	lpConnectFn = lpConnect;
	lpKeyFn = lpKey;
	callbackCookie = cookie;
	lpLogFn(callbackCookie, "Phone DLL for EX03 loaded\n");
}

void __stdcall GetPhoneCapabilities(struct S_PHONE_CAPABILITIES **caps) {
	static struct S_PHONE_CAPABILITIES capabilities = {
        0
    };
    *caps = &capabilities;
}

void __stdcall ShowSettings(HANDLE parent) {
    MessageBox((HWND)parent, "No additional settings.", "Device DLL", MB_ICONINFORMATION);
}

int __stdcall Connect(void) {
	appSettings.Read(configPath.c_str());
	if (frmMain == NULL) {
		frmMain = new TfrmMain(NULL);
		frmMain->Visible = true;
		frmMain->Show();
	}
	int rc = T2S::Init();
	if (rc != 0)
	{

	}
	RunScriptAsync(appSettings.Scripts.onLoad.c_str());
	return 0;
}

int __stdcall Disconnect(void) {
	if (frmMain) {
	#if 0
		while (frmMain->isScriptRunning()) {
			Application->ProcessMessages();
			Sleep(100);
		}
	#endif
		delete frmMain;
		frmMain = NULL;
	}
	appSettings.Write(configPath.c_str());
	T2S::Shutdown();
	RunScriptAsync(appSettings.Scripts.onUnload.c_str());
    return 0;
}

static bool bSettingsReaded = false;

static int GetDefaultSettings(struct S_PHONE_SETTINGS* settings) {
    settings->ring = 1;

    bSettingsReaded = true;
    return 0;
}

int __stdcall GetPhoneSettings(struct S_PHONE_SETTINGS* settings) {
    //settings->iTriggerSrcChannel = 0;

    std::string path = Utils::GetDllPath();
    path = Utils::ReplaceFileExtension(path, ".cfg");
    if (path == "")
        return GetDefaultSettings(settings);


    GetDefaultSettings(settings);


    //int mode = root.get("TriggerMode", TRIGGER_MODE_AUTO).asInt();
    settings->ring = true;//root.get("ring", settings->ring).asInt();


    bSettingsReaded = true;
    return 0;
}

int __stdcall SavePhoneSettings(struct S_PHONE_SETTINGS* settings) {

    return 0;
}

int __stdcall SetRegistrationState(int state) {
	phoneState.registered = state;
    return 0;
}

int __stdcall SetCallState(int state, const char* display) {
	phoneState.callState = state;
	phoneState.display = display;
	if (state)
	{
		RunScriptAsync(appSettings.Scripts.onLoad.c_str());
	}
	return 0;
}


int __stdcall Ring(int state) {
#if 0
    //MessageBox(NULL, "Ring", "Device DLL", MB_ICONINFORMATION);
    if (state) {
        if (connected == false) {
            RingThreadStart();
        }
    } else {
        RingThreadStop();
    }
#endif
    return 0;
}

void __stdcall SetAudioError(void) {
	phoneState.audioError = true;
}

void __stdcall SetProfileDir(const char* dir) {
	Paths::SetProfileDir(dir);
}

void __stdcall SetPagingTxCallback(CALLBACK_PAGING_TX lpPagingTx) {
	lpPagingTxFn = lpPagingTx;
}

int __stdcall SetPagingTxState(int state) {
	phoneState.paging = state;
	return 0;
}

void __stdcall SetClearDialCallback(CALLBACK_CLEAR_DIAL lpClearDial) {
	lpClearDialFn = lpClearDial;
}

void __stdcall SetRunScriptAsyncCallback(CALLBACK_RUN_SCRIPT_ASYNC lpRunScriptAsync) {
	lpRunScriptAsyncFn = lpRunScriptAsync;
}

