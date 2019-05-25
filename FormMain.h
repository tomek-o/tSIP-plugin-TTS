//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ActnList.hpp>
#include <StdActns.hpp>
#include <Buttons.hpp>

class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlBottom;
	TPanel *pnlBottom2;
	TRichEdit *redInput;
	TMainMenu *MainMenu;
	TMenuItem *miHelp;
	TMenuItem *miFile;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TActionList *actionList;
	TMenuItem *Open1;
	TMenuItem *SaveAs1;
	TAction *actFileOpen;
	TAction *actFileSaveAs;
	TAction *actFileSave;
	TMenuItem *Save1;
	TMenuItem *miOpenRecent;
	TMenuItem *miAbout;
	TRichEdit *redHistory;
	TButton *btnSend;
	TSpeedButton *btnSendOnEnter;
	TTimer *tmrPoll;
	TPopupMenu *popupHistory;
	TMenuItem *miClearHistory;
	TMenuItem *miConfiguration;
	TMenuItem *miSettings;
	TPanel *pnlPlaying;
	TLabel *lblPlaying;
	TLabel *Label1;
	TButton *btnCancelPlaying;
	void __fastcall miAboutClick(TObject *Sender);
	void __fastcall btnSendClick(TObject *Sender);
	void __fastcall tmrPollTimer(TObject *Sender);
	void __fastcall redInputKeyPress(TObject *Sender, char &Key);
	void __fastcall btnSendOnEnterClick(TObject *Sender);
	void __fastcall miClearHistoryClick(TObject *Sender);
	void __fastcall btnCancelPlayingClick(TObject *Sender);
	void __fastcall miSettingsClick(TObject *Sender);
private:	// User declarations
	void Send(void);
	void AddOutputText(AnsiString text);
	void ClearOutput(void);
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
