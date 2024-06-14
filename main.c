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
        float biaya;

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

void print(Pasien *head) {
    Pasien *display = head;
    while (display != NULL) {
        if (display->no != 0) {
            printf("%d. Nama: %s \t|| Alamat: %s\t|| Domisili: %s\t|| Lahir di: %s\t|| Tanggal Lahir: %s\t|| Umur: %d\t\t|| BPJS: %d\t|| ID: %s\n", display->no, display->nama, display->alamat, display->kota, display->tempat_lahir, display->tanggal_lahir, display->umur, display->bpjs, display->id);
        }
        display = display->next;
    }
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

// -------------------------- FUNGSI BAGIAN DIGTA -------------------------------------------
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
    getchar();  // To consume the newline character left by scanf

    printf("Masukkan ID pasien: ");
    fgets(id, MAX_STR, stdin);
    id[strcspn(id, "\n")] = '\0';
}

Pasien *find_patient_by_id(Pasien *head, char id[]) {
    Pasien *current = head;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void update_csv(const char *filename, char id[], Pasien updated_patient) {
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

    // Read header line and write it to the temporary file
    if (fgets(line, sizeof(line), file)) {
        fputs(line, tempfile);
    }

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%*d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*d,%*d,%[^,\n]", temp_id);

        if (strcmp(temp_id, id) == 0) {
            fprintf(tempfile, "%d,%s,%s,%s,%s,%s,%d,%d,%s", updated_patient.no, updated_patient.nama, updated_patient.alamat, updated_patient.kota, updated_patient.tempat_lahir, updated_patient.tanggal_lahir, updated_patient.umur, updated_patient.bpjs, updated_patient.id);
            updated = 1;
        } else {
            fputs(line, tempfile);
        }
    }

    fclose(file);
    fclose(tempfile);

    if (updated) {
        remove(filename);
        rename("temp.csv", filename);
        printf("Data pasien dengan ID %s berhasil diubah.\n", id);
    } else {
        remove(filename);
        rename("temp.csv", filename);
        printf("Data pasien dengan ID %s tidak ditemukan.\n", id);
    }
}



void delete_csv_line(const char *filename, char id[]) {
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
    int deleted = 0;

    // Read header line and write it to the temporary file
    if (fgets(line, sizeof(line), file)) {
        fputs(line, tempfile);
    }

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%*d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*d,%*d,%[^,\n]", temp_id);

        if (strcmp(temp_id, id) == 0) {
            deleted = 1;
            continue; // Skip writing this line to the temporary file
        }

        fputs(line, tempfile);
    }

    fclose(file);
    fclose(tempfile);

    // Replace the original file with the updated file
    if (deleted) {
        remove(filename);
        rename("temp.csv", filename);
        printf("Data pasien dengan ID %s berhasil dihapus.\n", id);
    } else {
        remove("temp.csv");
        printf("Data pasien dengan ID %s tidak ditemukan.\n", id);
    }
}
//-------------------------------------------------------------------------------------------


// -------------------------- FUNGSI BAGIAN ISSA ---------------------------------------------
void trim_trailing_whitespace(char *str) {
    char *end;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';
}

