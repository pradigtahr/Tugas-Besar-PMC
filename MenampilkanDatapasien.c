#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Struktur data yang digunakan
typedef struct Pasien {
    int indekspasien;
    char nama_pasien[100];
    char alamat[150];
    char kota[50];
    char tempat_lahir[50];
    char tanggal_lahir[30];
    int umur;
    char nomor_bpjs[20];
    char id_pasien[50];
    struct Pasien *next;
} Pasien;

typedef struct RiwayatPasien {
    int indeksriwayat;
    char tanggal_kunjungan[20];
    char id_pasien[50];
    char diagnosis[100];
    char tindakan[100];
    char kontrol[20];
    double biaya;
    struct RiwayatPasien *next;
} RiwayatPasien;

// Function to trim trailing whitespace //tambahan
void trim_trailing_whitespace(char *str) {
    char *end;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';
}

// Fungsi untuk membaca CSV riwayat
int baca_csv_riwayat(const char *nama_file, RiwayatPasien **head) {
    FILE *file = fopen(nama_file, "r");
    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return 0;
    }

    char baris[500];
    fgets(baris, sizeof(baris), file);  // Membaca header
    while (fgets(baris, sizeof(baris), file)) {
        RiwayatPasien *riwayat = (RiwayatPasien*)malloc(sizeof(RiwayatPasien));
        sscanf(baris, "%d,%19[^,],%49[^,],%99[^,],%99[^,],%19[^,],%lf",
               &riwayat->indeksriwayat, riwayat->tanggal_kunjungan, riwayat->id_pasien,
               riwayat->diagnosis, riwayat->tindakan, riwayat->kontrol, &riwayat->biaya);
        riwayat->next = NULL;
        
        // Trim trailing whitespace
        trim_trailing_whitespace(riwayat->id_pasien);

        if (*head == NULL) {
            *head = riwayat;
        } else {
            RiwayatPasien *temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = riwayat;
        }
    }
    fclose(file);
    return 1;
}

// Fungsi untuk membaca CSV data pasien 
int baca_csv_pasien(const char *nama_file, Pasien **head) {
    FILE *file = fopen(nama_file, "r");
    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return 0;
    }

    char baris[500];
    fgets(baris, sizeof(baris), file);  // Membaca header
    while (fgets(baris, sizeof(baris), file)) {
        Pasien *pasien = (Pasien*)malloc(sizeof(Pasien));
        sscanf(baris, "%d,%99[^,],%149[^,],%49[^,],%49[^,],%29[^,],%d,%19[^,],%49[^,]",
               &pasien->indekspasien, pasien->nama_pasien, pasien->alamat, pasien->kota,
               pasien->tempat_lahir, pasien->tanggal_lahir, &pasien->umur, pasien->nomor_bpjs, pasien->id_pasien);
        pasien->next = NULL;

        // Trim trailing whitespace
        trim_trailing_whitespace(pasien->id_pasien);

        if (*head == NULL) {
            *head = pasien;
        } else {
            Pasien *temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = pasien;
        }
    }
    fclose(file);
    return 1;
}

// Fungsi untuk menampilkan informasi pasien berdasarkan ID pasien //tambahan
void informasi_pasien(Pasien *head_pasien, const char *id_pasien) {
    Pasien *current = head_pasien;
    int found = 0;

    printf("Informasi Pasien dengan ID: %s\n", id_pasien);
    while (current != NULL) {
        if (strcmp(current->id_pasien, id_pasien) == 0) {
            found = 1;
                printf("%d. Nama: %s \t|| Alamat: %s\t|| Domisili: %s\t|| Lahir di: %s\t|| Tanggal Lahir: %s\t|| Umur: %d\t\t|| BPJS: %s\t|| ID: %s\n", current->indekspasien, current->nama_pasien, 
               current->alamat, current->kota, current->tempat_lahir, current->tanggal_lahir, current->umur, current->nomor_bpjs, current->id_pasien);
        }
        current = current->next;
    }

    if (!found) {
        printf("Tidak ada informasi pasien ditemukan untuk ID pasien: %s\n", id_pasien);
    }
}

// Fungsi untuk menampilkan riwayat medis seorang pasien berdasarkan ID pasien //tambahan
void informasi_riwayat_pasien(RiwayatPasien *head_riwayat, const char *id_pasien) {
    RiwayatPasien *current = head_riwayat;
    int found = 0;

    printf("Riwayat Medis :\n");

    while (current != NULL) {
        if (strcmp(current->id_pasien, id_pasien) == 0) {
            found = 1;
            printf("Tanggal : %s,Diagnosis :%s, Tindakan :%s\n",
            current->tanggal_kunjungan, current->diagnosis, current->tindakan);
        }
        current = current->next;
    }
    if (!found) {
        printf("Tidak ada riwayat medis ditemukan untuk ID pasien: %s\n", id_pasien);
    }
}

// Fungsi main
int main() {
    Pasien *head_pasien = NULL;
    RiwayatPasien *head_riwayat = NULL;
    baca_csv_pasien("DataPasien.csv", &head_pasien);
    baca_csv_riwayat("RiwayatPasien.csv", &head_riwayat);
    char id_pasien[50]; //tambahan
                printf("Masukkan ID Pasien: ");
                scanf(" %[^\n]", id_pasien);
                printf("\n");
                informasi_pasien(head_pasien, id_pasien);
                informasi_riwayat_pasien(head_riwayat, id_pasien);
    return 0;
}
