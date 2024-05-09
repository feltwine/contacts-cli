// contact.c
#include "contact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define MAX_FIELD_LENGTH 50

int init_contacts_dir(const char *contactsPath, DIR **contactsDir)
{
    *contactsDir = opendir(contactsPath);
    if (*contactsDir == NULL)
    {
        if (mkdir(contactsPath, 0777) != 0)
        {
            fprintf(stderr, "Error: Could not create contacts directory.\n");
            return 0;
        }
        *contactsDir = opendir(contactsPath);
        if (*contactsDir == NULL) {
            fprintf(stderr, "Error: Could not open contacts directory.\n");
            return 0;
        }
    }
    return 1;
}

void value_to_struct(char *target, const char *value) {
    size_t len = strlen(value);
    if (len < MAX_FIELD_LENGTH) {
        strlcpy(target, value, MAX_FIELD_LENGTH + 1); // Ensure null-termination
    } else {
        fprintf(stderr, "Error: Value too long.\n");
        // Handle error or truncate the value if appropriate
    }
}


Contact* file_to_contact(const char *filePath, const char *fileName)
{
    FILE *currentFile = fopen(filePath, "r");
    if (currentFile == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filePath);
        return NULL;
    }

    Contact *newContact = (Contact*)malloc(sizeof(Contact));
    if (newContact == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory.\n");
        fclose(currentFile);
        return NULL;
    }

    strcpy(newContact->fileName, fileName);

    char line[256];
    while (fgets(line, sizeof(line), currentFile))
    {
        char key[50], value[150];
        if (sscanf(line, "%49[^:]: \"%149[^\"]\"", key, value) == 2)
        {
            if (strcmp(key, "first-name") == 0)
               value_to_struct(newContact->firstName, value);
            else if (strcmp(key, "last-name") == 0)
                value_to_struct(newContact->lastName, value);
            else if (strcmp(key, "email-address") == 0)
                value_to_struct(newContact->emailAddress, value);
            else if (strcmp(key, "phone-number") == 0)
                value_to_struct(newContact->phoneNum, value);
        }
    }

    fclose(currentFile);
    return newContact;
}

void add_contact(Contact **head, Contact *newContact) {
    
    if (*head == NULL) 
    {
        *head = newContact;
    } 
    else 
    {
        Contact *last = *head;
        while (last->next != NULL) 
        {
            last = last->next;
        }
        last->next = newContact;
        newContact->prev = last;
        newContact->next = NULL;
    }
}

void free_contacts(Contact *head) {
    while (head != NULL) 
    {
        Contact *temp = head;
        head = head->next;
        free(temp);
    }
}

char* new_file_name(DIR *path)
{
    srand(time(NULL));
    int fileNum = rand() % 10000;
    char *fileName = malloc(9 * sizeof(char)); // Allocate memory for the file name
    if (fileName == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory.\n");
        return NULL;
    }
    sprintf(fileName, "%04d.txt", fileNum);

    struct dirent *entry;
    int unique = 1;
    while (unique == 1)
        unique = 0;
        while ((entry = readdir(path)) != NULL)
        {
            if (strcmp(entry->d_name, fileName) == 0)
            {
                unique = 1;
                fileNum = rand() % 10000;
                sprintf(fileName, "%04d.txt", fileNum);
                break;
            }
        }


    return fileName;
}

void generate_new_file(char *fileName)
{
    if (chdir("contacts") != 0)
    {
        fprintf(stderr, "Error: Failed to change directory to 'contacts/'.\n");
        exit(1);
    }

    FILE *newFile = fopen(fileName, "w");
    if (newFile == NULL)
    {
        fprintf(stderr, "Error: Failed to create new file.\n");
        exit(1);
    }

    fprintf(newFile, "first-name: \"\"\n");
    fprintf(newFile, "last-name: \"\"\n");
    fprintf(newFile, "email-address: \"\"\n");
    fprintf(newFile, "phone-number: \"\"\n");

    fclose(newFile);
}

