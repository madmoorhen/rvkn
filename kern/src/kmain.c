/* Includes */
#include <utils/base.h>
#include <utils/log.h>
#include <devicetree.h>

/* UART */
static volatile unsigned char *uart = (unsigned char *)0x10000000;
static void uart_putc(char c) { if (c) *uart = c; }

/* From linker */
extern u8_t __kern_start, __kern_end;
 
/* Entry point */
void kmain(u64_t mhartid, u64_t misa, void *dtb) {
  /* No multiprocessor support - spin all other harts */
  if (mhartid) { while (1); }

  /* Set logging target */
  log_target_set(LOG_TARGET_UART);

  u64_t stimecmp = 0;
  ASM( "csrr %0, stimecmp" : "=r"(stimecmp) );

  /* Test supervisor mode */
  u64_t sstatus = 0;
  ASM( "csrr %0, sstatus" : "=r"(sstatus) );
  log_msg(LOG_LEVEL_INFO, "sstatus = %#0lx", sstatus);
  log_msg(LOG_LEVEL_INFO, "Not in user mode - good");

  log_msg(
      LOG_LEVEL_INFO, "rv%s%c with extensions: ",
      ((misa>>30) & 0x3) == 1 ? "32" : "64",
      (misa & 0x10) ? 'e' : 'i'
  );
  char exts[27]; exts[26] = 0;
  for (u64_t i = 0; i < 26; i++) exts[i] = '-';
  for (u64_t i = 0; i < 26; i++) if ((misa >> i)&1) exts[i] = ('A'+i);
  log_msg(LOG_LEVEL_INFO, "%s", exts);

  log_msg(LOG_LEVEL_INFO, "nmhartid = %lu", mhartid);
  log_msg(LOG_LEVEL_INFO, "stimecmp = %lu", stimecmp);
  log_msg(LOG_LEVEL_INFO, "dtb = %p", dtb);
  log_msg(LOG_LEVEL_INFO, "kernel start: %p", &__kern_start);
  log_msg(LOG_LEVEL_INFO, "kernel end: %p", &__kern_end);

  log_devicetree(dtb);

  volatile u16_t *poweroff = (u16_t *)0x100000;
  *poweroff = ENDIAN16(0x5555);

	//while(1) { uart_putc(*uart); }
  __builtin_unreachable();
}
