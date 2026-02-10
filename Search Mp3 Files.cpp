#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

// Function to check if a file has .mp3 extension
int is_mp3_file(const char *filename) {
    if (filename == NULL) return 0;
    
    int len = strlen(filename);
    if (len < 4) return 0;
    
    // Check if the file ends with .mp3 or .MP3
    const char *ext = filename + len - 4;
    return (strcasecmp(ext, ".mp3") == 0);
}

// Function to search for MP3 files in a directory
void search_mp3_files(const char *path, int show_details) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];
    int file_count = 0;
    
    // Open directory
    dir = opendir(path);
    if (dir == NULL) {
        printf("Error: Cannot open directory '%s'\n", path);
        return;
    }
    
    printf("\n🔍 Searching for MP3 files in: %s\n", path);
    printf("═══════════════════════════════════════\n\n");
    
    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip current and parent directory entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Construct full path
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        // Get file status
        if (stat(full_path, &file_stat) == 0) {
            // Check if it's a regular file
            if (S_ISREG(file_stat.st_mode)) {
                // Check if it's an MP3 file
                if (is_mp3_file(entry->d_name)) {
                    file_count++;
                    
                    if (show_details) {
                        // Display with detailed information
                        printf("%d. 📁 File: %s\n", file_count, entry->d_name);
                        printf("   📍 Path: %s\n", full_path);
                        printf("   📏 Size: %.2f KB\n", (double)file_stat.st_size / 1024);
                       // printf("   🕐 Modified: %s", ctime(&file_stat.st_mtime));
                        printf("   ──────────────────────────────\n");
                    } else {
                        // Simple list display
                        printf("%d. %s\n", file_count, entry->d_name);
                    }
                }
            }
        }
    }
    
    // Close directory
    closedir(dir);
    
    printf("\n═══════════════════════════════════════\n");
    if (file_count == 0) {
        printf("❌ No MP3 files found.\n");
    } else {
        printf("✅ Found %d MP3 file(s)\n", file_count);
    }
    printf("═══════════════════════════════════════\n");
}

// Function to get current directory
void get_current_directory(char *buffer, size_t size) {
    if (getcwd(buffer, size) != NULL) {
        printf("Current directory: %s\n", buffer);
    } else {
        perror("getcwd() error");
        strcpy(buffer, ".");
    }
}

// Function to search recursively in subdirectories
void search_mp3_recursive(const char *base_path, int depth, int *total_count) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char path[1024];
    
    dir = opendir(base_path);
    if (dir == NULL) {
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);
        
        if (stat(path, &file_stat) == 0) {
            if (S_ISREG(file_stat.st_mode)) {
                // Check if it's an MP3 file
                if (is_mp3_file(entry->d_name)) {
                    (*total_count)++;
                    printf("%d. %s\n", *total_count, path);
                }
            } else if (S_ISDIR(file_stat.st_mode)) {
                // Recursively search subdirectories
                search_mp3_recursive(path, depth + 1, total_count);
            }
        }
    }
    
    closedir(dir);
}

int main() {
    int choice;
    char path[1024];
    char current_dir[1024];
    int recursive_search = 0;
    
    printf("🎵 MP3 File Search Program 🎵\n");
    printf("═══════════════════════════════════════\n\n");
    
    // Get current directory
    get_current_directory(current_dir, sizeof(current_dir));
    
    while (1) {
        printf("\n📋 MENU:\n");
        printf("1. Search in current directory\n");
        printf("2. Search in specific directory\n");
        printf("3. Recursive search (current directory)\n");
        printf("4. Recursive search (specific directory)\n");
        printf("5. Search with detailed information\n");
        printf("6. Exit\n");
        printf("\nEnter your choice (1-6): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            while(getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                // Search in current directory
                search_mp3_files(".", 0);
                break;
                
            case 2:
                printf("Enter directory path: ");
                scanf("%s", path);
                search_mp3_files(path, 0);
                break;
                
            case 3:
                // Recursive search in current directory
                printf("\n🔍 Recursive search in current directory:\n");
                printf("═══════════════════════════════════════\n\n");
                {
                    int total = 0;
                    search_mp3_recursive(".", 0, &total);
                    printf("\nTotal MP3 files found: %d\n", total);
                }
                break;
                
            case 4:
                printf("Enter directory path for recursive search: ");
                scanf("%s", path);
                printf("\n🔍 Recursive search in: %s\n", path);
                printf("═══════════════════════════════════════\n\n");
                {
                    int total = 0;
                    search_mp3_recursive(path, 0, &total);
                    printf("\nTotal MP3 files found: %d\n", total);
                }
                break;
                
            case 5:
                printf("Enter directory path: ");
                scanf("%s", path);
                search_mp3_files(path, 1); // 1 for detailed view
                break;
                
            case 6:
                printf("\nThank you for using MP3 File Search! 👋\n");
                exit(0);
                
            default:
                printf("Invalid choice! Please enter 1-6.\n");
        }
        
        // Pause before showing menu again
        printf("\nPress Enter to continue...");
        while(getchar() != '\n'); // Clear input buffer
        getchar(); // Wait for Enter key
    }
    
    return 0;
}