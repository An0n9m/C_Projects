//Copyright (C) 2024 An0n9m. All rights reserved.

# File Encryption and Decryption

This project contains two C programs for encrypting and decrypting files using a simple XOR-based encryption method. 

## Features

- **File Encryption**: Encrypts the content of a file using a specified key or a default key.
- **File Decryption**: Decrypts the content of an encrypted file using the same key that was used for encryption.

## Requirements

- C Compiler (e.g., `gcc`)

## Usage

### Compilation

To compile the programs, use the following commands:

```sh
gcc -o FileEncrypt FileEncrypt.c
gcc -o FileDecrypt FileDecrypt.c

To encrypt a file, run:
./FileEncrypt <file_path> <password (optional)>

To decrypt a file, run:
./Decrypt -p <file_path> <password (optional)>
./Decrypt -r <file_path> <password (optional)>

.-p: Print the decrypted content to the console.
.-r: Replace the source file with the decrypted content.
.<file_path>: The path to the file you want to decrypt.
.<password (optional)>: The password used for encryption. If not provided, the default key will be used.
