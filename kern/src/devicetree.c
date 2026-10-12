/* Implements devicetree.h */
#include <devicetree.h>

/* Includes */
#include <utils/printf.h>

/* UART */
static volatile unsigned char *uart = (unsigned char *)0x10000000;
static void uart_putc(char c) { if (c) *uart = c; }

/* Flattened Device Tree token types */
#define FDT_BEGIN_NODE  ((u32_t)0x00000001)
#define FDT_END_NODE    ((u32_t)0x00000002)
#define FDT_PROP        ((u32_t)0x00000003)
#define FDT_NOP         ((u32_t)0x00000004)
#define FDT_END         ((u32_t)0x00000009)

/* Flattened Device Tree Header */
typedef struct __attribute__((packed)) {
  u32_t magic;
  u32_t totalsize;
  u32_t off_dt_struct;
  u32_t off_dt_strings;
  u32_t off_mem_rsvmap;
  u32_t version;
  u32_t last_comp_version;
  u32_t boot_cpuid_phys;
  u32_t size_dt_strings;
  u32_t size_dt_struct;
} fdt_header_t;

/* Flattened Device Tree Reserved memory Entry */
typedef struct __attribute__((packed)) {
  u64_t address;
  u64_t size;
} fdt_reserve_entry_t;

/* Flattened Device Tree Property */
typedef struct __attribute__((packed)) {
  u32_t len;
  u32_t nameoff;
} fdt_property_t;

/* Log device tree to UART (TODO: remove) */
void log_devicetree(void *dtb) {
  /* Header */
  fdt_header_t *fdt_header = (fdt_header_t *)dtb;
  printf(
      uart_putc,
      "fdt_header.magic = %#08x (%s)\r\n"
      "fdt_header.totalsize = %#08x\r\n"
      "fdt_header.off_dt_struct = %#08x\r\n"
      "fdt_header.off_dt_strings = %#08x\r\n"
      "fdt_header.off_mem_rsvmap = %#08x\r\n"
      "fdt_header.version = %#08x\r\n"
      "fdt_header.last_comp_version = %#08x\r\n"
      "fdt_header.boot_cpuid_phys = %#08x\r\n"
      "fdt_header.size_dt_strings = %#08x\r\n"
      "fdt_header.size_dt_struct = %#08x\r\n",
      ENDIAN32(fdt_header->magic),
      ENDIAN32(fdt_header->magic) == 0xd00dfeed ? "valid" : "invalid",
      ENDIAN32(fdt_header->totalsize),
      ENDIAN32(fdt_header->off_dt_struct),
      ENDIAN32(fdt_header->off_dt_strings),
      ENDIAN32(fdt_header->off_mem_rsvmap),
      ENDIAN32(fdt_header->version),
      ENDIAN32(fdt_header->last_comp_version),
      ENDIAN32(fdt_header->boot_cpuid_phys),
      ENDIAN32(fdt_header->size_dt_strings),
      ENDIAN32(fdt_header->size_dt_struct)
  );

  /* Reserved memory entries */
  fdt_reserve_entry_t *e = (fdt_reserve_entry_t *)(
      (u8_t *)dtb + ENDIAN32(fdt_header->off_mem_rsvmap)
  );
  while (1) {
    if (e->address == 0 && e->size == 0) break;
    printf(
        uart_putc,
        "reserved memory entry:\r\n\taddr = %p\r\n\tsize = %#016lx\r\n",
        (void *)(ENDIAN64(e->address)), ENDIAN64(e->size)
    );
    e++;
  }

  /* Actual device tree */
  u32_t *cur = (u32_t *)((u8_t *)dtb + ENDIAN32(fdt_header->off_dt_struct));
  u64_t level = 0;
  bool parsing = true;
  while (parsing) {
    switch (ENDIAN32(*cur)) {
      case FDT_BEGIN_NODE:
        cur++;
        for (u64_t i = 0; i < level; i++) uart_putc('\t');
        level++;
        printf(uart_putc, "%s {\r\n", (char *)cur);
        while ((*cur)&0xff) { cur++; }
        break;
      case FDT_END_NODE:
        cur++;
        if (level > 0) level--;
        else printf(uart_putc, "Level below zero!!!\r\n");
        for (u64_t i = 0; i < level; i++) uart_putc('\t');
        printf(uart_putc, "}\r\n");
        break;
      case FDT_PROP: {
        cur++;
        fdt_property_t *prop = (fdt_property_t *)cur;
        cur += 2; // sizeof(fdt_property_t) in u32s, padded to nearest
        char *name = (char *)dtb
            + ENDIAN32(fdt_header->off_dt_strings) + ENDIAN32(prop->nameoff);
        for (u64_t i = 0; i < level; i++) uart_putc('\t');
        printf(uart_putc, "%s =", name);
        for (u64_t i = 0; i < ENDIAN32(prop->len); i++) {
          printf(uart_putc, " %#02x", ((*cur)>>((i%4)*8))&0xff);
          if ((i+1)%4 == 0) cur++;
        } if (ENDIAN32(prop->len)%4) cur++;
        printf(uart_putc, "\r\n");
      } break;
      case FDT_END:
        //for (u64_t i = 0; i < level; i++) uart_putc('\t');
        //printf(uart_putc, "FDT_END\r\n");
        parsing = false;
        break;
      case FDT_NOP:
        cur++;
        //for (u64_t i = 0; i < level; i++) uart_putc('\t');
        //printf(uart_putc, "FDT_NOP\r\n");
        break;
      default:
        cur++;
        //for (u64_t i = 0; i < level; i++) uart_putc('\t');
        //printf(uart_putc, "Unrecognised token\r\n");
        break;
    }
  }
}
