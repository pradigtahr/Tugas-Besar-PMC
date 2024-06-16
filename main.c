#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_STR 256

// Deklarasi Struktur
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


// -------------------------- FUNGSI PARSING -------------------------------------------
int add_pasien(Pasien **head, int nomor, char tokenNama[], char tokenAlamat[], char tokenKota[], char tokenTempatLahir[], char tokenTanggalLahir[], int umur, int bpjs, char tokenID[]) {
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

    if (*head == NULL) {
        *head = temp;
    } else {
        Pasien *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = temp;
    }

    return 0;
}

int add_riwayat(RiwayatPasien **head, int nomor, char tokenTanggal[MAX_STR], char tokenID[MAX_STR], char tokenDiagnosis[MAX_STR], char tokenTindakan[MAX_STR], char tokenKontrol[MAX_STR], float biaya) {
    RiwayatPasien *temp = (RiwayatPasien*)malloc(sizeof(Pasien));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    temp->indeksriwayat = nomor;
    strcpy(temp->tanggal_kunjungan, tokenTanggal);
    strcpy(temp->id_pasien, tokenID);
    strcpy(temp->diagnosis, tokenDiagnosis);
    strcpy(temp->tindakan, tokenTindakan);
    strcpy(temp->kontrol, tokenKontrol);
    temp->biaya = biaya;
    temp->next = NULL;

    if (*head == NULL) {
        *head = temp;
    } else {
        RiwayatPasien *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = temp;
    }

    return 0;
}

Pasien *input_data_pasien() {
    Pasien *data_pasien = NULL;

    char filename[MAX_STR] = "DataPasien.csv";
    FILE *stream = fopen(filename, "r");
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

        add_pasien(&data_pasien, nomor, tokenNama, tokenAlamat, tokenKota, tokenTempatLahir, tokenTanggalLahir, umur, bpjs, tokenID);
    }

    fclose(stream);

    return data_pasien;
}

RiwayatPasien *input_riwayat_pasien() {
    RiwayatPasien *riwayat_pasien = NULL;

    char filename[MAX_STR] = "RiwayatPasien.csv";
    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("File tidak ditemukan\n");
        return NULL;
    }

    char line[MAX_STR];
    while (fgets(line, MAX_STR, stream)) {
        char tokenNomor[MAX_STR], tokenTanggal[MAX_STR], tokenID[MAX_STR], tokenDiagnosis[MAX_STR], tokenTindakan[MAX_STR], tokenKontrol[MAX_STR], tokenBiaya[MAX_STR];
        int nomor;
        double biaya;

        strcpy(tokenNomor, strtok(line, ","));
        strcpy(tokenTanggal, strtok(NULL, ","));
        strcpy(tokenID, strtok(NULL, ","));
        strcpy(tokenDiagnosis, strtok(NULL, ","));
        strcpy(tokenTindakan, strtok(NULL, ","));
        strcpy(tokenKontrol, strtok(NULL, ","));
        strcpy(tokenBiaya, strtok(NULL, "\n"));

        sscanf(tokenNomor, "%d", &nomor);
        sscanf(tokenBiaya, "%lf", &biaya);

        add_riwayat(&riwayat_pasien, nomor, tokenTanggal, tokenID, tokenDiagnosis, tokenTindakan, tokenKontrol, biaya);
    }

    fclose(stream);

    return riwayat_pasien;
}

void header(void){
    printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n");
    printf("TUGAS BESAR\n");
    printf("EL2208 - PEMECAHAN MASSALAH DENGAN C\n");
    printf("Aplikasi Pencatatan Database Pasien\n");
    printf("Silakan pilih opsi menu yang sesuai (1-6)\n");
    printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n");
}

//-------------------------------------------------------------------------------------------

// -------------------------- FUNGSI BAGIAN 1 -----------------------------------------------
void save_to_csv(const char *filename, int nomor, char nama[], char alamat[], char kota[], char tempatLahir[], char tanggalLahir[], int umur, int bpjs, char id[]) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "\n%d,%s,%s,%s,%s,%s,%d,%d,%s", nomor, nama, alamat, kota, tempatLahir, tanggalLahir, umur, bpjs, id);
    fclose(file);
}

