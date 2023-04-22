#include <stdio.h>
#include <limits.h>
#include "bigint.h"

void dump(void* p, int n) {
    unsigned char* p1 = (unsigned char*)p;
    while (n--) {
        printf("%p - %02x\n", p1, *p1);
        p1++;
    }
}

void compare_big(BigInt a, BigInt b) {
    for (int i = 0; i < 16; i++) {
        if (a[i] != b[i]) {
            printf("FAILED\n");
            return;
        }
    }
    printf("SUCCESS\n");
}

void compare_long(BigInt a, void* b) {
    unsigned char* b1 = (unsigned char*)b;
    for (int i = 0; i < 8; i++) {
        if (a[i] != *(b1 + i)) {
            printf("FAILED\n");
            return;
        }
    }
    if (*(b1 + 7) & 0x80) {
        for (int i = 8; i < 16; i++) {
            if (a[i] != 0xFF) {
                printf("FAILED\n");
                return;
            }
        }
    } else {
        for (int i = 8; i < 16; i++) {
            if (a[i] != 0x00) {
                printf("FAILED\n");
                return;
            }
        }
    }
    printf("SUCCESS\n");
}

void teste_big_val(BigInt a, long b) {
    big_val(a, b);
    compare_long(a, &b);
}

void teste_big_comp2(BigInt a, BigInt b, BigInt c, long d) {
    big_val(a, d);
    if (d == LONG_MIN) {
        BigInt temp1, temp2, temp3;
        big_val(temp2, -(d + 1));
        big_val(temp1, 1);
        big_sum(temp3, temp2, temp1);
        big_comp2(c, a); 
        compare_big(temp3, c);
        return;
    }
    big_val(b, -d);
    big_comp2(c, a);
    compare_big(c, b);
}


void teste_big_sum(BigInt a, BigInt b, BigInt c, long d1, long d2) {
    big_val(a, d1);
    big_val(b, d2);
    big_sum(c, a, b);
    big_val(a, d1 + d2);
    compare_big(a, c);
}

void teste_big_sub(BigInt a, BigInt b, BigInt c, long d1, long d2) {
    big_val(a, d1);
    big_val(a, d2);
    big_sub(c, a, b);
    big_val(a, d1 - d2);
    compare_big(a, c);
}

int main(void) {
    // Testagem big_val()
    printf("big_val()\n");
    BigInt a;
    teste_big_val(a, -87635434);
    teste_big_val(a, 534534354);
    teste_big_val(a, -23);
    teste_big_val(a, 32);
    teste_big_val(a, LONG_MAX);
    teste_big_val(a, LONG_MIN);
    teste_big_val(a, 0);

    // Testagem big_comp2()
    printf("big_comp2()\n");
    BigInt b, c;
    teste_big_comp2(a, b, c, -422345254);
    teste_big_comp2(a, b, c, 534647343);
    teste_big_comp2(a, b, c, -34);
    teste_big_comp2(a, b, c, 45);
    teste_big_comp2(a, b, c, LONG_MAX);
    teste_big_comp2(a, b, c, LONG_MIN);
    teste_big_comp2(a, b, c, 0);

    // Testagem big_sum()
    printf("big_sum()\n");
    teste_big_sum(a, b, c, 324234253, 345436254);
    teste_big_sum(a, b, c, -657242423, -123457568);
    teste_big_sum(a, b, c, LONG_MAX, LONG_MAX);
    printf("\n");
    dump(c, 16);
    teste_big_sum(a, b, c, LONG_MIN, LONG_MIN);
    printf("\n");
    dump(c, 16);
    teste_big_sum(a, b, c, 435434, -23423512); // subtract
    
    // Testagem big_sub()
    printf("big_sub()\n");
    teste_big_sub(a, b, c, 1233423, 3453463);
    teste_big_sub(a, b, c, 2342234, 234);
    teste_big_sub(a, b, c, LONG_MAX, LONG_MAX);


    return 0;
}
