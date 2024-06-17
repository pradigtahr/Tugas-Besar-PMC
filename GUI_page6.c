#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur untuk data dari file CSV
typedef struct {
    char *no;
    char *full_name;
    char *address;
    char *city;
    char *birth_place;
    char *birth_date;
    int age;
    char *bpjs_number;
    char *id_number;
} Personn;

// Struktur untuk data riwayat medis dari file CSV
typedef struct {
    char *no;
    char *date;
    char *id_number;
    char *diagnosis;
    char *action;
    char *control;
    int cost;
} MedicalRecord2;

// Global variables
GList *people_listt = NULL; // Global list to store people data
GList *medical_records_listt = NULL; // Global list to store medical records data

// Fungsi untuk membersihkan memori dari data Person dalam GList
void free_personn(gpointer data) {
    Personn *person = (Personn *)data;
    g_free(person->no);
    g_free(person->full_name);
    g_free(person->address);
    g_free(person->city);
    g_free(person->birth_place);
    g_free(person->birth_date);
    g_free(person->bpjs_number);
    g_free(person->id_number);
    g_free(person);
}

// Fungsi untuk membersihkan memori dari data MedicalRecord2 dalam GList
void free_medical_recordd(gpointer data) {
    MedicalRecord2 *record = (MedicalRecord2 *)data;
    g_free(record->no);
    g_free(record->date);
    g_free(record->id_number);
    g_free(record->diagnosis);
    g_free(record->action);
    g_free(record->control);
    g_free(record);
}

// Fungsi untuk membaca file CSV data pasien
gboolean read_csvv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        g_printerr("Tidak dapat membuka file %s.\n", filename);
        return FALSE;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char *no = strtok(line, ",");
        char *full_name = strtok(NULL, ",");
        char *address = strtok(NULL, ",");
        char *city = strtok(NULL, ",");
        char *birth_place = strtok(NULL, ",");
        char *birth_date = strtok(NULL, ",");
        int age = atoi(strtok(NULL, ","));
        char *bpjs_number = strtok(NULL, ",");
        char *id_number = strtok(NULL, ",");

        Personn *person = g_malloc(sizeof(Personn));
        person->no = g_strdup(no);
        person->full_name = g_strdup(full_name);
        person->address = g_strdup(address);
        person->city = g_strdup(city);
        person->birth_place = g_strdup(birth_place);
        person->birth_date = g_strdup(birth_date);
        person->age = age;
        person->bpjs_number = g_strdup(bpjs_number);
        person->id_number = g_strdup(id_number);

        people_listt = g_list_append(people_listt, person);
    }

    fclose(file);
    return TRUE;
}

// Fungsi untuk membaca file CSV riwayat medis
gboolean read_medical_records_csvv(const char *filename) {
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

        MedicalRecord2 *record = g_malloc(sizeof(MedicalRecord2));
        record->no = g_strdup(no);
        record->date = g_strdup(date);
        record->id_number = g_strdup(id_number);
        record->diagnosis = g_strdup(diagnosis);
        record->action = g_strdup(action);
        record->control = g_strdup(control);
        record->cost = cost;

        medical_records_listt = g_list_append(medical_records_listt, record);
    }

    fclose(file);
    return TRUE;
}

