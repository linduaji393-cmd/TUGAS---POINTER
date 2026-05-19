#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;
const int    KAPASITAS   = 3;
const string F_KENDARAAN = "data_kendaraan.txt";
const string F_AKUN      = "data_akun.txt";
const string F_ANTRIAN   = "data_antrian.txt";
struct Node {
    string noPolisi, nama;
    bool   parkir;
    Node  *kiri, *kanan;
    Node(string np, string nm, bool p = false)
        : noPolisi(np), nama(nm), parkir(p), kiri(nullptr), kanan(nullptr) {}
};
struct Aksi { string tipe, noPolisi; bool dariAntrian; };
struct Akun { string user, pass; };
class BST {
    Node* akar = nullptr;
    Node* sisip(Node* n, const string& np, const string& nm, bool p) {
        if (!n) return new Node(np, nm, p);
        if (np < n->noPolisi) n->kiri  = sisip(n->kiri,  np, nm, p);
        else if (np > n->noPolisi) n->kanan = sisip(n->kanan, np, nm, p);
        return n;
    }
    Node* cari(Node* n, const string& np) const {
        if (!n || n->noPolisi == np) return n;
        return np < n->noPolisi ? cari(n->kiri, np) : cari(n->kanan, np);
    }
    void inOrder(Node* n, vector<Node*>& v) const {
        if (!n) return;
        inOrder(n->kiri, v); v.push_back(n); inOrder(n->kanan, v);
    }
    Node* minNode(Node* n) { while (n->kiri) n = n->kiri; return n; }
    Node* hapus(Node* n, const string& np, bool& ok) {
        if (!n) { ok = false; return nullptr; }
        if      (np < n->noPolisi) n->kiri  = hapus(n->kiri,  np, ok);
        else if (np > n->noPolisi) n->kanan = hapus(n->kanan, np, ok);
        else {
            ok = true;
            if (!n->kiri) { auto* t = n->kanan; delete n; return t; }
            if (!n->kanan){ auto* t = n->kiri;  delete n; return t; }
            auto* s = minNode(n->kanan);
            n->noPolisi = s->noPolisi; n->nama = s->nama; n->parkir = s->parkir;
            n->kanan = hapus(n->kanan, s->noPolisi, ok);
        }
        return n;
    }
    void hapusSemua(Node* n) {
        if (!n) return; hapusSemua(n->kiri); hapusSemua(n->kanan); delete n;
    }
public:
    ~BST() { hapusSemua(akar); }
    bool tambah(const string& np, const string& nm, bool p = false) {
        if (cari(akar, np)) return false;
        akar = sisip(akar, np, nm, p); return true;
    }
    Node* get(const string& np) const { return cari(akar, np); }
    vector<Node*> semua() const {
        vector<Node*> v; inOrder(akar, v); return v;
    }
    int hitungParkir() const {
        int c = 0; for (auto* k : semua()) if (k->parkir) c++; return c;
    }
    bool hapusNode(const string& np) {
        bool ok = false; akar = hapus(akar, np, ok); return ok;
    }
};
BST           bst;
stack<Aksi>   undoStack;
queue<string> antrian;
vector<Akun>  akuns;
void simpanKendaraan() {
    ofstream f(F_KENDARAAN);
    for (auto* k : bst.semua())
        f << k->noPolisi << "|" << k->nama << "|" << k->parkir << "\n";
}
void muatKendaraan() {
    ifstream f(F_KENDARAAN); if (!f) return;
    string baris;
    while (getline(f, baris)) {
        if (baris.empty()) continue;
        istringstream ss(baris); string np, nm, st;
        getline(ss, np, '|'); getline(ss, nm, '|'); getline(ss, st, '|');
        bst.tambah(np, nm, st == "1");
    }
}
void simpanAkun() {
    ofstream f(F_AKUN);
    for (auto& a : akuns) f << a.user << "|" << a.pass << "\n";
}
void muatAkun() {
    ifstream f(F_AKUN); if (!f) return;
    string baris;
    while (getline(f, baris)) {
        if (baris.empty()) continue;
        istringstream ss(baris); string u, p;
        getline(ss, u, '|'); getline(ss, p, '|');
        if (!u.empty()) akuns.push_back({u, p});
    }
}
void simpanAntrian() {
    ofstream f(F_ANTRIAN);
    queue<string> tmp = antrian;
    while (!tmp.empty()) { f << tmp.front() << "\n"; tmp.pop(); }
}
void muatAntrian() {
    ifstream f(F_ANTRIAN); if (!f) return;
    string baris;
    while (getline(f, baris)) if (!baris.empty()) antrian.push(baris);
}
const int LEBAR = 58;
void garis() { cout << string(LEBAR, '=') << "\n"; }
void garisTipis() { cout << string(LEBAR, '-') << "\n"; }
void judul(const string& s) {
    garis();
    int sp = (LEBAR - (int)s.size()) / 2;
    cout << string(max(0, sp), ' ') << s << "\n";
    garis();
}
string upper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper); return s;
}
void cetakBaris(const string& label, const string& nilai, int lebarLabel = 22) {
    string lb = (int)label.size() > lebarLabel ? label.substr(0, lebarLabel) : label;
    cout << lb << string(lebarLabel - (int)lb.size(), ' ') << ": " << nilai << "\n";
}
void cetakBaris(const string& label, int nilai, int lebarLabel = 22) {
    cetakBaris(label, to_string(nilai), lebarLabel);
}
bool tanyaKembali() {
    garis();
    cout << "Kembali ke menu? (Y/T): "; char c; cin >> c;
    return toupper(c) == 'Y';
}
string padKanan(const string& s, int w) {
    if ((int)s.size() >= w) return s.substr(0, w);
    return s + string(w - s.size(), ' ');
}
string padKiri(const string& s, int w) {
    if ((int)s.size() >= w) return s.substr(0, w);
    return string(w - s.size(), ' ') + s;
}
void tambahKendaraan() {
    judul("TAMBAH KENDARAAN");
    string np, nm;
    cout << "Nomor polisi : "; cin.ignore(); getline(cin, np); np = upper(np);
    cout << "Nama pemilik : "; getline(cin, nm);
    garis();
    if (bst.tambah(np, nm)) {
        simpanKendaraan();
        cout << "Kendaraan berhasil ditambahkan.\n";
    } else {
        cout << "Nomor polisi sudah terdaftar.\n";
    }
    garis();
}
void tampilKendaraan() {
    judul("DATA KENDARAAN");
    cout << padKanan("No",          4)  << "  "
         << padKanan("No. Polisi",  13) << "  "
         << padKanan("Nama Pemilik",22) << "  "
         << padKanan("Status",      13) << "\n";
    garis();
    auto semua = bst.semua();
    if (semua.empty()) {
        cout << "Belum ada data kendaraan.\n";
    } else {
        for (int i = 0; i < (int)semua.size(); i++) {
            cout << padKanan(to_string(i+1),      4)  << "  "
                 << padKanan(semua[i]->noPolisi,  13) << "  "
                 << padKanan(semua[i]->nama,      22) << "  "
                 << padKanan(semua[i]->parkir ? "Di Parkiran" : "Di Luar", 13) << "\n";
        }
    }
    garis();
    cetakBaris("Slot parkir terisi",
               to_string(bst.hitungParkir()) + "/" + to_string(KAPASITAS));
    cetakBaris("Antrian", (int)antrian.size());
    garis();
}
void kendaraanMasuk() {
    judul("KENDARAAN MASUK");
    string np;
    cout << "Nomor polisi : "; cin.ignore(); getline(cin, np); np = upper(np);
    garis();
    auto* n = bst.get(np);
    if (!n)        { cout << "Kendaraan tidak ditemukan.\n";       garis(); return; }
    if (n->parkir) { cout << "Kendaraan sudah ada di parkiran.\n"; garis(); return; }
    queue<string> tmp = antrian;
    while (!tmp.empty()) {
        if (tmp.front() == np) { cout << "Kendaraan sudah ada di antrian.\n"; garis(); return; }
        tmp.pop();
    }
    if (bst.hitungParkir() >= KAPASITAS) {
        antrian.push(np);
        undoStack.push({"MASUK_ANTRIAN", np, false});
        simpanAntrian();
        cetakBaris("Status",          "Parkiran penuh, masuk antrian");
        cetakBaris("Posisi antrian",  (int)antrian.size());
    } else {
        n->parkir = true;
        undoStack.push({"MASUK", np, false});
        simpanKendaraan();
        cetakBaris("Status",      "Berhasil masuk parkiran");
        cetakBaris("Slot terisi", to_string(bst.hitungParkir()) + "/" + to_string(KAPASITAS));
    }
    garis();
}
void kendaraanKeluar() {
    judul("KENDARAAN KELUAR");
    string np;
    cout << "Nomor polisi : "; cin.ignore(); getline(cin, np); np = upper(np);
    garis();
    auto* n = bst.get(np);
    if (!n)         { cout << "Kendaraan tidak ditemukan.\n";     garis(); return; }
    if (!n->parkir) { cout << "Kendaraan tidak sedang parkir.\n"; garis(); return; }
    n->parkir = false;
    undoStack.push({"KELUAR", np, false});
    simpanKendaraan();
    cetakBaris("Status",      "Berhasil keluar");
    cetakBaris("Slot terisi", to_string(bst.hitungParkir()) + "/" + to_string(KAPASITAS));
    if (!antrian.empty()) {
        string na = antrian.front(); antrian.pop(); simpanAntrian();
        auto* nd = bst.get(na);
        if (nd) {
            nd->parkir = true;
            undoStack.push({"MASUK", na, true});
            simpanKendaraan();
            cetakBaris("Antrian masuk otomatis", na);
            cetakBaris("Slot terisi", to_string(bst.hitungParkir()) + "/" + to_string(KAPASITAS));
        }
    } else {
        cetakBaris("Antrian", "Kosong");
    }
    garis();
}
void hapusKendaraan() {
    judul("HAPUS KENDARAAN");
    string np;
    cout << "Nomor polisi : "; cin.ignore(); getline(cin, np); np = upper(np);
    garis();
    auto* n = bst.get(np);
    if (!n) { cout << "Kendaraan tidak ditemukan.\n"; garis(); return; }
    cetakBaris("No. Polisi", n->noPolisi, 15);
    cetakBaris("Nama",       n->nama,     15);
    cetakBaris("Status",     n->parkir ? "Di Parkiran" : "Di Luar", 15);
    garis();
    cout << "Yakin hapus? (Y/T): "; char k; cin >> k;
    garis();
    if (toupper(k) == 'Y') {
        bst.hapusNode(np); simpanKendaraan();
        cout << "Data berhasil dihapus.\n";
    } else {
        cout << "Penghapusan dibatalkan.\n";
    }
    garis();
}
void undoAksi() {
    judul("UNDO AKSI TERAKHIR");
    if (undoStack.empty()) {
        cout << "Tidak ada aksi untuk di-undo.\n";
        garis(); return;
    }
    Aksi a = undoStack.top(); undoStack.pop();
    auto* n = bst.get(a.noPolisi);
    if (a.tipe == "MASUK_ANTRIAN") {
        queue<string> tmp; bool hapus = false;
        while (!antrian.empty()) {
            string f = antrian.front(); antrian.pop();
            if (!hapus && f == a.noPolisi) { hapus = true; continue; }
            tmp.push(f);
        }
        antrian = tmp; simpanAntrian();
        cetakBaris("No. Polisi", a.noPolisi);
        cetakBaris("Hasil", hapus ? "Dikeluarkan dari antrian" : "Tidak ada di antrian");
    } else if (a.tipe == "MASUK") {
        if (!n) { cout << "Data tidak ditemukan.\n"; garis(); return; }
        n->parkir = false; simpanKendaraan();
        if (a.dariAntrian) {
            queue<string> tmp; tmp.push(a.noPolisi);
            while (!antrian.empty()) { tmp.push(antrian.front()); antrian.pop(); }
            antrian = tmp; simpanAntrian();
            cetakBaris("No. Polisi", a.noPolisi);
            cetakBaris("Hasil",      "Kembali ke depan antrian");
        } else {
            cetakBaris("No. Polisi", a.noPolisi);
            cetakBaris("Hasil",      "Dikeluarkan dari parkiran");
        }
        cetakBaris("Slot terisi", to_string(bst.hitungParkir()) + "/" + to_string(KAPASITAS));
    } else if (a.tipe == "KELUAR") {
        if (!n) { cout << "Data tidak ditemukan.\n"; garis(); return; }
        if (bst.hitungParkir() >= KAPASITAS) {
            cetakBaris("Hasil", "Gagal, parkiran penuh");
            undoStack.push(a); garis(); return;
        }
        n->parkir = true; simpanKendaraan();
        cetakBaris("No. Polisi", a.noPolisi);
        cetakBaris("Hasil",      "Masuk kembali ke parkiran");
        cetakBaris("Slot terisi", to_string(bst.hitungParkir()) + "/" + to_string(KAPASITAS));
    }
    garis();
}
void tampilAntrian() {
    judul("ANTRIAN PARKIR");
    cetakBaris("Slot parkir terisi",
               to_string(bst.hitungParkir()) + "/" + to_string(KAPASITAS));
    cetakBaris("Total antrian", (int)antrian.size());
    garis();
    if (antrian.empty()) {
        cout << "Antrian kosong.\n";
    } else {
        cout << padKanan("Urutan",       7)  << "  "
             << padKanan("No. Polisi",  13) << "  "
             << padKanan("Nama Pemilik",22) << "\n";
        garis();
        queue<string> tmp = antrian; int no = 1;
        while (!tmp.empty()) {
            string np = tmp.front(); tmp.pop();
            auto* nd  = bst.get(np);
            cout << padKanan(to_string(no++),          7)  << "  "
                 << padKanan(np,                       13) << "  "
                 << padKanan(nd ? nd->nama : "-",      22) << "\n";
        }
    }
    garis();
}
void daftarAkun() {
    judul("DAFTAR AKUN BARU");
    string u, p, k;
    cout << "Username : "; cin >> u;
    for (auto& a : akuns) if (a.user == u) {
        garis();
        cout << "Username sudah digunakan.\n";
        garis(); return;
    }
    cout << "Password : "; cin >> p;
    cout << "Konfirm  : "; cin >> k;
    garis();
    if (p != k) {
        cout << "Password tidak cocok.\n";
    } else {
        akuns.push_back({u, p}); simpanAkun();
        cout << "Akun berhasil dibuat.\n";
    }
    garis();
}
string login() {
    while (true) {
        judul("SISTEM PARKIR INAP BANDARA GSA");
        cout << "1. Login\n2. Daftar Akun\n0. Keluar\n";
        garis();
        cout << "Pilih : "; int p; cin >> p; cout << "\n";
        if (p == 0) return "";
        if (p == 2) { daftarAkun(); cout << "\n"; continue; }
        if (p == 1) {
            judul("LOGIN");
            string u, pw;
            cout << "Username : "; cin >> u;
            cout << "Password : "; cin >> pw;
            garis();
            for (auto& a : akuns) if (a.user == u && a.pass == pw) {
                cout << "Login berhasil. Selamat datang, " << u << "!\n";
                garis(); cout << "\n";
                return u;
            }
            cout << "Username atau password salah.\n";
            garis(); cout << "\n";
        } else {
            cout << "Pilihan tidak valid.\n\n";
        }
    }
}
void tampilMenu(const string& user) {
    judul("SISTEM PARKIR INAP BANDARA GSA");
    cetakBaris("Pengguna",          user);
    cetakBaris("Slot parkir terisi",
               to_string(bst.hitungParkir()) + "/" + to_string(KAPASITAS));
    cetakBaris("Antrian",           (int)antrian.size());
    garis();
    cout << "1. Tambah Data Kendaraan\n";
    cout << "2. Tampil Data Kendaraan\n";
    cout << "3. Kendaraan Masuk\n";
    cout << "4. Kendaraan Keluar\n";
    cout << "5. Hapus Data Kendaraan\n";
    cout << "6. Undo Aksi Terakhir\n";
    cout << "7. Tampil Antrian Parkir\n";
    cout << "0. Keluar\n";
    garis();
    cout << "Pilih : ";
}
int main() {
    muatAkun();
    bool adaAdmin = false;
    for (auto& a : akuns) if (a.user == "admin") { adaAdmin = true; break; }
    if (!adaAdmin) { akuns.push_back({"admin","1234"}); simpanAkun(); }
    muatKendaraan();
    muatAntrian();
    if (bst.semua().empty()) {
        bst.tambah("B 1234 ABC", "Andi Saputra",  true);
        bst.tambah("D 5678 XYZ", "Budi Santoso",  true);
        bst.tambah("F 9012 DEF", "Citra Dewi",    true);
        bst.tambah("H 3456 GHI", "Doni Pratama",  false);
        bst.tambah("B 7890 JKL", "Eka Rahmawati", false);
        bst.tambah("G 1111 MNO", "Fajar Nugroho", false);
        bst.tambah("L 2222 PQR", "Gita Lestari",  false);
        antrian.push("G 1111 MNO");
        antrian.push("L 2222 PQR");
        simpanKendaraan(); simpanAntrian();
        undoStack.push({"MASUK",         "B 1234 ABC", false});
        undoStack.push({"MASUK",         "D 5678 XYZ", false});
        undoStack.push({"MASUK",         "F 9012 DEF", false});
        undoStack.push({"MASUK_ANTRIAN", "G 1111 MNO", false});
        undoStack.push({"MASUK_ANTRIAN", "L 2222 PQR", false});
    }
    string userLogin = login();
    if (userLogin.empty()) {
        cout << "Program selesai. Sampai jumpa!\n"; return 0;
    }
    int pilihan;
    do {
        tampilMenu(userLogin);
        cin >> pilihan; cout << "\n";
        if (pilihan == 0) break;
        if (pilihan < 1 || pilihan > 7) { cout << "Pilihan tidak valid.\n\n"; continue; }
        bool kembali = false;
        do {
            switch (pilihan) {
                case 1: tambahKendaraan(); break;
                case 2: tampilKendaraan(); break;
                case 3: kendaraanMasuk();  break;
                case 4: kendaraanKeluar(); break;
                case 5: hapusKendaraan();  break;
                case 6: undoAksi();        break;
                case 7: tampilAntrian();   break;
            }
            kembali = tanyaKembali();
        } while (!kembali);
        cout << "\n";
    } while (pilihan != 0);
    cout << "\nProgram selesai. Terima kasih, " << userLogin << "!\n";
    return 0;
}
