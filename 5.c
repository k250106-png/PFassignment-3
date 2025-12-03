#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
char *readLineFromStream(FILE *stream) {
    size_t cap = 80;
    size_t len = 0;
    char *buf = malloc(cap);
    if (!buf) {
        fprintf(stderr, "Memory allocation failed in readLineFromStream\n");
        return NULL;
    }
    int c;
    while ((c = fgetc(stream)) != EOF) {
        if (len + 1 >= cap) {
            size_t newcap = cap * 2;
            char *t = realloc(buf, newcap);
            if (!t) {
                free(buf);
                fprintf(stderr, "Memory allocation failed while reading line\n");
                return NULL;
            }
            buf = t;
            cap = newcap;
        }
        if (c == '\n') break;
        buf[len++] = (char)c;
    }
    if (len == 0 && c == EOF) {
        free(buf);
        return NULL;
    }
    buf[len] = '\0';
    char *exact = malloc(len + 1);
    if (!exact) {
        free(buf);
        fprintf(stderr, "Memory allocation failed while finalizing line\n");
        return NULL;
    }
    memcpy(exact, buf, len + 1);
    free(buf);
    return exact;
}

char *duplicateString(const char *s) {
    size_t l = strlen(s);
    char *p = malloc(l + 1);
    if (!p) {
        fprintf(stderr, "Memory allocation failed in duplicateString\n");
        return NULL;
    }
    memcpy(p, s, l + 1);
    return p;
}

int ensureCapacity(char ***linesPtr, int *capPtr, int minNeeded) {
    if (*capPtr >= minNeeded) return 1;
    int newcap = (*capPtr == 0) ? 4 : (*capPtr * 2);
    while (newcap < minNeeded) newcap *= 2;
    char **t = realloc(*linesPtr, newcap * sizeof(char *));
    if (!t) {
        fprintf(stderr, "Reallocation failed in ensureCapacity\n");
        return 0;
    }
    *linesPtr = t;
    *capPtr = newcap;
    return 1;
}

int insertLine(char ***linesPtr, int *sizePtr, int *capPtr, int index, const char *text) {
    if (index < 0 || index > *sizePtr) {
        fprintf(stderr, "insertLine: index %d out of range (0..%d)\n", index, *sizePtr);
        return 0;
    }
    if (!ensureCapacity(linesPtr, capPtr, *sizePtr + 1)) return 0;
    if (index < *sizePtr) {
        memmove(&((*linesPtr)[index + 1]), &((*linesPtr)[index]), ((*sizePtr) - index) * sizeof(char *));
    }
    char *dup = duplicateString(text);
    if (!dup) {
        if (index < *sizePtr) memmove(&((*linesPtr)[index]), &((*linesPtr)[index + 1]), ((*sizePtr) - index) * sizeof(char *));
        return 0;
    }
    (*linesPtr)[index] = dup;
    (*sizePtr)++;
    return 1;
}

int deleteLine(char ***linesPtr, int *sizePtr, int *capPtr, int index) {
    if (index < 0 || index >= *sizePtr) {
        fprintf(stderr, "deleteLine: index %d out of range (0..%d)\n", index, (*sizePtr) - 1);
        return 0;
    }
    free((*linesPtr)[index]);
    if (index < (*sizePtr) - 1) {
        memmove(&((*linesPtr)[index]), &((*linesPtr)[index + 1]), ((*sizePtr) - index - 1) * sizeof(char *));
    }
    (*sizePtr)--;
    return 1;
}

void printAllLines(char **lines, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d: %s\n", i + 1, lines[i]);
    }
}

void freeAll(char **lines, int size) {
    if (!lines) return;
    for (int i = 0; i < size; ++i) free(lines[i]);
    free(lines);
}

