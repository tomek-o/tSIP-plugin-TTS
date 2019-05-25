/** \file
*/
//---------------------------------------------------------------------------

#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
#include <System.hpp>
#include <deque>

class Settings
{
public:
	int Read(AnsiString asFileName);
	int Write(AnsiString asFileName);
	void SetDefault(void);
	struct _gui
	{
		enum { SCALING_MIN = 50 };
		enum { SCALING_MAX = 200 };
		int scalingPct;					///< scaling (percentage)
	} gui;
	struct _frmMain
	{
		int iPosX, iPosY;				///< main window coordinates
		int iHeight, iWidth;			///< main window size
		bool bWindowMaximized;			///< is main window maximize?
		bool bAlwaysOnTop;
	} frmMain;
	struct _tts
	{
		bool bSendOnEnter;
		_tts(void):
			bSendOnEnter(true)
		{}
	} Tts;
	struct _scripts
	{
		AnsiString onLoad;
		AnsiString onUnload;
		_scripts(void);
	} Scripts;
};

extern Settings appSettings;

#endif
