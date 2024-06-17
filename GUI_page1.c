#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur untuk data pasien
typedef struct {
    int no;
    char nama[100];
    char alamat[100];
    char kota[50];
    char tempat_lahir[50];
    char tanggal_lahir[12];
    int umur;
    char no_bpjs[20];
    char id_pasien[20];
} Pasien;

GtkEntry *entryNo, *entryNama, *entryAlamat, *entryKota, *entryTempatLahir, *entryTanggalLahir, *entryUmur, *entryNoBPJS, *entryIDPasien;
GtkListStore *liststore;
GtkTreeView *treeview;

// Fungsi untuk membaca data dari CSV saat aplikasi dimulai
void bacaDataDariCSV() {
    FILE *file = fopen("DataPasien.csv", "r");
    if (file == NULL) {
        g_print("Gagal membuka file DataPasien.csv.\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        char *token = strtok(buffer, ",");
        int no = atoi(token);
        char *nama = strtok(NULL, ",");
        char *alamat = strtok(NULL, ",");
        char *kota = strtok(NULL, ",");
        char *tempat_lahir = strtok(NULL, ",");
        char *tanggal_lahir = strtok(NULL, ",");
        int umur = atoi(strtok(NULL, ","));
        char *no_bpjs = strtok(NULL, ",");
        char *id_pasien = strtok(NULL, "\n");

        gtk_list_store_insert_with_values(liststore, NULL, -1,
                                         0, no,
                                         1, nama,
                                         2, alamat,
                                         3, kota,
                                         4, tempat_lahir,
                                         5, tanggal_lahir,
                                         6, umur,
                                         7, no_bpjs,
                                         8, id_pasien,
                                         -1);
    }

    fclose(file);
}

// Fungsi untuk menyimpan perubahan data pasien setelah diedit
void simpanPerubahanPasien() {
    const gchar *no_str = gtk_entry_get_text(entryNo);
    const gchar *nama = gtk_entry_get_text(entryNama);
    const gchar *alamat = gtk_entry_get_text(entryAlamat);
    const gchar *kota = gtk_entry_get_text(entryKota);
    const gchar *tempat_lahir = gtk_entry_get_text(entryTempatLahir);
    const gchar *tanggal_lahir = gtk_entry_get_text(entryTanggalLahir);
    const gchar *umur_str = gtk_entry_get_text(entryUmur);
    const gchar *no_bpjs = gtk_entry_get_text(entryNoBPJS);
    const gchar *id_pasien = gtk_entry_get_text(entryIDPasien);

    // Validasi input, misalnya memastikan umur adalah angka
    int no = atoi(no_str);
    int umur = atoi(umur_str);

    // Simpan data ke dalam liststore
    GtkTreeIter iter;
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
                       0, no,
                       1, nama,
                       2, alamat,
                       3, kota,
                       4, tempat_lahir,
                       5, tanggal_lahir,
                       6, umur,
                       7, no_bpjs,
                       8, id_pasien,
                       -1);

    // Simpan data ke dalam file CSV
    FILE *file = fopen("DataPasien.csv", "a");
    if (file == NULL) {
        g_print("Gagal membuka file DataPasien.csv.\n");
        return;
    }
    fprintf(file, "%s,%s,%s,%s,%s,%s,%d,%s,%s\n", no_str, nama, alamat,
            kota, tempat_lahir, tanggal_lahir, umur,
            no_bpjs, id_pasien);
    fclose(file);

    // Bersihkan kotak teks setelah data ditambahkan
    gtk_entry_set_text(entryNo, "");
    gtk_entry_set_text(entryNama, "");
    gtk_entry_set_text(entryAlamat, "");
    gtk_entry_set_text(entryKota, "");
    gtk_entry_set_text(entryTempatLahir, "");
    gtk_entry_set_text(entryTanggalLahir, "");
    gtk_entry_set_text(entryUmur, "");
    gtk_entry_set_text(entryNoBPJS, "");
    gtk_entry_set_text(entryIDPasien, "");
}

