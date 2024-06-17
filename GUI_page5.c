#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur untuk menyimpan data pasien
typedef struct Pasien {
    int no;
    char tanggal[20];
    char idPasien[20];
    char diagnosis[50];
    struct Pasien *next;
} Pasiennn;

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
Pasiennn* createPatient(int no, char* tanggal, char* idPasien, char* diagnosis) {
    Pasiennn* newPasien = (Pasiennn*)malloc(sizeof(Pasiennn));
    newPasien->no = no;
    strcpy(newPasien->tanggal, tanggal);
    strcpy(newPasien->idPasien, idPasien);
    strcpy(newPasien->diagnosis, diagnosis);
    newPasien->next = NULL;
    return newPasien;
}

// Fungsi untuk menyisipkan pasien
void insertPatient(Pasiennn** head, int no, char* tanggal, char* idPasien, char* diagnosis) {
    Pasiennn* newPasien = createPatient(no, tanggal, idPasien, diagnosis);
    if (*head == NULL) {
        *head = newPasien;
        return;
    }
    Pasiennn* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newPasien;
}

// Fungsi untuk mengekstrak tahun dari string tanggal
int extractYear(char* tanggal) {
    int year;
    sscanf(tanggal + strlen(tanggal) - 4, "%d", &year);
    return year;
}

// Fungsi untuk menghitung pasien dan diagnosis per bulan dan tahun
void countPatients(Pasiennn* head, CountData* countData, int* totalRecords, YearlyData* yearlyData, int* totalYears) {
    char months[12][20] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni",
                           "Juli", "Agustus", "September", "Oktober", "November", "Desember"};

    int monthIndex[12][100] = {0};  // Assumsi tidak lebih dari 100 tahun
    int yearIndex[100] = {0};       // Assumsi tidak lebih dari 100 tahun
    *totalRecords = 0;
    *totalYears = 0;

    Pasiennn* temp = head;
    while (temp != NULL) {
        int year = extractYear(temp->tanggal);
        for (int i = 0; i < 12; i++) {
            if (strstr(temp->tanggal, months[i]) != NULL) {
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

// Fungsi untuk membandingkan dua struktur CountData untuk pengurutan
int compare(const void* a, const void* b) {
    CountData* dataA = (CountData*)a;
    CountData* dataB = (CountData*)b;
    return dataB->patientCount - dataA->patientCount;
}

// Fungsi untuk membandingkan dua struktur YearlyData untuk pengurutan
int compareYearly(const void* a, const void* b) {
    YearlyData* dataA = (YearlyData*)a;
    YearlyData* dataB = (YearlyData*)b;
    return dataA->year - dataB->year;
}

// Fungsi untuk mengosongkan memori yang dialokasikan untuk linked list
void freePatients(Pasiennn* head) {
    Pasiennn* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Fungsi callback untuk tombol Keluar
void on_button_exit_clicked(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

// Fungsi utama untuk membuat antarmuka GTK
void create_gui_program5() {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *scrolled_window1;
    GtkWidget *scrolled_window2;
    GtkTextBuffer *buffer1;
    GtkTextBuffer *buffer2;



    // Membuat window utama
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Cek jumlah pasien dan penyakit");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Membuat grid untuk menata elemen-elemen GUI
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Membuat label untuk bagian atas (bulan dan diagnosis)
    label = gtk_label_new("Jumlah Pasien dan Diagnosis (dari yang terbanyak):");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Membuat scrolled window untuk bagian atas (bulan dan diagnosis)
    scrolled_window1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window1),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window1, 0, 1, 1, 1);

    // Membuat text view untuk menampilkan teks di bagian atas (bulan dan diagnosis)
    GtkWidget *text_view1 = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view1), FALSE);
    buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view1));
    gtk_container_add(GTK_CONTAINER(scrolled_window1), text_view1);

    // Membuat label untuk bagian bawah (tahun)
    label = gtk_label_new("Total Pasien tiap Tahun:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

    // Membuat scrolled window untuk bagian bawah (tahun)
    scrolled_window2 = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window2),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window2, 0, 3, 1, 1);

    // Membuat text view untuk menampilkan teks di bagian bawah (tahun)
    GtkWidget *text_view2 = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view2), FALSE);
    buffer2 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view2));
    gtk_container_add(GTK_CONTAINER(scrolled_window2), text_view2);

    // Mengambil data dari file CSV
    FILE *file = fopen("RiwayatMedis.csv", "r");
    if (!file) {
        printf("File tidak dapat dibuka.\n");
        return 1;
    }

    Pasiennn* head = NULL;
    char line[200];
    fgets(line, sizeof(line), file);  // Skip header line

    while (fgets(line, sizeof(line), file)) {
        int no;
        char tanggal[20], idPasien[20], diagnosis[50];
        sscanf(line, "%d,%[^,],%[^,],%[^,]", &no, tanggal, idPasien, diagnosis);
        insertPatient(&head, no, tanggal, idPasien, diagnosis);
    }

    fclose(file);

    CountData countData[120] = {0};  // Maksimal 120 bulan
    YearlyData yearlyData[100] = {0};  // Maksimal 100 tahun
    int totalRecords = 0;
    int totalYears = 0;
    countPatients(head, countData, &totalRecords, yearlyData, &totalYears);

    // Mengurutkan data
    qsort(countData, totalRecords, sizeof(CountData), compare);
    qsort(yearlyData, totalYears, sizeof(YearlyData), compareYearly);

    // Menampilkan hasil perhitungan pada text view
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_offset(buffer1, &iter, 0);
    for (int i = 0; i < totalRecords; i++) {
        gtk_text_buffer_insert(buffer1, &iter, "Bulan dan Tahun     : ", -1);
        gtk_text_buffer_insert(buffer1, &iter, countData[i].month, -1);
        char year[10];
        sprintf(year, " %d\n", countData[i].year);
        gtk_text_buffer_insert(buffer1, &iter, year, -1);
        char patientCount[50];
        sprintf(patientCount, "Jumlah Pasien       : %d\n", countData[i].patientCount);
        gtk_text_buffer_insert(buffer1, &iter, patientCount, -1);
        gtk_text_buffer_insert(buffer1, &iter, "Diagnosis Penyakit  : \n", -1);
        for (int j = 0; j < countData[i].diagCount; j++) {
            gtk_text_buffer_insert(buffer1, &iter, " - ", -1);
            gtk_text_buffer_insert(buffer1, &iter, countData[i].diagnoses[j], -1);
            gtk_text_buffer_insert(buffer1, &iter, "\n", -1);
        }
        gtk_text_buffer_insert(buffer1, &iter, "\n", -1);
    }

    gtk_text_buffer_get_iter_at_offset(buffer2, &iter, 0);
    for (int i = 0; i < totalYears; i++) {
        char year[50];
        sprintf(year, "Tahun: %d, Total Jumlah Pasien: %d\n", yearlyData[i].year, yearlyData[i].totalPatients);
        gtk_text_buffer_insert(buffer2, &iter, year, -1);
    }

    freePatients(head);

    // Membuat tombol Keluar
    GtkWidget *button_exit = gtk_button_new_with_label("Keluar");
    g_signal_connect(button_exit, "clicked", G_CALLBACK(on_button_exit_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_exit, 0, 4, 1, 1);

    // Menampilkan window utama
    gtk_widget_show_all(window);

    // Memulai loop utama GTK
    gtk_main();

    return 0;
}
