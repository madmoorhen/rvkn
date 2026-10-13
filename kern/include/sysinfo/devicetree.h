/* Include guard */
#ifndef SYSINFO_DEVICETREE_H
#define SYSINFO_DEVICETREE_H

/* Includes */
#include <utils/base.h>

/* Device tree node handle */
typedef devicetree_node_handle_t
const devicetree_node_handle_t DEVICETREE_ROOT_NODE = 0;

/* Device tree property */
typedef struct {
  const char *name;
  u64_t size;
  void *data;
} devicetree_property_t;

/* Device tree string list */
typedef struct {
  const char *first;
  u64_t num_strings;
} devicetree_stringlist_t;

/* Load device tree */
extern void devicetree_load(void *dtb);

/* Find a node with a specific name */
extern devicetree_node_handle_t devicetree_find_node_name(const char *name);
/* Find a node with a specific phandle */
extern devicetree_node_handle_t devicetree_find_node_phandle(u32_t phandle);

/* Get a subnode from a node given its name */
extern devicetree_node_handle_t devicetree_node_get_node_name(
    devicetree_node_handle_t node, const char *name
);
/* Get a subnode from a node given its phandle */
extern devicetree_node_handle_t devicetree_node_get_node_name(
    devicetree_node_handle_t node, u32_t phandle
);
/* Get the number of properties in a node */
extern u64_t devicetree_node_get_num_properties(devicetree_node_handle_t node);
/* Get a property from a node given its name */
extern devicetree_property_t devicetree_node_get_property_name(
    devicetree_node_handle_t node, const char *name
);
/* Get a property from a node given its index */
extern devicetree_property_t devicetree_node_get_property_index(
    devicetree_node_handle_t node, const char *name
);

/* Get the u32 from a u32 property */
extern u32_t devicetree_property_get_u32(devicetree_property_t property);
/* Get the u64 from a u64 property */
extern u64_t devicetree_property_get_u64(devicetree_property_t property);
/* Get a string from a string property */
extern const char *devicetree_property_get_string(
    devicetree_property_t property
);
/* Get a stringlist from a stringlist property */
extern devicetree_stringlist_t devicetree_property_get_stringlist(
    devicetree_property_t property
);

#endif /* SYSINFO_DEVICETREE_H */