void add_new_patient(Pasien **head, int nomor, char tokenNama[], char tokenAlamat[], char tokenKota[], char tokenTempatLahir[], char tokenTanggalLahir[], int umur, int bpjs, char tokenID[]) {
    // Tambahkan ke dalam linked list
    add_pasien(head, nomor, tokenNama, tokenAlamat, tokenKota, tokenTempatLahir, tokenTanggalLahir, umur, bpjs, tokenID);
    // Tambahkan ke dalam file csv
    save_to_csv("DataPasien.csv", nomor, tokenNama, tokenAlamat, tokenKota, tokenTempatLahir, tokenTanggalLahir, umur, bpjs, tokenID);
}

void get_patient_data(int *nomor, char nama[], char alamat[], char kota[], char tempatLahir[], char tanggalLahir[], int *umur, int *bpjs, char id[]) {
    printf("Masukkan nomor pasien: ");
    scanf("%d", nomor);
    getchar();

    printf("Masukkan nama lengkap: ");
    fgets(nama, MAX_STR, stdin);
    nama[strcspn(nama, "\n")] = '\0';

    printf("Masukkan alamat: ");
    fgets(alamat, MAX_STR, stdin);
    alamat[strcspn(alamat, "\n")] = '\0';

    printf("Masukkan kota: ");
    fgets(kota, MAX_STR, stdin);
    kota[strcspn(kota, "\n")] = '\0';

    printf("Masukkan tempat lahir: ");
    fgets(tempatLahir, MAX_STR, stdin);
    tempatLahir[strcspn(tempatLahir, "\n")] = '\0';

    printf("Masukkan tanggal lahir (<Tanggal>, <Bulan> <Tahun>): ");
    fgets(tanggalLahir, MAX_STR, stdin);
    tanggalLahir[strcspn(tanggalLahir, "\n")] = '\0';

    printf("Masukkan umur: ");
    scanf("%d", umur);

    printf("Masukkan nomor BPJS: ");
    scanf("%d", bpjs);
    getchar();

    printf("Masukkan ID pasien: ");
    fgets(id, MAX_STR, stdin);
    id[strcspn(id, "\n")] = '\0';
}

void get_patient_data2(int *nomor, char nama[], char alamat[], char kota[], char tempatLahir[], char tanggalLahir[], int *umur, int *bpjs) {
    printf("Masukkan nomor pasien: ");
    scanf("%d", nomor);
    getchar();

    printf("Masukkan nama lengkap: ");
    fgets(nama, MAX_STR, stdin);
    nama[strcspn(nama, "\n")] = '\0';

    printf("Masukkan alamat: ");
    fgets(alamat, MAX_STR, stdin);
    alamat[strcspn(alamat, "\n")] = '\0';

    printf("Masukkan kota: ");
    fgets(kota, MAX_STR, stdin);
    kota[strcspn(kota, "\n")] = '\0';

    printf("Masukkan tempat lahir: ");
    fgets(tempatLahir, MAX_STR, stdin);
    tempatLahir[strcspn(tempatLahir, "\n")] = '\0';

    printf("Masukkan tanggal lahir (<Tanggal>, <Bulan> <Tahun>): ");
    fgets(tanggalLahir, MAX_STR, stdin);
    tanggalLahir[strcspn(tanggalLahir, "\n")] = '\0';

    printf("Masukkan umur: ");
    scanf("%d", umur);

    printf("Masukkan nomor BPJS: ");
    scanf("%d", bpjs);
    getchar();
}

Pasien createNode(int nomor, char nama[], char alamat[], char kota[], char tempatLahir[], char tanggalLahir[], int umur, int bpjs, char id[]) {
    Pasien updated_patient;
    updated_patient.no = nomor;
    strcpy(updated_patient.nama, nama);
    strcpy(updated_patient.alamat, alamat);
    strcpy(updated_patient.kota, kota);
    strcpy(updated_patient.tempat_lahir, tempatLahir);
    strcpy(updated_patient.tanggal_lahir, tanggalLahir);
    updated_patient.umur = umur;
    updated_patient.bpjs = bpjs;
    strcpy(updated_patient.id, id);

    return updated_patient;
}