int update_contact_file(Contact *contact)
{
    // Check if the current directory is "contacts/"
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    if (strcmp(cwd, "contacts/") != 0)
    {
        // Change current directory to "contacts/"
        if (chdir("contacts/") != 0)
        {
            perror("chdir() error");
            return 1;
        }
    }

    FILE *file = fopen(contact->fileName, "r+");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file %s.\n", contact->fileName);
        return 1;
    }

    char tempFileName[256];
    strcpy(tempFileName, "temp.txt");
    FILE *tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL)
    {
        fprintf(stderr, "Failed to create temporary file.\n");
        fclose(file);
        return 1;
    }

    char line[400];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "first-name: ", strlen("first-name: ")) == 0) 
        {
            fprintf(tempFile, "first-name: \"%s\"\n", contact->firstName);
        } 
        else if (strncmp(line, "last-name: ", strlen("last-name: ")) == 0) 
        {
            fprintf(tempFile, "last-name: \"%s\"\n", contact->lastName);
        } 
        else if (strncmp(line, "email-address: ", strlen("email-address: ")) == 0) {
            fprintf(tempFile, "email-address: \"%s\"\n", contact->emailAddress);
        } 
        else if (strncmp(line, "phone-number: ", strlen("phone-number: ")) == 0) 
        {
            fprintf(tempFile, "phone-number: \"%s\"\n", contact->phoneNum);
        } 
        else 
        {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(contact->fileName);
    rename(tempFileName, contact->fileName);

    return 0;
}

void remove_contact(Contact *contact) {
    if (contact == NULL) {
        return;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    if (chdir("contacts") == -1) {
        fprintf(stderr, "Error: Could not change directory to 'contacts/'.\n");
        return;
    }
    if (remove(contact->fileName) == -1) 
    {
        fprintf(stderr, "Error: Could not delete file: %s.\n", contact->fileName);
        chdir(cwd);
        return;
    }

    if (chdir(cwd) == -1) {
        fprintf(stderr, "Error: Could not change back to the original directory.\n");
        return;
    }

    if (contact->prev != NULL) {
        contact->prev->next = contact->next;
    }
    if (contact->next != NULL) {
        contact->next->prev = contact->prev;
    }
    free(contact);
}



void create_new_contact(DIR *path, Contact *head)
{
    Contact *newContact = (Contact*)malloc(sizeof(Contact));
    strncpy(newContact->fileName, new_file_name(path), sizeof(newContact->fileName));
    generate_new_file(newContact->fileName);

    printf("First name: ");
    scanf("%s", newContact->firstName);
    printf("Last name: ");
    scanf("%s", newContact->lastName);
    printf("Email address: ");
    scanf("%s", newContact->emailAddress);
    printf("Phone number: ");
    scanf("%s", newContact->phoneNum);
    
    if (update_contact_file(newContact) != 0)
    {
        fprintf(stderr, "Error: Failed to update contact file %s.\n", newContact->fileName);
        remove_contact(newContact);
    }

    add_contact(&head, newContact);

    show_contact_detail(newContact);
}

void update_contact(Contact *contact)
{
    int choice;

    printf("Which information would you like to change?\n");
    printf("1. First Name\n");
    printf("2. Last Name\n");
    printf("3. Email Address\n");
    printf("4. Phone Number\n");
    printf("Enter option numbers (separated by spaces): ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            scanf("%s", contact->firstName);
            break;
        case 2:
            scanf("%s", contact->lastName);
            break;
        case 3:
            scanf("%s", contact->emailAddress);
            break;
        case 4:
            scanf("%s", contact->phoneNum);
            break;
        default:
            printf("Invalid choice\n");
            break;
    }


    update_contact_file(contact);
}

Contact* find_contact_by_index(Contact *head, int index) 
{
    int i = 1;
    Contact *current = head;
    while (current != NULL && i < index) 
    {
        current = current->next;
        i++;
    }
    return current;
}

void show_contact_detail(Contact *contact)
{
    if (contact == NULL) {
        printf("Contact is NULL.\n");
        return;
    }

    printf("Contact details: \n");
    printf("First name:     %s\n", contact->firstName);
    printf("Last name:      %s\n", contact->lastName);
    printf("Phone number:   %s\n", contact->phoneNum);
    printf("Email address:  %s\n", contact->emailAddress);

    printf("Press Enter to continue...");
    fflush(stdout); // Flush the output buffer to ensure prompt display

    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // Wait for Enter key
    getchar();
}