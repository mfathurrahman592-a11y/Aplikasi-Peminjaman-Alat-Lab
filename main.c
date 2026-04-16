#include "header.h"

// Fungsi User
void tampilAlatUser() {
    printf("\n=== DAFTAR ALAT ===\n");
    for (int i = 0; i < jumlahAlat; i++) {
        printf("|%2d| ID:%d | %-10s | %-10s | %-10s | Stok: %d|\n",
            i + 1,
            alat[i].id,
            alat[i].nama,
            alat[i].merek,
            alat[i].model,
            alat[i].jumlahUnit);
    }
}

void simpanPeminjaman(char username[], char namaAlat[]) {
    FILE *file = fopen("peminjaman.txt", "a");
    fprintf(file, "%s %s 1\n", username, namaAlat);
    fclose(file);
}

void lihatPeminjaman(char username[]) {
    FILE *file = fopen("peminjaman.txt", "r");
    char user[50], nama[50];
    int jumlah, found = 0;

    if (!file) {
        printf("Belum ada data peminjaman.\n");
        return;
    }

    printf("\n=== ALAT DIPINJAM ===\n");

    while (fscanf(file, "%s %s %d", user, nama, &jumlah) != EOF) {
    if (!strcmp(user, username)) {
        printf("%s - %s (%d)\n", user, nama, jumlah);
        found = 1;
    }
    }

    fclose(file);

    if (!found)
        printf("Tidak ada alat dipinjam.\n");

    getchar(); getchar();
}

void kembalikanAlat(char username[]) {
    FILE *file = fopen("peminjaman.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    char user[50], nama[50], input[50];
    int jumlah, found = 0;

    if (!file || !temp) {
        printf("Error membuka file!\n");
        return;
    }

    printf("Nama alat: ");
    scanf("%s", input);

    while (fscanf(file, "%s %s %d", user, nama, &jumlah) != EOF) {
        if (!found && !strcmp(user, username) && !strcmp(nama, input)) {

            for (int i = 0; i < jumlahAlat; i++) {
                if (!strcmp(alat[i].nama, nama)) {
                    alat[i].jumlahUnit += jumlah; 
                }
            }

            found = 1;
        } else {
            fprintf(temp, "%s %s %d\n", user, nama, jumlah);
        }
    }

    fclose(file);
    fclose(temp);

    remove("peminjaman.txt");
    rename("temp.txt", "peminjaman.txt");

    saveAlat();

    printf(found ? "Berhasil dikembalikan!\n" : "Data tidak ditemukan!\n");
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
                printf("%d. Batal\nPilih: ", 0);
                scanf("%d", &nomor);

                if (nomor == 0) {
                    printf("\n====================================\n");
                    printf("Peminjaman dibatalkan.\n");
                    printf("====================================\n");
                    break;
                }

                if (nomor >= 1 && nomor <= jumlahAlat) {
                    if (alat[nomor - 1].jumlahUnit > 0) {

                        alat[nomor - 1].jumlahUnit--;  
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