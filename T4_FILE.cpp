#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct Produk {
    char nama[50];
    char kategori[30];
    double harga;
    int stok;
    float rating;
};

Produk p[100];
int n = 0;

void garis() {
    printf("========================================================================================================\n");
}

void load() {
    FILE *f = fopen("shopease_db.txt", "r");
    if (!f) return;

    while (fscanf(f, " %[^|]|%[^|]|%lf|%d|%f\n",
           p[n].nama, p[n].kategori, &p[n].harga, &p[n].stok, &p[n].rating) != EOF) {
        n++;
    }

    fclose(f);
}

void save() {
    FILE *f = fopen("shopease_db.txt", "w");

    for (int i = 0; i < n; i++) {
        fprintf(f, "%s|%s|%.0lf|%d|%.1f\n",
                p[i].nama, p[i].kategori, p[i].harga, p[i].stok, p[i].rating);
    }

    fclose(f);
}

bool kembaliMenu() {
    char j;

    printf("\nKembali ke menu utama? (y/t) : ");
    cin >> j;

    if (j == 'y' || j == 'Y')
        return true;
    else
        return false;
}

void tambah() {

    printf("\n============================================================\n");
    printf("|                       TAMBAH PRODUK                      |\n");
    printf("============================================================\n");

    cin.ignore();

    printf("| Nama Produk   : ");
    cin.getline(p[n].nama, 50);

    printf("| Kategori      : ");
    cin.getline(p[n].kategori, 30);

    printf("| Harga (Rp)    : ");
    cin >> p[n].harga;

    printf("| Stok          : ");
    cin >> p[n].stok;

    printf("| Rating (0-5)  : ");
    cin >> p[n].rating;

    printf("============================================================\n");

    n++;
    save();
}

void bubble() {

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            if (strcmp(p[j].nama, p[j + 1].nama) > 0) {

                Produk t = p[j];
                p[j] = p[j + 1];
                p[j + 1] = t;
            }
        }
    }
}

int partisi(int low, int high) {

    float pivot = p[high].rating;
    int i = low - 1;

    for (int j = low; j < high; j++) {

        if (p[j].rating > pivot) {

            i++;

            Produk t = p[i];
            p[i] = p[j];
            p[j] = t;
        }
    }

    Produk t = p[i + 1];
    p[i + 1] = p[high];
    p[high] = t;

    return i + 1;
}

void quick(int low, int high) {

    if (low < high) {

        int pi = partisi(low, high);

        quick(low, pi - 1);
        quick(pi + 1, high);
    }
}

void tabel() {

    printf("\nDAFTAR PRODUK\n");

    garis();

    printf("| %-3s | %-30s | %-20s | %-12s | %-6s | %-6s |\n",
           "No", "Nama Produk", "Kategori", "Harga", "Stok", "Rating");

    garis();

    for (int i = 0; i < n; i++) {

        printf("| %-3d | %-30s | %-20s | %-12.0lf | %-6d | %-6.1f |\n",
               i + 1, p[i].nama, p[i].kategori, p[i].harga, p[i].stok, p[i].rating);
    }

    garis();
}

void cari() {

    char key[50];
    bool ketemu = false;

    cin.ignore();

    printf("\nMasukkan Nama Produk : ");
    cin.getline(key, 50);

    for (int i = 0; i < n; i++) {

        if (strcmp(key, p[i].nama) == 0) {

            printf("\nProduk Ditemukan\n");
            printf("-----------------\n");
            printf("Nama     : %s\n", p[i].nama);
            printf("Kategori : %s\n", p[i].kategori);
            printf("Harga    : %.0lf\n", p[i].harga);
            printf("Stok     : %d\n", p[i].stok);
            printf("Rating   : %.1f\n", p[i].rating);

            ketemu = true;
            break;
        }
    }

    if (!ketemu)
        printf("\nProduk tidak ditemukan!\n");
}

int main() {

    int menu, mode;

    load();

    do {

        printf("\n============================================================\n");
        printf("|                SISTEM KATALOG PRODUK SHOPEASE            |\n");
        printf("============================================================\n");
        printf("| 1 | Tambah Produk                                        |\n");
        printf("| 2 | Tampilkan Produk                                     |\n");
        printf("| 3 | Cari Produk                                          |\n");
        printf("| 4 | Keluar                                               |\n");
        printf("============================================================\n");

        printf("Pilih Menu : ");
        cin >> menu;

        if (menu == 1) {

            bool ulang;

            do {
                tambah();
                ulang = !kembaliMenu();
            } while (ulang);
        }

        else if (menu == 2) {

            bool ulang;

            do {

                printf("\n============================================================\n");
                printf("|                    MODE TAMPIL PRODUK                    |\n");
                printf("============================================================\n");
                printf("| 1. Urut Nama Produk (Bubble Sort)                        |\n");
                printf("| 2. Urut Rating (Quick Sort)                              |\n");
                printf("============================================================\n");

                printf("Pilih Mode : ");
                cin >> mode;

                if (mode == 1)
                    bubble();
                else if (mode == 2)
                    quick(0, n - 1);

                tabel();

                ulang = !kembaliMenu();

            } while (ulang);
        }

        else if (menu == 3) {

            bool ulang;

            do {
                cari();
                ulang = !kembaliMenu();
            } while (ulang);
        }

    } while (menu != 4);

    save();

    printf("\nTerima kasih menggunakan sistem ShopEase\n");

    return 0;
}
