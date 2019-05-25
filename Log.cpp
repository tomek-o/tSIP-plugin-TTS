/** \file
 *  \brief Global logging unit
 */

/* Copyright (C) 2008-2009 Tomasz Ostrowski <tomasz.o.ostrowski at gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.

 * Miniscope is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <sys\timeb.h>
#include "Log.h"

extern void Log(char* txt);

CLog::CLog()
{
};

void CLog::log(const char *lpData, ...)
{
	va_list ap;
	char buf[1024]; //determines max message length

    int size = 0;

	if ((int)sizeof(buf)-size-2 > 0)
	{
		va_start(ap, lpData);
		size += vsnprintf(buf + size, sizeof(buf)-size-2, lpData, ap);
		va_end(ap);
	}
	if (size > (int)sizeof(buf) - 2)
		size = sizeof(buf) - 2;
	buf[size] = '\n';
	buf[size+1] = 0;

	Log(buf);
}

