#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


char komutOku(void);
int satirOku(char *satir, size_t boyut);

#define MAKS_GUN 15
#define TUR_BASINA_GUN 3  


int saglik, enerji, oksijen;   
int su, parca;                 
int hasar;                     
int kilitMuhurlu;              
int gun, tur;                  
char komutOku(void)
{
    char satir[64];

    if (fgets(satir, sizeof(satir), stdin) == NULL)
        return 'Q';

    return (char)toupper((unsigned char)satir[0]);
}

int satirOku(char *satir, size_t boyut)
{
    if (fgets(satir, boyut, stdin) == NULL)
        return 0;

    return 1;
}

int furtinaTur;                
int furtinaBaz;                
int suEsikAyar;                

void sinirla(void)
{
    if (saglik < 0)    saglik = 0;
    if (saglik > 100)  saglik = 100;
    if (enerji < 0)    enerji = 0;
    if (enerji > 100)  enerji = 100;
    if (oksijen < 0)   oksijen = 0;
    if (oksijen > 100) oksijen = 100;
    if (hasar < 0)     hasar = 0;
    if (hasar > 100)   hasar = 100;
    if (su < 0)        su = 0;
    if (parca < 0)     parca = 0;
}

void zorlukSec(void)
{
    char secim;
    int gecerli = 0;

    do {
        printf("Zorluk seviyesi secin (K: Kolay, N: Normal, Z: Zor): ");
        secim = komutOku();

        switch (secim) {
            case 'k': case 'K':
                saglik = 100; enerji = 100; oksijen = 100;
                su = 12; parca = 4;
                furtinaTur = 3; furtinaBaz = 4; suEsikAyar = 10;
                printf(">> KOLAY seviye secildi.\n");
                gecerli = 1;
                break;
            case 'n': case 'N':
                saglik = 100; enerji = 90; oksijen = 90;
                su = 8; parca = 3;
                furtinaTur = 5; furtinaBaz = 6; suEsikAyar = 0;
                printf(">> NORMAL seviye secildi.\n");
                gecerli = 1;
                break;
            case 'z': case 'Z':
                saglik = 100; enerji = 80; oksijen = 80;
                su = 5; parca = 2;
                furtinaTur = 7; furtinaBaz = 9; suEsikAyar = -10;
                printf(">> ZOR seviye secildi.\n");
                gecerli = 1;
                break;
            default:
                printf("!! Gecersiz seviye. Lutfen K, N veya Z girin.\n");
                break;
        }
    } while (!gecerli);
}


void durumSatiri(void)
{
    printf("\n[Gun %d/%d | Tur %d] Saglik:%d Enerji:%d Oksijen:%d Su:%d Parca:%d Hasar:%d Kilit:%s\n",
           gun, MAKS_GUN, tur, saglik, enerji, oksijen, su, parca, hasar,
           kilitMuhurlu ? "MUHURLU" : "ACIK");
}


int kesif(void)
{
    int zar, zar2, esikSu, esikParca, miktar;

    if (enerji < 15) {
        printf("!! Kesif icin enerji yetersiz (gerekli: 15, mevcut: %d). Komut iptal.\n", enerji);
        return 0;
    }

    if (enerji > 40 && oksijen > 30)
        esikSu = 50;
    else
        esikSu = 25;
    esikSu += suEsikAyar;
    esikParca = esikSu + 20;

    enerji  -= 15;
    oksijen -= 10;
    printf("Kesfe ciktin: enerji -15, oksijen -10.\n");

    zar = rand() % 100;
    if (zar < 10 || hasar > 70) {
        saglik -= 10;
        printf("Zar: %d / Esik: 10 (hasar>70: %s) -> YARALANDIN, saglik -10\n",
               zar, hasar > 70 ? "evet" : "hayir");
    } else {
        printf("Zar: %d / Esik: 10 (hasar>70: hayir) -> Yaralanma yok\n", zar);
    }

    zar2 = rand() % 100;
    if (zar2 < esikSu) {
        miktar = 3 + (rand() % 100) % 4;       /* 3-6 birim su */
        su += miktar;
        printf("Zar: %d / Esik: %d -> BASARILI: %d birim su buldun!\n", zar2, esikSu, miktar);
    } else if (zar2 < esikParca) {
        parca += 1;
        printf("Zar: %d / Esik: %d -> BASARILI: 1 yedek parca buldun!\n", zar2, esikParca);
    } else {
        printf("Zar: %d / Esik: %d -> BASARISIZ: eli bos dondun.\n", zar2, esikParca);
    }
    return 1;
}

