#include <stdio.h>
#include "bigint.h"

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

void big_sub(BigInt res, BigInt a, BigInt b){
    BigInt c;
    big_comp2(c,b);
    big_sum(res,a,c);
}

void copyBigInt(BigInt res, BigInt a){
    for(int i = 0;i<NUM_BITS/8;i++){
        *(res+i) = *(a+i);   
    }
}

void big_mul(BigInt res, BigInt a, BigInt b){
    BigInt aAux;
    BigInt bAux;
    copyBigInt(aAux, a);
    copyBigInt(bAux, b);
    
    long zero = 0;
    big_val(res,zero);
    
    int boolANeg = (*(a+NUM_BITS/8-1))&(0x80);
    int boolBNeg = (*(b+NUM_BITS/8-1))&(0X80);

    if (boolANeg)
        big_comp2(aAux, aAux);

    if (boolBNeg)
        big_comp2(bAux, bAux);
    
    int n;
    int val;
    int acres;
    for (int i=0; i<NUM_BITS/8; i++){
        acres = 0;
        for (int j=0; j<NUM_BITS/8-i; j++){
            val = (*(aAux+i)) * (*(bAux+j)) + *(res+i+j) + acres;
            acres = (int)val/256;
            n = val - acres*256;
            *(res+i+j) = n; 
        }
    }

    if ((!(boolANeg && boolBNeg)) && (boolANeg || boolBNeg))
        big_comp2(res,res);
}

void big_shl(BigInt res, BigInt a, int n){
    long zero = 0;
    BigInt aux;
    big_val(aux,zero);

    int numBytes = (int)n/8;
    int numBits = n - numBytes*8;

    *(aux+numBytes) = 1;
    *(aux+numBytes) = *(aux+numBytes) << numBits;
    big_mul(res,a,aux);
}

void big_shr(BigInt res, BigInt a, int n){
    BigInt aux;
    long zero = 0;
    big_val(aux,zero);
    int numBytes = (int)n/8;
    int numBits = n-numBytes*8;

    unsigned char byteAux = 0;
    unsigned char secondByteAux = 0xFF;
    for (int i = 0; i<numBits; i++){
        byteAux *= 2;
        byteAux += 1;
    }
    secondByteAux = secondByteAux<<(8-numBits);
    secondByteAux = ~secondByteAux;

    unsigned char bitsPerdidos = 0;
    unsigned char bitsAux;

    for (int i = NUM_BITS/8-1; i>=numBytes; i--){
        *(aux+i-numBytes) = *(a+i);
        if (numBits > 0){
            bitsAux = ((byteAux) & (*(aux+i-numBytes)));
            *(aux+i-numBytes) = *(aux+i-numBytes)>>numBits;
            *(aux+i-numBytes) = (*(aux+i-numBytes))&(secondByteAux);
            *(aux+i-numBytes) += bitsPerdidos;
            bitsPerdidos = bitsAux;
            bitsPerdidos = bitsPerdidos<<(8-numBits);
        }
    }
    copyBigInt(res,aux);
}

void big_sar(BigInt res, BigInt a, int n){
    int isNeg = (*(a+15))&(0x80);
    BigInt aux;
    long zero = 0;
    big_val(aux,zero);
    int numBytes = (int)n/8;
    int numBits = n-numBytes*8;

    if (isNeg){
        for (int i = 0; i<NUM_BITS/8; i++){
            *(aux+i) = 0xFF;
        }
    }

    unsigned char byteAux = 0;
    unsigned char secondByteAux = 0xFF;
    for (int i = 0; i<numBits; i++){
        byteAux *= 2;
        byteAux += 1;
    }
    secondByteAux = secondByteAux<<(8-numBits);
    secondByteAux = ~secondByteAux;

    unsigned char bitsPerdidos = 0;
    unsigned char bitsAux;

    for (int i = NUM_BITS/8-1; i>=numBytes; i--){
        *(aux+i-numBytes) = *(a+i);
        if (numBits > 0){
            bitsAux = ((byteAux) & (*(aux+i-numBytes)));
            *(aux+i-numBytes) = *(aux+i-numBytes)>>numBits;
            *(aux+i-numBytes) = (*(aux+i-numBytes))&(secondByteAux);
            *(aux+i-numBytes) += bitsPerdidos;
            bitsPerdidos = bitsAux;
            bitsPerdidos = bitsPerdidos<<(8-numBits);
        }
    }

    if(isNeg){
        secondByteAux = ~secondByteAux;
        *(aux+15) = (*(aux+15))|secondByteAux;   
    }

    copyBigInt(res,aux);
}
