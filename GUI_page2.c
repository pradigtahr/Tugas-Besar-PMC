#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

typedef struct {
    int no;
    char tanggal[20];
    char id_pasien[20];
    char diagnosis[100];
    char tindakan[100];
    char kontrol[10];
    int biaya;
} Pasienn;

GtkListStore *liststore;
GtkWidget *entryNo1, *entryTanggal1, *entryIDPasien1, *entryDiagnosis1, *entryTindakan1, *entryKontrol1, *entryBiaya1;

void tambahPasien(Pasienn pasien) {
    FILE *file = fopen("RiwayatMedis.csv", "a");
    if (file == NULL) {
        g_print("Gagal membuka file RiwayatMedis.csv untuk penambahan data.\n");
        return;
    }

    fprintf(file, "%d,%s,%s,%s,%s,%s,%d\n", pasien.no, pasien.tanggal, pasien.id_pasien,
            pasien.diagnosis, pasien.tindakan, pasien.kontrol, pasien.biaya);

    fclose(file);

    // Tambahkan data ke liststore
    GtkTreeIter iter;
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter, 0, pasien.no, 1, pasien.tanggal,
                       2, pasien.id_pasien, 3, pasien.diagnosis, 4, pasien.tindakan,
                       5, pasien.kontrol, 6, pasien.biaya, -1);
}

void editPasien1(Pasienn pasien) {
    const char *id_pasien = pasien.id_pasien;

    // Buka file CSV untuk dibaca dan tulis ulang data yang ada
    FILE *file = fopen("RiwayatMedis.csv", "r");
    if (file == NULL) {
        g_print("Gagal membuka file RiwayatMedis.csv untuk edit data.\n");
        return;
    }

    FILE *temp_file = fopen("temp.csv", "w");
    if (temp_file == NULL) {
        g_print("Gagal membuat file sementara untuk edit data.\n");
        fclose(file);
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        char *token = strtok(buffer, ",");
        int no = atoi(token);
        char *tanggal = strtok(NULL, ",");
        char *id = strtok(NULL, ",");
        char *diagnosis = strtok(NULL, ",");
        char *tindakan = strtok(NULL, ",");
        char *kontrol = strtok(NULL, ",");
        int biaya = atoi(strtok(NULL, "\n"));

        if (strcmp(id, id_pasien) == 0) {
            fprintf(temp_file, "%d,%s,%s,%s,%s,%s,%d\n", pasien.no, pasien.tanggal, pasien.id_pasien,
                    pasien.diagnosis, pasien.tindakan, pasien.kontrol, pasien.biaya);
        } else {
            fprintf(temp_file, "%d,%s,%s,%s,%s,%s,%d\n", no, tanggal, id,
                    diagnosis, tindakan, kontrol, biaya);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Hapus file asli dan ganti dengan file sementara
    if (remove("RiwayatMedis.csv") != 0) {
        g_print("Gagal menghapus file RiwayatMedis.csv.\n");
        return;
    }
    if (rename("temp.csv", "RiwayatMedis.csv") != 0) {
        g_print("Gagal mengganti nama file sementara.\n");
        return;
    }

    // Hapus data dari liststore dan refresh treeview
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(liststore), &iter)) {
        do {
            gchar *data_id_pasien;
            gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter, 2, &data_id_pasien, -1);
            if (strcmp(data_id_pasien, id_pasien) == 0) {
                gtk_list_store_set(liststore, &iter,
                                   0, pasien.no, 1, pasien.tanggal,
                                   2, pasien.id_pasien, 3, pasien.diagnosis,
                                   4, pasien.tindakan, 5, pasien.kontrol,
                                   6, pasien.biaya, -1);
                break;
            }
            g_free(data_id_pasien);
        } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(liststore), &iter));
    }
}

