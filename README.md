# RLE File Compression and Decompression System

A menu-driven file compression and decompression application developed in **C using the Run-Length Encoding (RLE) algorithm**. The system compresses text files by identifying consecutive repeated characters and reconstructs the original data through decompression.

## Features

- **File Compression:** Encodes consecutive repeated characters into a count-character representation.
- **File Decompression:** Reconstructs the original content from the compressed file.
- **Compression Statistics:** Displays original file size, compressed file size, and compression percentage.
- **File Verification:** Compares the original and decompressed files character by character to verify data integrity.
- **Error Handling:** Handles common file access errors and empty input files.

## How It Works

RLE replaces consecutive occurrences of the same character with a count followed by that character.

**Example**

Original data:
```text
AAAABBBCCDD
```

Compressed data:
```text
4A3B2C2D
```

After decompression, the original data is reconstructed.

## Technologies Used

- **Language:** C (C99)
- **Libraries:** Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`)
- **Core Concepts:** File handling, structures, arrays, functions, loops, and conditional statements
- **Algorithm:** Run-Length Encoding (RLE)

## Getting Started

### Prerequisites

- GCC compiler or MinGW on Windows
- A terminal or code editor such as Visual Studio Code

### Compile

Compile the source code using GCC:

```bash
gcc -Wall -Wextra -std=c99 rle_compressor.c -o rle_compressor
```

### Run

**Windows (PowerShell):**
```powershell
.\rle_compressor.exe
```

**Linux:**
```bash
./rle_compressor
```

## Compression Statistics

The program calculates the compression percentage using:

\[
\text{Compression Percentage} =
\frac{\text{Original Size} - \text{Compressed Size}}
{\text{Original Size}} \times 100
\]

A positive percentage indicates a reduction in file size, while a negative percentage indicates that the compressed file is larger than the original.

## Limitations

- Designed primarily for plain-text files.
- The current encoding format can be ambiguous when the original data contains numeric characters, because digits are also used to represent run counts.
- RLE is most effective for data containing consecutive repeated characters; it may offer little benefit or increase file size for varied content.
- Empty files are detected and reported rather than compressed.

## Academic Project

Developed as a **C Programming Project-Based Learning (PBL)** project.

**Project Title:** File Compression and Decompression System using Run-Length Encoding (RLE)