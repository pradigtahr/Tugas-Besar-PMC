#include <gtk/gtk.h>
#include <stdlib.h>
#include "page1.c"
#include "page2.c"
#include "page3.c"
#include "page4.c"
#include "page5.c"
#include "page6.c"

// Fungsi untuk menjalankan program eksternal
static void run_program(const char *program_name) {
    char command[256];
    snprintf(command, sizeof(command), "./%s", program_name);

    int ret = system(command);
    if (ret == -1) {
        g_print("Failed to execute command: %s\n", command);
    }
}

// Callback untuk menjalankan Program 1 (GTK)
static void run_program_1(GtkWidget *widget, gpointer data) {
    create_gui_program1(); // Memanggil fungsi untuk membuat GUI program1
}

// Callback untuk menjalankan Program 2
static void run_program_2(GtkWidget *widget, gpointer data) {
    create_gui_program2(); // Memanggil fungsi untuk membuat GUI program1
}

static void run_program_3(GtkWidget *widget, gpointer data) {
    create_gui_program3(); // Memanggil fungsi untuk membuat GUI program1
}

static void run_program_4(GtkWidget *widget, gpointer data) {
    create_gui_program4(); // Memanggil fungsi untuk membuat GUI program1
}

static void run_program_5(GtkWidget *widget, gpointer data) {
    create_gui_program5(); // Memanggil fungsi untuk membuat GUI program1
}

static void run_program_6(GtkWidget *widget, gpointer data) {
    create_gui_program6(); // Memanggil fungsi untuk membuat GUI program1
}

// Toggle fullscreen
static void toggle_fullscreen(GtkWidget *widget, gpointer data) {
    GtkWidget *window = GTK_WIDGET(data);
    gtk_window_fullscreen(GTK_WINDOW(window));
}

// Callback untuk keluar dari aplikasi
static void app_quit(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    // Inisialisasi GTK
    gtk_init(&argc, &argv);
    GtkWidget *image;
    // Membuat window utama
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Aplikasi Pencatatan Database Pasien");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    // Membuat box container
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);


    // Membuat menu bar
    GtkWidget *menubar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0);

    // Membuat menu utama (File)
    GtkWidget *file_menu = gtk_menu_new();
    GtkWidget *file_menu_item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu_item);


    GtkWidget *file_menu2 = gtk_menu_new();
    GtkWidget *file_menu_item2 = gtk_menu_item_new_with_label("Data Pasien");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item2), file_menu2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu_item2);


    GtkWidget *file_menu3 = gtk_menu_new();
    GtkWidget *file_menu_item3 = gtk_menu_item_new_with_label("Informasi Pasien");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item3), file_menu3);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu_item3);

    // Menambahkan item menu (Menjalankan Program 1)
    GtkWidget *menu_program1 = gtk_menu_item_new_with_label("Sunting/Cari data pasien");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu2), menu_program1);
    g_signal_connect(menu_program1, "activate", G_CALLBACK(run_program_1), NULL);

    // Menambahkan item menu (Menjalankan Program 2)
    GtkWidget *menu_program2 = gtk_menu_item_new_with_label("Sunting/Cari riwayat, diagnosis, dan tindakan pada pasien");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu2), menu_program2);
    g_signal_connect(menu_program2, "activate", G_CALLBACK(run_program_2), NULL);

    GtkWidget *menu_program3 = gtk_menu_item_new_with_label("Cek informasi dan riwayat medis pasien");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu3), menu_program3);
    g_signal_connect(menu_program3, "activate", G_CALLBACK(run_program_3), NULL);

        GtkWidget *menu_program4 = gtk_menu_item_new_with_label("Cek pendapatan bulanan, tahunan, dan rata-rata pendapatan per-tahun");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu3), menu_program4);
    g_signal_connect(menu_program4, "activate", G_CALLBACK(run_program_4), NULL);

        GtkWidget *menu_program5 = gtk_menu_item_new_with_label("Cek jumlah pasien dan penyakit yang diderita pasien");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu3), menu_program5);
    g_signal_connect(menu_program5, "activate", G_CALLBACK(run_program_5), NULL);

    GtkWidget *menu_program6 = gtk_menu_item_new_with_label("Cek informasi pasien yang memerlukan kontrol");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu3), menu_program6);
    g_signal_connect(menu_program6, "activate", G_CALLBACK(run_program_6), NULL);

    // Menambahkan separator di menu File
    GtkWidget *separator = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), separator);

    GtkWidget *menu_fullscreen = gtk_menu_item_new_with_label("Fullscreen");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_fullscreen);
    g_signal_connect(menu_fullscreen, "activate", G_CALLBACK(toggle_fullscreen), window);

    // Menambahkan item menu (Exit)
    GtkWidget *menu_exit = gtk_menu_item_new_with_label("Exit");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_exit);
    g_signal_connect(menu_exit, "activate", G_CALLBACK(app_quit), NULL);

    // Menambahkan item menu (Fullscreen)
    image = gtk_image_new_from_file("background23.jpg"); // Ganti dengan path gambar latar belakang Anda
    gtk_container_add(GTK_CONTAINER(box), image);

    // Menampilkan semua widget
    gtk_widget_show_all(window);

    // Memulai GTK main loop
    gtk_main();

    return 0;
}
