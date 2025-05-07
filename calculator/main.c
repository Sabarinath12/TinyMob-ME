#include <gtk/gtk.h>
#include <ctype.h>
#include <math.h>

static GtkWidget *entry;

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *text = gtk_button_get_label(GTK_BUTTON(widget));
    const char *current_text = gtk_entry_get_text(GTK_ENTRY(entry));

    if (g_strcmp0(text, "=") == 0) {
        // Simple evaluation (note: this is not a safe eval - for demo only)
        double result = 0;
        char expression[256];
        snprintf(expression, sizeof(expression), "echo \"%s\" | bc -l", current_text);
        
        FILE *fp = popen(expression, "r");
        if (fp) {
            fscanf(fp, "%lf", &result);
            pclose(fp);
            
            char result_text[256];
            snprintf(result_text, sizeof(result_text), "%g", result);
            gtk_entry_set_text(GTK_ENTRY(entry), result_text);
        }
    }
    else if (g_strcmp0(text, "C") == 0) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    }
    else {
        char new_text[256];
        snprintf(new_text, sizeof(new_text), "%s%s", current_text, text);
        gtk_entry_set_text(GTK_ENTRY(entry), new_text);
    }
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
        "    background-color: #232526;"
        "}"
        "entry {"
        "    font-family: 'Inter', sans-serif;"
        "    font-size: 32px;"
        "    font-weight: 300;"
        "    color: white;"
        "    background-color: #2a2a2a;"
        "    border: 1px solid #404040;"
        "    padding: 20px;"
        "    margin-bottom: 10px;"
        "}"
        "button {"
        "    font-family: 'Inter', sans-serif;"
        "    font-size: 24px;"
        "    font-weight: 400;"
        "    color: white;"
        "    border-radius: 8px;"
        "    border: none;"
        "    padding: 20px;"
        "    transition: all 100ms ease;"
        "}"
        "button.number {"
        "    background-color: #2a2a2a;"
        "    border: 1px solid #404040;"
        "}"
        "button.number:hover {"
        "    background-color: #363636;"
        "}"
        "button.operator {"
        "    background-color: #2a6bff;"
        "}"
        "button.operator:hover {"
        "    background-color: #387aff;"
        "}"
        "button.clear {"
        "    background-color: #ff3b30;"
        "}"
        "button.clear:hover {"
        "    background-color: #ff5a50;"
        "}"
        "button.equals {"
        "    background-color: #2dd36f;"
        "}"
        "button.equals:hover {"
        "    background-color: #3ae456;"
        "}";

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    g_object_unref(provider);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    load_css();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 480);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Calculator display
    entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1.0); // Right aligned
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // Button layout
    const char *buttons[5][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"C", "0", "=", "+"}
    };

    // Create buttons with appropriate CSS classes
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            GtkWidget *button = gtk_button_new_with_label(buttons[i][j]);
            g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

            // Set CSS classes based on button type
            if (isdigit(buttons[i][j][0]) || g_strcmp0(buttons[i][j], "0") == 0) {
                gtk_style_context_add_class(gtk_widget_get_style_context(button), "number");
            } 
            else if (g_strcmp0(buttons[i][j], "C") == 0) {
                gtk_style_context_add_class(gtk_widget_get_style_context(button), "clear");
            }
            else if (g_strcmp0(buttons[i][j], "=") == 0) {
                gtk_style_context_add_class(gtk_widget_get_style_context(button), "equals");
            }
            else {
                gtk_style_context_add_class(gtk_widget_get_style_context(button), "operator");
            }

            gtk_grid_attach(GTK_GRID(grid), button, j, i + 1, 1, 1);
        }
    }

    // Grid configuration
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
