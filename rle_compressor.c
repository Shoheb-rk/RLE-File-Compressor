/*
 * ============================================================
 *   FILE COMPRESSION AND DECOMPRESSION SYSTEM USING RLE
 * ============================================================
 * Project  : Run-Length Encoding (RLE) File Compressor
 * Language : Standard C (C99)
 * Author   : [Shoheb Rehaman Khanadal]

 *
 * WHAT IS RLE?
 *   Run-Length Encoding replaces consecutive repeated characters
 *   with a count followed by the character.
 *   Example:  AAAABBBCCDD  →  4A3B2C2D
 *
 * FILES PRODUCED:
 *   • compressed.rle   – RLE-encoded output
 *   • decompressed.txt – Reconstructed original content
 *
 * COMPILE:  gcc rle_compressor.c -o rle_compressor
 * RUN:      ./rle_compressor   (Linux/Mac)
 *           rle_compressor.exe (Windows)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Constants ─────────────────────────────────────────────── */
#define FILE_PATH_LEN 256   /* Maximum length of a file-path string  */

/* ── Structure: holds statistics about a compression job ───── */
typedef struct {
    long originalSize;    /* Byte count of the source file       */
    long compressedSize;  /* Byte count after compression        */
    double ratio;         /* Compression ratio as a percentage   */
} FileStats;

/* ══════════════════════════════════════════════════════════════
 *  FUNCTION PROTOTYPES
 * ══════════════════════════════════════════════════════════════ */
void  displayMenu(void);
void  compressFile(void);
void  decompressFile(void);
void  showStatistics(void);
int   verifyFiles(const char *originalFile, const char *decompressedFile);
void  verifyMenu(void);
long  getFileSize(const char *filename);
void  printSeparator(void);


/* ══════════════════════════════════════════════════════════════
 *  MAIN  – menu-driven entry point
 * ══════════════════════════════════════════════════════════════ */
int main(void)
{
    int choice;

    printf("\n");
    printSeparator();
    printf("     Welcome to RLE File Compression System\n");
    printSeparator();

    do {
        displayMenu();
        printf("  Enter your choice: ");

        /* Read integer; clear remainder of line to avoid loops */
        if (scanf("%d", &choice) != 1) {
            choice = 0;               /* treat bad input as invalid */
        }
        while (getchar() != '\n');    /* flush input buffer          */

        switch (choice) {
            case 1: compressFile();   break;
            case 2: decompressFile(); break;
            case 3: showStatistics(); break;
            case 4: verifyMenu();     break;
            case 5:
                printf("\n  Goodbye! Thank you for using RLE Compressor.\n\n");
                break;
            default:
                printf("\n  [!] Invalid choice. Please enter 1-5.\n");
        }

    } while (choice != 5);

    return 0;
}


/* ══════════════════════════════════════════════════════════════
 *  displayMenu  –  prints the main menu to stdout
 * ══════════════════════════════════════════════════════════════ */
void displayMenu(void)
{
    printf("\n");
    printSeparator();
    printf("         ===== RLE FILE COMPRESSOR =====\n");
    printSeparator();
    printf("  1. Compress File\n");
    printf("  2. Decompress File\n");
    printf("  3. Show Compression Statistics\n");
    printf("  4. Verify Files\n");
    printf("  5. Exit\n");
    printSeparator();
}


/* ══════════════════════════════════════════════════════════════
 *  compressFile
 *  ─────────────
 *  ALGORITHM (RLE Compression):
 *    1. Open the source file for reading (character by character).
 *    2. Read the first character; initialise count = 1.
 *    3. While not EOF:
 *         a. Read the next character.
 *         b. If it equals the current character, increment count.
 *         c. Otherwise:
 *              – Write count (only if count > 1) followed by
 *                the current character to the output file.
 *              – Reset: current ← new char, count ← 1.
 *    4. Write the last run.
 *    5. Close both files.
 *
 *  TIME  COMPLEXITY : O(n)  – single pass through the file
 *  SPACE COMPLEXITY : O(1)  – only a few variables needed
 * ══════════════════════════════════════════════════════════════ */