void print(Pasien *head, char id[]) {
    Pasien *display = head;
    int found = 0;
    while (display != NULL) {
        if (strcmp(display->id, id) == 0) {
            found = 1;
            printf("%d. Nama: %s \t|| Alamat: %s\t|| Domisili: %s\t|| Lahir di: %s\t|| Tanggal Lahir: %s\t|| Umur: %d\t\t|| BPJS: %d\t|| ID: %s\n", display->no, display->nama, display->alamat, display->kota, display->tempat_lahir, display->tanggal_lahir, display->umur, display->bpjs, display->id);
        }
        display = display->next;
    }

    if (!found){
        printf("Tidak ada pasien dengan ID tersebut.\n");
    }
}

int search_id(Pasien *head,  char id[]){
    Pasien *display = head;
    int found = 0;
    while (display != NULL) {
        if (strcmp(display->id, id) == 0) {
            found = 1;
            return 1;
        }
        display = display->next;
    }

    if (!found){
        return 0;
    }
}

// Fungsi untuk update linked list
void update_linked_list(Pasien *head, Pasien updated_patient) {
    Pasien *current = head;
    while (current != NULL) {
        if (strcmp(current->id, updated_patient.id) == 0) {
            // Update the patient data in the linked list
            current->no = updated_patient.no;
            strcpy(current->nama, updated_patient.nama);
            strcpy(current->alamat, updated_patient.alamat);
            strcpy(current->kota, updated_patient.kota);
            strcpy(current->tempat_lahir, updated_patient.tempat_lahir);
            strcpy(current->tanggal_lahir, updated_patient.tanggal_lahir);
            current->umur = updated_patient.umur;
            current->bpjs = updated_patient.bpjs;
            break;
        }
        current = current->next;
    }
}

// Fungsi untuk update file csv
void update_csv_and_list(Pasien *head, const char *filename, char id[], Pasien updated_patient) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    FILE *tempfile = fopen("temp.csv", "w");
    if (!tempfile) {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    char line[MAX_STR];
    char temp_id[MAX_STR];
    int updated = 0;
    int first_line = 1;

    while (fgets(line, sizeof(line), file)) {
        // Ekstrak id saat ini
        sscanf(line, "%*d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*d,%*d,%[^,\n]", temp_id);

        // Tulis ke dalam file data yang baru
        if (strcmp(temp_id, id) == 0) {
            fprintf(tempfile, "%d,%s,%s,%s,%s,%s,%d,%d,%s\n", updated_patient.no, updated_patient.nama, updated_patient.alamat, 
            updated_patient.kota, updated_patient.tempat_lahir, updated_patient.tanggal_lahir, updated_patient.umur, 
            updated_patient.bpjs, updated_patient.id);
            updated = 1;
        } else {
            fputs(line, tempfile);
        }

        first_line = 0;
    }

    fclose(file);
    fclose(tempfile);

    if (updated) {
        remove(filename);
        rename("temp.csv", filename);
        update_linked_list(head, updated_patient);
        printf("Data pasien dengan ID %s berhasil diubah.\n", id);
    } else {
        remove("temp.csv");
        printf("Data pasien dengan ID %s tidak ditemukan.\n", id);
    }
}

void delete_patient(Pasien **head, const char *filename, char id[]) {
    Pasien *prev = NULL;
    Pasien *current = *head;
    int found = 0;

    // hapus dari linked list
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            found = 1;
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    // hapus dari file
    if (found) {
        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("Error opening file");
            return;
        }

        FILE *tempfile = fopen("temp.csv", "w");
        if (!tempfile) {
            perror("Error opening temporary file");
            fclose(file);
            return;
        }

        char line[MAX_STR];
        char temp_id[MAX_STR];

        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%*d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*d,%*d,%[^,\n]", temp_id);

            if (strcmp(temp_id, id) == 0) {
                continue; // line dengan id spesifik dihapus/dilewati
            }

            else (fputs(line, tempfile));
        }

        fclose(file);
        fclose(tempfile);

        // Tukar file original dengan file yang sudah diperbaharui
        remove(filename);
        rename("temp.csv", filename);
        printf("Data pasien dengan ID %s berhasil dihapus.\n", id);

    } else {
        printf("Pasien dengan ID %s tidak ditemukan file.\n", id);
    }
}
//---------------------------------------------------------------------------------------------

