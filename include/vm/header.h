#ifndef C_PLUS_HEADER_H_
#define C_PLUS_HEADER_H_

#define CPLUS_VM_MAGIC_HEADER 0x43504C5553
#define CPLUS_VM_MAGIC_HEADER_STR "\x43\x50\x4C\x55\x53"
#define CPLUS_VM_MAGIC_NUMBER_SIZE 5
#define CPLUS_VM_STANDARD_VERSION 1

#include <stdint.h>

typedef struct {
    uint8_t magic[CPLUS_VM_MAGIC_NUMBER_SIZE];
    uint8_t version;
    uint32_t file_size;
    uint32_t instruction_count;
} CPlusHeader;

#endif /* C_PLUS_HEADER_H_ */