void compressFile(void)
{
    char inputPath[FILE_PATH_LEN];
    char outputPath[FILE_PATH_LEN];
    FILE *inFile  = NULL;
    FILE *outFile = NULL;
    int  current, next;
    int  count;

    printf("\n  --- Compress File ---\n");
    printf("  Enter the path of the file to compress: ");
    fgets(inputPath, FILE_PATH_LEN, stdin);
    inputPath[strcspn(inputPath, "\n")] = '\0';   /* strip newline */

    printf("  Enter the path for the compressed output file: ");
    fgets(outputPath, FILE_PATH_LEN, stdin);
    outputPath[strcspn(outputPath, "\n")] = '\0';

    /* ── Open files ─────────────────────────────────────────── */
    inFile = fopen(inputPath, "r");
    if (inFile == NULL) {
        printf("\n  [ERROR] Cannot open input file: %s\n", inputPath);
        printf("          Please check the path and try again.\n");
        return;
    }

    outFile = fopen(outputPath, "w");
    if (outFile == NULL) {
        printf("\n  [ERROR] Cannot create output file: %s\n", outputPath);
        fclose(inFile);
        return;
    }

    /* ── Handle empty file ──────────────────────────────────── */
    current = fgetc(inFile);
    if (current == EOF) {
        printf("\n  [!] Input file is empty. Nothing to compress.\n");
        fclose(inFile);
        fclose(outFile);
        return;
    }

    /* ── Main RLE loop ──────────────────────────────────────── */
    count = 1;

    while ((next = fgetc(inFile)) != EOF) {
        if (next == current) {
            count++;          /* same character – extend the run  */
        } else {
            /* Write the completed run */
            if (count > 1) {
                fprintf(outFile, "%d", count);   /* e.g. "4"     */
            }
            fputc(current, outFile);             /* e.g. "A"     */

            current = next;   /* start a new run                  */
            count   = 1;
        }
    }

    /* Write the final run (loop exits before writing it) */
    if (count > 1) {
        fprintf(outFile, "%d", count);
    }
    fputc(current, outFile);

    fclose(inFile);
    fclose(outFile);

    printf("\n  [OK] Compression complete!\n");
    printf("       Input  : %s\n", inputPath);
    printf("       Output : %s\n", outputPath);

    /* ── Quick size summary ─────────────────────────────────── */
    long origSize = getFileSize(inputPath);
    long compSize = getFileSize(outputPath);
    if (origSize > 0) {
        double ratio = ((double)(origSize - compSize) / origSize) * 100.0;
        printf("       Original size   : %ld bytes\n", origSize);
        printf("       Compressed size : %ld bytes\n", compSize);
        printf("       Ratio           : %.2f%%\n", ratio);
    }
}


/* ══════════════════════════════════════════════════════════════
 *  decompressFile
 *  ───────────────
 *  ALGORITHM (RLE Decompression):
 *    1. Open the compressed file for reading.
 *    2. While not EOF:
 *         a. Read a character.
 *         b. If it is a digit, accumulate it into a count string.
 *         c. If it is a non-digit (letter/symbol/space):
 *              – Convert the accumulated digit string to an
 *                integer (default 1 if no digits were seen).
 *              – Write the character 'count' times to output.
 *              – Reset the count buffer.
 *    3. Close both files.
 *
 *  TIME  COMPLEXITY : O(n)  – single pass through the compressed file
 *  SPACE COMPLEXITY : O(1)
 * ══════════════════════════════════════════════════════════════ */
void decompressFile(void)
{
    char inputPath[FILE_PATH_LEN];
    char outputPath[FILE_PATH_LEN];
    FILE *inFile  = NULL;
    FILE *outFile = NULL;
    int  ch;
    char countBuf[20];   /* stores digit characters before a letter */
    int  bufIdx;
    int  count;

    printf("\n  --- Decompress File ---\n");
    printf("  Enter the path of the compressed file: ");
    fgets(inputPath, FILE_PATH_LEN, stdin);
    inputPath[strcspn(inputPath, "\n")] = '\0';

    printf("  Enter the path for the decompressed output file: ");
    fgets(outputPath, FILE_PATH_LEN, stdin);
    outputPath[strcspn(outputPath, "\n")] = '\0';

    /* ── Open files ─────────────────────────────────────────── */
    inFile = fopen(inputPath, "r");
    if (inFile == NULL) {
        printf("\n  [ERROR] Cannot open compressed file: %s\n", inputPath);
        printf("          Please check the path and try again.\n");
        return;
    }

    outFile = fopen(outputPath, "w");
    if (outFile == NULL) {
        printf("\n  [ERROR] Cannot create output file: %s\n", outputPath);
        fclose(inFile);
        return;
    }

    /* ── Main decompression loop ────────────────────────────── */
    memset(countBuf, 0, sizeof(countBuf));
    bufIdx = 0;

    while ((ch = fgetc(inFile)) != EOF) {

        if (ch >= '0' && ch <= '9') {
            /* Accumulate digit into buffer (handles counts > 9) */
            if (bufIdx < (int)(sizeof(countBuf) - 1)) {
                countBuf[bufIdx++] = (char)ch;
                countBuf[bufIdx]   = '\0';
            }
        } else {
            /* Non-digit: this is the character to repeat         */
            count = (bufIdx > 0) ? atoi(countBuf) : 1;

            int i;
            for (i = 0; i < count; i++) {
                fputc(ch, outFile);
            }

            /* Reset digit buffer for the next run               */
            memset(countBuf, 0, sizeof(countBuf));
            bufIdx = 0;
        }
    }

    fclose(inFile);
    fclose(outFile);

    printf("\n  [OK] Decompression complete!\n");
    printf("       Input  : %s\n", inputPath);
    printf("       Output : %s\n", outputPath);
}


