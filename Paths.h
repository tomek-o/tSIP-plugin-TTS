//---------------------------------------------------------------------------

#ifndef PathsH
#define PathsH
//---------------------------------------------------------------------------

#include <Classes.hpp>

namespace Paths
{
	void SetProfileDir(AnsiString dir);

	/** \brief Return profile directory, without trailing backslash
	*/
	AnsiString GetProfileDir(void);
};

#endif
