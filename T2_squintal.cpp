#include <iostream>
using namespace std;

struct Buku {
    string nama;
    int harga;
};

int sequentialSearch(Buku data[], int n, string cari) {
    for (int i = 0; i < n; i++) {
        if (data[i].nama == cari) {
            return i;
        }
    }
    return -1;
}

int main() {

    Buku daftarBuku[5] = {
        {"Algoritma", 75000},
        {"BasisData", 85000},
        {"Jaringan", 90000},
        {"Pemrograman", 80000},
        {"StrukturData", 95000}
    };

    int n = 5;
    string cari;

    cout << "=====================================\n";
    cout << "     PROGRAM SEQUENTIAL SEARCH\n";
    cout << "=====================================\n\n";

    cout << "Daftar Buku:\n";
    for (int i = 0; i < n; i++) {
        cout << i+1 << ". " << daftarBuku[i].nama << endl;
    }

    cout << "\nMasukkan nama buku yang dicari : ";
    cin >> cari;

    int hasil = sequentialSearch(daftarBuku, n, cari);

    cout << "\n-------------------------------------\n";
    if (hasil != -1) {
        cout << "Buku ditemukan!\n";
        cout << "Nama  : " << daftarBuku[hasil].nama << endl;
        cout << "Harga : Rp " << daftarBuku[hasil].harga << endl;
    } else {
        cout << "Buku tidak ditemukan.\n";
    }
    cout << "=====================================\n";

    return 0;
}