void hapusPasien1(const char *id_pasien) {
    // Buka file CSV untuk dibaca dan tulis ulang tanpa data yang akan dihapus
    FILE *file = fopen("RiwayatMedis.csv", "r");
    if (file == NULL) {
        g_print("Gagal membuka file RiwayatMedis.csv untuk hapus data.\n");
        return;
    }

    FILE *temp_file = fopen("temp.csv", "w");
    if (temp_file == NULL) {
        g_print("Gagal membuat file sementara untuk hapus data.\n");
        fclose(file);
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        char *token = strtok(buffer, ",");
        int no = atoi(token);
        char *tanggal = strtok(NULL, ",");
        char *data_id_pasien = strtok(NULL, ",");
        char *diagnosis = strtok(NULL, ",");
        char *tindakan = strtok(NULL, ",");
        char *kontrol = strtok(NULL, ",");
        int biaya = atoi(strtok(NULL, "\n"));

        if (strcmp(data_id_pasien, id_pasien) != 0) {
            fprintf(temp_file, "%d,%s,%s,%s,%s,%s,%d\n", no, tanggal, data_id_pasien,
                    diagnosis, tindakan, kontrol, biaya);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Hapus file asli dan ganti dengan file sementara
    if (remove("RiwayatMedis.csv") != 0) {
        g_print("Gagal menghapus file RiwayatMedis.csv.\n");
        return;
    }
    if (rename("temp.csv", "RiwayatMedis.csv") != 0) {
        g_print("Gagal mengganti nama file sementara.\n");
        return;
    }

    // Hapus data dari liststore dan refresh treeview
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(liststore), &iter)) {
        do {
            gchar *data_id_pasien;
            gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter, 2, &data_id_pasien, -1);
            if (strcmp(data_id_pasien, id_pasien) == 0) {
                gtk_list_store_remove(liststore, &iter);
                break;
            }
            g_free(data_id_pasien);
        } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(liststore), &iter));
    }
}

void populateListStoreFromCSV() {
    FILE *file = fopen("RiwayatMedis.csv", "r");
    if (file != NULL) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) {
            char *token = strtok(buffer, ",");
            int no = atoi(token);
            char *tanggal = strtok(NULL, ",");
            char *id_pasien = strtok(NULL, ",");
            char *diagnosis = strtok(NULL, ",");
            char *tindakan = strtok(NULL, ",");
            char *kontrol = strtok(NULL, ",");
            int biaya = atoi(strtok(NULL, "\n"));

            GtkTreeIter iter;
            gtk_list_store_append(liststore, &iter);
            gtk_list_store_set(liststore, &iter, 0, no, 1, tanggal,
                               2, id_pasien, 3, diagnosis, 4, tindakan,
                               5, kontrol, 6, biaya, -1);
        }
        fclose(file);
    } else {
        g_print("Gagal membuka file RiwayatMedis.csv untuk membaca data.\n");
    }
}

void on_button_tambah_clicked(GtkButton *button, gpointer user_data) {
    Pasienn pasien;
    pasien.no = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entryNo1));
    strcpy(pasien.tanggal, gtk_entry_get_text(GTK_ENTRY(entryTanggal1)));
    strcpy(pasien.id_pasien, gtk_entry_get_text(GTK_ENTRY(entryIDPasien1)));
    strcpy(pasien.diagnosis, gtk_entry_get_text(GTK_ENTRY(entryDiagnosis1)));
    strcpy(pasien.tindakan, gtk_entry_get_text(GTK_ENTRY(entryTindakan1)));
    strcpy(pasien.kontrol, gtk_entry_get_text(GTK_ENTRY(entryKontrol1)));
    pasien.biaya = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entryBiaya1));

    tambahPasien(pasien);
}

void on_button_edit_clicked(GtkButton *button, gpointer user_data) {
    Pasienn pasien;
    pasien.no = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entryNo1));
    strcpy(pasien.tanggal, gtk_entry_get_text(GTK_ENTRY(entryTanggal1)));
    strcpy(pasien.id_pasien, gtk_entry_get_text(GTK_ENTRY(entryIDPasien1)));
    strcpy(pasien.diagnosis, gtk_entry_get_text(GTK_ENTRY(entryDiagnosis1)));
    strcpy(pasien.tindakan, gtk_entry_get_text(GTK_ENTRY(entryTindakan1)));
    strcpy(pasien.kontrol, gtk_entry_get_text(GTK_ENTRY(entryKontrol1)));
    pasien.biaya = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entryBiaya1));

    editPasien1(pasien);
}

