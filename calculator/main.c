#include <gtk/gtk.h>
#include <ctype.h>

static GtkWidget *entry;

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *text = gtk_button_get_label(GTK_BUTTON(widget));
    const char *current_text = gtk_entry_get_text(GTK_ENTRY(entry));

    // If "=" is clicked, evaluate the expression
    if (g_strcmp0(text, "=") == 0) {
        double result = 0;
        sscanf(current_text, "%lf", &result);
        char result_text[512];
        snprintf(result_text, sizeof(result_text), "%f", result);
        gtk_entry_set_text(GTK_ENTRY(entry), result_text);
    }
    // If "C" is clicked, clear the entry field
    else if (g_strcmp0(text, "C") == 0) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    else {
        // Check if the input is valid: only numbers and operators are allowed
        if (isdigit(text[0]) || g_strcmp0(text, "+") == 0 || g_strcmp0(text, "-") == 0 ||
            g_strcmp0(text, "*") == 0 || g_strcmp0(text, "/") == 0) {

            // Append the clicked button text to the entry
            char new_text[256];
            snprintf(new_text, sizeof(new_text), "%s%s", current_text, text);
            gtk_entry_set_text(GTK_ENTRY(entry), new_text);
        }
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 480);  // Set size to match the home screen size
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Entry for calculator display
    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // Define button labels
    const char *buttons[5][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"C", "0", "=", "+"}
    };

    // Add buttons to grid
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            GtkWidget *button = gtk_button_new_with_label(buttons[i][j]);
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

            // Set button properties to expand and fill the space
            gtk_widget_set_halign(button, GTK_ALIGN_FILL);
            gtk_widget_set_valign(button, GTK_ALIGN_FILL);
            gtk_grid_attach(GTK_GRID(grid), button, j, i + 1, 1, 1);
        }
    }

    // Adjust the grid properties to ensure it looks good on a feature phone display
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5); // Space between rows
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5); // Space between columns

    // Create and apply custom CSS for styling (larger buttons, more spacing)
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        "button {"
        "   font-size: 18px;"
        "   padding: 15px;"
        "   width: 60px;"
        "   height: 60px;"
        "   text-align: center;"
        "   background-color: orange;"   // Orange background for buttons
        "   color: black;"               // Black text color
        "   border-radius: 10px;"
        "}"
        "window {"
        "   background-color: black;"   // Black background for the window
        "}"
        , -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
