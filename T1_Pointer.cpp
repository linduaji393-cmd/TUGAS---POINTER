#include <iostream>
#include <iomanip> // untuk setw dan setprecision
using namespace std;

int main() {
    int n;
    cout << "Masukkan panjang array: ";
    cin >> n;

    int *arr = new int[n];   // alokasi array dinamis
    int *ptr = arr;          // pointer ke elemen pertama

    cout << "\nMasukkan " << n << " nilai array:\n";
    for (int i = 0; i < n; i++) {
        cout << "Nilai ke-" << i + 1 << " : ";
        cin >> *(ptr + i);   // akses array pakai pointer
    }

    // Cetak isi array dengan format tabel
    cout << "\n==============================\n";
    cout << "        DATA ARRAY\n";
    cout << "==============================\n";
    cout << left << setw(10) << "Index" << setw(10) << "Nilai" << endl;
    cout << "------------------------------\n";

    for (int i = 0; i < n; i++) {
        cout << left << setw(10) << i << setw(10) << *(ptr + i) << endl;
    }

    // Cari maksimum dan minimum
    int maksimum = *ptr;
    int minimum  = *ptr;

    for (int i = 0; i < n; i++) {
        if (*(ptr + i) > maksimum) maksimum = *(ptr + i);
        if (*(ptr + i) < minimum)  minimum  = *(ptr + i);
    }

    // Hitung jumlah dan rata-rata
    int jumlah = 0;
    for (int i = 0; i < n; i++) {
        jumlah += *(ptr + i);
    }

    double rataRata = (double)jumlah / n;

    // Output hasil perhitungan
    cout << "==============================\n";
    cout << "        HASIL ANALISIS\n";
    cout << "==============================\n";
    cout << "Maksimum   : " << maksimum << endl;
    cout << "Minimum    : " << minimum << endl;
    cout << "Jumlah     : " << jumlah << endl;
    cout << fixed << setprecision(2);
    cout << "Rata-rata  : " << rataRata << endl;
    cout << "==============================\n";

    delete[] arr; // hapus memori dinamis
    return 0;
}
