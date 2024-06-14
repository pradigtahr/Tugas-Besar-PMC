#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

// Function to map month names to month numbers
int get_month_number(const char *month_name) {
    if (strcmp(month_name, "Januari") == 0) return 1;
    if (strcmp(month_name, "Februari") == 0) return 2;
    if (strcmp(month_name, "Maret") == 0) return 3;
    if (strcmp(month_name, "April") == 0) return 4;
    if (strcmp(month_name, "Mei") == 0) return 5;
    if (strcmp(month_name, "Juni") == 0) return 6;
    if (strcmp(month_name, "Juli") == 0) return 7;
    if (strcmp(month_name, "Agustus") == 0) return 8;
    if (strcmp(month_name, "September") == 0) return 9;
    if (strcmp(month_name, "Oktober") == 0) return 10;
    if (strcmp(month_name, "November") == 0) return 11;
    if (strcmp(month_name, "Desember") == 0) return 12;
    return 0;
}

// Function to trim trailing whitespace
void trim_trailing_whitespace(char *str) {
    char *end;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';
}

// Function to read CSV data for patient history
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

// Function to calculate monthly and yearly revenue
void hitung_pendapatan(RiwayatPasien *head_riwayat) {
    double pendapatan_bulanan[12] = {0};
    double pendapatan_tahunan = 0;
    RiwayatPasien *current = head_riwayat;

    while (current != NULL) {
        int hari, tahun;
        char bulan_str[20];
        sscanf(current->tanggal_kunjungan, "%d %19s %d", &hari, bulan_str, &tahun);
        int bulan = get_month_number(bulan_str);

        pendapatan_bulanan[bulan - 1] += current->biaya;
        pendapatan_tahunan += current->biaya;
        current = current->next;
    }

    printf("\nPendapatan Bulanan:\n");
    for (int i = 0; i < 12; i++) {
        printf("Bulan %d: Rp.%.2f\n", i + 1, pendapatan_bulanan[i]);
    }
    printf("\nPendapatan Tahunan: Rp.%.2f\n", pendapatan_tahunan);
}

// Function to calculate average yearly revenue
void rata_rata_pendapatan_tahunan(RiwayatPasien *head_riwayat) {
    double total_pendapatan = 0;
    int tahun_pertama = 0, tahun_terakhir = 0;
    int jumlah_tahun = 0;
    RiwayatPasien *current = head_riwayat;

    while (current != NULL) {
        int hari, tahun;
        char bulan_str[20];
        sscanf(current->tanggal_kunjungan, "%d %19s %d", &hari, bulan_str, &tahun);
        total_pendapatan += current->biaya;

        if (tahun_pertama == 0 || tahun < tahun_pertama) {
            tahun_pertama = tahun;
        }
        if (tahun > tahun_terakhir) {
            tahun_terakhir = tahun;
        }

        current = current->next;
    }

    jumlah_tahun = tahun_terakhir - tahun_pertama + 1;
    double rata_rata_tahunan = total_pendapatan / jumlah_tahun;

    printf("\nRata-rata Pendapatan Tahunan: Rp.%.2f\n", rata_rata_tahunan);
}

// Main function
int main() {
    RiwayatPasien *head_riwayat = NULL;
    baca_csv_riwayat("RiwayatPasien.csv", &head_riwayat);
    
    hitung_pendapatan(head_riwayat);
    rata_rata_pendapatan_tahunan(head_riwayat);

    return 0;
}
