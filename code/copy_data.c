#include <stdint.h>

extern uint32_t _sidata, _sdata, _edata;

void _copy_data(void) {
	uint32_t* src = &_sidata;
	uint32_t* dst = &_sdata;
	while (dst < &_edata) {
		*dst++ = *src++;
	}
}
