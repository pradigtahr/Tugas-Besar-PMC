#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

// Struktur untuk data riwayat medis dari file CSV
typedef struct {
    char *no;
    char *date;
    char *id_number;
    char *diagnosis;
    char *action;
    char *control;
    int cost;
} MedicalRecord1;

// Global variables
GList *medical_records_list1 = NULL; // Global list to store medical records data

// Fungsi untuk membersihkan memori dari data MedicalRecord1 dalam GList
void free_medical_record1(gpointer data) {
    MedicalRecord1 *record = (MedicalRecord1 *)data;
    g_free(record->no);
    g_free(record->date);
    g_free(record->id_number);
    g_free(record->diagnosis);
    g_free(record->action);
    g_free(record->control);
    g_free(record);
}

// Fungsi untuk membaca file CSV riwayat medis
gboolean read_medical_records_csv1(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        g_printerr("Tidak dapat membuka file %s.\n", filename);
        return FALSE;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char *no = strtok(line, ",");
        char *date = strtok(NULL, ",");
        char *id_number = strtok(NULL, ",");
        char *diagnosis = strtok(NULL, ",");
        char *action = strtok(NULL, ",");
        char *control = strtok(NULL, ",");
        int cost = atoi(strtok(NULL, ","));

        MedicalRecord1 *record = g_malloc(sizeof(MedicalRecord1));
        record->no = g_strdup(no);
        record->date = g_strdup(date);
        record->id_number = g_strdup(id_number);
        record->diagnosis = g_strdup(diagnosis);
        record->action = g_strdup(action);
        record->control = g_strdup(control);
        record->cost = cost;

        medical_records_list1 = g_list_append(medical_records_list1, record);
    }

    fclose(file);
    return TRUE;
}

// Fungsi untuk menghitung pendapatan bulanan, tahunan, dan rata-rata biaya pertahun
void calculate_income_and_average(const char *search_id, GtkWidget *labelMonthly, GtkWidget *labelYearly, GtkWidget *labelAverage) {
    // Pendapatan bulanan (12 bulan)
    int monthly_income = 0;

    // Pendapatan tahunan
    int yearly_income = 0;

    // Variabel untuk menyimpan biaya per tahun
    GHashTable *yearly_costs = g_hash_table_new(g_str_hash, g_str_equal);

    // Iterasi melalui medical_records_list1 untuk menghitung pendapatan bulanan dan tahunan
    for (GList *iter = medical_records_list1; iter != NULL; iter = g_list_next(iter)) {
        MedicalRecord1 *record = (MedicalRecord1 *)iter->data;

        // Cek apakah ID pasien cocok dengan ID yang diinputkan
        if (strcmp(record->id_number, search_id) == 0) {
            // Ambil tahun dari tanggal (misal: tanggal dalam format "YYYY-MM-DD")
            char *year_str = strtok(record->date, "-");
            int year = atoi(year_str);

            // Tambahkan biaya ke pendapatan bulanan
            monthly_income += record->cost;

            // Tambahkan biaya ke pendapatan tahunan menggunakan hash table
            int *year_cost = g_hash_table_lookup(yearly_costs, year_str);
            if (year_cost == NULL) {
                // Jika tahun belum ada dalam hash table, tambahkan biaya baru
                g_hash_table_insert(yearly_costs, g_strdup(year_str), g_memdup(&record->cost, sizeof(int)));
            } else {
                // Jika tahun sudah ada dalam hash table, tambahkan biaya ke biaya yang sudah ada
                *year_cost += record->cost;
            }
        }
    }

    // Hitung rata-rata biaya pertahun
    double average_yearly_cost = 0.0;
    int num_years = g_hash_table_size(yearly_costs);
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, yearly_costs);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        int *year_cost = (int *)value;
        yearly_income += *year_cost;
    }

    if (num_years > 0) {
        average_yearly_cost = (double)yearly_income / num_years;
    }

    // Tampilkan hasil dalam label-label GTK
    char buf[256];
    sprintf(buf, "Pendapatan Bulanan: Rp %d", monthly_income);
    gtk_label_set_text(GTK_LABEL(labelMonthly), buf);

    sprintf(buf, "Pendapatan Tahunan: Rp %d", yearly_income);
    gtk_label_set_text(GTK_LABEL(labelYearly), buf);

    sprintf(buf, "Rata-rata Biaya Pertahun: Rp %.2f", average_yearly_cost);
    gtk_label_set_text(GTK_LABEL(labelAverage), buf);

    // Hapus hash table
    g_hash_table_destroy(yearly_costs);
}

// Fungsi yang akan dipanggil ketika tombol ditekan
void on_button_clicked(GtkButton *button, gpointer user_data) {
    // Data user data berisi array dari GtkLabel dan GtkEntry
    GtkWidget **widgets = (GtkWidget **)user_data;
    GtkWidget *labelMonthly = widgets[0];
    GtkWidget *labelYearly = widgets[1];
    GtkWidget *labelAverage = widgets[2];
    GtkWidget *entryID = widgets[3];

    // Dapatkan ID yang dimasukkan pengguna
    const char *search_id = gtk_entry_get_text(GTK_ENTRY(entryID));

    // Hitung pendapatan dan rata-rata biaya pertahun
    calculate_income_and_average(search_id, labelMonthly, labelYearly, labelAverage);
}

// Fungsi utama
void create_gui_program4() {

    // Baca file CSV riwayat medis
    if (!read_medical_records_csv1("RiwayatMedis.csv")) {
        g_printerr("Gagal membaca file CSV RiwayatMedis.csv.\n");
        return -1;
    }

    // Buat window utama
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Cek pendapatan");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Buat box utama untuk menampung widget
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Entry untuk input ID pasien
    GtkWidget *entryID = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entryID, FALSE, FALSE, 0);

    // Button untuk menghitung
    GtkWidget *button = gtk_button_new_with_label("Hitung");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    // Label untuk menampilkan hasil
    GtkWidget *labelMonthly = gtk_label_new("Pendapatan Bulanan: Rp 0");
    gtk_box_pack_start(GTK_BOX(box), labelMonthly, FALSE, FALSE, 0);

    GtkWidget *labelYearly = gtk_label_new("Pendapatan Tahunan: Rp 0");
    gtk_box_pack_start(GTK_BOX(box), labelYearly, FALSE, FALSE, 0);

    GtkWidget *labelAverage = gtk_label_new("Rata-rata Biaya Pertahun: Rp 0.00");
    gtk_box_pack_start(GTK_BOX(box), labelAverage, FALSE, FALSE, 0);

    // Array untuk menyimpan widget-widget yang diperlukan
    GtkWidget *widgets[] = {labelMonthly, labelYearly, labelAverage, entryID};

    // Hubungkan fungsi callback ke button clicked
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), widgets);

    // Tampilkan semua widget
    gtk_widget_show_all(window);

    // Jalankan main loop GTK
    gtk_main();

    // Bebaskan memori yang digunakan untuk data MedicalRecord1
    g_list_free_full(medical_records_list1, free_medical_record1);

    return 0;
}

//page 2
