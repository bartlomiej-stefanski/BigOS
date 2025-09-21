#define STB_SPRINTF_DECORATE(name) name
#define STB_SPRINTF_NOFLOAT
#include <stb_sprintf.h>

static volatile unsigned char* serial = (volatile unsigned char*)TASTE_SERIAL;

static void taste_putc(char c) {
	*serial = c;
}

void taste_puts(const char* s) {
	while (*s) taste_putc(*s++);
}

static char* serial_output_handler(const char* buf, void* user, int len) {
	while (len--) taste_putc(*buf++);
	return (char*)user;
}

void taste_printf(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);
	char buf[STB_SPRINTF_MIN];
	vsprintfcb(serial_output_handler, buf, buf, fmt, va);
	va_end(va);
}
