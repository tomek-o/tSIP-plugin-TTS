//---------------------------------------------------------------------------

#ifndef T2SH
#define T2SH
//---------------------------------------------------------------------------

#include <System.hpp>

namespace T2S {

int Init(void);
int WriteToFile(AnsiString filename, AnsiString text);
void Shutdown(void);

};

#endif
