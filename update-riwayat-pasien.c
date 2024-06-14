#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

RiwayatPasien *head = NULL;

void add_record();
void update_record();
void search_record();
void delete_record();
int validate_date(const char* date);
void format_date(char* date);
void load_data();
void save_data();

int main() {
    load_data();  // Load data dari file csv
    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Tambah Riwayat Pasien\n");
        printf("2. Ubah Riwayat Pasien\n");
        printf("3. Cari Riwayat Pasien\n");
        printf("4. Hapus Riwayat Pasien\n");
        printf("5. Keluar\n");
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
                save_data();  // Menyimpan data sebelum exit
                exit(0);
            default:
                printf("Opsi tidak valid. Silakan coba lagi.\n");
        }
    }
    return 0;
}

void add_record() {
    RiwayatPasien *new_record = (RiwayatPasien*)malloc(sizeof(RiwayatPasien));
    if (new_record == NULL) {
        printf("Tidak dapat mengalokasikan memori.\n");
        return;
    }

    printf("Masukkan Indeks Riwayat: ");
    scanf("%d", &new_record->indeksriwayat);
    getchar();

    printf("Masukkan Tanggal (DD Bulan YYYY): ");
    fgets(new_record->tanggal_kunjungan, sizeof(new_record->tanggal_kunjungan), stdin);
    new_record->tanggal_kunjungan[strcspn(new_record->tanggal_kunjungan, "\n")] = 0;
    if (!validate_date(new_record->tanggal_kunjungan)) {
        printf("Format tanggal tidak valid.\n");
        free(new_record);
        return;
    }

    printf("Masukkan ID Pasien: ");
    fgets(new_record->id_pasien, sizeof(new_record->id_pasien), stdin);
    new_record->id_pasien[strcspn(new_record->id_pasien, "\n")] = 0;

    printf("Masukkan Diagnosis: ");
    fgets(new_record->diagnosis, sizeof(new_record->diagnosis), stdin);
    new_record->diagnosis[strcspn(new_record->diagnosis, "\n")] = 0;

    printf("Masukkan Tindakan: ");
    fgets(new_record->tindakan, sizeof(new_record->tindakan), stdin);
    new_record->tindakan[strcspn(new_record->tindakan, "\n")] = 0;

    printf("Masukkan Tanggal Kontrol (DD Bulan YYYY): ");
    fgets(new_record->kontrol, sizeof(new_record->kontrol), stdin);
    new_record->kontrol[strcspn(new_record->kontrol, "\n")] = 0;

    printf("Masukkan Biaya: Rp");
    scanf("%lf", &new_record->biaya);
    getchar();

    new_record->next = head;
    head = new_record;

    printf("Riwayat pasien berhasil ditambahkan.\n");
}

void update_record() {
    char id_pasien[50];
    char tanggal_kunjungan[20];
    printf("Masukkan ID Pasien yang akan diubah: ");
    fgets(id_pasien, sizeof(id_pasien), stdin);
    id_pasien[strcspn(id_pasien, "\n")] = 0;

    printf("Masukkan Tanggal Kunjungan (DD Bulan YYYY) yang akan diubah: ");
    fgets(tanggal_kunjungan, sizeof(tanggal_kunjungan), stdin);
    tanggal_kunjungan[strcspn(tanggal_kunjungan, "\n")] = 0;

    RiwayatPasien *current = head;
    while (current != NULL) {
        if (strcmp(current->id_pasien, id_pasien) == 0 && strcmp(current->tanggal_kunjungan, tanggal_kunjungan) == 0) {
            printf("Masukkan Tanggal Baru (DD Bulan YYYY): ");
            fgets(current->tanggal_kunjungan, sizeof(current->tanggal_kunjungan), stdin);
            current->tanggal_kunjungan[strcspn(current->tanggal_kunjungan, "\n")] = 0;
            if (!validate_date(current->tanggal_kunjungan)) {
                printf("Format tanggal tidak valid.\n");
                return;
            }

            printf("Masukkan Diagnosis Baru: ");
            fgets(current->diagnosis, sizeof(current->diagnosis), stdin);
            current->diagnosis[strcspn(current->diagnosis, "\n")] = 0;

            printf("Masukkan Tindakan Baru: ");
            fgets(current->tindakan, sizeof(current->tindakan), stdin);
            current->tindakan[strcspn(current->tindakan, "\n")] = 0;

            printf("Masukkan Tanggal Kontrol Baru (DD Bulan YYYY): ");
            fgets(current->kontrol, sizeof(current->kontrol), stdin);
            current->kontrol[strcspn(current->kontrol, "\n")] = 0;
            if (!validate_date(current->kontrol)) {
                printf("Format tanggal kontrol tidak valid.\n");
                return;
            }

            printf("Masukkan Biaya Baru: Rp");
            scanf("%lf", &current->biaya);
            getchar();

            printf("Riwayat pasien berhasil diubah.\n");
            return;
        }
        current = current->next;
    }

    printf("Tidak ada riwayat pasien yang cocok dengan ID dan tanggal tersebut.\n");
}

