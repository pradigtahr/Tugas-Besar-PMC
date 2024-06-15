#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 50

// Struktur untuk menyimpan data pasien
typedef struct RiwayatPasien {
    int indeksriwayat;
    char tanggal_kunjungan[MAX_STR];
    char id_pasien[MAX_STR];
    char diagnosis[MAX_STR];
    char tindakan[MAX_STR];
    char kontrol[MAX_STR];
    double biaya;
    struct RiwayatPasien *next;
} RiwayatPasien;

// Struktur untuk menyimpan data hitungan
typedef struct CountData {
    char month[20];
    int year;
    int patientCount;
    char diagnoses[100][50];
    int diagCount;
} CountData;

// Struktur untuk menyimpan data tahunan
typedef struct YearlyData {
    int year;
    int totalPatients;
} YearlyData;

// Fungsi untuk membuat node pasien baru
RiwayatPasien* createPatient(int indeksriwayat, char* tanggal_kunjungan, char* id_pasien, char* diagnosis, char* tindakan, char* kontrol, double biaya) {
    RiwayatPasien* newPasien = (RiwayatPasien*)malloc(sizeof(RiwayatPasien));
    newPasien->indeksriwayat = indeksriwayat;
    strcpy(newPasien->tanggal_kunjungan, tanggal_kunjungan);
    strcpy(newPasien->id_pasien, id_pasien);
    strcpy(newPasien->diagnosis, diagnosis);
    strcpy(newPasien->tindakan, tindakan);
    strcpy(newPasien->kontrol, kontrol);
    newPasien->biaya = biaya;
    newPasien->next = NULL;
    return newPasien;
}

// Fungsi untuk menyisipkan pasien
void insertPatient(RiwayatPasien** head, int indeksriwayat, char* tanggal_kunjungan, char* id_pasien, char* diagnosis, char* tindakan, char* kontrol, double biaya) {
    RiwayatPasien* newPasien = createPatient(indeksriwayat, tanggal_kunjungan, id_pasien, diagnosis, tindakan, kontrol, biaya);
    if (*head == NULL) {
        *head = newPasien;
        return;
    }
    RiwayatPasien* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newPasien;
}

// Fungsi untuk mengekstrak tahun dari string tanggal
int extractYear(char* tanggal_kunjungan) {
    int year;
    sscanf(tanggal_kunjungan + strlen(tanggal_kunjungan) - 4, "%d", &year);
    return year;
}

// Fungsi untuk menghitung pasien dan diagnosis per bulan dan tahun
void countPatients(RiwayatPasien* head, CountData* countData, int* totalRecords, YearlyData* yearlyData, int* totalYears) {
    char months[12][20] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", 
                           "Juli", "Agustus", "September", "Oktober", "November", "Desember"};

    int monthIndex[12][100] = {0};  
    int yearIndex[100] = {0};
    *totalRecords = 0;
    *totalYears = 0;

    RiwayatPasien* temp = head;
    while (temp != NULL) {
        int year = extractYear(temp->tanggal_kunjungan);
        for (int i = 0; i < 12; i++) {
            if (strstr(temp->tanggal_kunjungan, months[i]) != NULL) {
                if (monthIndex[i][year - 2000] == 0) {  // Assumsi tahun dari 2000
                    strcpy(countData[*totalRecords].month, months[i]);
                    countData[*totalRecords].year = year;
                    monthIndex[i][year - 2000] = *totalRecords + 1;
                    (*totalRecords)++;
                }
                countData[monthIndex[i][year - 2000] - 1].patientCount++;
                strcpy(countData[monthIndex[i][year - 2000] - 1].diagnoses[countData[monthIndex[i][year - 2000] - 1].diagCount], temp->diagnosis);
                countData[monthIndex[i][year - 2000] - 1].diagCount++;
                break;
            }
        }
        
        if (yearIndex[year - 2000] == 0) {
            yearlyData[*totalYears].year = year;
            yearIndex[year - 2000] = *totalYears + 1;
            (*totalYears)++;
        }
        yearlyData[yearIndex[year - 2000] - 1].totalPatients++;
        
        temp = temp->next;
    }
}

// Fungsi bubble sort untuk mengurutkan CountData
void bubbleSortCountData(CountData* data, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j].patientCount < data[j + 1].patientCount) {
                CountData temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

// Fungsi bubble sort untuk mengurutkan YearlyData
void bubbleSortYearlyData(YearlyData* data, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j].year > data[j + 1].year) {
                YearlyData temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk mengosongkan memori yang dialokasikan untuk linked list
void freePatients(RiwayatPasien* head) {
    RiwayatPasien* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    FILE *file = fopen("C:\\Users\\Mutia\\Documents\\TUBES PMC\\RiwayatPasien.csv", "r");
    if (!file) {
        printf("File tidak dapat dibuka.\n");
        return 1;
    }

    RiwayatPasien* head = NULL;
    char line[300];
    fgets(line, sizeof(line), file);  // Skip header line

    while (fgets(line, sizeof(line), file)) {
        int indeksriwayat;
        char tanggal_kunjungan[MAX_STR], id_pasien[MAX_STR], diagnosis[MAX_STR], tindakan[MAX_STR], kontrol[MAX_STR];
        double biaya;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%lf", &indeksriwayat, tanggal_kunjungan, id_pasien, diagnosis, tindakan, kontrol, &biaya);
        insertPatient(&head, indeksriwayat, tanggal_kunjungan, id_pasien, diagnosis, tindakan, kontrol, biaya);
    }

    fclose(file);

    CountData countData[120] = {0};  
    YearlyData yearlyData[100] = {0};  
    int totalRecords = 0;
    int totalYears = 0;
    countPatients(head, countData, &totalRecords, yearlyData, &totalYears);

    // Menggunakan Bubble Sort untuk mengurutkan data
    bubbleSortCountData(countData, totalRecords);
    bubbleSortYearlyData(yearlyData, totalYears);

    // Output hasil 
    for (int i = 0; i < totalRecords; i++) {
        printf("Bulan dan Tahun     : %s %d\n", countData[i].month, countData[i].year);
        printf("Jumlah Pasien       : %d\n", countData[i].patientCount);
        printf("Diagnosis Penyakit  : \n");
        for (int j = 0; j < countData[i].diagCount; j++) {
            printf(" - %s\n", countData[i].diagnoses[j]);
        }
        printf("\n");
    }

    // Total Pasien tiap Tahun
    for (int i = 0; i < totalYears; i++) {
        printf("Tahun: %d, Total Jumlah Pasien: %d\n", yearlyData[i].year, yearlyData[i].totalPatients);
    }

    freePatients(head);

    return 0;
}
