#include <gtk/gtk.h>
#include <time.h>
#include <glib.h>

static GtkWidget *label_time;
static GtkWidget *label_date;
static GtkWidget *main_container;

static gboolean update_time(gpointer user_data) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char time_str[12]; // HH:MM:SS AM/PM format
    strftime(time_str, sizeof(time_str), "%I:%M:%S %p", tm_info);
    gtk_label_set_text(GTK_LABEL(label_time), time_str);

    char date_str[20]; // Day, Month, Year format
    strftime(date_str, sizeof(date_str), "%A, %B %d, %Y", tm_info);
    gtk_label_set_text(GTK_LABEL(label_date), date_str);

    return TRUE; // Continue calling this function to keep updating the time
}

static void on_swipe_to_unlock(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(main_container);

    // Launch nav_home/main from the current directory
    GError *error = NULL;
    if (!g_spawn_command_line_async("/home/sabarinath/faeture_phone/home/main", &error)) {
        g_printerr("Failed to launch nav_home/main: %s\n", error->message);
        g_clear_error(&error);
    }

    // Quit current GTK app
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Lock Screen");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 480); // 320x480 display
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Main container (Black background)
    main_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_container);
    gtk_widget_override_background_color(main_container, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 0, 1}); // Black background

    // Time label
    label_time = gtk_label_new("00:00:00");
    gtk_widget_override_font(label_time, pango_font_description_from_string("Arial 40"));
    gtk_label_set_xalign(GTK_LABEL(label_time), 0.5); // Center the label
    gtk_box_pack_start(GTK_BOX(main_container), label_time, FALSE, FALSE, 0);

    // Date label
    label_date = gtk_label_new("Monday, January 01, 2022");
    gtk_widget_override_font(label_date, pango_font_description_from_string("Arial 20"));
    gtk_label_set_xalign(GTK_LABEL(label_date), 0.5); // Center the label
    gtk_box_pack_start(GTK_BOX(main_container), label_date, FALSE, FALSE, 0);

    // Swipe area to unlock (simulated with a button for now)
    GtkWidget *swipe_button = gtk_button_new_with_label("Swipe to Unlock");
    gtk_widget_override_background_color(swipe_button, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 0.647, 0, 1}); // Orange background for button
    gtk_widget_override_font(swipe_button, pango_font_description_from_string("Arial 18"));
    gtk_box_pack_start(GTK_BOX(main_container), swipe_button, FALSE, FALSE, 20);

    // Connect swipe action
    g_signal_connect(swipe_button, "clicked", G_CALLBACK(on_swipe_to_unlock), NULL);

    // Update the time every second
    g_timeout_add_seconds(1, update_time, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
