#include "fdt.h"
#include "string.h"
#include <stddef.h>
#include <stdbool.h>

#define end32(data)   \
 ((((data) >> 24) & 0x000000FF) | (((data) >>  8) & 0x0000FF00) | \
  (((data) <<  8) & 0x00FF0000) | (((data) << 24) & 0xFF000000) )

#define end64(data)   \
( (((data) >> 56) & 0x00000000000000FF) | (((data) >> 40) & 0x000000000000FF00) | \
  (((data) >> 24) & 0x0000000000FF0000) | (((data) >>  8) & 0x00000000FF000000) | \
  (((data) <<  8) & 0x000000FF00000000) | (((data) << 24) & 0x0000FF0000000000) | \
  (((data) << 40) & 0x00FF000000000000) | (((data) << 56) & 0xFF00000000000000) ) 

#define ALIGN(x,a)              __ALIGN_MASK(x,(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))

void* get_entry(void* dtb, const char* compatible) {
    struct fdt_header* hdr = (struct fdt_header*)dtb;
    if (end32(hdr->magic) != FDT_MAGIC) {
        return 0;
    }
    uint32_t* sb = (uint32_t*)((uintptr_t)dtb + end32(hdr->off_dt_struct));
    char* strings = (char*)((uintptr_t)dtb + end32(hdr->off_dt_strings));

    while (end32(*sb) != FDT_END) {
        switch (end32(*sb)) {
            case FDT_BEGIN_NODE: {
                void* ret = sb;
                char* name = (char*)(sb + 1);

                char c;
                while ((c = *name++)) {}

                sb = (uint32_t*)(ALIGN((uintptr_t)name, 4));

                while (end32(*sb) != FDT_END_NODE) {
                    switch (end32(*sb)) {
                        case FDT_PROP : {
                            sb++;
                            uint32_t len = end32(*sb);
                            sb++;
                            uint32_t nameoff = end32(*sb);

                            char* name = &strings[nameoff];

                            if(!strcmp(name, "compatible")) {
                                char* n = (char*)(sb + 1);

                                if (!strcmp(n, compatible)) {
                                    return ret;
                                }

                            }

                            while ((c = *name++)) {}

                            sb = (uint32_t*) ((uintptr_t)sb + len);

                            sb = (uint32_t*)(ALIGN((uintptr_t)sb, 4));
                            break;
                        }
                        case FDT_BEGIN_NODE : {
                            goto out;
                        }
                    }
                    sb++;
                }
                out:
                break;
            }

            default: {
                sb++;
            }
        }
    }

    return 0;
}

void* get_address(void* elem) {
    uint32_t* sb = (uint32_t*)((uintptr_t)elem);
    char* name = (char*)(sb + 1);

    size_t len = strlen(name);
    size_t atloc = 0;
    for (size_t i = 0; i < len; i++) {
        if (name[i] == '@') {
            atloc = i;
        }
    }
    if (!atloc) {
        return (void*)0;
    }

    return (void*)strtoul(name + atloc + 1, 16);
}