int baca_csv_riwayat(const char *nama_file, RiwayatPasien **head) {
    FILE *file = fopen(nama_file, "r");
    if (file == NULL) {
        printf("File tidak ditemukan.\n");
        return 0;
    }

    char baris[MAX_STR];
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

void informasi_pasien(Pasien *head_pasien, char id_pasien[]) {
    Pasien *current = head_pasien;
    int found = 0;

    printf("Informasi Pasien dengan ID: %s\n", id_pasien);
    while (current != NULL) {
        if (strcmp(current->id, id_pasien) == 0) {
            found = 1;
            printf("%d. Nama: %s \t|| Alamat: %s\t|| Domisili: %s\t|| Lahir di: %s\t|| Tanggal Lahir: %s\t|| Umur: %d\t\t|| BPJS: %s\t|| ID: %s\n", current->no, current->nama, 
            current->alamat, current->kota, current->tempat_lahir, current->tanggal_lahir, current->umur, current->bpjs, current->id);
        }
        current = current->next;
    }

    if (!found) {
        printf("Tidak ada informasi pasien yang ditemukan untuk ID pasien: %s\n", id_pasien);
    }
}

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
// --------------------------------------------------------------------------------------------

void start_program() {
    // Parsing data pasien
    Pasien *data_pasien = input_data_pasien();

    // Parsing data riwayat pasien
    RiwayatPasien *riwayat_pasien = input_riwayat_pasien();
    int opsi;

    do {
        header();
        printf("1. Sunting data pasien\n");
        printf("2. Sunting riwayat, diagnosis, dan tindakan pada pasien\n");
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
                printf("Masukkan ID pasien yang ingin diubah: ");
                fgets(id, MAX_STR, stdin);
                id[strcspn(id, "\n")] = '\0';

                Pasien *patient_to_update = find_patient_by_id(data_pasien, id);
                if (patient_to_update) {
                    int nomor, umur, bpjs;
                    char nama[MAX_STR], alamat[MAX_STR], kota[MAX_STR], tempatLahir[MAX_STR], tanggalLahir[MAX_STR];
                    get_patient_data(&nomor, nama, alamat, kota, tempatLahir, tanggalLahir, &umur, &bpjs, id);

                    patient_to_update->no = nomor;
                    strcpy(patient_to_update->nama, nama);
                    strcpy(patient_to_update->alamat, alamat);
                    strcpy(patient_to_update->kota, kota);
                    strcpy(patient_to_update->tempat_lahir, tempatLahir);
                    strcpy(patient_to_update->tanggal_lahir, tanggalLahir);
                    patient_to_update->umur = umur;
                    patient_to_update->bpjs = bpjs;
                    strcpy(patient_to_update->id, id);

                    update_csv("DataPasien.csv", id, *patient_to_update);
                } else {
                    printf("Pasien dengan ID %s tidak ditemukan.\n", id);
                }
            } else if (strcmp(choice1, "hapus") == 0) {
                char id[MAX_STR];
                printf("Masukkan ID pasien yang ingin dihapus: ");
                fgets(id, MAX_STR, stdin);
                id[strcspn(id, "\n")] = '\0';

                Pasien *prev = NULL;
                Pasien *current = data_pasien;
                while (current != NULL) {
                    if (strcmp(current->id, id) == 0) {
                        if (prev == NULL) {
                            data_pasien = current->next;
                        } else {
                            prev->next = current->next;
                        }
                        free(current);
                        delete_csv_line("DataPasien.csv", id);
                        break;
                    }
                    prev = current;
                    current = current->next;
                }
            } else if (strcmp(choice1, "cari") == 0) {
                char id[MAX_STR];
                printf("Masukkan ID pasien yang ingin dicari: ");
                fgets(id, MAX_STR, stdin);
                id[strcspn(id, "\n")] = '\0';

                Pasien *patient = find_patient_by_id(data_pasien, id);
                if (patient) {
                    printf("Pasien ditemukan:\n");
                    printf("ID: %s\n", patient->id);
                    printf("Nama: %s\n", patient->nama);
                    printf("Alamat: %s\n", patient->alamat);
                    printf("Kota: %s\n", patient->kota);
                    printf("Tempat Lahir: %s\n", patient->tempat_lahir);
                    printf("Tanggal Lahir: %s\n", patient->tanggal_lahir);
                    printf("Umur: %d\n", patient->umur);
                    printf("BPJS: %d\n", patient->bpjs);
                    printf("Nomor: %d\n", patient->no);
                } else {
                    printf("Pasien dengan ID %s tidak ditemukan.\n", id);
                }
            } else {
                printf("Input tidak valid!\n");
            }
            printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n");
            break;
        }

        // Punya gastya
        case 2:
            
            break;
        
        // Punya Issa
        case 3:
            char id_pasien[MAX_STR];
                printf("Masukkan ID Pasien: ");
                fgets(id_pasien, MAX_STR, stdin);
                id_pasien[strcspn(id_pasien, "\n")] = '\0';

                // Display patient information
                informasi_pasien(data_pasien, id_pasien);

                // Display patient medical history
                informasi_riwayat_pasien(riwayat_pasien, id_pasien);
                break;

            break;
        case 4:

            break;
        case 5:

            break;
        case 6:

            break;
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

    return 0;
}
