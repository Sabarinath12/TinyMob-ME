//Take kernel logs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#define BUFFER_SIZE 4096
#define DEFAULT_BUFFER_LINES 100

typedef enum {
    FORMAT_JSON,
    FORMAT_XML
} OutputFormat;

volatile sig_atomic_t keep_running = 1;

void handle_signal(int signal) {
    keep_running = 0;
}

void setup_signal_handler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void write_json_header(FILE *fp) {
    fprintf(fp, "[\n");
}

void write_json_footer(FILE *fp) {
    fprintf(fp, "\n]\n");
}

void write_xml_header(FILE *fp) {
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<logs>\n");
}

void write_xml_footer(FILE *fp) {
    fprintf(fp, "</logs>\n");
}

void escape_xml(const char *input, char *output, size_t output_size) {
    size_t i, j;
    for (i = 0, j = 0; input[i] != '\0' && j < output_size - 1; i++) {
        if (input[i] == '&') {
            const char *replacement = "&amp;";
            size_t len = strlen(replacement);
            if (j + len < output_size - 1) {
                strcpy(output + j, replacement);
                j += len;
            }
        } else if (input[i] == '<') {
            const char *replacement = "&lt;";
            size_t len = strlen(replacement);
            if (j + len < output_size - 1) {
                strcpy(output + j, replacement);
                j += len;
            }
        } else if (input[i] == '>') {
            const char *replacement = "&gt;";
            size_t len = strlen(replacement);
            if (j + len < output_size - 1) {
                strcpy(output + j, replacement);
                j += len;
            }
        } else if (input[i] == '"') {
            const char *replacement = "&quot;";
            size_t len = strlen(replacement);
            if (j + len < output_size - 1) {
                strcpy(output + j, replacement);
                j += len;
            }
        } else if (input[i] == '\'') {
            const char *replacement = "&apos;";
            size_t len = strlen(replacement);
            if (j + len < output_size - 1) {
                strcpy(output + j, replacement);
                j += len;
            }
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

void print_usage(const char *program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Options:\n");
    printf("  -o FILE    Output file (default: kernel_logs.json or kernel_logs.xml)\n");
    printf("  -f FORMAT  Output format: json or xml (default: json)\n");
    printf("  -b LINES   Buffer size in lines (default: 100)\n");
    printf("  -h         Display this help message\n");
}

int main(int argc, char *argv[]) {
    char *output_file = NULL;
    OutputFormat format = FORMAT_JSON;
    int buffer_lines = DEFAULT_BUFFER_LINES;
    int opt;
    
    while ((opt = getopt(argc, argv, "o:f:b:h")) != -1) {
        switch (opt) {
            case 'o':
                output_file = optarg;
                break;
            case 'f':
                if (strcmp(optarg, "json") == 0) {
                    format = FORMAT_JSON;
                } else if (strcmp(optarg, "xml") == 0) {
                    format = FORMAT_XML;
                } else {
                    fprintf(stderr, "Error: Format must be either 'json' or 'xml'\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'b':
                buffer_lines = atoi(optarg);
                if (buffer_lines <= 0) {
                    fprintf(stderr, "Error: Buffer size must be a positive integer\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }
    
    // Set default output file if not specified
    if (output_file == NULL) {
        output_file = (format == FORMAT_JSON) ? "klog.json" : "klog.xml";
    }
    
    // Open output file
    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        fprintf(stderr, "Error: Could not open output file '%s': %s\n", output_file, strerror(errno));
        return EXIT_FAILURE;
    }
    
    // Write header
    if (format == FORMAT_JSON) {
        write_json_header(output_fp);
    } else {
        write_xml_header(output_fp);
    }
    
    // Set up signal handler
    setup_signal_handler();
    
    // Open pipe to dmesg command
    FILE *dmesg_pipe = popen("dmesg --follow", "r");
    if (dmesg_pipe == NULL) {
        fprintf(stderr, "Error: Failed to execute dmesg command: %s\n", strerror(errno));
        fclose(output_fp);
        return EXIT_FAILURE;
    }
    
    // Buffer for log lines
    char **buffer = (char **)malloc(buffer_lines * sizeof(char *));
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        pclose(dmesg_pipe);
        fclose(output_fp);
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < buffer_lines; i++) {
        buffer[i] = NULL;
    }
    
    char line[BUFFER_SIZE];
    int buffer_index = 0;
    int first_entry = 1;
    
    printf("Collecting kernel logs to %s. Press Ctrl+C to stop.\n", output_file);
    
    // Process log lines
    while (keep_running && fgets(line, sizeof(line), dmesg_pipe) != NULL) {
        // Remove newline at the end
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Get current timestamp
        time_t now = time(NULL);
        
        // Free previous entry at this index if it exists
        if (buffer[buffer_index] != NULL) {
            free(buffer[buffer_index]);
        }
        
        // Allocate and format the new entry
        if (format == FORMAT_JSON) {
            buffer[buffer_index] = (char *)malloc(BUFFER_SIZE * 2);
            if (buffer[buffer_index] != NULL) {
                if (first_entry) {
                    snprintf(buffer[buffer_index], BUFFER_SIZE * 2, 
                            "  {\"timestamp\": %ld, \"message\": \"%s\"}", 
                            now, line);
                    first_entry = 0;
                } else {
                    snprintf(buffer[buffer_index], BUFFER_SIZE * 2, 
                            ",\n  {\"timestamp\": %ld, \"message\": \"%s\"}", 
                            now, line);
                }
            }
        } else {
            // For XML, we need to escape special characters
            char escaped_line[BUFFER_SIZE * 2];
            escape_xml(line, escaped_line, sizeof(escaped_line));
            
            buffer[buffer_index] = (char *)malloc(BUFFER_SIZE * 3);
            if (buffer[buffer_index] != NULL) {
                snprintf(buffer[buffer_index], BUFFER_SIZE * 3, 
                        "  <log>\n    <timestamp>%ld</timestamp>\n    <message>%s</message>\n  </log>\n", 
                        now, escaped_line);
            }
        }
        
        // Move to next buffer position
        buffer_index = (buffer_index + 1) % buffer_lines;
        
        // Flush buffer if it's full
        if (buffer_index == 0) {
            for (int i = 0; i < buffer_lines; i++) {
                if (buffer[i] != NULL) {
                    fprintf(output_fp, "%s", buffer[i]);
                }
            }
            fflush(output_fp);
        }
    }
    
    // Write remaining entries
    for (int i = 0; i < buffer_lines; i++) {
        int idx = (buffer_index + i) % buffer_lines;
        if (buffer[idx] != NULL) {
            fprintf(output_fp, "%s", buffer[idx]);
            free(buffer[idx]);
        }
    }
    
    // Write footer
    if (format == FORMAT_JSON) {
        write_json_footer(output_fp);
    } else {
        write_xml_footer(output_fp);
    }
    
    // Clean up
    free(buffer);
    pclose(dmesg_pipe);
    fclose(output_fp);
    
    printf("Kernel log collection completed.\n");
    
    return EXIT_SUCCESS;
}