int oksijenUret(void)
{
    char satir[64];
    int miktar, cift, artik, ciftSayisi, uretilen;

    if (enerji < 10) {
        printf("!! Oksijen uretimi icin enerji yetersiz (gerekli: 10, mevcut: %d). Komut iptal.\n", enerji);
        return 0;
    }
    if (su <= 0) {
        printf("!! Hic suyun yok, oksijen uretilemez. Komut iptal.\n");
        return 0;
    }

    printf("Kac birim su kullanilsin? (1-%d): ", su);
    if (!satirOku(satir, sizeof(satir)) || sscanf(satir, "%d", &miktar) != 1) {
        printf("!! Gecersiz miktar. Komut iptal.\n");
        return 0;
    }
    if (miktar <= 0) {
        printf("!! Miktar pozitif olmali. Komut iptal.\n");
        return 0;
    }
    if (miktar > su) {
        printf("!! Mevcut sudan (%d) fazla girilemez. Komut iptal.\n", su);
        return 0;
    }

    artik = miktar % 2;
    cift = miktar - artik;
    ciftSayisi = cift / 2;          /* 2 su -> 3 oksijen */
    uretilen = ciftSayisi * 3;

    if (oksijen + uretilen > 100) {
        ciftSayisi = (100 - oksijen) / 3;
        cift = ciftSayisi * 2;
        uretilen = ciftSayisi * 3;
        printf("Depo kapasitesi (100) siniri nedeniyle uretim sinirlandi.\n");
    }

    if (ciftSayisi == 0) {
        printf("!! Bu miktarla oksijen uretilemiyor (en az 2 su ve depoda yer gerekli). Komut iptal.\n");
        return 0;
    }

    su      -= cift;
    oksijen += uretilen;
    enerji  -= 10;
    printf("%d su harcandi -> +%d oksijen, enerji -10.", cift, uretilen);
    if (artik == 1 && miktar - artik == cift)
        printf(" (Tek sayi girildi: 1 birim su artik olarak depoda kaldi.)");
    printf("\n");
    return 1;
}

int tamir(void)
{
    int gerekli, onarim;
    const char *kademe;

    if (parca <= 0) {
        printf("!! Yedek parca yok, tamir yapilamaz. Komut iptal.\n");
        return 0;
    }
    if (hasar <= 0) {
        printf("Modul zaten saglam, tamire gerek yok. Komut iptal.\n");
        return 0;
    }
    if (enerji < 10) {
        printf("!! Tamir icin enerji yetersiz (gerekli: 10, mevcut: %d). Komut iptal.\n", enerji);
        return 0;
    }

    if (hasar <= 30) {
        kademe = "HAFIF"; gerekli = 1; onarim = 15;
    } else if (hasar <= 70) {
        kademe = "ORTA";  gerekli = 2; onarim = 30;
    } else {
        kademe = "AGIR";  gerekli = 3; onarim = 50;
    }

    if (parca < gerekli) {
        onarim = onarim * parca / gerekli;
        gerekli = parca;
    }

    parca  -= gerekli;
    hasar  -= onarim;
    enerji -= 10;
    printf("%s hasar kademesi: %d parca harcandi, hasar -%d, enerji -10.\n",
           kademe, gerekli, onarim);
    return 1;
}

void durumRaporu(void)
{
    printf("\n===== DURUM RAPORU =====\n");
    printf("Gun: %d/%d   Tur: %d\n", gun, MAKS_GUN, tur);
    printf("Saglik : %3d\nEnerji : %3d\nOksijen: %3d\n", saglik, enerji, oksijen);
    printf("Su     : %3d birim\nParca  : %3d adet\nModul hasari: %3d\n", su, parca, hasar);
    printf("Hava kilidi: %s\n", kilitMuhurlu ? "MUHURLU" : "ACIK");

    if (oksijen < 25) printf("UYARI: Oksijen kritik seviyede (%d < 25)!\n", oksijen);
    if (saglik < 30)  printf("UYARI: Saglik kritik seviyede (%d < 30)!\n", saglik);
    if (enerji < 20)  printf("UYARI: Enerji kritik seviyede (%d < 20)!\n", enerji);
    if (hasar > 70)   printf("UYARI: Modul hasari kritik seviyede (%d > 70)!\n", hasar);
    if (su == 0)      printf("UYARI: Su kalmadi!\n");
    if (parca == 0)   printf("UYARI: Yedek parca kalmadi!\n");

    if (oksijen >= 25 && saglik >= 30 && enerji >= 20 && hasar <= 70)
        printf("TUM SISTEMLER NORMAL\n");
    printf("========================\n");
}

