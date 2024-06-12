#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 256  // Increased buffer size to handle longer lines

typedef struct Pasien {
    int no;
    char nama[MAX_STR];
    char alamat[MAX_STR];
    char kota[MAX_STR];
    char tempat_lahir[MAX_STR];
    char tanggal_lahir[MAX_STR];
    int umur;
    int bpjs;
    char id[MAX_STR];
    struct Pasien *next;
} Pasien;

int add(Pasien **head, int nomor, char tokenNama[], char tokenAlamat[], char tokenKota[], char tokenTempatLahir[], char tokenTanggalLahir[], int umur, int bpjs, char tokenID[]) {
    Pasien *temp = (Pasien *)malloc(sizeof(Pasien));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    temp->no = nomor;
    strcpy(temp->nama, tokenNama);
    strcpy(temp->alamat, tokenAlamat);
    strcpy(temp->kota, tokenKota);
    strcpy(temp->tempat_lahir, tokenTempatLahir);
    strcpy(temp->tanggal_lahir, tokenTanggalLahir);
    temp->umur = umur;
    temp->bpjs = bpjs;
    strcpy(temp->id, tokenID);
    temp->next = NULL;

    if(*head == NULL) {
        *head = temp;
    } else {
        Pasien *current = *head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = temp;
    }

    return 0;
}

Pasien* input_data_pasien() {
    Pasien *data_pasien = NULL;

    char filename[MAX_STR] = "DataPasien.csv";
    FILE* stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File tidak ditemukan\n");
        return NULL;
    }

    char line[MAX_STR];
    while (fgets(line, MAX_STR, stream)) {
        char tokenNomor[MAX_STR], tokenNama[MAX_STR], tokenAlamat[MAX_STR], tokenKota[MAX_STR], tokenTempatLahir[MAX_STR], tokenTanggalLahir[MAX_STR], tokenUmur[MAX_STR], tokenBPJS[MAX_STR], tokenID[MAX_STR];
        int nomor, umur, bpjs;

        strcpy(tokenNomor, strtok(line, ","));
        strcpy(tokenNama, strtok(NULL, ","));
        strcpy(tokenAlamat, strtok(NULL, ","));
        strcpy(tokenKota, strtok(NULL, ","));
        strcpy(tokenTempatLahir, strtok(NULL, ","));
        strcpy(tokenTanggalLahir, strtok(NULL, ","));
        strcpy(tokenUmur, strtok(NULL, ","));
        strcpy(tokenBPJS, strtok(NULL, ","));
        strcpy(tokenID, strtok(NULL, "\n"));

        sscanf(tokenNomor, "%d", &nomor);
        sscanf(tokenUmur, "%d", &umur);
        sscanf(tokenBPJS, "%d", &bpjs);

        add(&data_pasien, nomor, tokenNama, tokenAlamat, tokenKota, tokenTempatLahir, tokenTanggalLahir, umur, bpjs, tokenID);
    }

    fclose(stream);

    return data_pasien;
}

void print(Pasien *head) {
    Pasien* display = head;
    while (display != NULL) {
        printf("%d. Nama: %s \t|| Alamat: %s\t|| Domisili: %s\t|| Lahir di: %s\t|| Tanggal Lahir: %s\t|| Umur: %d\t\t|| BPJS: %d\t|| ID: %s\n", display->no, display->nama, 
        display->alamat, display->kota, display->tempat_lahir, display->tanggal_lahir, display->umur, display->bpjs, display->id);
        display = display->next;
    }
}

int main(void) {
    Pasien *daftar_pasien = input_data_pasien();
    if (daftar_pasien != NULL) {
        print(daftar_pasien);
    } else {
        printf("Tidak ada data!\n");
    }

    return 0;
}