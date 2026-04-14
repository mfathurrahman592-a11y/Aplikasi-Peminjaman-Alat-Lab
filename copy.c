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

// ================= MENU ADMIN =================
void menuAdmin() {
    int pilihan, nomor;
    char namaBaru[50];
    int stokBaru;

    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1. Lihat Data Alat\n");
        printf("2. Tambah Alat\n");
        printf("3. Update Alat\n");
        printf("4. Hapus Alat\n");
        printf("5. Logout\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                printf("\n=== DATA ALAT ===\n");
                for (int i = 0; i < jumlahAlat; i++) {
                    printf("%d. %s - Stok: %d\n",
                           i + 1,
                           alat[i].nama,
                           alat[i].stok);
                }
                printf("\nTekan Enter...");
                getchar();
                getchar();
                break;

            case 2:
                printf("Nama alat: ");
                scanf("%s", alat[jumlahAlat].nama);

                printf("Stok: ");
                scanf("%d", &alat[jumlahAlat].stok);

                jumlahAlat++;
                saveAlat();

                printf("Alat berhasil ditambahkan!\n");
                break;

            case 3:
                printf("Pilih nomor alat yang ingin diupdate: ");
                scanf("%d", &nomor);

                if (nomor >= 1 && nomor <= jumlahAlat) {
                    printf("Nama baru: ");
                    scanf("%s", namaBaru);

                    printf("Stok baru: ");
                    scanf("%d", &stokBaru);

                    strcpy(alat[nomor - 1].nama, namaBaru);
                    alat[nomor - 1].stok = stokBaru;

                    saveAlat();
                    printf("Data berhasil diupdate!\n");
                } else {
                    printf("Nomor tidak valid!\n");
                }
                break;

            case 4:
                printf("Pilih nomor alat yang ingin dihapus: ");
                scanf("%d", &nomor);

                if (nomor >= 1 && nomor <= jumlahAlat) {
                    for (int i = nomor - 1; i < jumlahAlat - 1; i++) {
                        alat[i] = alat[i + 1];
                    }

                    jumlahAlat--;
                    saveAlat();

                    printf("Data berhasil dihapus!\n");
                } else {
                    printf("Nomor tidak valid!\n");
                }
                break;
        }

    } while (pilihan != 5);
}

// ================= USER =================
void lihatPeminjaman(char username[]) {
    FILE *file = fopen("peminjaman.txt", "r");
    char user[50], namaAlat[50];
    int jumlah;
    int found = 0;

    if (file == NULL) {
        printf("Belum ada data peminjaman.\n");
        return;
    }

    printf("\n=== ALAT YANG DIPINJAM ===\n");

    while (fscanf(file, "%s %s %d", user, namaAlat, &jumlah) != EOF) {
        if (strcmp(user, username) == 0) {
            printf("- %s (%d)\n", namaAlat, jumlah);
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada alat yang sedang dipinjam.\n");
    }

    fclose(file);

    printf("\nTekan Enter...");
    getchar();
    getchar();
}

void kembalikanAlat(char username[]) {
    FILE *file = fopen("peminjaman.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    char user[50], namaAlat[50], namaReturn[50];
    int jumlah;
    int found = 0;

    printf("Masukkan nama alat yang ingin dikembalikan: ");
    scanf("%s", namaReturn);

    while (fscanf(file, "%s %s %d", user, namaAlat, &jumlah) != EOF) {
        if (strcmp(user, username) == 0 &&
            strcmp(namaAlat, namaReturn) == 0 &&
            found == 0) {

            found = 1;

            for (int i = 0; i < jumlahAlat; i++) {
                if (strcmp(alat[i].nama, namaAlat) == 0) {
                    alat[i].stok += jumlah;
                    break;
                }
            }

        } else {
            fprintf(temp, "%s %s %d\n", user, namaAlat, jumlah);
        }
    }

    fclose(file);
    fclose(temp);

    remove("peminjaman.txt");
    rename("temp.txt", "peminjaman.txt");

    saveAlat();

    if (found) {
        printf("Alat berhasil dikembalikan!\n");
    } else {
        printf("Data peminjaman tidak ditemukan!\n");
    }
}

void menuUser(char username[]) {
    int pilihan, nomor;

    do {
        printf("\n=== MENU USER ===\n");
        printf("1. Lihat Alat Tersedia\n");
        printf("2. Pinjam Alat\n");
        printf("3. Lihat Alat Dipinjam\n");
        printf("4. Kembalikan Alat\n");
        printf("5. Logout\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                printf("\n=== DAFTAR ALAT ===\n");
                for (int i = 0; i < jumlahAlat; i++) {
                    printf("%d. %s - Stok: %d\n",
                           i + 1,
                           alat[i].nama,
                           alat[i].stok);
                }
                printf("\nTekan Enter...");
                getchar();
                getchar();
                break;

            case 2:
                printf("Pilih nomor alat: ");
                scanf("%d", &nomor);

                if (nomor >= 1 && nomor <= jumlahAlat) {
                    if (alat[nomor - 1].stok > 0) {
                        alat[nomor - 1].stok--;
                        saveAlat();
                        simpanPeminjaman(username, alat[nomor - 1].nama);
                        printf("Peminjaman berhasil!\n");
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
                kembalikanAlat(username);
                break;
        }

    } while (pilihan != 5);
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