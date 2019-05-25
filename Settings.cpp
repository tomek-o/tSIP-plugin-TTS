//---------------------------------------------------------------------------


#pragma hdrstop

#include "Settings.h"
#include <algorithm>
#include <fstream>
#include <json/json.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

#pragma link "jsoncpp.lib"

Settings appSettings;

inline void strncpyz(char* dst, const char* src, int dstsize) {
	strncpy(dst, src, dstsize);
	dst[dstsize-1] = '\0';
}

Settings::_scripts::_scripts(void)
{
	/*
	local callState = GetCallState()
	if callState == 0 then
		return
	end

	for i=1, 2000 do
		callState = GetCallState()
		if callState == 6 then	-- CALL_STATE_ESTABLISHED
			SwitchAudioSource("nullaudio", "")
			return
		end
		Sleep(100)
	end
	*/
	onLoad =
		"local callState = GetCallState()\r\n"
		"if callState == 0 then\r\n"
		"\treturn\r\n"
		"end\r\n"
		"\r\n"
		"for i=1, 2000 do\r\n"
		"\tcallState = GetCallState()\r\n"
		"\tif callState == 6 then\t-- CALL_STATE_ESTABLISHED\r\n"
		"\t\tSwitchAudioSource(\"nullaudio\", \"\")\r\n"
		"\t\treturn\r\n"
		"\tend\r\n"
		"\tSleep(100)\r\n"
		"end\r\n"
		;
	onUnload =
		"if callState == 6 then\t-- CALL_STATE_ESTABLISHED\r\n"
		"\tSwitchAudioSource(\"winwave\", \"\")\r\n"
		"end\r\n"
		;
}

void Settings::SetDefault(void)
{
	frmMain.iWidth = 350;
	frmMain.iHeight = 300;
	frmMain.iPosX = 30;
	frmMain.iPosY = 30;
	frmMain.bWindowMaximized = false;
	frmMain.bAlwaysOnTop = false;
}

int Settings::Read(AnsiString asFileName)
{
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;

    SetDefault();

	try
	{
		std::ifstream ifs(asFileName.c_str());
		std::string strConfig((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();
		bool parsingSuccessful = reader.parse( strConfig, root );
		if ( !parsingSuccessful )
		{
			return 2;
		}
	}
	catch(...)
	{
		return 1;
	}

	int maxX = GetSystemMetrics(SM_CXSCREEN);
	/** \todo Ugly fixed taskbar margin */
	int maxY = GetSystemMetrics(SM_CYSCREEN) - 32;

	const Json::Value &frmMainJson = root["frmMain"];
	frmMain.iWidth = frmMainJson.get("AppWidth", 350).asInt();
	frmMain.iHeight = frmMainJson.get("AppHeight", 300).asInt();
	if (frmMain.iWidth < 250 || frmMain.iWidth > maxX + 20)
	{
		frmMain.iWidth = 250;
	}
	if (frmMain.iHeight < 200 || frmMain.iHeight > maxY + 20)
	{
		frmMain.iHeight = 200;
	}
	frmMain.iPosX = frmMainJson.get("AppPositionX", 30).asInt();
	frmMain.iPosY = frmMainJson.get("AppPositionY", 30).asInt();
	if (frmMain.iPosX < 0)
		frmMain.iPosX = 0;
	if (frmMain.iPosY < 0)
		frmMain.iPosY = 0;
	if (frmMain.iPosX + frmMain.iWidth > maxX)
		frmMain.iPosX = maxX - frmMain.iWidth;
	if (frmMain.iPosY + frmMain.iHeight > maxY)
		frmMain.iPosY = maxY - frmMain.iHeight;
	frmMain.bWindowMaximized = frmMainJson.get("Maximized", false).asBool();
	frmMain.bAlwaysOnTop = frmMainJson.get("AlwaysOnTop", false).asBool();

	{
		const Json::Value &jv = root["Tts"];
		Tts.bSendOnEnter = jv.get("SendOnEnter", Tts.bSendOnEnter).asBool();
	}

	{
		const Json::Value &jv = root["Scripts"];
		Scripts.onLoad = jv.get("OnLoad", Scripts.onLoad.c_str()).asString().c_str();
		Scripts.onUnload = jv.get("OnUnload", Scripts.onUnload.c_str()).asString().c_str();
	}

	return 0;
}

int Settings::Write(AnsiString asFileName)
{
	Json::Value root;
	Json::StyledWriter writer;

	root["frmMain"]["AppWidth"] = frmMain.iWidth;
	root["frmMain"]["AppHeight"] = frmMain.iHeight;
	root["frmMain"]["AppPositionX"] = frmMain.iPosX;
	root["frmMain"]["AppPositionY"] = frmMain.iPosY;
	root["frmMain"]["Maximized"] = frmMain.bWindowMaximized;
	root["frmMain"]["AlwaysOnTop"] = frmMain.bAlwaysOnTop;

	root["Tts"]["SendOnEnter"] = Tts.bSendOnEnter;

	{
		Json::Value &jv = root["Scripts"];
		jv["OnLoad"] = Scripts.onLoad.c_str();
		jv["OnUnload"] = Scripts.onUnload.c_str();
	}

	std::string outputConfig = writer.write( root );

	try
	{
		std::ofstream ofs(asFileName.c_str());
		ofs << outputConfig;
		ofs.close();
	}
	catch(...)
	{
    	return 1;
	}

	return 0;
}


