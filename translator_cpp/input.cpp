float f1() {
    return 5;
}

char main() {
    char a, b, c = 8 + 7;
    do {
        c = a + b + f1();
        
        do {
            return 1;
            b = b - 1;
        } while (b);
        
    } while (c--);
    
    do {
        a = 1;
    } while (a--);
    
    return a + b;
}