// Fungsi untuk mengedit data pasien berdasarkan ID pasien
void editPasien() {
    const gchar *id_pasien = gtk_entry_get_text(entryIDPasien);

    // Baca data dari file CSV dan cari berdasarkan ID pasien
    FILE *file = fopen("DataPasien.csv", "r");
    if (file == NULL) {
        g_print("Gagal membuka file DataPasien.csv.\n");
        return;
    }

    // Buat variabel untuk menyimpan data yang akan diedit
    int edit_index = -1;
    Pasien pasien_to_edit;

    char buffer[256];
    int index = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        char *token = strtok(buffer, ",");
        int no = atoi(token);
        char *nama = strtok(NULL, ",");
        char *alamat = strtok(NULL, ",");
        char *kota = strtok(NULL, ",");
        char *tempat_lahir = strtok(NULL, ",");
        char *tanggal_lahir = strtok(NULL, ",");
        int umur = atoi(strtok(NULL, ","));
        char *no_bpjs = strtok(NULL, ",");
        char *data_id_pasien = strtok(NULL, "\n");

        if (strcmp(data_id_pasien, id_pasien) == 0) {
            // Simpan data yang akan diedit
            edit_index = index;
            pasien_to_edit.no = no;
            strcpy(pasien_to_edit.nama, nama);
            strcpy(pasien_to_edit.alamat, alamat);
            strcpy(pasien_to_edit.kota, kota);
            strcpy(pasien_to_edit.tempat_lahir, tempat_lahir);
            strcpy(pasien_to_edit.tanggal_lahir, tanggal_lahir);
            pasien_to_edit.umur = umur;
            strcpy(pasien_to_edit.no_bpjs, no_bpjs);
            strcpy(pasien_to_edit.id_pasien, data_id_pasien);
            break;
        }

        index++;
    }

    fclose(file);

    // Jika data ditemukan, tampilkan data di kotak teks untuk diedit
    if (edit_index != -1) {
        gtk_entry_set_text(entryNo, g_strdup_printf("%d", pasien_to_edit.no));
        gtk_entry_set_text(entryNama, pasien_to_edit.nama);
        gtk_entry_set_text(entryAlamat, pasien_to_edit.alamat);
        gtk_entry_set_text(entryKota, pasien_to_edit.kota);
        gtk_entry_set_text(entryTempatLahir, pasien_to_edit.tempat_lahir);
        gtk_entry_set_text(entryTanggalLahir, pasien_to_edit.tanggal_lahir);
        gtk_entry_set_text(entryUmur, g_strdup_printf("%d", pasien_to_edit.umur));
        gtk_entry_set_text(entryNoBPJS, pasien_to_edit.no_bpjs);
        gtk_entry_set_text(entryIDPasien, pasien_to_edit.id_pasien);

        // Setelah data ditampilkan di kotak teks, hilangkan data lama dari liststore
        GtkTreeIter iter;
        gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(liststore), &iter, g_strdup_printf("%d", edit_index));
        gtk_list_store_remove(liststore, &iter);
    } else {
        g_print("Data tidak ditemukan untuk ID Pasien '%s'.\n", id_pasien);
    }
}

// Fungsi untuk menghapus data pasien berdasarkan ID pasien
// Fungsi untuk menghapus data pasien berdasarkan ID pasien
void hapusPasien() {
    const gchar *id_pasien = gtk_entry_get_text(entryIDPasien);

    // Buka file CSV untuk dibaca dan tulis ulang tanpa data yang akan dihapus
    FILE *file = fopen("DataPasien.csv", "r");
    if (file == NULL) {
        g_print("Gagal membuka file DataPasien.csv.\n");
        return;
    }

    FILE *temp_file = fopen("temp.csv", "w");
    if (temp_file == NULL) {
        g_print("Gagal membuat file sementara.\n");
        fclose(file);
        return;
    }

    char buffer[256];
    int data_found = 0;  // Flag untuk menandai apakah data ditemukan
    while (fgets(buffer, sizeof(buffer), file)) {
        char *token = strtok(buffer, ",");
        int no = atoi(token);
        char *nama = strtok(NULL, ",");
        char *alamat = strtok(NULL, ",");
        char *kota = strtok(NULL, ",");
        char *tempat_lahir = strtok(NULL, ",");
        char *tanggal_lahir = strtok(NULL, ",");
        int umur = atoi(strtok(NULL, ","));
        char *no_bpjs = strtok(NULL, ",");
        char *data_id_pasien = strtok(NULL, "\n");

        // Jika ID pasien cocok, skip penulisan data ini ke file sementara
        if (strcmp(data_id_pasien, id_pasien) == 0) {
            data_found = 1;  // Set flag bahwa data ditemukan
            continue;
        }

        // Tulis data ke file sementara jika ID pasien tidak cocok
        fprintf(temp_file, "%d,%s,%s,%s,%s,%s,%d,%s,%s\n", no, nama, alamat,
                kota, tempat_lahir, tanggal_lahir, umur,
                no_bpjs, data_id_pasien);
    }

    fclose(file);
    fclose(temp_file);

    // Hapus file asli dan ganti dengan file sementara
    if (remove("DataPasien.csv") != 0) {
        g_print("Gagal menghapus file DataPasien.csv.\n");
        return;
    }
    if (rename("temp.csv", "DataPasien.csv") != 0) {
        g_print("Gagal mengganti nama file sementara.\n");
        return;
    }

    // Hapus data dari liststore dan refresh treeview
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(liststore), &iter)) {
        do {
            gchar *data_id_pasien;
            gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter, 8, &data_id_pasien, -1);
            if (strcmp(data_id_pasien, id_pasien) == 0) {
                gtk_list_store_remove(liststore, &iter);
                break;
            }
            g_free(data_id_pasien);
        } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(liststore), &iter));
    }

    // Bersihkan kotak teks setelah data dihapus
    gtk_entry_set_text(entryNo, "");
    gtk_entry_set_text(entryNama, "");
    gtk_entry_set_text(entryAlamat, "");
    gtk_entry_set_text(entryKota, "");
    gtk_entry_set_text(entryTempatLahir, "");
    gtk_entry_set_text(entryTanggalLahir, "");
    gtk_entry_set_text(entryUmur, "");
    gtk_entry_set_text(entryNoBPJS, "");
    gtk_entry_set_text(entryIDPasien, "");

    // Tampilkan pesan jika data tidak ditemukan
    if (!data_found) {
        g_print("Data dengan ID Pasien '%s' tidak ditemukan.\n", id_pasien);
    }
}