int uyu(void)
{
    int enerjiArtis = 25;
    int saglikArtis = 10;

    printf("Dinlenmeye cekildin...\n");
    enerji += enerjiArtis;

    if (oksijen >= 20) {
        if (saglik < 100) {                 
            saglik += saglikArtis;
            printf("Enerji +%d, saglik +%d.\n", enerjiArtis, saglikArtis);
        } else {
            printf("Enerji +%d. Sagligin zaten tam.\n", enerjiArtis);
        }
    } else {
        saglik -= 5;
        printf("Enerji +%d ama oksijen 20'nin altinda: saglik -5!\n", enerjiArtis);
    }

    oksijen -= 2;                           /* oksijen tuketimi devam eder */
    printf("Oksijen tuketimi: -2\n");
    return 1;
}

int furtina(void)
{
    int i, zar, esik, carpan, hs;
    int hasarliTur = 0;
    int bsaglik, boksijen;

    if (enerji < 10) {
        printf("!! Siginak hazirligi icin enerji yetersiz (gerekli: 10, mevcut: %d). Komut iptal.\n", enerji);
        return 0;
    }
    enerji -= 10;
    bsaglik = saglik;
    boksijen = oksijen;

    printf("\n*** KUM FIRTINASI BASLADI (%d tur) *** (enerji -10)\n", furtinaTur);

    for (i = 1; i <= furtinaTur; i++) {
        zar = rand() % 100;

    
        if (i % 2 == 0)
            carpan = 2;
        else
            carpan = 1;
        hs = furtinaBaz * carpan;

        if (saglik < 40 || oksijen < 40)
            esik = 20;
        else
            esik = 40;

        printf("Tur %d%s | Zar: %d / Esik: %d -> ", i, carpan == 2 ? " (siddetli ruzgar x2)" : "", zar, esik);

        if (zar < esik) {
            printf("KORUNDUN, zarar yok.\n");
        } else if (zar < esik + 20) {
            saglik -= hs;
            hasarliTur++;
            printf("CAN KAYBI: saglik -%d\n", hs);
        } else if (zar < esik + 40) {
            if (kilitMuhurlu == 1) {
                printf("Oksijen sizintisi, ama hava kilidi muhurlu: ATLANDI.\n");
                continue;               
            }
            oksijen -= hs;
            hasarliTur++;
            printf("OKSIJEN SIZINTISI: oksijen -%d\n", hs);
        } else {
            hasar += hs;
            hasarliTur++;
            printf("MODUL HASARI: hasar +%d\n", hs);
        }

        sinirla();
        if (saglik <= 0 || oksijen <= 0) {
            printf("!! Kritik seviye: firtina aniden sonlandirildi (break).\n");
            break;
        }
    }

    sinirla();
    printf("--- Firtina ozeti: %d turda hasar alindi, toplam saglik kaybi: %d, toplam oksijen kaybi: %d ---\n",
           hasarliTur, bsaglik - saglik, boksijen - oksijen);
    return 1;
}

int sifre(void)
{
    char satir[64];
    char girilen = 0;
    char sifreHarf;
    int deneme = 0;

    if (kilitMuhurlu == 1) {
        printf("Hava kilidi zaten muhurlu. Komut iptal.\n");
        return 0;
    }
    if (enerji < 5) {
        printf("!! Sifre denemesi icin enerji yetersiz (gerekli: 5, mevcut: %d). Komut iptal.\n", enerji);
        return 0;
    }

    sifreHarf = 'A' + (gun % 26);
    printf("\n--- HAVA KILIDI --- Ipucu: Sifre, alfabede A harfinden bugunun gun sayisi (%d) kadar ilerideki harftir.\n", gun);
    printf("3 deneme hakkin var (her yanlis denemede enerji -5).\n");

    do {
        int gecerli = 0;
        while (!gecerli) {
            printf("Deneme %d/3 - tek harf girin: ", deneme + 1);
            if (!satirOku(satir, sizeof(satir))) {
                girilen = '#';
                deneme = 3;
                break;
            }
            girilen = (char)toupper((unsigned char)satir[0]);
            if (girilen >= 'A' && girilen <= 'Z')
                gecerli = 1;
            else
                printf("!! Sifre A-Z araliginda bir harf olmalidir.\n");
        }
        deneme++;

        if (girilen != sifreHarf) {
            enerji -= 5;
            sinirla();
            if (girilen < sifreHarf)
                printf("Yanlis! Ipucu: ileri git (enerji -5)\n");
            else
                printf("Yanlis! Ipucu: geri git (enerji -5)\n");
        }
    } while (girilen != sifreHarf && deneme < 3);

    if (girilen == sifreHarf) {
        kilitMuhurlu = 1;
        printf("DOGRU SIFRE! Hava kilidi muhurlendi (kilitMuhurlu = 1).\n");
    } else {
        hasar += 20;
        printf("3 deneme de basarisiz! Kilit mekanizmasi zarar gordu: modul hasari +20.\n");
    }
    return 1;
}