// -------------------------- FUNGSI BAGIAN 2 -------------------------------------------------
#define FILENAME "RiwayatPasien.csv"
#define MAX_LINE_LENGTH 1024

// Fungsi untuk melakukan validasi format tanggal
int validate_date(const char* date) {
    char day[3], month[10], year[5];
    if (sscanf(date, "%2s %9s %4s", day, month, year) != 3) {
        return 0;
    }
    int day_num = atoi(day);
    if (day_num < 1 || day_num > 31) {
        return 0;
    }
    return 1;
}

// Fungsi untuk melakukan format terhadap tanggal
void format_date(char* date) {
    char day[3], month[10], year[5];
    sscanf(date, "%2s %9s %4s", day, month, year);
    sprintf(date, "%s %s %s", day, month, year);
}

// Fungsi untuk menambahkan riwayat pasien
void add_record() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Tidak dapat membuka file.\n");
        return;
    }

    char nomor[10], tanggal[30], id_pasien[20], diagnosis[100], tindakan[100], kontrol[30], biaya[20];
    printf("Masukkan Nomor: ");
    fgets(nomor, sizeof(nomor), stdin);
    nomor[strcspn(nomor, "\n")] = 0; // Menghapus newline

    printf("Masukkan Tanggal (DD Bulan YYYY): ");
    fgets(tanggal, sizeof(tanggal), stdin);
    tanggal[strcspn(tanggal, "\n")] = 0; // Menghapus newline
    if (!validate_date(tanggal)) {
        printf("Format tanggal tidak valid.\n");
        fclose(file);
        return;
    }

    printf("Masukkan ID Pasien: ");
    fgets(id_pasien, sizeof(id_pasien), stdin);
    id_pasien[strcspn(id_pasien, "\n")] = 0; // Menghapus newline

    printf("Masukkan Diagnosis: ");
    fgets(diagnosis, sizeof(diagnosis), stdin);
    diagnosis[strcspn(diagnosis, "\n")] = 0; // Menghapus newline

    printf("Masukkan Tindakan: ");
    fgets(tindakan, sizeof(tindakan), stdin);
    tindakan[strcspn(tindakan, "\n")] = 0; // Menghapus newline

    printf("Masukkan Tanggal Kontrol: ");
    fgets(kontrol, sizeof(kontrol), stdin);
    kontrol[strcspn(kontrol, "\n")] = 0; // Menghapus newline

    printf("Masukkan Biaya: Rp");
    fgets(biaya, sizeof(biaya), stdin);
    biaya[strcspn(biaya, "\n")] = 0; // Menghapus newline

    fprintf(file, "%s,%s,%s,%s,%s,%s,%s\n", nomor, tanggal, id_pasien, diagnosis, tindakan, kontrol, biaya);
    fclose(file);

    printf("Riwayat pasien berhasil ditambahkan.\n");
}

