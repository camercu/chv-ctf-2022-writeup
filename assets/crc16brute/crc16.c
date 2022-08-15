#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// 0xc1c0

uint16_t crc16_update(uint16_t crc, uint8_t a) {
    int i;
    crc ^= a;
    for (i = 0; i < 8; ++i) {
        if (crc & 1)
            crc = (crc >> 1) ^ 0xA001;
        else
            crc = (crc >> 1);
    }
    return crc;
}

uint16_t crc16(uint8_t *buf, size_t len, uint16_t initial) {
    size_t i;
    uint16_t crc = initial;
    for (i = 0; i < len; i++) {
        crc = crc16_update(crc, buf[i]);
    }
    return crc;
}

void print_table(void) {
    uint16_t crc;
    printf("table = { ");
    for (int b = 0; b < 256; b++){
        crc = crc16_update((uint16_t)b << 8, (uint8_t)b);
        printf("0x%04x, ", crc);
    }
    printf(" };\n");
}

int main(void) {
    print_table();
    return 0;
}
