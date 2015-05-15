char a, b = 8, c, d, e, f, g, h, i, j;

char fun() {
    
    a = (a - b - ( c - d - ( e - b - ( g - h - (i - j)))));
    
//    b = 5 - (b - 7 - (a - fun()) - 5) - 1;
    return 1 - 8;
    a = b;
}

char cc = a - fun();

char main() {
    a = fun();
    return a;
}