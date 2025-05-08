#include <gtk/gtk.h>
#include <time.h>
#include <glib.h>

static GtkWidget *label_time;
static GtkWidget *label_date;
static GtkWidget *main_container;

static gboolean update_time(gpointer user_data) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char time_str[12];
    strftime(time_str, sizeof(time_str), "%I:%M %p", tm_info);
    gtk_label_set_text(GTK_LABEL(label_time), time_str);

    char date_str[30];
    strftime(date_str, sizeof(date_str), "%A, %B %d", tm_info);
    gtk_label_set_text(GTK_LABEL(label_date), date_str);

    return TRUE;
}

static void on_unlock_clicked(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(main_container);

    GError *error = NULL;
    if (!g_spawn_command_line_async("/home/main", &error)) {
        g_printerr("Failed to launch nav_home/main: %s\n", error->message);
        g_clear_error(&error);
    }

    gtk_main_quit();
}

static void load_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    
    gtk_style_context_add_provider_for_screen(screen,
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *css = 
        "window {"
        "    background: #000000;"
        "}"
        "label#time-label {"
        "    font-family: 'Inter', sans-serif;"
        "    font-size: 72px;"
        "    font-weight: 300;"
        "    color: #ffffff;"
        "    margin-top: 120px;"
        "}"
        "label#date-label {"
        "    font-family: 'Inter', sans-serif;"
        "    font-size: 16px;"
        "    color: #a0a0a0;"
        "    margin-bottom: 100px;"
        "}"
        ".ios-status-bar {"
        "    background-color: rgba(0, 0, 0, 0.65);"
        "    padding: 8px 12px;"
        "    border-radius: 0;"
        "}"
        ".signal-dots {"
        "    spacing: 4px;"
        "}"
        ".signal-dot {"
        "    min-width: 6px;"
        "    min-height: 6px;"
        "    border-radius: 50%;"
        "    background-color: rgba(255,255,255,0.4);"
        "    border: 1px solid rgba(255,255,255,0.4);"
        "}"
        ".signal-dot.active {"
        "    background-color: #ffffff;"
        "    border-color: #ffffff;"
        "}"
        ".carrier-label {"
        "    color: #ffffff;"
        "    font-family: 'Helvetica Neue', sans-serif;"
        "    font-size: 14px;"
        "    font-weight: 500;"
        "    margin-left: 8px;"
        "}"
        ".network-label {"
        "    color: #ffffff;"
        "    font-family: 'Helvetica Neue', sans-serif;"
        "    font-size: 14px;"
        "    font-weight: 500;"
        "}"
        ".unlock-indicator {"
        "    color: rgba(255,255,255,0.9);"
        "    font-family: 'Helvetica Neue', sans-serif;"
        "    font-size: 20px;"
        "    font-weight: 400;"
        "    letter-spacing: 0.5px;"
        "    padding: 16px 32px;"
        "    border-radius: 24px;"
        "    background-color: rgba(255,255,255,0.1);"
        "    border: 1px solid rgba(255,255,255,0.15);"
        "    margin-bottom: 40px;"
        "    transition: all 200ms ease;"
        "}"
        ".unlock-indicator:hover {"
        "    background-color: rgba(255,255,255,0.15);"
        "    transform: translateY(-2px);"
        "}"
        "@keyframes subtle-bounce {"
        "    0%, 100% { transform: translateY(0); }"
        "    50% { transform: translateY(-4px); }"
        "}"
        ".unlock-indicator {"
        "    animation: subtle-bounce 2s ease-in-out infinite;"
        "}";

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    g_object_unref(provider);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    load_css();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "System Lock");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 480);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    main_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_container);

    // style Status Bar
    GtkWidget *status_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(status_bar);
    gtk_box_pack_start(GTK_BOX(main_container), status_bar, FALSE, FALSE, 0);
    
    // Left side: Signal dots and Carrier
    GtkWidget *left_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *signal_dots = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_widget_set_name(signal_dots, "signal-dots");
    
    // Add 5 signal dots
    for(int i = 0; i < 5; i++) {
        GtkWidget *dot = gtk_label_new("");
        gtk_widget_set_size_request(dot, 6, 6);
        gtk_widget_set_name(dot, i < 3 ? "signal-dot active" : "signal-dot");
        gtk_box_pack_start(GTK_BOX(signal_dots), dot, FALSE, FALSE, 0);
    }
    
   
    
    // Right side: Network type
    GtkWidget *right_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *network_label = gtk_label_new("LTE");
  
    gtk_box_pack_end(GTK_BOX(right_container), network_label, FALSE, FALSE, 4);
    
    
    // Pack status bar elements
    gtk_box_pack_start(GTK_BOX(status_bar), left_container, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(status_bar), right_container, FALSE, FALSE, 0);

    // Time and Date
    GtkWidget *time_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(main_container), time_box, TRUE, TRUE, 0);
    
    label_time = gtk_label_new("00:00");
    gtk_widget_set_name(label_time, "time-label");
    gtk_label_set_xalign(GTK_LABEL(label_time), 0.5);
    gtk_box_pack_start(GTK_BOX(time_box), label_time, FALSE, FALSE, 0);

    label_date = gtk_label_new("Monday, January 01");
    gtk_widget_set_name(label_date, "date-label");
    gtk_label_set_xalign(GTK_LABEL(label_date), 0.5);
    gtk_box_pack_start(GTK_BOX(time_box), label_date, FALSE, FALSE, 8);

    // Unlock Indicator
    GtkWidget *unlock_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_end(GTK_BOX(main_container), unlock_container, TRUE, TRUE, 0);
    
    GtkWidget *unlock_indicator = gtk_event_box_new();
    GtkWidget *unlock_label = gtk_label_new(">>");
    gtk_widget_set_name(unlock_indicator, "unlock-indicator");
    gtk_container_add(GTK_CONTAINER(unlock_indicator), unlock_label);
    gtk_widget_set_halign(unlock_indicator, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(unlock_indicator, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(unlock_container), unlock_indicator, FALSE, FALSE, 0);
    g_signal_connect(unlock_indicator, "button-press-event", G_CALLBACK(on_unlock_clicked), NULL);

    g_timeout_add_seconds(1, update_time, NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
