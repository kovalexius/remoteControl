#include "log_to_file.h"
#include <stdarg.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void log_to_file(const char *format, ...)
{
	FILE* logfile;
	static char* logfile_str = 0;
	va_list args;
	char buf[256];
	time_t log_clock;
	/*
	if (!rfbEnableClientLogging)
		return;
		*/
	if (logfile_str == 0) {
		logfile_str = getenv("VNCLOG");
		if (logfile_str == 0)
			logfile_str = "vnc.log";
	}
	logfile = fopen(logfile_str, "a");
	va_start(args, format);
	time(&log_clock);
	strftime(buf, 255, "%d/%m/%Y %X ", localtime(&log_clock));
	fprintf(logfile, buf);
	vfprintf(logfile, format, args);
	fflush(logfile);
	va_end(args);
	fclose(logfile);
}