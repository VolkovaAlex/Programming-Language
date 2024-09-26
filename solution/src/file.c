#include "file.h"

#include <stdint.h>
#include <errno.h> 

bool file_open(FILE** file, const char* name, const char* mode) {
    *file = fopen(name, mode);
    if (*file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return false;
    }
    return true;
}

int file_close(FILE** file) {
    if (*file == NULL) {
        return 0;
    }
    int status = fclose(*file);
    *file = NULL;
    return status;
}

FILE* reader_file_open(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return NULL;
    }
    return file;
}

FILE* writer_file_open(char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return NULL;
    }
    return file;
}

void file_skip(FILE* file, size_t offset) {
    if (file == NULL) {
        return;
    }
    fseek(file, (long)offset, SEEK_CUR);
}

void file_seek(FILE* file, size_t offset) {
    if (file == NULL) {
        return;
    }
    fseek(file, (long)offset, SEEK_SET);
}

bool reader_file_read(FILE* file, void* buffer, size_t struct_size, size_t n, size_t padding) {
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return false;
    }
    size_t bytes_read = fread(buffer, struct_size, n, file);
    if (bytes_read != n) {
        if (ferror(file)) {
            fprintf(stderr, "Error reading file.\n");
            return false;
        }
        else {
            return true;
        }
    }
    file_skip(file, padding);

    return true;
}

bool writer_file_write(FILE* file, const void* buffer, size_t struct_size, size_t n, size_t padding) {
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return false;
    }
    size_t bytes_written = fwrite(buffer, struct_size, n, file);
    if (bytes_written != n) {
        fprintf(stderr, "Error writing file.\n");
        return false;
    }
    uint8_t ptr[3] = { 0 };
    fwrite(&ptr, 1, padding, file);

    return true;
}