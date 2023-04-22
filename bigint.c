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
    big_sum(res, a, b);
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