// Fungsi untuk melakukan perubahan terhadap riwayat pasien
void update_record() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Tidak dapat membuka file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char id_pasien[20];
    int count = 0;
    char matched_line[MAX_LINE_LENGTH];

    printf("Masukkan ID Pasien yang akan diubah: ");
    fgets(id_pasien, sizeof(id_pasien), stdin);
    id_pasien[strcspn(id_pasien, "\n")] = 0; // Menghapus newline

    while (fgets(line, sizeof(line), file)) {
        char old_id_pasien[20];
        sscanf(line, "%*[^,],%*[^,],%19[^,],%*[^,],%*[^,],%*[^,],%*s", old_id_pasien);

        if (strcmp(old_id_pasien, id_pasien) == 0) {
            count++;
            strcpy(matched_line, line); // Simpan baris yang cocok
        }
    }

    fclose(file);

    if (count == 0) {
        printf("Tidak ada riwayat pasien yang cocok dengan ID tersebut.\n");
        return;
    }

    char tanggal[30];

    if (count > 1) {
        printf("Terdapat %d riwayat dengan ID %s.\n", count, id_pasien);
        printf("Masukkan Tanggal (DD Bulan YYYY) yang akan diubah: ");
        fgets(tanggal, sizeof(tanggal), stdin);
        tanggal[strcspn(tanggal, "\n")] = 0; // Menghapus newline
        if (!validate_date(tanggal)) {
            printf("Format tanggal tidak valid.\n");
            return;
        }
    } else {
        // Ekstrak tanggal dari satu-satunya baris yang cocok
        sscanf(matched_line, "%*[^,],%29[^,],%*s", tanggal);
    }

    // Cari apakah ada data yang cocok
    file = fopen(FILENAME, "r");
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char nomor[10], old_tanggal[30], old_id_pasien[20], diagnosis[100], tindakan[100], kontrol[30], biaya[20];
        sscanf(line, "%9[^,],%29[^,],%19[^,],%99[^,],%99[^,],%29[^,],%19[^\n]", nomor, old_tanggal, old_id_pasien, diagnosis, tindakan, kontrol, biaya);

        if (strcmp(old_id_pasien, id_pasien) == 0 && strcmp(old_tanggal, tanggal) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Tidak ada riwayat pasien yang cocok dengan ID dan tanggal tersebut.\n");
        return;
    }

    // Input data baru jika data ditemukan
    printf("Masukkan Tanggal Baru (DD Bulan YYYY): ");
    char new_tanggal[30];
    fgets(new_tanggal, sizeof(new_tanggal), stdin);
    new_tanggal[strcspn(new_tanggal, "\n")] = 0; // Menghapus newline
    if (!validate_date(new_tanggal)) {
        printf("Format tanggal tidak valid.\n");
        return;
    }

    printf("Masukkan Diagnosis Baru: ");
    char new_diagnosis[100];
    fgets(new_diagnosis, sizeof(new_diagnosis), stdin);
    new_diagnosis[strcspn(new_diagnosis, "\n")] = 0; // Menghapus newline

    printf("Masukkan Tindakan Baru: ");
    char new_tindakan[100];
    fgets(new_tindakan, sizeof(new_tindakan), stdin);
    new_tindakan[strcspn(new_tindakan, "\n")] = 0; // Menghapus newline

    printf("Masukkan Tanggal Kontrol Baru (DD Bulan YYYY): ");
    char new_kontrol[30];
    fgets(new_kontrol, sizeof(new_kontrol), stdin);
    new_kontrol[strcspn(new_kontrol, "\n")] = 0; // Menghapus newline
    if (!validate_date(new_kontrol)) {
        printf("Format tanggal kontrol tidak valid.\n");
        return;
    }

    printf("Masukkan Biaya Baru: Rp");
    char new_biaya[20];
    fgets(new_biaya, sizeof(new_biaya), stdin);
    new_biaya[strcspn(new_biaya, "\n")] = 0; // Menghapus newline

    // Buka file lagi untuk menulis
    file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Tidak dapat membuat file sementara.\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char nomor[10], old_tanggal[30], old_id_pasien[20], diagnosis[100], tindakan[100], kontrol[30], biaya[20];
        sscanf(line, "%9[^,],%29[^,],%19[^,],%99[^,],%99[^,],%29[^,],%19[^\n]", nomor, old_tanggal, old_id_pasien, diagnosis, tindakan, kontrol, biaya);

        if (strcmp(old_id_pasien, id_pasien) == 0 && strcmp(old_tanggal, tanggal) == 0) {
            fprintf(temp, "%s,%s,%s,%s,%s,%s,%s\n", nomor, new_tanggal, old_id_pasien, new_diagnosis, new_tindakan, new_kontrol, new_biaya);
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.csv", FILENAME);

    printf("Riwayat pasien berhasil diubah.\n");
}

