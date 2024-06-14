#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur untuk menyimpan data pasien
typedef struct {
    int no;
    char tanggal[20];
    char id_pasien[20];
    char diagnosis[50];
    char tindakan[50];
    char kontrol[20];
    int biaya;
} Pasien;

// Fungsi untuk membaca data dari file
int bacaData(const char *filename, Pasien **data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Tidak dapat membuka file.\n");
        return -1;
    }

    char buffer[256];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (count == 0) {
            count++;
            continue; // Lewati header
        }
        count++;
    }
    rewind(file);

    *data = (Pasien *)malloc((count - 1) * sizeof(Pasien));
    if (!*data) {
        printf("Gagal mengalokasikan memori.\n");
        fclose(file);
        return -1;
    }

    int i = 0;
    count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (count == 0) {
            count++;
            continue; // Lewati header
        }
        sscanf(buffer, "%d,%19[^,],%19[^,],%49[^,],%49[^,],%19[^,],%d",
               &((*data)[i].no),
               (*data)[i].tanggal,
               (*data)[i].id_pasien,
               (*data)[i].diagnosis,
               (*data)[i].tindakan,
               (*data)[i].kontrol,
               &((*data)[i].biaya));
        i++;
    }

    fclose(file);
    return i;
}

// Fungsi untuk mencari pasien berdasarkan ID dan menampilkan informasi kontrol
void cariKontrol(const char *id_pasien, Pasien *data, int count) {
    printf("Informasi kontrol untuk pasien dengan ID %s:\n", id_pasien);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(data[i].id_pasien, id_pasien) == 0) {
            printf("No: %d\nTanggal: %s\nDiagnosis: %s\nTindakan: %s\nKontrol: %s\nBiaya: %d\n\n",
                   data[i].no, data[i].tanggal, data[i].diagnosis, data[i].tindakan, data[i].kontrol, data[i].biaya);
            found = 1;
        }
    }
    if (!found) {
        printf("Pasien dengan ID %s tidak ditemukan.\n", id_pasien);
    }
}

int main() {
    const char *filename = "RiwayatPasien.csv";
    Pasien *data;
    int count = bacaData(filename, &data);
    if (count == -1) {
        return 1;
    }

    char id_pasien[20];
    printf("Masukkan ID Pasien : ");
    fgets(id_pasien, sizeof(id_pasien), stdin);
    // Menghapus newline di akhir input
    id_pasien[strcspn(id_pasien, "\n")] = '\0';

    cariKontrol(id_pasien, data, count);

    free(data);
    return 0;
}