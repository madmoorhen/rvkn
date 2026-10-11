/* Includes */
#include <utils/base.h>
#include <utils/printf.h>

/* UART */
volatile unsigned char *uart = (unsigned char *)0x10000000;
void uart_putc(char c) { *uart = c; }

/* From linker */
extern u8_t __kern_start, __kern_end;
 
/* Entry point */
void kmain(u64_t mhartid, u64_t misa, void *dtb) {
  u64_t stimecmp = 0;
  ASM( "csrr %0, stimecmp" : "=r"(stimecmp) );

  /* Test supervisor mode */
  u64_t sstatus = 0;
  ASM( "csrr %0, sstatus" : "=r"(sstatus) );
  printf(uart_putc, "sstatus = %#0lx\r\n", sstatus);
  printf(uart_putc, "Not in user mode - good\r\n");

  printf(
      uart_putc, "rv%s%c with extensions: ",
      ((misa>>30) & 0x3) == 1 ? "32" : "64",
      (misa & 0x10) ? 'e' : 'i'
  );
  for (u64_t i = 0; i < 26; i++) if ((misa >> i)&1) uart_putc('A'+i);
  printf(uart_putc, "\r\nmhartid = %lu\r\n", mhartid);
  printf(uart_putc, "stimecmp = %lu\r\n", stimecmp);
  printf(uart_putc, "dtb = %p\r\n", dtb);
  printf(uart_putc, "kernel start: %p\r\n", &__kern_start);
  printf(uart_putc, "kernel end: %p\r\n", &__kern_end);

	while(1) { uart_putc(*uart); }
  __builtin_unreachable();
}