// Fungsi untuk melakukan pencarian data riwayat pasien berdasarkan ID pasien
void search_record() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Tidak dapat membuka file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char id_pasien[20];
    printf("Masukkan ID Pasien yang akan dicari: ");
    fgets(id_pasien, sizeof(id_pasien), stdin);
    id_pasien[strcspn(id_pasien, "\n")] = 0; // Menghapus newline
    printf("\n");

    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char nomor[10], tanggal[30], old_id_pasien[20], diagnosis[100], tindakan[100], kontrol[30], biaya[20];
        sscanf(line, "%9[^,],%29[^,],%19[^,],%99[^,],%99[^,],%29[^,],%19[^\n]", nomor, tanggal, old_id_pasien, diagnosis, tindakan, kontrol, biaya);

        if (strcmp(old_id_pasien, id_pasien) == 0) {
            printf("Nomor: %s\nTanggal: %s\nID Pasien: %s\nDiagnosis: %s\nTindakan: %s\nTanggal Kontrol: %s\nBiaya: Rp%s\n\n",
                   nomor, tanggal, old_id_pasien, diagnosis, tindakan, kontrol, biaya);
            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada riwayat pasien yang cocok dengan ID tersebut.\n");
    }

    fclose(file);
}

// Fungsi untuk menghapus riwayat pasien tertentu berdasarkan ID pasien dan tanggal kunjungan
void delete_record() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Tidak dapat membuka file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char id_pasien[20];
    int count = 0;
    char matched_line[MAX_LINE_LENGTH];

    printf("Masukkan ID Pasien yang akan dihapus: ");
    fgets(id_pasien, sizeof(id_pasien), stdin);
    id_pasien[strcspn(id_pasien, "\n")] = 0; // Menghapus newline

    while (fgets(line, sizeof(line), file)) {
        char old_id_pasien[20];
        sscanf(line, "%*[^,],%*[^,],%19[^,],%*[^,],%*[^,],%*[^,],%*s", old_id_pasien);

        if (strcmp(old_id_pasien, id_pasien) == 0) {
            count++;
            strcpy(matched_line, line); // Simpan baris yang cocok
        }
    }

    fclose(file);

    if (count == 0) {
        printf("Tidak ada riwayat pasien yang cocok dengan ID tersebut.\n");
        return;
    }

    char tanggal[30];

    if (count > 1) {
        printf("Terdapat %d riwayat dengan ID %s.\n", count, id_pasien);
        printf("Masukkan Tanggal (DD Bulan YYYY) yang akan dihapus: ");
        fgets(tanggal, sizeof(tanggal), stdin);
        tanggal[strcspn(tanggal, "\n")] = 0; // Menghapus newline
        if (!validate_date(tanggal)) {
            printf("Format tanggal tidak valid.\n");
            return;
        }
    } else {
        // Ekstrak tanggal dari satu-satunya baris yang cocok
        sscanf(matched_line, "%*[^,],%29[^,],%*s", tanggal);
    }

    // Buka file lagi untuk menghapus
    file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Tidak dapat membuat file sementara.\n");
        fclose(file);
        return;
    }

    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char nomor[10], old_tanggal[30], old_id_pasien[20], diagnosis[100], tindakan[100], kontrol[30], biaya[20];
        sscanf(line, "%9[^,],%29[^,],%19[^,],%99[^,],%99[^,],%29[^,],%19[^\n]", nomor, old_tanggal, old_id_pasien, diagnosis, tindakan, kontrol, biaya);

        if (strcmp(old_id_pasien, id_pasien) == 0 && strcmp(old_tanggal, tanggal) == 0) {
            found = 1;
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if (found) {
        printf("Riwayat pasien berhasil dihapus.\n");
    } else {
        printf("Tidak ada riwayat pasien yang cocok dengan ID dan tanggal tersebut.\n");
    }
}
// --------------------------------------------------------------------------------------------

// -------------------------- FUNGSI BAGIAN 3 -------------------------------------------------
void informasi_pasien(Pasien *head_pasien, char id_pasien[]) {
    Pasien *current = head_pasien;
    int found = 0;

    printf("Informasi Pasien dengan ID: %s\n", id_pasien);
    while (current != NULL) {
        if (strcmp(current->id, id_pasien) == 0) {
            found = 1;
            printf("%d. Nama: %s \t|| Alamat: %s\t|| Domisili: %s\t|| Lahir di: %s\t|| Tanggal Lahir: %s\t|| Umur: %d\t\t|| BPJS: %d\t|| ID: %s\n", current->no, current->nama, 
            current->alamat, current->kota, current->tempat_lahir, current->tanggal_lahir, current->umur, current->bpjs, current->id);
        }
        current = current->next;
    }

    if (!found) {
        printf("Tidak ada informasi pasien yang ditemukan untuk ID pasien: %s\n", id_pasien);
    }
}

