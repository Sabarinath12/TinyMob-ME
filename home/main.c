#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>

static void open_app(GtkWidget *widget, gpointer data) {
    const char *app_name = gtk_button_get_label(GTK_BUTTON(widget));
    GtkWidget *current_window = GTK_WIDGET(data);
    gtk_widget_destroy(current_window);

    if (g_strcmp0(app_name, "Calculator") == 0) {
        g_spawn_command_line_async("/calculator/main", NULL);
    } else if (g_strcmp0(app_name, "Messages") == 0) {
        g_spawn_command_line_async("/messages/main", NULL);
    }
    // Add other apps here
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
        "    background-color: #1a1a1a;"
        "}"
        "button {"
        "    font-family: 'Inter', sans-serif;"
        "    font-size: 18px;"
        "    font-weight: 400;"
        "    color: #e0e0e0;"
        "    background-color: #2d2d2d;"
        "    border: 1px solid #383838;"
        "    border-radius: 12px;"
        "    padding: 20px;"
        "    margin: 8px;"
        "    transition: all 150ms ease;"
        "}"
        "button:hover {"
        "    background-color: #353535;"
        "    box-shadow: 0 2px 8px rgba(0,0,0,0.3);"
        "}"
        "button:active {"
        "    background-color: #252525;"
        "    transform: scale(0.98);"
        "}"
        ".app-icon {"
        "    font-size: 32px;"
        "    margin-bottom: 8px;"
        "}"
        ".center-box {"
        "    justify-content: center;"
        "    align-items: center;"
        "}";

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    g_object_unref(provider);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    load_css();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Home Screen");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 480);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Configure grid layout
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    // Create app buttons with icons
    const char *apps[][2] = {
        {"Calculator"},
        {"Messages"},
        {"Contacts"},
        {"Settings"},
        {"Calendar"},
        {"Clock"}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            int index = i * 2 + j;
            if (index >= 6) break;

            GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_widget_set_name(box, "center-box");  // Apply centering style
            gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
            gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
            
            GtkWidget *icon = gtk_label_new(apps[index][0]);
            GtkWidget *label = gtk_label_new(apps[index][1]);
            
            // Center-align the labels
            gtk_label_set_xalign(GTK_LABEL(icon), 0.5);
            gtk_label_set_xalign(GTK_LABEL(label), 0.5);
            
            gtk_widget_set_name(icon, "app-icon");
            gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
            
            GtkWidget *button = gtk_button_new();
            gtk_container_add(GTK_CONTAINER(button), box);
            
            g_signal_connect(button, "clicked", G_CALLBACK(open_app), window);
            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
        }
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
