#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ENTRIES 100
#define MASTER_PASSWORD "admin123"
#define FILENAME "passwords.dat"

typedef struct {
    char website[50];
    char username[50];
    char password[50];
} PasswordEntry;

PasswordEntry entries[MAX_ENTRIES];
int entry_count = 0;

void encrypt(char *text) {
    for(int i = 0; text[i] != '\0'; i++) {
        text[i] = text[i] + 3;
    }
}

void decrypt(char *text) {
    for(int i = 0; text[i] != '\0'; i++) {
        text[i] = text[i] - 3;
    }
}

int verify_master_password() {
    char input[50];
    printf("Enter master password: ");
    scanf("%s", input);
    return strcmp(input, MASTER_PASSWORD) == 0;
}

void add_password() {
    if(entry_count >= MAX_ENTRIES) {
        printf("Password storage full!\n");
        return;
    }
    
    printf("Enter website: ");
    scanf("%s", entries[entry_count].website);
    printf("Enter username: ");
    scanf("%s", entries[entry_count].username);
    printf("Enter password: ");
    scanf("%s", entries[entry_count].password);
    
    encrypt(entries[entry_count].password);
    entry_count++;
    printf("Password added successfully!\n");
}

void view_passwords() {
    if(entry_count == 0) {
        printf("No passwords stored.\n");
        return;
    }
    
    printf("\nStored Passwords:\n");
    printf("================\n");
    for(int i = 0; i < entry_count; i++) {
        char temp_password[50];
        strcpy(temp_password, entries[i].password);
        decrypt(temp_password);
        
        printf("%d. Website: %s\n", i+1, entries[i].website);
        printf("   Username: %s\n", entries[i].username);
        printf("   Password: %s\n", temp_password);
        printf("   --------------------\n");
    }
}

void search_password() {
    char website[50];
    printf("Enter website to search: ");
    scanf("%s", website);
    
    for(int i = 0; i < entry_count; i++) {
        if(strcmp(entries[i].website, website) == 0) {
            char temp_password[50];
            strcpy(temp_password, entries[i].password);
            decrypt(temp_password);
            
            printf("Found:\n");
            printf("Website: %s\n", entries[i].website);
            printf("Username: %s\n", entries[i].username);
            printf("Password: %s\n", temp_password);
            return;
        }
    }
    printf("Password not found for %s\n", website);
}

void generate_password() {
    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%";
    int length;
    
    printf("Enter password length (8-20): ");
    scanf("%d", &length);
    
    if(length < 8 || length > 20) {
        printf("Invalid length!\n");
        return;
    }
    
    printf("Generated password: ");
    for(int i = 0; i < length; i++) {
        printf("%c", chars[rand() % (sizeof(chars) - 1)]);
    }
    printf("\n");
}

void save_to_file() {
    FILE *file = fopen(FILENAME, "wb");
    if(file == NULL) {
        printf("Error saving file!\n");
        return;
    }
    
    fwrite(&entry_count, sizeof(int), 1, file);
    fwrite(entries, sizeof(PasswordEntry), entry_count, file);
    fclose(file);
    printf("Data saved successfully!\n");
}

void load_from_file() {
    FILE *file = fopen(FILENAME, "rb");
    if(file == NULL) {
        printf("No previous data found.\n");
        return;
    }
    
    fread(&entry_count, sizeof(int), 1, file);
    fread(entries, sizeof(PasswordEntry), entry_count, file);
    fclose(file);
    printf("Data loaded successfully! (%d entries)\n", entry_count);
}

void display_menu() {
    printf("\n=== Password Manager ===\n");
    printf("1. Add Password\n");
    printf("2. View All Passwords\n");
    printf("3. Search Password\n");
    printf("4. Generate Password\n");
    printf("5. Save Data\n");
    printf("6. Load Data\n");
    printf("0. Exit\n");
    printf("Choose option: ");
}

int main() {
    if(!verify_master_password()) {
        printf("Access denied!\n");
        return 1;
    }
    
    printf("Access granted!\n");
    int choice;
    
    do {
        display_menu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: add_password(); break;
            case 2: view_passwords(); break;
            case 3: search_password(); break;
            case 4: generate_password(); break;
            case 5: save_to_file(); break;
            case 6: load_from_file(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);
    
    return 0;
}
