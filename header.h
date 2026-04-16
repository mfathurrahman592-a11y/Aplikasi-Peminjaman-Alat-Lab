#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

struct Akun {
    char username[50], password[50], role[20];
};

struct Alat {
    int id;
    char nama[50];
    char merek[50];
    char model[50];
    int tahunProduksi;
    int jumlahUnit;
};

// Global variable
extern struct Akun akun[MAX];
extern struct Alat alat[MAX];
extern int jumlahAkun, jumlahAlat;

// fungsi umum
void garis();
void tampilAlat();
void saveAlat();
void loadAkun();
void loadAlat();

// fungsi admin
void menuAdmin();

// fungsi user
void menuUser(char username[]);

// login
void loginCLA(char username[], char password[]);

#endif