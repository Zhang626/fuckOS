#include <types.h>
#include <string.h>
#include <fuckOS/tty.h>

#include <mm/layout.h>

console_t console[NUMCON];
static void vga_graphic_putc(uint32_t c);
static void vga_text_putc(uint32_t c);

void console_init()
{
	int i = 0;
	//for(i;i<NUMCON;i++) {
	//}
	console[NUMCON].display_addr = (uint32_t*)0xB8000;
	console[NUMCON].original_start_addr = (uint32_t*)0xB8000;
	console[NUMCON].current_start_addr = (uint32_t*)0xB8000;
	console[NUMCON].cursor = 0;
	console_clear();
}

void console_write_char(uint32_t c)
{
	vga_text_putc(c);
	
}

static void vga_graphic_putc(uint32_t c)
{
	
}


#define CRT_ROWS	25
#define CRT_COLS	80
#define CRT_SIZE	(CRT_ROWS * CRT_COLS)
#define crt_pos 	(console[NUMCON].cursor)

#define crt_buf  	((uint16_t *)console[NUMCON].display_addr)

void console_clear()
{
	int i = 0;
	for(i;i<CRT_SIZE;i++)
		crt_buf[i] = 0;
}


static void vga_text_putc(uint32_t c)
{
	
	

	// if no attribute given, then use black on white
	if (!(c & ~0xFF))
		c |= 0x0700;
	
	switch (c & 0xff) {
	case '\b':
		if (crt_pos > 0) {
			crt_pos--;
			crt_buf[crt_pos] = (c & ~0xff) | ' ';
		}
		break;
	case '\n':
		crt_pos += CRT_COLS;
		// fallthru 
	case '\r':
		crt_pos -= (crt_pos % CRT_COLS);
		break;
	case '\t':
		vga_text_putc(' ');
		vga_text_putc(' ');
		vga_text_putc(' ');
		vga_text_putc(' ');
		vga_text_putc(' ');
		break;
	default:
		crt_buf[crt_pos++] = c;		// write the character 
		break;
	}

	// What is the purpose of this?
	if (crt_pos >= CRT_SIZE) {
		int i;

		memmove(crt_buf, crt_buf + CRT_COLS, (CRT_SIZE - CRT_COLS) * sizeof(uint16_t));
		for (i = CRT_SIZE - CRT_COLS; i < CRT_SIZE; i++)
			crt_buf[i] = 0x0700 | ' ';
		crt_pos -= CRT_COLS;
	}

}