void sonucRaporu(int bitis)
{
    int hayatta, puan;

    hayatta = gun - 1;
    if (hayatta > MAKS_GUN) hayatta = MAKS_GUN;
    if (hayatta < 0) hayatta = 0;

    puan = (hayatta * 100) + saglik + oksijen - hasar;

    printf("\n==============================\n");
    if (bitis == 1)
        printf("   KAZANDIN! Kurtarma ekibi geldi.\n");
    else if (bitis == 2)
        printf("   KAYBETTIN! %s\n", saglik <= 0 ? "Sagligin tukendi." : "Oksijenin tukendi.");
    else
        printf("   Simulasyondan cikildi.\n");
    printf("==============================\n");
    printf("Hayatta kalinan gun : %d\n", hayatta);
    printf("Toplam tur          : %d\n", tur);
    printf("Saglik: %d | Enerji: %d | Oksijen: %d\n", saglik, enerji, oksijen);
    printf("Su: %d | Yedek parca: %d | Modul hasari: %d | Kilit: %s\n",
           su, parca, hasar, kilitMuhurlu ? "MUHURLU" : "ACIK");
    printf("PUAN = (gun x 100) + saglik + oksijen - hasar = %d\n", puan);
    printf("==============================\n");
}

int main(void)
{
    char komut;
    int turHarcandi;
    int bitis = 0;     

    srand((unsigned int)time(NULL));

    kilitMuhurlu = 0;
    gun = 1;
    tur = 0;
    hasar = 0;

    printf("=====================================================\n");
    printf("   MARS KOLONISI YASAM DESTEK SIMULATORU\n");
    printf("   Kurtarma ekibi gelene kadar %d gun hayatta kal!\n", MAKS_GUN);
    printf("=====================================================\n");
    printf("Komutlar: G:Kesif O:Oksijen uret T:Tamir D:Durum U:Uyu K:Kum firtinasi H:Hava kilidi Q:Cikis\n\n");

    zorlukSec();

    do {
        durumSatiri();
        printf("Komut (G O T D U K H Q): ");
        komut = komutOku();
        turHarcandi = 0;

        switch (komut) {
            case 'g': case 'G':
                turHarcandi = kesif();
                break;
            case 'o': case 'O':
                turHarcandi = oksijenUret();
                break;
            case 't': case 'T':
                turHarcandi = tamir();
                break;
            case 'd': case 'D':
                durumRaporu();              
                break;
            case 'u': case 'U':
                turHarcandi = uyu();
                break;
            case 'k': case 'K':
                turHarcandi = furtina();
                break;
            case 'h': case 'H':
                turHarcandi = sifre();
                break;
            case 'q': case 'Q':
                bitis = 3;
                break;
            default:
                printf("!! Taninmayan komut. Gecerli komutlar: G O T D U K H Q (tur harcanmadi)\n");
                break;
        }

        
        if (turHarcandi == 1) {
            tur++;
            sinirla();
            if (tur % TUR_BASINA_GUN == 0) {
                gun++;
                oksijen -= 8;
                printf("\n>>> Yeni gun basladi. Oksijen -8.");
                if (hasar > 70) {
                    saglik -= 5;
                    printf(" Modul hasari 70'in uzerinde: saglik -5.");
                }
                printf("\n");
                if (gun <= MAKS_GUN)
                    printf(">>> GUN %d\n", gun);
            }
        }

        sinirla();

        
        if (bitis == 0) {
            if (saglik <= 0 || oksijen <= 0)
                bitis = 2;
            else if (gun > MAKS_GUN)
                bitis = 1;
        }
    } while (bitis == 0);

    sonucRaporu(bitis);
    return 0;
}