// Fungsi callback untuk tombol Tambah
void on_tambah_clicked(GtkButton *button, gpointer user_data) {
    simpanPerubahanPasien();
}

// Fungsi callback untuk tombol Cari
void on_cari_clicked(GtkButton *button, gpointer user_data) {
    editPasien();
}

// Fungsi callback untuk tombol Hapus
void on_hapus_clicked(GtkButton *button, gpointer user_data) {
    hapusPasien();
}

// Fungsi utama
void create_gui_program1() {

    // Membuat window utama
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sunting/Cari data pasien");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Membuat box untuk layout
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Membuat scrolled window untuk treeview
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    // Membuat treeview untuk menampilkan data pasien
    liststore = gtk_list_store_new(9, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING);
    bacaDataDariCSV();

    treeview = GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore)));
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(treeview));

    // Menambahkan kolom-kolom ke dalam treeview
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    char *columns[] = {"No", "Nama Lengkap", "Alamat", "Kota", "Tempat Lahir",
                       "Tanggal Lahir", "Umur (th)", "No BPJS", "ID Pasien"};

    for (int i = 0; i < G_N_ELEMENTS(columns); i++) {
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(columns[i],
                                                         renderer,
                                                         "text", i,
                                                         NULL);
        gtk_tree_view_append_column(treeview, column);
    }

    // Membuat kotak teks dan tombol untuk input data pasien
    GtkWidget *input_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(box), input_box, FALSE, FALSE, 0);

    // Kotak teks untuk input data pasien
    entryNo = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryNo, "No");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryNo), FALSE, FALSE, 0);

    entryNama = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryNama, "Nama Lengkap");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryNama), FALSE, FALSE, 0);

    entryAlamat = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryAlamat, "Alamat");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryAlamat), FALSE, FALSE, 0);

    entryKota = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryKota, "Kota");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryKota), FALSE, FALSE, 0);

    entryTempatLahir = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryTempatLahir, "Tempat Lahir");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryTempatLahir), FALSE, FALSE, 0);

    entryTanggalLahir = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryTanggalLahir, "Tanggal Lahir (YYYY-MM-DD)");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryTanggalLahir), FALSE, FALSE, 0);

    entryUmur = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryUmur, "Umur (th)");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryUmur), FALSE, FALSE, 0);

    entryNoBPJS = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryNoBPJS, "No BPJS");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryNoBPJS), FALSE, FALSE, 0);

    entryIDPasien = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(entryIDPasien, "ID Pasien");
    gtk_box_pack_start(GTK_BOX(input_box), GTK_WIDGET(entryIDPasien), FALSE, FALSE, 0);

    // Tombol untuk aksi tambah, edit, cari, dan hapus
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_START);
    gtk_box_pack_start(GTK_BOX(box), button_box, FALSE, FALSE, 0);

    GtkWidget *buttonTambah = gtk_button_new_with_label("Tambah");
    g_signal_connect(buttonTambah, "clicked", G_CALLBACK(on_tambah_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), buttonTambah);

    GtkWidget *buttonCari = gtk_button_new_with_label("Cari");
    g_signal_connect(buttonCari, "clicked", G_CALLBACK(on_cari_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), buttonCari);

    GtkWidget *buttonHapus = gtk_button_new_with_label("Hapus");
    g_signal_connect(buttonHapus, "clicked", G_CALLBACK(on_hapus_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), buttonHapus);

    // Menampilkan window utama
    gtk_widget_show_all(window);

    // Menjalankan GTK main loop
    gtk_main();

    return 0;
}
