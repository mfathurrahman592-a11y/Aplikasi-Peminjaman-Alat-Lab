
#include "header.h"
struct Akun akun[MAX];
struct Alat alat[MAX];

int jumlahAkun = 0, jumlahAlat = 0;

void garis() {
    printf("========================================\n");
}

void tampilAlat() {
    printf("\n=== DATA ALAT ===\n");
    for (int i = 0; i < jumlahAlat; i++) {
        printf("|%2d| ID:%d | %-10s | %-10s | %-10s | %d | stok %3d|\n",
            i + 1,
            alat[i].id,
            alat[i].nama,
            alat[i].merek,
            alat[i].model,
            alat[i].tahunProduksi,
            alat[i].jumlahUnit);
    }
}


void saveAlat() {
    FILE *file = fopen("alat.txt", "w");
    for (int i = 0; i < jumlahAlat; i++) {
        fprintf(file, "%d %s %s %s %d %d\n",
            alat[i].id,
            alat[i].nama,
            alat[i].merek,
            alat[i].model,
            alat[i].tahunProduksi,
            alat[i].jumlahUnit);
    }
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

    while (fscanf(file, "%d %s %s %s %d %d",
                  &alat[jumlahAlat].id,
                  alat[jumlahAlat].nama,
                  alat[jumlahAlat].merek,
                  alat[jumlahAlat].model,
                  &alat[jumlahAlat].tahunProduksi,
                  &alat[jumlahAlat].jumlahUnit) != EOF) {
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
                printf("ID Alat: "); scanf("%d", &alat[jumlahAlat].id);
                printf("Nama alat: "); scanf("%s", alat[jumlahAlat].nama);
                printf("Merek: "); scanf("%s", alat[jumlahAlat].merek);
                printf("Model: "); scanf("%s", alat[jumlahAlat].model);
                printf("Tahun Produksi: "); scanf("%d", &alat[jumlahAlat].tahunProduksi);
                printf("Jumlah Unit: "); scanf("%d", &alat[jumlahAlat].jumlahUnit);

                jumlahAlat++;
                saveAlat();

                printf("\n====================================\n");
                printf("Data alat berhasil ditambahkan!\n");
                printf("====================================\n");
                break;

            case 3:
                tampilAlat();
                printf("%d. Batal\nPilih: ", 0);
                scanf("%d", &nomor);

                if (nomor >= 1 && nomor <= jumlahAlat) {
                    printf("ID baru: ");
                    scanf("%d", &alat[nomor - 1].id);

                    printf("Nama baru: ");
                    scanf("%s", alat[nomor - 1].nama);

                    printf("Merek baru: ");
                    scanf("%s", alat[nomor - 1].merek);

                    printf("Model baru: ");
                    scanf("%s", alat[nomor - 1].model);

                    printf("Tahun Produksi baru: ");
                    scanf("%d", &alat[nomor - 1].tahunProduksi);

                    printf("Jumlah Unit baru: ");
                    scanf("%d", &alat[nomor - 1].jumlahUnit);

                    saveAlat();

                    printf("\n====================================\n");
                    printf("Data alat berhasil diupdate!\n");
                    printf("====================================\n");
                } else {
                    printf("Update dibatalkan atau nomor tidak valid!\n");
                }
                break;

            case 4:
                tampilAlat();
                printf("%d. Batal\nPilih: ", 0);
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
