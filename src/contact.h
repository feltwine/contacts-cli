// contact.h
#ifndef CONTACT_H
#define CONTACT_H

#include <dirent.h>

typedef struct Contact {
    char fileName[10];
    struct Contact *prev;
    struct Contact *next;
    char firstName[100];
    char lastName[100];
    char emailAddress[300];
    char phoneNum[50];
} Contact;

int init_contacts_dir(const char *contactsPath, DIR **contactsDir);
Contact* file_to_contact(const char *filePath, const char *fileName);
void add_contact(Contact **head, Contact *newContact);
void free_contacts(Contact *head);
void create_new_contact(DIR *path, Contact *head);
void remove_contact(Contact *contact);
void update_contact(Contact *contact);
Contact* find_contact_by_index(Contact *head, int index);

#endif
