//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormMain.h"
#include "FormAbout.h"
#include "FormSettings.h"
#include "BtnController.h"
#include "Settings.h"
#include "PhoneState.h"
#include "T2S.h"
#include "Utils.h"
#include "Paths.h"
#include "Log.h"
#include <memory>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain = NULL;

namespace
{

std::string path = Utils::GetDllPath();
std::string dllName = Utils::ExtractFileNameWithoutExtension(path);

}

extern int RunScriptAsync(const char* script);


//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
#if 1
	// disable 'X' system menu button
	HMENU hSysMenu = GetSystemMenu(this->Handle, false);
	if (hSysMenu != NULL)
	{
		EnableMenuItem(hSysMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
		DrawMenuBar(this->Handle);
	}
#endif
	btnSendOnEnter->Down = appSettings.Tts.bSendOnEnter;
}
//---------------------------------------------------------------------------

void TfrmMain::AddOutputText(AnsiString text)
{
	redHistory->Text = redHistory->Text + text + "\n";
	redHistory->SelStart = redHistory->GetTextLen();
	redHistory->Perform( EM_SCROLLCARET, 0 , 0 );
}

void TfrmMain::ClearOutput(void)
{
	redHistory->Clear();
}

void __fastcall TfrmMain::miAboutClick(TObject *Sender)
{
	TfrmAbout *frmAbout = new TfrmAbout(this);
	frmAbout->ShowModal();
	delete frmAbout;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnSendClick(TObject *Sender)
{
	Send();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tmrPollTimer(TObject *Sender)
{
	if (phoneState.audioError)
	{
		phoneState.audioError = false;
		pnlPlaying->Visible = false;
		AddOutputText(lblPlaying->Caption);
		lblPlaying->Caption = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::redInputKeyPress(TObject *Sender, char &Key)
{
	if (btnSendOnEnter->Down)
	{
		if (Key == VK_RETURN)
		{
			Send();
			Key = 0;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Send(void)
{
	int TODO__USE_MORE_DETAILED_CALL_STATE;
	int TODO__UPDATE_CALL_STATE_ON_PLUGIN_LOAD;
	//if (phoneState.callState != 1)
	if (0)
	{
    	return;
	}

	AnsiString text = redInput->Text;
	if (text.Trim() == "")
		return;
	AnsiString filenameWoPath;
	filenameWoPath.sprintf("%s_tmp.wav", dllName.c_str());
	AnsiString filename = Paths::GetProfileDir() + "\\" + filenameWoPath;
	int rc = T2S::WriteToFile(filename, text);
	if (rc != 0)
	{
		LOG("Failed to create/write wav file %s\n", filename.c_str());
		return;
	}
	else
	{
		//LOG("TTS: [%s] written to %s\n", redInput->Text, filename.c_str());
	}
	redInput->Text = "";

	phoneState.audioError = false;
	lblPlaying->Caption = text;
	pnlPlaying->Visible = true;

	AnsiString script;
	script.sprintf("SwitchAudioSource(\"aufile\", \"%s\")", filenameWoPath.c_str());
	RunScriptAsync(script.c_str());
}

void __fastcall TfrmMain::btnSendOnEnterClick(TObject *Sender)
{
	appSettings.Tts.bSendOnEnter = btnSendOnEnter->Down;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::miClearHistoryClick(TObject *Sender)
{
	ClearOutput();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnCancelPlayingClick(TObject *Sender)
{
	AnsiString script;
	script.sprintf("SwitchAudioSource(\"nullaudio\", \"\")");
	RunScriptAsync(script.c_str());
	pnlPlaying->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::miSettingsClick(TObject *Sender)
{
	TfrmSettings *frmSettings = new TfrmSettings(NULL);

	frmSettings->appSettings = &appSettings;
	frmSettings->ShowModal();
	if (appSettings.frmMain.bAlwaysOnTop)
		this->FormStyle = fsStayOnTop;
	else
		this->FormStyle = fsNormal;

	delete frmSettings;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::miInputPasteClick(TObject *Sender)
{
	redInput->PasteFromClipboard();	
}
//---------------------------------------------------------------------------