void informasi_riwayat_pasien(RiwayatPasien *head_riwayat, char id_pasien[]) {
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
// --------------------------------------------------------------------------------------------

// -------------------------- FUNGSI BAGIAN 4 ----------------------------------------------
int get_month_number(char month_name[]) {
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
// --------------------------------------------------------------------------------------------

// -------------------------- FUNGSI BAGIAN 5 -------------------------------------------------
// Fungsi untuk mengekstrak tahun dari string tanggal
int extractYear(char* tanggal) {
    int year;
    sscanf(tanggal + strlen(tanggal) - 4, "%d", &year);
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


// --------------------------------------------------------------------------------------------

// -------------------------- FUNGSI BAGIAN 6 ---------------------------------------------
void cariKontrol(const char *id_pasien, RiwayatPasien *data) {
    RiwayatPasien *current = data;
    int found = 0;

    printf("\nInformasi kontrol untuk pasien dengan ID %s:\n", id_pasien);
    while (current != NULL) {
        if (strcmp(current->id_pasien, id_pasien) == 0) {
            printf("No: %d\nTanggal: %s\nDiagnosis: %s\nTindakan: %s\nKontrol: %s\nBiaya: %.2lf\n\n",
                    current->indeksriwayat, current->tanggal_kunjungan, current->diagnosis, current->tindakan, 
                    current->kontrol, current->biaya);
            found = 1;
        }
        current = current->next;
    }
    if (!found) {
        printf("Pasien dengan ID %s tidak ditemukan.\n", id_pasien);
    }
}
// --------------------------------------------------------------------------------------------

void start_program() {
    // Parsing data pasien
    Pasien *data_pasien = input_data_pasien();

    // Parsing data riwayat pasien
    RiwayatPasien *riwayat_pasien = input_riwayat_pasien();
    int opsi;

    do {
        header();
        printf("1. Sunting/Cari data pasien\n");
        printf("2. Sunting/Cari riwayat, diagnosis, dan tindakan pada pasien\n");
        printf("3. Cek informasi dan riwayat medis pasien\n");
        printf("4. Cek pendapatan bulanan, tahunan, dan rata-rata pendapatan per-tahun\n");
        printf("5. Cek jumlah pasien dan penyakit yang diderita pasien\n");
        printf("6. Cek informasi pasien yang memerlukan kontrol\n");
        printf("7. Keluar\n");

        // Mendapatkan input user
        printf(">> ");
        scanf("%d", &opsi);
        getchar();  // To consume the newline character left by scanf

        switch (opsi) {
        // Punya digta
        case 1: {
            char choice1[MAX_STR];
            printf("Apa yang ingin anda sunting (tambah/ubah/hapus/cari)?\n");
            printf(">> ");
            fgets(choice1, MAX_STR, stdin);
            choice1[strcspn(choice1, "\n")] = '\0';  // Remove the newline character

            if (strcmp(choice1, "tambah") == 0) {
                int nomor, umur, bpjs;
                char nama[MAX_STR], alamat[MAX_STR], kota[MAX_STR], tempatLahir[MAX_STR], tanggalLahir[MAX_STR], id[MAX_STR];
                get_patient_data(&nomor, nama, alamat, kota, tempatLahir, tanggalLahir, &umur, &bpjs, id);
                add_new_patient(&data_pasien, nomor, nama, alamat, kota, tempatLahir, tanggalLahir, umur, bpjs, id);
                printf("Pasien baru telah ditambahkan.\n");

            } else if (strcmp(choice1, "ubah") == 0) {
                char id[MAX_STR];
                int found;

                // input id pasien
                printf("Masukkan ID pasien yang ingin diubah: ");
                fgets(id, MAX_STR, stdin);
                id[strcspn(id, "\n")] = '\0';

                // cari id
                found = search_id(data_pasien, id);

                if (found) {
                    // Mendapatkan data-data dari pasien yang diperbaharui
                    int nomor, umur, bpjs;
                    char nama[MAX_STR], alamat[MAX_STR], kota[MAX_STR], tempatLahir[MAX_STR], tanggalLahir[MAX_STR];
                    get_patient_data2(&nomor, nama, alamat, kota, tempatLahir, tanggalLahir, &umur, &bpjs);

                    // Membuat node baru sebagai elemen linked list yang diubah
                    Pasien updatedPatient = createNode(nomor, nama, alamat, kota, tempatLahir, tanggalLahir, umur, bpjs, id);

                    // Update file csv DataPasien dan linked list
                    update_csv_and_list(data_pasien, "DataPasien.csv", id, updatedPatient);
                    

                } else {
                    printf("Pasien dengan ID %s tidak ditemukan.\n", id);
                }
            
            } else if (strcmp(choice1, "hapus") == 0) {
                char id[MAX_STR];
                printf("Masukkan ID pasien yang ingin dihapus: ");
                fgets(id, MAX_STR, stdin);
                id[strcspn(id, "\n")] = '\0';

                delete_patient(&data_pasien, "DataPasien.csv", id);
            
            } else if (strcmp(choice1, "cari") == 0) {
                char id[MAX_STR];
                // input id pasien
                printf("Masukkan ID pasien yang ingin dicari: ");
                fgets(id, MAX_STR, stdin);
                id[strcspn(id, "\n")] = '\0';
                
                // Cari pasien
                print(data_pasien, id);
            } else {
                printf("Input tidak valid!\n");
            }
            break;
        }

        // Punya gastya
        case 2: {
            int choice;
            while (1) {
                printf("\nMenu:\n");
                printf("1. Tambah Riwayat Pasien\n");
                printf("2. Ubah Riwayat Pasien\n");
                printf("3. Cari Riwayat Pasien\n");
                printf("4. Hapus Riwayat Pasien\n");
                printf("5. Ke menu utama\n");
                printf("Pilih opsi: ");
                scanf("%d", &choice);
                getchar(); // Membersihkan buffer newline

                switch (choice) {
                    case 1:
                        add_record();
                        break;
                    case 2:
                        update_record();
                        break;
                    case 3:
                        search_record();
                        break;
                    case 4:
                        delete_record();
                        break;
                    case 5:
                        printf("\n");
                        goto exit_submenu;
                    default:
                        printf("Opsi tidak valid. Silakan coba lagi.\n");
                }
            }
            exit_submenu: break;
        }
            
        // Punya Issa
        case 3:{
            char id_pasien[MAX_STR];
            printf("Masukkan ID Pasien: ");
            fgets(id_pasien, MAX_STR, stdin);
            id_pasien[strcspn(id_pasien, "\n")] = '\0';

            // Display patient information
            informasi_pasien(data_pasien, id_pasien);

            // Display patient medical history
            informasi_riwayat_pasien(riwayat_pasien, id_pasien);
            break;
        }
        
        case 4:{
            hitung_pendapatan(riwayat_pasien);
            rata_rata_pendapatan_tahunan(riwayat_pasien);

            break;
        }

        case 5:{
                CountData countData[120] = {0};  
                YearlyData yearlyData[100] = {0};  
                int totalRecords = 0;
                int totalYears = 0;
                countPatients(riwayat_pasien, countData, &totalRecords, yearlyData, &totalYears);

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

            break;
        }

        case 6:{
            char id_pasien[MAX_STR];
            printf("Masukkan ID Pasien: ");
            fgets(id_pasien, MAX_STR, stdin);
            id_pasien[strcspn(id_pasien, "\n")] = '\0';

            cariKontrol(id_pasien, riwayat_pasien);
            break;
        }

        case 7:
            printf("Keluar dari program...");
            break;
        default:
            printf("Input tidak valid! Silakan masukkan angka dari 1-6!\n");
            break;
        }

    } while (opsi != 7);
}

int main() {
    start_program();
    // Parsing data pasien
    // Pasien *data_pasien = input_data_pasien();

    // // Parsing data riwayat pasien
    // RiwayatPasien *riwayat_pasien = input_riwayat_pasien();
    // int opsi;

    // print1(riwayat_pasien);
    // print1(data_pasien);

    return 0;
}   
