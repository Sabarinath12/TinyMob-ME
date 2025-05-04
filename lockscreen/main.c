#include <gtk/gtk.h>
#include <time.h>
#include <glib.h>

static GtkWidget *label_time;
static GtkWidget *label_date;
static GtkWidget *main_container;
static gboolean airplane_on = FALSE;
static gboolean torch_on = FALSE;

// Toggle Airplane Mode
static void toggle_airplane(GtkButton *button, gpointer user_data) {
    airplane_on = !airplane_on;
    const char *label = airplane_on ? "Airplane Mode: ON" : "Airplane Mode: OFF";
    gtk_button_set_label(button, label);

    // Change color when clicked
    GdkRGBA new_color = airplane_on ? (GdkRGBA){1.0, 0.647, 0.0, 1.0} : (GdkRGBA){0.6, 0.6, 0.6, 1.0}; // Orange for ON, Gray for OFF
    gtk_widget_override_background_color(GTK_WIDGET(button), GTK_STATE_FLAG_NORMAL, &new_color);
}

// Toggle Torch
static void toggle_torch(GtkButton *button, gpointer user_data) {
    torch_on = !torch_on;
    const char *label = torch_on ? "Torch: ON" : "Torch: OFF";
    gtk_button_set_label(button, label);

    // Change color when clicked
    GdkRGBA new_color = torch_on ? (GdkRGBA){1.0, 0.647, 0.0, 1.0} : (GdkRGBA){0.6, 0.6, 0.6, 1.0}; // Orange for ON, Gray for OFF
    gtk_widget_override_background_color(GTK_WIDGET(button), GTK_STATE_FLAG_NORMAL, &new_color);
}

// Update Time and Date
static gboolean update_time(gpointer user_data) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char time_str[12]; // HH:MM:SS AM/PM
    strftime(time_str, sizeof(time_str), "%I:%M:%S %p", tm_info);
    gtk_label_set_text(GTK_LABEL(label_time), time_str);

    char date_str[30]; // Day, Month DD, YYYY
    strftime(date_str, sizeof(date_str), "%A, %B %d, %Y", tm_info);
    gtk_label_set_text(GTK_LABEL(label_date), date_str);

    return TRUE;
}

// Handle Unlock
static void on_swipe_to_unlock(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(main_container);

    GError *error = NULL;
    if (!g_spawn_command_line_async("/home/main", &error)) {
        g_printerr("Failed to launch nav_home/main: %s\n", error->message);
        g_clear_error(&error);
    }

    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Lock Screen");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 480);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Main container (Black background)
    main_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_container);
    gtk_widget_override_background_color(main_container, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0, 0, 0, 1}); // Black background

    // Time label
    label_time = gtk_label_new("00:00:00");
    gtk_widget_override_font(label_time, pango_font_description_from_string("Arial Bold 40"));
    gtk_label_set_xalign(GTK_LABEL(label_time), 0.5);
    gtk_box_pack_start(GTK_BOX(main_container), label_time, FALSE, FALSE, 15);

    // Date label
    label_date = gtk_label_new("Monday, January 01, 2022");
    gtk_widget_override_font(label_date, pango_font_description_from_string("Arial 20"));
    gtk_label_set_xalign(GTK_LABEL(label_date), 0.5);
    gtk_box_pack_start(GTK_BOX(main_container), label_date, FALSE, FALSE, 5);

    // --- Stylish Buttons Section ---
    GtkWidget *btn_airplane = gtk_button_new_with_label("Airplane Mode: OFF");
    gtk_widget_override_font(btn_airplane, pango_font_description_from_string("Arial 16"));
    gtk_widget_override_background_color(btn_airplane, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0.6, 0.6, 0.6, 1}); // Gray for OFF
    gtk_widget_set_size_request(btn_airplane, 220, 45);
    gtk_widget_set_margin_top(btn_airplane, 15);
    gtk_widget_set_margin_bottom(btn_airplane, 15);
    gtk_widget_override_color(btn_airplane, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 1, 1}); // White text
    gtk_box_pack_start(GTK_BOX(main_container), btn_airplane, FALSE, FALSE, 0);
    g_signal_connect(btn_airplane, "clicked", G_CALLBACK(toggle_airplane), NULL);

    GtkWidget *btn_torch = gtk_button_new_with_label("Torch: OFF");
    gtk_widget_override_font(btn_torch, pango_font_description_from_string("Arial 16"));
    gtk_widget_override_background_color(btn_torch, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0.6, 0.6, 0.6, 1}); // Gray for OFF
    gtk_widget_set_size_request(btn_torch, 220, 45);
    gtk_widget_set_margin_top(btn_torch, 10);
    gtk_widget_set_margin_bottom(btn_torch, 10);
    gtk_widget_override_color(btn_torch, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1, 1, 1, 1});
    gtk_box_pack_start(GTK_BOX(main_container), btn_torch, FALSE, FALSE, 0);
    g_signal_connect(btn_torch, "clicked", G_CALLBACK(toggle_torch), NULL);
    // --- End Buttons ---

    // Unlock button
    GtkWidget *swipe_button = gtk_button_new_with_label("UNLOCK");
    gtk_widget_override_background_color(swipe_button, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1.0, 0.647, 0.0, 1}); // Orange
    gtk_widget_override_font(swipe_button, pango_font_description_from_string("Arial 18"));
    gtk_widget_set_margin_top(swipe_button, 30);
    gtk_widget_set_margin_bottom(swipe_button, 30);
    gtk_widget_set_size_request(swipe_button, 220, 45);
    gtk_box_pack_start(GTK_BOX(main_container), swipe_button, FALSE, FALSE, 0);
    g_signal_connect(swipe_button, "clicked", G_CALLBACK(on_swipe_to_unlock), NULL);

    g_timeout_add_seconds(1, update_time, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
