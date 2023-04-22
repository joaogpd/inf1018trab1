#include "bigint.h"

/* Atribuição (com extensão) */
void big_val (BigInt res, long val) {
    unsigned char* val1 = (unsigned char*)&val;
    for (int i = 0; i < 8; i++) {
        res[i] = *(val1 + i);
    }
    if (*(val1 + 7) & 0x80) {
        for (int i = 8; i < 16; i++) 
            res[i] = 0xFF;
    } else {
        for (int i = 8; i < 16; i++) 
            res[i] = 0x00;
    }
}

/* Operações Aritméticas */

/* res = -a */
void big_comp2(BigInt res, BigInt a) {
    for (int i = 0; i < 16; i++) 
        res[i] = ~(a[i]);
    int temp, excesso = 0;
    temp = res[0] + 1;
    for (int j = 0; j < 16; j++) {
        temp += excesso;
        if (temp > 0xFF) excesso = temp - 0xFF;
        else res[j] = temp - excesso;
        if (excesso == 0) break;
    }
}

/* res = a + b */
void big_sum(BigInt res, BigInt a, BigInt b) {
    int temp, excesso = 0;
    for (int i = 0; i < 16; i++) {
        temp = a[i] + b[i] + excesso;    
        excesso = ((temp << 8) & 0xFF) + ((temp > 0xFF) ? 1 : 0);
        res[i] = temp & 0xFF;
    }
}

/* res = a - b */
void big_sub(BigInt res, BigInt a, BigInt b) {
    BigInt btemp;
    big_comp2(btemp, b);
    big_sum(res, a, btemp);
}

/* res = a * b */
void big_mul(BigInt res, BigInt a, BigInt b);

/* Operações de Deslocamento */

/* res = a << n */
void big_shl(BigInt res, BigInt a, int n);

/* res = a >> n (lógico)*/
void big_shr(BigInt res, BigInt a, int n);

/* res = a >> n (aritmético)*/
void big_sar(BigInt res, BigInt a, int n);


