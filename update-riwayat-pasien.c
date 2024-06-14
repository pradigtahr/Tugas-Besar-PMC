#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "RiwayatPasien.csv"
#define MAX_LINE_LENGTH 1024

void add_record();
void update_record();
void search_record();
void delete_record();
int validate_date(const char* date);
void format_date(char* date);

int main() {
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
                exit(0);
            default:
                printf("Opsi tidak valid. Silakan coba lagi.\n");
        }
    }
    return 0;
}

// Fungsi untuk menambah riwayat pasien
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

// Fungsi untuk melakukan forat terhadap tanggal
void format_date(char* date) {
    char day[3], month[10], year[5];
    sscanf(date, "%2s %9s %4s", day, month, year);
    sprintf(date, "%s %s %s", day, month, year);
}