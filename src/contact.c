// contact.c
#include "contact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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

void value_to_struct(char *target, const char *value)
{
    strncpy(target, value, strlen(value));
    target[strlen(value)] = '\0';
}

Contact* file_to_contact(const char *fileName)
{
    FILE *currentFile = fopen(fileName, "r");
    if (currentFile == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", fileName);
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
    newContact->next = NULL;
    newContact->prev = NULL;

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

void add_contact(Contact **head, Contact *newContact)
{
    if (*head == NULL)
    {
        *head = newContact;
        return;
    }

    Contact *last = *head;
    while(last->next != NULL)
    {
        last = last->next;
    }

    last->next = newContact;
    newContact->prev = last;
}

void free_contacts(Contact *head)
{
    while (head != NULL)
    {
        Contact *temp = head;
        head = head->next;
        free(temp);
    }
}
