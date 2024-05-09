#ifndef CONTACT_H
#define CONTACT_H

#include <dirent.h>

typedef struct Contact {
    char fileName[10];
    char firstName[50];
    char lastName[50];
    char emailAddress[150];
    char phoneNum[20];
    struct Contact *prev;
    struct Contact *next;
} Contact;

int init_contacts_dir(const char *contactsPath, DIR **contactsDir);
Contact* file_to_contact(const char *filePath, const char *fileName);
void add_contact(Contact **head, Contact *newContact);
void free_contacts(Contact *head);
char* new_file_name(DIR *path);
void generate_new_file(char *fileName);
int update_contact_file(Contact *contact);
void remove_contact(Contact **head, Contact *contact);
void create_new_contact(DIR *path, Contact **head);
void update_contact(Contact *contact);
Contact* find_contact_by_index(Contact *head, int index);
void show_contact_detail(Contact *contact);
Contact* search_contact(Contact *head, const char *searchName);

#endif
