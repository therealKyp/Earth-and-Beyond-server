/*
 * WinCompatibility.h
 *
 *  Created on: Jan 6, 2009
 *      Author: agfot
 */

#ifndef WINCOMPATIBILITY_H_
#define WINCOMPATIBILITY_H_

#ifndef WIN32
#ifndef _snprintf
#define _snprintf snprintf
#endif
#ifndef _alloca
#define _alloca alloca
#endif
#ifndef _sleep
#define _sleep sleep
#endif
#ifndef _atoi64
#define _atoi64 int64_t
#endif
#ifndef _strcmpi
#define _strcmpi stricmp
#endif
#ifndef _stricmp
#define _stricmp stricmp
#endif
#ifndef _isnan
#define _isnan isnan
#endif
#define GetLastError() (errno)
#endif


#endif /* WINCOMPATIBILITY_H_ */
