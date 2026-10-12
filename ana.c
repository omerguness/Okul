/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

/* Tüm tutarlar kuruş cinsinden */
int main(void) {
    int kupur[] = {20000, 10000, 5000, 2000, 1000, 500, 100, 50, 25, 10, 5, 1};
    int tutar, odenen, tl, kr, kalan;

    printf("Ürün tutarı (TL kuruş): ");
    scanf("%d %d", &tl, &kr);
    tutar = tl * 100 + kr;

    /* Ödenen para yetersizse tekrar sor */
    do {
        printf("Ödenen para (TL kuruş): ");
        scanf("%d %d", &tl, &kr);
        odenen = tl * 100 + kr;
    } while (odenen < tutar);

    kalan = odenen - tutar;
    printf("\nPara üstü: %d TL %d kuruş\n\n", kalan / 100, kalan % 100);

    for (int i = 0; i < 12; i++) {
        printf("%d.%02d TL: %d adet\n", kupur[i] / 100, kupur[i] % 100, kalan / kupur[i]);
        kalan %= kupur[i];
    }
    return 0;
}