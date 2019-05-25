//---------------------------------------------------------------------------

#ifndef PhoneStateH
#define PhoneStateH

#include <string>

//---------------------------------------------------------------------------
struct PhoneState
{
	bool registered;
	bool paging;
	int callState;
	std::string display;
	bool audioError;
	PhoneState(void):
		registered(false),
		paging(false),
		callState(0),
		audioError(false)
	{}
};

extern PhoneState phoneState;

#endif