/* ══════════════════════════════════════════════════════════════
 *  showStatistics
 *  ───────────────
 *  Asks for paths to both files and fills a FileStats structure,
 *  then displays original size, compressed size, and ratio.
 *
 *  Formula:
 *    Compression Ratio (%) =
 *        ((Original - Compressed) / Original) × 100
 *
 *  A positive ratio means the file shrank.
 *  A negative ratio means the compressed file is larger
 *  (this can happen with short or already-varied files).
 * ══════════════════════════════════════════════════════════════ */
void showStatistics(void)
{
    char origPath[FILE_PATH_LEN];
    char compPath[FILE_PATH_LEN];
    FileStats stats;

    printf("\n  --- Compression Statistics ---\n");
    printf("  Enter path of the ORIGINAL file   : ");
    fgets(origPath, FILE_PATH_LEN, stdin);
    origPath[strcspn(origPath, "\n")] = '\0';

    printf("  Enter path of the COMPRESSED file : ");
    fgets(compPath, FILE_PATH_LEN, stdin);
    compPath[strcspn(compPath, "\n")] = '\0';

    /* ── Get sizes ──────────────────────────────────────────── */
    stats.originalSize   = getFileSize(origPath);
    stats.compressedSize = getFileSize(compPath);

    if (stats.originalSize < 0) {
        printf("\n  [ERROR] Cannot access original file: %s\n", origPath);
        return;
    }
    if (stats.compressedSize < 0) {
        printf("\n  [ERROR] Cannot access compressed file: %s\n", compPath);
        return;
    }
    if (stats.originalSize == 0) {
        printf("\n  [!] Original file is empty (0 bytes).\n");
        return;
    }

    stats.ratio = ((double)(stats.originalSize - stats.compressedSize)
                   / (double)stats.originalSize) * 100.0;

    /* ── Display ────────────────────────────────────────────── */
    printf("\n");
    printSeparator();
    printf("         COMPRESSION STATISTICS\n");
    printSeparator();
    printf("  Original file    : %s\n",  origPath);
    printf("  Compressed file  : %s\n",  compPath);
    printf("  Original size    : %ld bytes\n", stats.originalSize);
    printf("  Compressed size  : %ld bytes\n", stats.compressedSize);

    if (stats.ratio >= 0) {
        printf("  Compression ratio: %.2f%% (file reduced)\n", stats.ratio);
    } else {
        printf("  Compression ratio: %.2f%% (file grew — input had few runs)\n",
               stats.ratio);
    }
    printSeparator();
}


/* ══════════════════════════════════════════════════════════════
 *  verifyFiles
 *  ─────────────
 *  Compares two files character by character.
 *
 *  Returns : 1  if files are identical
 *            0  if files differ
 *           -1  if a file could not be opened
 *
 *  ALGORITHM:
 *    1. Open both files.
 *    2. Read one character at a time from each.
 *    3. If any pair differs → return 0 (mismatch).
 *    4. After the loop, if both reached EOF simultaneously → 1.
 *       If one ended before the other → 0 (different lengths).
 * ══════════════════════════════════════════════════════════════ */
int verifyFiles(const char *originalFile, const char *decompressedFile)
{
    FILE *f1 = fopen(originalFile,    "r");
    FILE *f2 = fopen(decompressedFile,"r");

    if (f1 == NULL) {
        printf("\n  [ERROR] Cannot open original file: %s\n", originalFile);
        if (f2) fclose(f2);
        return -1;
    }
    if (f2 == NULL) {
        printf("\n  [ERROR] Cannot open decompressed file: %s\n",
               decompressedFile);
        fclose(f1);
        return -1;
    }

    int c1, c2;
    long position = 0;

    while (1) {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        position++;

        if (c1 == EOF && c2 == EOF) {
            /* Both ended at the same place → identical */
            fclose(f1);
            fclose(f2);
            return 1;
        }

        if (c1 != c2) {
            /* Mismatch found */
            printf("  [INFO] First difference at byte position %ld\n",
                   position);
            fclose(f1);
            fclose(f2);
            return 0;
        }
    }
}


