#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 256

int copy(const char *source, const char *destination);

int main() {
    char sourceFile[MAX_PATH_LENGTH];
    char activeFolder[MAX_PATH_LENGTH];

    // Get the current working directory
    if (getcwd(activeFolder, MAX_PATH_LENGTH) == NULL) {
        perror("getcwd");
        return 1;
    }

    // Specify the source log file path
    strcpy(sourceFile, "C:\\Program Files (x86)\\Dapodik\\webserver\\logs\\access.log");

    // Check if the source file exists
    if (access(sourceFile, F_OK) == 0) {
        // Construct the destination file path (current working directory + source file name)
        char destinationFile[MAX_PATH_LENGTH];
        strcpy(destinationFile, activeFolder);
        strcat(destinationFile, "\\access.log");

        // Copy the source file to the destination file
        if (copy(sourceFile, destinationFile) == 0) {
            printf("Log file copied successfully to '%s'!\n", destinationFile);
        } else {
            perror("copy");
            return 1;
        }
    } else {
        printf("Source log file '%s' does not exist.\n", sourceFile);
    }

    return 0;
}

// Function to copy a file (implementation depends on your system)
int copy(const char *source, const char *destination) {
    FILE *sourceFile, *destinationFile;
    char buffer[1024]; // Adjust buffer size as needed
    size_t bytesRead;

    // Open source file in read binary mode
    sourceFile = fopen(source, "rb");
    if (sourceFile == NULL) {
        perror("fopen (source)");
        return 1;
    }

    // Open destination file in write binary mode
    destinationFile = fopen(destination, "wb");
    if (destinationFile == NULL) {
        perror("fopen (destination)");
        fclose(sourceFile); // Close source file on error
        return 1;
    }

    // Read from source file in chunks and write to destination file
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        if (fwrite(buffer, 1, bytesRead, destinationFile) != bytesRead) {
            perror("fwrite");
            fclose(sourceFile);
            fclose(destinationFile);
            return 1;
        }
    }

    // Check for read errors
    if (ferror(sourceFile)) {
        perror("fread");
        fclose(sourceFile);
        fclose(destinationFile);
        return 1;
    }

    // Close both files
    fclose(sourceFile);
    fclose(destinationFile);

    return 0;
}
