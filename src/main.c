// main.c
#include "contact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


int main(void)
{
    /*** init ***/
    const char *contactsPath = "contacts/";
    DIR *contactsDir;
    Contact *head = NULL;
    struct dirent *entry;

    if (!init_contacts_dir(contactsPath, &contactsDir))
    {
        fprintf(stderr, "Error: Could not initialize contacts directory.\n");
        return 1;
    }

    // Read contacts from files
    while ((entry = readdir(contactsDir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fileName[256];
        snprintf(fileName, sizeof(fileName), "%s%s", contactsPath, entry->d_name);

        Contact *newContact = file_to_contact(fileName, entry->d_name);
        if (newContact != NULL)
        {
            add_contact(&head, newContact);
        }
    }

    /*** Create new contact ***/
    create_new_contact(contactsDir, head);

    // Print first contact's first name
    if (head != NULL) {
        printf("First name: %s\n", head->next->firstName);
    } 
    else 
    {
        printf("No contacts found.\n");
    }

    /*** Change contact data ***/

    update_contact(head->next);

    //

    remove_contact(head->next->next);

    closedir(contactsDir);
    // Free memory
    free_contacts(head);
    return 0;
}
