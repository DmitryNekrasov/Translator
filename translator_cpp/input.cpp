float f1() {
}

char main() {
    char a, b, c = 8 + 7;
    do {
        c = a + b;
        
        do {
            b = b - 1;
        } while (b);
        
    } while (c--);
}