/* ══════════════════════════════════════════════════════════════
 *  verifyMenu  –  UI wrapper for verifyFiles()
 * ══════════════════════════════════════════════════════════════ */
void verifyMenu(void)
{
    char origPath[FILE_PATH_LEN];
    char decompPath[FILE_PATH_LEN];
    int result;

    printf("\n  --- Verify Files ---\n");
    printf("  Enter path of the ORIGINAL file      : ");
    fgets(origPath, FILE_PATH_LEN, stdin);
    origPath[strcspn(origPath, "\n")] = '\0';

    printf("  Enter path of the DECOMPRESSED file  : ");
    fgets(decompPath, FILE_PATH_LEN, stdin);
    decompPath[strcspn(decompPath, "\n")] = '\0';

    result = verifyFiles(origPath, decompPath);

    printf("\n");
    printSeparator();
    if (result == 1) {
        printf("  Verification Successful:\n");
        printf("  Original and Decompressed Files Match!\n");
    } else if (result == 0) {
        printf("  Verification Failed:\n");
        printf("  Files Do Not Match.\n");
    }
    /* result == -1: error messages already printed inside verifyFiles */
    printSeparator();
}


/* ══════════════════════════════════════════════════════════════
 *  getFileSize  –  returns byte count of a file, or -1 on error
 * ══════════════════════════════════════════════════════════════ */
long getFileSize(const char *filename)
{
    FILE *fp = fopen(filename, "rb");   /* binary mode for exact size */
    if (fp == NULL) return -1L;

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}


/* ══════════════════════════════════════════════════════════════
 *  printSeparator  –  prints a decorative divider line
 * ══════════════════════════════════════════════════════════════ */
void printSeparator(void)
{
    printf("  ================================================\n");
}


/* ============================================================
 *
 *  ── COMPLETE DOCUMENTATION ────────────────────────────────
 *
 *  FUNCTION SUMMARY
 *  ─────────────────
 *  main()           – Runs the menu loop until the user exits.
 *  displayMenu()    – Prints the 5-option menu.
 *  compressFile()   – Reads a text file and writes RLE output.
 *  decompressFile() – Reads an RLE file and reconstructs text.
 *  showStatistics() – Shows size and compression ratio.
 *  verifyFiles()    – Byte-by-byte comparison; returns 1/0/-1.
 *  verifyMenu()     – UI layer that calls verifyFiles().
 *  getFileSize()    – Helper: returns byte count via fseek/ftell.
 *  printSeparator() – Helper: prints a divider line.
 *
 *
 *  SAMPLE INPUT / OUTPUT
 *  ──────────────────────
 *  Suppose input.txt contains:
 *      AAAABBBCCDDDDEEEE
 *
 *  After compression → compressed.rle contains:
 *      4A3B2C4D4E
 *
 *  After decompression → decompressed.txt contains:
 *      AAAABBBCCDDDDEEEE   (matches the original exactly)
 *
 *  Statistics for this example:
 *      Original size    : 17 bytes
 *      Compressed size  : 10 bytes
 *      Compression ratio: 41.18%
 *
 *
 *  COMPLEXITY ANALYSIS
 *  ────────────────────
 *  Compression:
 *    Time  : O(n) – one pass; n = number of characters in input
 *    Space : O(1) – only a few integer/char variables used
 *
 *  Decompression:
 *    Time  : O(m) – one pass; m = number of characters in compressed file
 *    Space : O(1) – small fixed-size digit buffer (20 bytes)
 *
 *  Verification:
 *    Time  : O(min(a, b)) – stops at first mismatch
 *    Space : O(1)
 *
 *
 *  LIMITATIONS OF BASIC RLE
 *  ─────────────────────────
 *  • Works best on files with long runs of repeated characters
 *    (e.g. simple bitmaps, repetitive logs).
 *  • For files with varied content (e.g. English prose), the
 *    compressed file can actually be LARGER because every
 *    single character becomes "1X" in some encodings.
 *    This implementation omits the "1" prefix (single chars are
 *    written as-is), which avoids inflation in most text files.
 *  • Binary files with byte values that happen to be ASCII
 *    digits (0x30–0x39) would confuse the decoder; this
 *    implementation is therefore designed for plain text files.
 *
 * ============================================================ */