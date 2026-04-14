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
}
