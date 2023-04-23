#include "bigint.h"

/* Atribuição (com extensão) */
void big_val(BigInt res, long val){
    unsigned char* p = (unsigned char*)(&val);
    int i = 0;
    while (i < sizeof(long)){
        *(res+i) = *(p+i);
        i++;      
    }
    unsigned char lastByte = ((0x80&(*(p+7))) == 0 ? 0x00 : 0xFF);
    for (i=0;i<sizeof(long);i++){
        *(res+8+i) = lastByte;
    }
}

/* Operações Aritméticas */

/* res = -a */
void big_comp2(BigInt res, BigInt a){
    int acres = 1;
    int boolAux;
    for (int i = 0; i<NUM_BITS/8; i++){ 
        *(res+i) = ~(*(a+i));
        boolAux = ((*(res+i)) != 0x00);
        if (acres){
            *(res+i) += acres;
            acres = (boolAux && ((*(res+i)) == 0x00));
        }
    }
}


/* res = a + b */
void big_sum(BigInt res, BigInt a, BigInt b){
    int acres = 0;
    for (int i = 0; i < NUM_BITS/8; i++){
        unsigned char v1 = *(a+i);
        unsigned char v2 = *(b+i);
        unsigned int sum = v1+v2;
        *(res+i) = sum + acres;
        acres = (sum+acres > 255); 
    }
}

/* res = a - b */
void big_sub(BigInt res, BigInt a, BigInt b) {
    BigInt btemp;
    big_comp2(btemp, b);
    big_sum(res, a, btemp);
}

void copy_BigInt(BigInt res, BigInt a) {
    for (int i = 0; i < 16; i++) {
        res[i] = a[i];
    }
}

/* res = a * b */
void big_mul(BigInt res, BigInt a, BigInt b) {
    BigInt a_aux;
    BigInt b_aux;
    copy_BigInt(a_aux, a);
    copy_BigInt(b_aux, b);

    long zero = 0;
    big_val(res, zero);

    int bool_a_neg = (a[15] & 0x80);
    int bool_b_neg = (b[15] & 0x80);

    if (bool_a_neg)
        big_comp2(a_aux, a_aux);

    if (bool_b_neg)
        big_comp2(b_aux, b_aux);

    int n, val, acres;
    for (int i = 0; i < 16; i++) {
        acres = 0;
        for (int j = 0; j < 16; j++) {
            val = (a_aux[i] * b_aux[j]) + res[i + j] + acres;
            acres = (int)val / 256;
            n = val - acres * 256;
            res[i + j] = n;
        }
    }

    if ((!(bool_a_neg && bool_b_neg)) && (bool_a_neg || bool_b_neg))
        big_comp2(res, res);
}

/* Operações de Deslocamento */

/* res = a << n */
void big_shl(BigInt res, BigInt a, int n) {
    long zero = 0;
    BigInt aux;
    big_val(aux, zero);
    
    int num_bytes = (int)n / 8;
    int num_bits = n - num_bytes * 8;

    aux[num_bytes] = 1;
    aux[num_bytes] = aux[num_bytes] << num_bits;
    big_mul(res, a, aux);
}

/* res = a >> n (lógico)*/
void big_shr(BigInt res, BigInt a, int n);

/* res = a >> n (aritmético)*/
void big_sar(BigInt res, BigInt a, int n);