// Fungsi untuk mencari orang berdasarkan ID Pasien
void search_peoplee(const gchar *search_text, GtkTextBuffer *buffer) {
    GtkTextIter iter;
    gtk_text_buffer_get_start_iter(buffer, &iter);
    gtk_text_buffer_set_text(buffer, "", -1); // Clear text buffer

    gboolean found = FALSE;
    for (GList *iter_list = people_listt; iter_list != NULL; iter_list = g_list_next(iter_list)) {
        Personn *person = (Personn *)iter_list->data;

        char *person_id_cleaned = g_strdup(person->id_number);
        gchar *person_id = g_strstrip(person_id_cleaned);
        char *search_text_cleaned = g_strdup(search_text);
        gchar *search_id = g_strstrip(search_text_cleaned);

        if (strcmp(person_id, search_id) == 0) {
            found = TRUE;
            gtk_text_buffer_insert(buffer, &iter, "No: ", -1);
            gtk_text_buffer_insert(buffer, &iter, person->no, -1);
            gtk_text_buffer_insert(buffer, &iter, ", Nama Lengkap: ", -1);
            gtk_text_buffer_insert(buffer, &iter, person->full_name, -1);
            gtk_text_buffer_insert(buffer, &iter, ", Alamat: ", -1);
            gtk_text_buffer_insert(buffer, &iter, person->address, -1);
            gtk_text_buffer_insert(buffer, &iter, ", Kota: ", -1);
            gtk_text_buffer_insert(buffer, &iter, person->city, -1);
            gtk_text_buffer_insert(buffer, &iter, ", Tempat Lahir: ", -1);
            gtk_text_buffer_insert(buffer, &iter, person->birth_place, -1);
            gtk_text_buffer_insert(buffer, &iter, ", Tanggal Lahir: ", -1);
            gtk_text_buffer_insert(buffer, &iter, person->birth_date, -1);
            gtk_text_buffer_insert(buffer, &iter, ", Umur: ", -1);
            char age_str[5];
            sprintf(age_str, "%d", person->age);
            gtk_text_buffer_insert(buffer, &iter, age_str, -1);
            gtk_text_buffer_insert(buffer, &iter, ", No BPJS: ", -1);
            gtk_text_buffer_insert(buffer, &iter, person->bpjs_number, -1);
            gtk_text_buffer_insert(buffer, &iter, ", ID Pasien: ", -1);
            gtk_text_buffer_insert(buffer, &iter, person_id, -1);
            gtk_text_buffer_insert(buffer, &iter, "\n", -1);

            // Cari dan tampilkan riwayat medis
            for (GList *iter_medical = medical_records_listt; iter_medical != NULL; iter_medical = g_list_next(iter_medical)) {
                MedicalRecord2 *record = (MedicalRecord2 *)iter_medical->data;
                char *record_id_cleaned = g_strdup(record->id_number);
                gchar *record_id = g_strstrip(record_id_cleaned);

                if (strcmp(record_id, search_id) == 0) {
                    gtk_text_buffer_insert(buffer, &iter, "Riwayat Kontrol:\n", -1);
                    gtk_text_buffer_insert(buffer, &iter, "Tanggal: ", -1);
                    gtk_text_buffer_insert(buffer, &iter, record->date, -1);
                    gtk_text_buffer_insert(buffer, &iter, ", Diagnosis: ", -1);
                    gtk_text_buffer_insert(buffer, &iter, record->diagnosis, -1);
                    gtk_text_buffer_insert(buffer, &iter, ", Tindakan: ", -1);
                    gtk_text_buffer_insert(buffer, &iter, record->action, -1);
                    gtk_text_buffer_insert(buffer, &iter, ", Kontrol: ", -1);
                    gtk_text_buffer_insert(buffer, &iter, record->control, -1);

                    gtk_text_buffer_insert(buffer, &iter, "\n", -1);
                }

                g_free(record_id_cleaned);
            }
        }

        g_free(person_id_cleaned);
        g_free(search_text_cleaned);
    }

    if (!found) {
        gtk_text_buffer_set_text(buffer, "Tidak ada hasil yang ditemukan.", -1);
    }
}

// Fungsi yang akan dipanggil ketika tombol pencarian ditekan
void search_button_clickedd(GtkWidget *widget, gpointer data) {
    GtkEntry *entry = GTK_ENTRY(data);
    const gchar *search_text = gtk_entry_get_text(entry);

    // Dapatkan GtkTextBuffer dari GtkTextView
    GtkWidget *text_view = g_object_get_data(G_OBJECT(widget), "text_view");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    // Panggil fungsi pencarian
    search_peoplee(search_text, buffer);
}

// Fungsi utama untuk menampilkan tab halaman 1
void display_pagee1(GtkWidget *notebook) {
    // Buat box utama untuk menampung widget
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Entry untuk input pencarian
    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // Tombol untuk memulai pencarian
    GtkWidget *button = gtk_button_new_with_label("Cari");
    g_signal_connect(button, "clicked", G_CALLBACK(search_button_clickedd), entry);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    // TextView untuk menampilkan hasil pencarian
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_box_pack_start(GTK_BOX(box), text_view, TRUE, TRUE, 0);

    // Buat GtkTextBuffer untuk TextView
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    // Simpan referensi text_view ke dalam button
    g_object_set_data(G_OBJECT(button), "text_view", text_view);

    // Tambahkan halaman ke notebook
    GtkWidget *label = gtk_label_new("Data Pasien");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, label);
}
// Fungsi utama untuk menampilkan tab halaman 2


// Fungsi utama
void create_gui_program6() {

    // Baca file CSV dan siapkan data
    if (!read_csvv("DataPasien.csv")) {
        g_printerr("Gagal membaca file CSV DataPasien.csv.\n");
        return -1;
    }

    // Baca file CSV riwayat medis
    if (!read_medical_records_csvv("RiwayatMedis.csv")) {
        g_printerr("Gagal membaca file CSV RiwayatMedis.csv.\n");
        return -1;
    }

    // Buat window utama
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Cek informasi pasien yang memerlukan kontrol");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Buat notebook untuk tab
    GtkWidget *notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);

    // Tambahkan halaman 1
    display_pagee1(notebook);


    // Tampilkan semua widget
    gtk_widget_show_all(window);

    // Jalankan main loop GTK
    gtk_main();

    // Bebaskan memori yang digunakan untuk data Person dan MedicalRecord2
    g_list_free_full(people_listt, free_personn);
    g_list_free_full(medical_records_listt, free_medical_recordd);

    return 0;
}


// page 1
