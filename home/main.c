#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>

static void open_app(GtkWidget *widget, gpointer data) {
    const char *app_name = (const char *)g_object_get_data(G_OBJECT(widget), "app_name");
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
        "    background-color: #2d2d2d;"
        "    border: 1px solid #383838;"
        "    border-radius: 12px;"
        "    padding: 0;"
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
        "    min-width: 64px;"
        "    min-height: 64px;"
        "    background-size: contain;"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "}";

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    g_object_unref(provider);
}

GtkWidget* create_app_button(const char *icon_path, const char *app_name, GtkWidget *window) {
    GtkWidget *button = gtk_button_new();
    GtkWidget *icon = gtk_image_new_from_file(icon_path);
    
    // Set the icon as the button content
    gtk_container_add(GTK_CONTAINER(button), icon);
    
    // Store the app name with the button
    g_object_set_data(G_OBJECT(button), "app_name", (gpointer)app_name);
    
    // Connect the click handler
    g_signal_connect(button, "clicked", G_CALLBACK(open_app), window);
    
    // Apply CSS class
    GtkStyleContext *context = gtk_widget_get_style_context(icon);
    gtk_style_context_add_class(context, "app-icon");
    
    return button;
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

    // Create app buttons with transparent PNG icons 144x144px
    const char *apps[][2] = {
        {"/path/to/calculator.png", "Calculator"},
        {"/path/to/messages.png", "Messages"},
        {"/path/to/contacts.png", "Contacts"},
        {"assets/set.png", "Settings"},
        {"/path/to/calendar.png", "Calendar"},
        {"/path/to/clock.png", "Clock"}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            int index = i * 2 + j;
            if (index >= 6) break;

            GtkWidget *button = create_app_button(apps[index][0], apps[index][1], window);
            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
        }
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
