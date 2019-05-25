//---------------------------------------------------------------------------


#pragma hdrstop

#include "T2S.h"
#include "Log.h"
//#include "sapi_simple.h"
#define CINTERFACE
#define COBJMACROS
#include <sapi.h>
#include <stdio.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

using namespace T2S;

#define PROMPT "T2S "

// GUIDs
const GUID CLSID_SpVoice = {0x96749377, 0x3391, 0x11d2, 0x9e,0xe3, 0x00,0xc0,0x4f,0x79,0x73,0x96};
const GUID IID_ISpVoice = {0x6c44df74, 0x72b9, 0x4992, 0xa1,0xec, 0xef,0x99,0x6e,0x04,0x22,0xd4};
const GUID IID_ISpeechFileStream = {0xaf67f125, 0xab39, 0x4e93, 0xb4,0xa2, 0xcc,0x2e,0x66,0xe1,0x82,0xa7};
const GUID CLSID_SpFileStream = {0x947812b3, 0x2ae1, 0x4644, 0xba,0x86, 0x9e,0x90,0xde,0xd7,0xec,0x91};

namespace
{

ISpVoice* pVoice = NULL;

// disabling Floating point divide by zero exception
// https://stackoverflow.com/questions/15259361/speech-api-sapi-floating-point-division-by-zero-in-c-builder-on-windows-7
struct s8087CW
{
    Word Saved8087CW;

    s8087CW(Word NewCW)
    {
        Saved8087CW = Default8087CW;
        Set8087CW(NewCW);
        // alternatively, the VCL documentation says to use SetExceptionMask() instead of Set8087CW() directly...
    }

    ~s8087CW()
    {
        Set8087CW(Saved8087CW);
    }
};

}	// namespace

int T2S::Init(void)
{
	// Initialize COM
	//if(CoInitializeEx(NULL, COINIT_MULTITHREADED)) {
	//	LOG(PROMPT"Couldn't initialize COM.\n");
	//	return 1;
	//}
	::CoInitialize(NULL);

	// Create SAPI voice
	if(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice)){
		LOG(PROMPT"Couldn't create voice.\n");
		//CoUninitialize();
		return 1;
	}
	//pVoice->lpVtbl->SetRate(0);		// 0...20
	//pVoice->lpVtbl->SetVolume(100);	// 0...100
	return 0;
}

void T2S::Shutdown(void)
{
    if (pVoice) {
        // Release voice
        ISpVoice_Release(pVoice);
		//pVoice->ISpVoiceVtbl->Release(pVoice);
		LOG(PROMPT"Voice released.");
		pVoice = NULL;
	}
	// Finish COM
	::CoUninitialize();
	LOG(PROMPT"COM uninitialized.");	
}

int T2S::WriteToFile(AnsiString filename, AnsiString text)
{
	s8087CW cw(0x133f);
	
	std::wstring widestr;
	widestr = std::wstring(text.c_str(), text.c_str() + text.Length());

	wchar_t wtext[4096];
	memset(wtext, 0, sizeof(wtext));
	// Comma then a space a few times should insert bit of silence
	snwprintf(wtext, sizeof(wtext)/sizeof(wtext[0]) - 1, L"%s", widestr.c_str());

#if 0
	int rc = pVoice->lpVtbl->Speak(pVoice, wtext, /*SPF_ASYNC|*/SPF_DEFAULT, NULL);
	if (rc) {
		LOG("Couldn't speak with voice, rc = %d = 0x%08X.\n", rc, rc);
		return 1;
	}
#else
	//SpeechVoiceSpeakFlags spFlags = SpeechVoiceSpeakFlags::SVSFlagsAsync;
	SpeechStreamFileMode spFileMode = SSFMCreateForWrite;
	ISpeechFileStream *fileStream = NULL;
	if (::CoCreateInstance(CLSID_SpFileStream, NULL, CLSCTX_ALL, IID_ISpeechFileStream, (void **)&fileStream) == S_OK)
	{
		std::wstring wfilename;
		wfilename = std::wstring(filename.c_str(), filename.c_str() + filename.Length());
		wchar_t filenameBuf[1024];
		memset(filenameBuf, 0, sizeof(filenameBuf));
		snwprintf(filenameBuf, sizeof(filenameBuf)/sizeof(filenameBuf[0]) - 1, L"%s", wfilename.c_str());

		fileStream->lpVtbl->Open(fileStream, filenameBuf, spFileMode, false);
		pVoice->lpVtbl->SetOutput(pVoice, (IUnknown*)fileStream, false);
		pVoice->lpVtbl->Speak(pVoice, wtext, SPF_DEFAULT, NULL);
		//pVoice->WaitUntilDone(1000);
		fileStream->lpVtbl->Close(fileStream);
	}
	else
	{
		LOG("Could not create wav file\n");
		return 2;
	}
#endif

	return 0;
}
