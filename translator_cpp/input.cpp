char cnt = 8;

float f1() {
    return 100;
}

float f() {
    cnt = 7 * 2;
    return f1() + cnt;
}

char t = 88;

char main() {
    char a = 20, b = 51, c[8] = "Hello!", d[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 20}, e;

    float x = 30, y = 5731, z, w[5] = {10, 20, 30, 40, 50}, v[3];

    c[5] = 5 * 3;
    c[6] = 5;
    c[7] = c[5] + c[6] * 15;

    char aa = 3;
    float bb = 0;

    do {
        bb = bb + 5;
        aa = aa - 1;
    } while (aa > -10);
    
    char magic = f();
    
}
