#ifndef _TASTE_SERIAL_
#define _TASTE_SERIAL_

void taste_puts(const char* s);

[[gnu::format(printf, 1, 2)]]
void taste_printf(const char* fmt, ...);

#endif
