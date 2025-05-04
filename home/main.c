#include <gtk/gtk.h>
#include <stdlib.h>  // For system() or g_spawn_command_line_async()
#include <unistd.h>  // For sleep()

// Callback to open Calculator window and run its binary
static void open_calculator(GtkWidget *widget, gpointer data) {
    // Close the current window
    GtkWidget *current_window = GTK_WIDGET(data);
    gtk_widget_destroy(current_window);

    // Launch the new process for the calculator
    const char *calc_bin_path = "/home/sabarinath/faeture_phone/calculator/main";

    // Launch the calculator binary as a new process
    g_spawn_command_line_async(calc_bin_path, NULL);  // Assuming `main` is the executable

    // Optionally, you can add error handling if the binary isn't found
    // If you need to check for the process status, you can use g_spawn_async() for more control.
}

// Main function to create the home screen
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Home Screen");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 480); // Feature phone screen size
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Adjust grid to have a simple, spacious, and usable layout for a feature phone
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10); // Add space between rows
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10); // Add space between columns

    // Create a custom CSS for styling (we'll make buttons larger and more spaced out)
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        "button {"
        "   font-size: 20px;"
        "   padding: 20px;"
        "   width: 100px;"
        "   height: 100px;"
        "   text-align: center;"
        "   background-color: orange;"   // Orange background for buttons
        "   color: black;"               // Black text color
        "   border-radius: 15px;"
        "}"
        "window {"
        "   background-color: black;"   // Black background for the window
        "}"
        , -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Buttons for the home screen
    GtkWidget *calc_button = gtk_button_new_with_label("Calculator");
    g_signal_connect(calc_button, "clicked", G_CALLBACK(open_calculator), window);  // Pass the current window
    gtk_grid_attach(GTK_GRID(grid), calc_button, 0, 0, 1, 1);

    GtkWidget *message_button = gtk_button_new_with_label("Messages");
    gtk_grid_attach(GTK_GRID(grid), message_button, 1, 0, 1, 1);

    GtkWidget *contacts_button = gtk_button_new_with_label("Contacts");
    gtk_grid_attach(GTK_GRID(grid), contacts_button, 0, 1, 1, 1);

    GtkWidget *settings_button = gtk_button_new_with_label("Settings");
    gtk_grid_attach(GTK_GRID(grid), settings_button, 1, 1, 1, 1);

    // Add more buttons if needed
    GtkWidget *calendar_button = gtk_button_new_with_label("Calendar");
    gtk_grid_attach(GTK_GRID(grid), calendar_button, 0, 2, 1, 1);

    GtkWidget *clock_button = gtk_button_new_with_label("Clock");
    gtk_grid_attach(GTK_GRID(grid), clock_button, 1, 2, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