int shrinkToFit(char ***linesPtr, int *capPtr, int size) {
    if (*capPtr == size) return 1;
    if (size == 0) {
        free(*linesPtr);
        *linesPtr = NULL;
        *capPtr = 0;
        return 1;
    }
    char **t = realloc(*linesPtr, size * sizeof(char *));
    if (!t) {
        fprintf(stderr, "realloc failed in shrinkToFit\n");
        return 0;
    }
    *linesPtr = t;
    *capPtr = size;
    return 1;
}
int saveToFile(char **lines, int size, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
        return 0;
    }
    for (int i = 0; i < size; ++i) {
        if (fprintf(f, "%s\n", lines[i]) < 0) {
            fprintf(stderr, "Write error to file %s\n", filename);
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}

int loadFromFile(char ***linesPtr, int *sizePtr, int *capPtr, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Failed to open file for reading: %s\n", filename);
        return 0;
    }
    for (int i = 0; i < *sizePtr; ++i) free((*linesPtr)[i]);
    free(*linesPtr);
    *linesPtr = NULL;
    *sizePtr = 0;
    *capPtr = 0;
    char *ln;
    while ((ln = readLineFromStream(f)) != NULL) {
        if (!ensureCapacity(linesPtr, capPtr, (*sizePtr) + 1)) {
            free(ln);
            fclose(f);
            return 0;
        }
        (*linesPtr)[(*sizePtr)++] = ln;
    }
    fclose(f);
    return 1;
}

void flushStdinLine() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void) {
    char **lines = NULL;
    int size = 0;
    int cap = 0;
    char cmd[64];
    printf("Minimal Line-Based Editor. Commands: insert, delete, print, shrink, save, load, exit\n");
    while (1) {
        printf("> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) break;
        size_t l = strlen(cmd);
        if (l && cmd[l-1] == '\n') cmd[l-1] = '\0';
        if (strcmp(cmd, "insert") == 0) {
            printf("Index (1-based, use %d for append): ", size + 1);
            int idx;
            if (scanf("%d", &idx) != 1) { flushStdinLine(); printf("Invalid number\n"); continue; }
            flushStdinLine();
            if (idx < 1) { printf("Index must be >= 1\n"); continue; }
            printf("Enter text: ");
            char *text = readLineFromStream(stdin);
            if (!text) { printf("No input read\n"); continue; }
            int zeroIndex = (idx > size) ? size : (idx - 1);
            if (!insertLine(&lines, &size, &cap, zeroIndex, text)) {
                free(text);
                fprintf(stderr, "Failed to insert line\n");
            } else {
                free(text);
            }
        } else if (strcmp(cmd, "delete") == 0) {
            printf("Index (1-based): ");
            int idx;
            if (scanf("%d", &idx) != 1) { flushStdinLine(); printf("Invalid number\n"); continue; }
            flushStdinLine();
            if (!deleteLine(&lines, &size, &cap, idx - 1)) {
                printf("Delete failed\n");
            }
        } else if (strcmp(cmd, "print") == 0) {
            printAllLines(lines, size);
        } else if (strcmp(cmd, "shrink") == 0) {
            if (!shrinkToFit(&lines, &cap, size)) {
                printf("shrinkToFit failed\n");
            } else {
                printf("Shrunk capacity to %d\n", cap);
            }
        } else if (strcmp(cmd, "save") == 0) {
            char fname[256];
            printf("Filename: ");
            if (!fgets(fname, sizeof(fname), stdin)) { printf("No filename\n"); continue; }
            size_t fnl = strlen(fname);
            if (fnl && fname[fnl-1] == '\n') fname[fnl-1] = '\0';
            if (!saveToFile(lines, size, fname)) printf("Save failed\n"); else printf("Saved to %s\n", fname);
        } else if (strcmp(cmd, "load") == 0) {
            char fname[256];
            printf("Filename: ");
            if (!fgets(fname, sizeof(fname), stdin)) { printf("No filename\n"); continue; }
            size_t fnl = strlen(fname);
            if (fnl && fname[fnl-1] == '\n') fname[fnl-1] = '\0';
            if (!loadFromFile(&lines, &size, &cap, fname)) printf("Load failed\n"); else printf("Loaded %d lines from %s\n", size, fname);
        } else if (strcmp(cmd, "exit") == 0) {
            break;
        } else if (strcmp(cmd, "") == 0) {
            continue;
        } else {
            printf("Unknown command: %s\n", cmd);
        }
    }
    freeAll(lines, size);
    return 0;
}

