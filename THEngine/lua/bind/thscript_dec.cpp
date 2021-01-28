#include "thscript_dec.h"

#ifdef _DEBUG
#include <algorithm>
#include <stdarg.h>

#ifdef _WIN32
#include <io.h>
#include <WS2tcpip.h>
#include <Winsock2.h>
#endif


void __print(const char * format, ...)
{
	char buf[10240] = {0};
	va_list args;
	va_start(args, format);
	auto nret = vsnprintf(buf, 10240 - 3, format, args);
	va_end(args);
	buf[strlen(buf)] = '\n';
#ifdef _WIN32
	OutputDebugString(buf);
#endif
}
#endif
