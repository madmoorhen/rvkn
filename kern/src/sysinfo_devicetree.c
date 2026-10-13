/* Implements sysinfo/devicetree.h */
#include <sysinfo/devicetree.h>

/* Device tree blob pointer */
static void *dtb = NULL;

/* Load device tree */
void devicetree_load(void *p) { dtb = p; }

/* TODO */
