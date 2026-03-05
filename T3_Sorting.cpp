
#include <iostream>
#include <iomanip>
using namespace std;

// Bubble Sort Ascending
void bubbleSort(string arr[], int n){
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j] > arr[j+1]){
                string temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Partition Quick Sort Descending
int partition(string arr[], int low, int high){
    string pivot = arr[high];
    int i = low - 1;

    for(int j=low;j<high;j++){
        if(arr[j] > pivot){
            i++;
            string temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    string temp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = temp;

    return i+1;
}

// Quick Sort Descending
void quickSort(string arr[], int low, int high){
    if(low < high){
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi-1);
        quickSort(arr, pi+1, high);
    }
}

int main(){

    int n;
    string nama[100], namaQuick[100];

    cout<<"===================================================="<<endl;
    cout<<"        PROGRAM PENGURUTAN NAMA MAHASISWA"<<endl;
    cout<<"   Bubble Sort (Ascending) & Quick Sort (Descending)"<<endl;
    cout<<"===================================================="<<endl;

    cout<<"Jumlah mahasiswa : ";
    cin>>n;
    cin.ignore();

    cout<<endl;

    for(int i=0;i<n;i++){
        cout<<"Nama mahasiswa ke-"<<i+1<<" : ";
        getline(cin,nama[i]);
        namaQuick[i] = nama[i];
    }

    bubbleSort(nama,n);
    quickSort(namaQuick,0,n-1);

    cout<<endl;
    cout<<"================== ASCENDING =================="<<endl;
    cout<<left<<setw(6)<<"No"<<setw(30)<<"Nama Mahasiswa"<<endl;
    cout<<"----------------------------------------------"<<endl;

    for(int i=0;i<n;i++){
        cout<<left<<setw(6)<<i+1<<setw(30)<<nama[i]<<endl;
    }

    cout<<endl;
    cout<<"================= DESCENDING ================="<<endl;
    cout<<left<<setw(6)<<"No"<<setw(30)<<"Nama Mahasiswa"<<endl;
    cout<<"----------------------------------------------"<<endl;

    for(int i=0;i<n;i++){
        cout<<left<<setw(6)<<i+1<<setw(30)<<namaQuick[i]<<endl;
    }

    cout<<"----------------------------------------------"<<endl;

    return 0;
}
