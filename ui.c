#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

GtkWidget *status_label;
GtkWidget *start_button;
GtkWidget *stop_button;
GtkTextBuffer *log_buffer;
int server_running = 0;
pthread_t server_thread;

void append_log(const char *message) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(log_buffer, &end);
    gtk_text_buffer_insert(log_buffer, &end, message, -1);
    gtk_text_buffer_insert(log_buffer, &end, "\n", -1);
}

void *server_thread_func(void *data) {
    append_log("Сервер запущен на порту 2121");
    while(server_running) {
        sleep(1);
    }
    return NULL;
}

void on_start_clicked(GtkButton *button, gpointer user_data) {
    if (!server_running) {
        server_running = 1;
        pthread_create(&server_thread, NULL, server_thread_func, NULL);
        gtk_label_set_text(GTK_LABEL(status_label), "Статус: Запущен");
        gtk_widget_set_sensitive(start_button, FALSE);
        gtk_widget_set_sensitive(stop_button, TRUE);
        append_log("Команда: Старт сервера");
    }
}

void on_stop_clicked(GtkButton *button, gpointer user_data) {
    if (server_running) {
        server_running = 0;
        pthread_join(server_thread, NULL);
        gtk_label_set_text(GTK_LABEL(status_label), "Статус: Остановлен");
        gtk_widget_set_sensitive(start_button, TRUE);
        gtk_widget_set_sensitive(stop_button, FALSE);
        append_log("Команда: Стоп сервера");
    }
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *log_view;
    GtkWidget *scroll;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "FTP Сервер с архивом");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    start_button = gtk_button_new_with_label("Старт сервера");
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), start_button, 0, 0, 1, 1);

    stop_button = gtk_button_new_with_label("Стоп сервера");
    g_signal_connect(stop_button, "clicked", G_CALLBACK(on_stop_clicked), NULL);
    gtk_widget_set_sensitive(stop_button, FALSE);
    gtk_grid_attach(GTK_GRID(grid), stop_button, 1, 0, 1, 1);

    status_label = gtk_label_new("Статус: Остановлен");
    gtk_grid_attach(GTK_GRID(grid), status_label, 2, 0, 1, 1);

    log_buffer = gtk_text_buffer_new(NULL);
    log_view = gtk_text_view_new_with_buffer(log_buffer);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(log_view), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(log_view), TRUE);
    
    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), log_view);
    gtk_grid_attach(GTK_GRID(grid), scroll, 0, 1, 3, 1);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.ftpserver", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}