void on_button_hapus_clicked(GtkButton *button, gpointer user_data) {
    const char *id_pasien = gtk_entry_get_text(GTK_ENTRY(entryIDPasien1));
    hapusPasien1(id_pasien);
}

void create_gui_program2() {

    // Buat window utama
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sunting/Cari riwayat, diagnosis, dan tindakan pada pasien");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Buat box utama untuk menaruh elemen-elemen di dalamnya
    GtkWidget *box_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box_main);

    // Buat scrolled window untuk menampung tree view
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(box_main), scrolled_window, TRUE, TRUE, 0);

    // Buat tree view untuk menampilkan data
    liststore = gtk_list_store_new(7, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_INT);

    GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
    gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_window), 200);
gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled_window), 400);

    // Menambahkan kolom-kolom ke dalam tree view
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("No", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Tanggal", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID Pasienn", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Diagnosis", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Tindakan", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Kontrol", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Biaya (Rp)", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Buat box untuk menaruh entry dan tombol
    GtkWidget *box_input = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(box_main), box_input, FALSE, FALSE, 10);

    // Entry dan label untuk input data
    GtkWidget *labelNo = gtk_label_new("No:");
    entryNo1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_input), labelNo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), entryNo1, FALSE, FALSE, 0);

    GtkWidget *labelTanggal = gtk_label_new("Tanggal:");
    entryTanggal1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_input), labelTanggal, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), entryTanggal1, FALSE, FALSE, 0);

    GtkWidget *labelIDPasien = gtk_label_new("ID Pasienn:");
    entryIDPasien1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_input), labelIDPasien, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), entryIDPasien1, FALSE, FALSE, 0);

    GtkWidget *labelDiagnosis = gtk_label_new("Diagnosis:");
    entryDiagnosis1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_input), labelDiagnosis, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), entryDiagnosis1, FALSE, FALSE, 0);

    GtkWidget *labelTindakan = gtk_label_new("Tindakan:");
    entryTindakan1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_input), labelTindakan, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), entryTindakan1, FALSE, FALSE, 0);

    GtkWidget *labelKontrol = gtk_label_new("Kontrol:");
    entryKontrol1 = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box_input), labelKontrol, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), entryKontrol1, FALSE, FALSE, 0);

    GtkWidget *labelBiaya = gtk_label_new("Biaya (Rp):");
    entryBiaya1 = gtk_spin_button_new_with_range(0, 1000000000, 1000);
    gtk_box_pack_start(GTK_BOX(box_input), labelBiaya, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), entryBiaya1, FALSE, FALSE, 0);

    // Tombol-tombol untuk tambah, edit, dan hapus data
    GtkWidget *buttonTambah = gtk_button_new_with_label("Tambah");
    GtkWidget *buttonEdit = gtk_button_new_with_label("Edit");
    GtkWidget *buttonHapus = gtk_button_new_with_label("Hapus");

    g_signal_connect(buttonTambah, "clicked", G_CALLBACK(on_button_tambah_clicked), NULL);
    g_signal_connect(buttonEdit, "clicked", G_CALLBACK(on_button_edit_clicked), NULL);
    g_signal_connect(buttonHapus, "clicked", G_CALLBACK(on_button_hapus_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(box_input), buttonTambah, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), buttonEdit, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_input), buttonHapus, FALSE, FALSE, 0);

    // Populasi liststore dari file CSV
    populateListStoreFromCSV();

    // Tampilkan semua elemen
    gtk_widget_show_all(window);

    // Jalankan aplikasi GTK
    gtk_main();

    return 0;
}
