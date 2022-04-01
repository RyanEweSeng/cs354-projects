
// for each of the following functions 
// correct the prototype
// write function body
// replace return with the correct return statement
//
// you may write as many helper functions here as you wish

int algorithm_1() {
    int twelve_rbp = 8000;
    int eight_rbp = 63;

    return twelve_rbp + eight_rbp;  
}

int algorithm_2(int edi, int esi) {
    if (edi <= esi) {
        return esi + (edi * 3);
    } else {
        return edi / esi;
    }
}

int algorithm_3(int edi, int esi) {
    int product = 0;
    for (int i = esi; i > 0; i--) {
        product = product + edi;
    }

    return product;
}

void algorithm_4(int edi, int* rsi) {
    *rsi = edi * 3;
}