void search_record() {
    char id_pasien[50];
    printf("Masukkan ID Pasien yang akan dicari: ");
    fgets(id_pasien, sizeof(id_pasien), stdin);
    id_pasien[strcspn(id_pasien, "\n")] = 0;
    printf("\n");

    RiwayatPasien *current = head;
    int found = 0;
    while (current != NULL) {
        if (strcmp(current->id_pasien, id_pasien) == 0) {
            printf("Indeks: %d\nTanggal: %s\nID Pasien: %s\nDiagnosis: %s\nTindakan: %s\nTanggal Kontrol: %s\nBiaya: Rp%.2lf\n\n",
                   current->indeksriwayat, current->tanggal_kunjungan, current->id_pasien, current->diagnosis, current->tindakan, current->kontrol, current->biaya);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("Tidak ada riwayat pasien yang cocok dengan ID tersebut.\n");
    }
}

void delete_record() {
    char id_pasien[50];
    char tanggal_kunjungan[20];
    printf("Masukkan ID Pasien yang akan dihapus: ");
    fgets(id_pasien, sizeof(id_pasien), stdin);
    id_pasien[strcspn(id_pasien, "\n")] = 0;

    printf("Masukkan Tanggal Kunjungan (DD Bulan YYYY) yang akan dihapus: ");
    fgets(tanggal_kunjungan, sizeof(tanggal_kunjungan), stdin);
    tanggal_kunjungan[strcspn(tanggal_kunjungan, "\n")] = 0;

    RiwayatPasien *current = head;
    RiwayatPasien *previous = NULL;
    while (current != NULL) {
        if (strcmp(current->id_pasien, id_pasien) == 0 && strcmp(current->tanggal_kunjungan, tanggal_kunjungan) == 0) {
            if (previous == NULL) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Riwayat pasien berhasil dihapus.\n");
            return;
        }
        previous = current;
        current = current->next;
    }

    printf("Tidak ada riwayat pasien yang cocok dengan ID dan tanggal tersebut.\n");
}

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

void load_data() {
    FILE *file = fopen("RiwayatPasien.csv", "r");
    if (file == NULL) {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        RiwayatPasien *new_record = (RiwayatPasien*)malloc(sizeof(RiwayatPasien));
        if (new_record == NULL) {
            printf("Tidak dapat mengalokasikan memori.\n");
            fclose(file);
            return;
        }

        sscanf(line, "%d,%19[^,],%49[^,],%99[^,],%99[^,],%19[^,],%lf", 
               &new_record->indeksriwayat, new_record->tanggal_kunjungan, new_record->id_pasien, 
               new_record->diagnosis, new_record->tindakan, new_record->kontrol, &new_record->biaya);

        new_record->next = head;
        head = new_record;
    }

    fclose(file);
}

void save_data() {
    FILE *file = fopen("RiwayatPasien.csv", "w");
    if (file == NULL) {
        printf("Tidak dapat membuka file.\n");
        return;
    }

    RiwayatPasien *current = head;
    while (current != NULL) {
        fprintf(file, "%d,%s,%s,%s,%s,%s,%.2lf\n", 
                current->indeksriwayat, current->tanggal_kunjungan, current->id_pasien, 
                current->diagnosis, current->tindakan, current->kontrol, current->biaya);
        current = current->next;
    }

    fclose(file);
}
