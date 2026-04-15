#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

struct Akun {
    char username[50], password[50], role[20];
};

struct Alat {
    char nama[50];
    int stok;
};

struct Akun akun[MAX];
struct Alat alat[MAX];

int jumlahAkun = 0, jumlahAlat = 0;

void garis() {
    printf("========================================\n");
}

void tampilAlat() {
    printf("\n=== DATA ALAT ===\n");
    for (int i = 0; i < jumlahAlat; i++)
        printf("|%2d| |%-20s| stock %3d|\n", i + 1, alat[i].nama, alat[i].stok);
}

void saveAlat() {
    FILE *file = fopen("alat.txt", "w");
    for (int i = 0; i < jumlahAlat; i++)
        fprintf(file, "%s %d\n", alat[i].nama, alat[i].stok);
    fclose(file);
}
// simpan peminjaman alat
void simpanPeminjaman(char username[], char namaAlat[]) {
    FILE *file = fopen("peminjaman.txt", "a");
    fprintf(file, "%s %s 1\n", username, namaAlat);
    fclose(file);
}


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

void menuAdmin() {
    int pilih, nomor;
    char nama[50];
    int stok;

    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1. Lihat Data Alat\n");
        printf("2. Tambah Alat\n");
        printf("3. Update Alat\n");
        printf("4. Hapus Alat\n");
        printf("5. Logout\n");
        printf("Pilih: ");
        scanf("%d", &pilih);

        switch (pilih) {

            case 1:
                tampilAlat();
                printf("\nTekan Enter...");
                getchar(); getchar();
                break;

            case 2:
                printf("Nama alat: ");
                scanf("%s", alat[jumlahAlat].nama);
                printf("Stok: ");
                scanf("%d", &alat[jumlahAlat].stok);

                jumlahAlat++;
                saveAlat();

                printf("\n====================================\n");
                printf("Data alat berhasil ditambahkan!\n");
                printf("====================================\n");
                break;

            case 3:
                tampilAlat();
                printf("%d. Batal\nPilih: ", jumlahAlat + 1);
                scanf("%d", &nomor);

                if (nomor >= 1 && nomor <= jumlahAlat) {
                    printf("Nama baru: ");
                    scanf("%s", nama);
                    printf("Stok baru: ");
                    scanf("%d", &stok);

                    strcpy(alat[nomor - 1].nama, nama);
                    alat[nomor - 1].stok = stok;

                    saveAlat();

                    printf("\n====================================\n");
                    printf(" Data alat berhasil diupdate!\n");
                    printf("====================================\n");
                } else {
                    printf("Update dibatalkan atau nomor tidak valid!\n");
                }
                break;

            case 4:
                tampilAlat();
                printf("%d. Batal\nPilih: ", jumlahAlat + 1);
                scanf("%d", &nomor);

                if (nomor >= 1 && nomor <= jumlahAlat) {
                    for (int i = nomor - 1; i < jumlahAlat - 1; i++)
                        alat[i] = alat[i + 1];

                    jumlahAlat--;
                    saveAlat();

                    printf("\n====================================\n");
                    printf("Data alat berhasil dihapus!\n");
                    printf("====================================\n");
                } else {
                    printf("Hapus dibatalkan atau nomor tidak valid!\n");
                }
                break;
        }

    } while (pilih != 5);
}

void menuUser(char username[]) {
    int pilih, nomor;

    do {
        printf("\n=== MENU USER ===\n");
        printf("1. Lihat Alat\n");
        printf("2. Pinjam Alat\n");
        printf("3. Lihat Peminjaman\n");
        printf("4. Kembalikan Alat\n");
        printf("5. Logout\n");
        printf("Pilih: ");
        scanf("%d", &pilih);

        switch (pilih) {

            case 1:
                garis();
                printf("DAFTAR ALAT TERSEDIA\n");
                garis();
                tampilAlatUser();

                printf("\nTekan Enter...");
                getchar(); getchar();
                break;

            case 2:
                tampilAlatUser();
                printf("%d. Batal\nPilih: ", jumlahAlat + 1);
                scanf("%d", &nomor);

                if (nomor == jumlahAlat + 1) {
                    printf("Peminjaman dibatalkan.\n");
                    break;
                }

                if (nomor >= 1 && nomor <= jumlahAlat) {
                    if (alat[nomor - 1].stok > 0) {
                        alat[nomor - 1].stok--;
                        saveAlat();
                        simpanPeminjaman(username, alat[nomor - 1].nama);

                        printf("\n====================================\n");
                        printf("Berhasil meminjam alat: %s\n", alat[nomor - 1].nama);
                        printf("====================================\n");
                    } else {
                        printf("Stok habis!\n");
                    }
                } else {
                    printf("Pilihan tidak valid!\n");
                }
                break;

            case 3:
                lihatPeminjaman(username);
                break;

            case 4:
                lihatPeminjaman(username);
                kembalikanAlat(username);

                printf("\n====================================\n");
                printf("Proses pengembalian selesai!\n");
                printf("====================================\n");
                break;
        }

    } while (pilih != 5);
}

// ================= LOGIN CLA =================
void loginCLA(char username[], char password[]) {
    int berhasil = 0;

    for (int i = 0; i < jumlahAkun; i++) {
        if (strcmp(username, akun[i].username) == 0 &&
            strcmp(password, akun[i].password) == 0) {

            berhasil = 1;

            printf("Login berhasil sebagai %s\n", akun[i].role);

            if (strcmp(akun[i].role, "admin") == 0) {
                menuAdmin();
            } else {
                menuUser(username);
            }
            break;
        }
    }

    if (!berhasil) {
        printf("Login gagal! Username atau password salah.\n");
    }
}

// ================= MAIN =================
int main(int argc, char *argv[]) {
    loadAkun();
    loadAlat();

    if (argc != 3) {
        printf("Cara penggunaan: %s <username> <password>\n", argv[0]);
        return 1;
    }

    loginCLA(argv[1], argv[2]);

    return 0;
}
