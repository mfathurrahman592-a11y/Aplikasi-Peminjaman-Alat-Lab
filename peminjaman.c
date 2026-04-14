#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

struct Akun {
    char username[50];
    char password[50];
    char role[20];
};

struct Alat {
    char nama[50];
    int stok;
};

struct Akun akun[MAX];
struct Alat alat[MAX];

int jumlahAkun = 0;
int jumlahAlat = 0;

// Membaca data akun dari file
void loadAkun() {
    FILE *file = fopen("akun.txt", "r");
    if (file == NULL) {
        printf("File akun tidak ditemukan!\n");
        return;
    }

    while (fscanf(file, "%s %s %s",
                  akun[jumlahAkun].username,
                  akun[jumlahAkun].password,
                  akun[jumlahAkun].role) != EOF) {
        jumlahAkun++;
    }

    fclose(file);
}
// Membaca data alat dari file
void loadAlat() {
    FILE *file = fopen("alat.txt", "r");
    if (file == NULL) {
        printf("File alat tidak ditemukan!\n");
        return;
    }

    while (fscanf(file, "%s %d",
                  alat[jumlahAlat].nama,
                  &alat[jumlahAlat].stok) != EOF) {
        jumlahAlat++;
    }

    fclose(file);
}

// Menyimpan data alat ke file
void saveAlat() {
    FILE *file = fopen("alat.txt", "w");

    for (int i = 0; i < jumlahAlat; i++) {
        fprintf(file, "%s %d\n",
                alat[i].nama,
                alat[i].stok);
    }

    fclose(file);
}

// Simpan data peminjaman
void simpanPeminjaman(char username[], char namaAlat[]) {
    FILE *file = fopen("peminjaman.txt", "a");
    fprintf(file, "%s %s 1\n", username, namaAlat);
    fclose(file);
}

}
