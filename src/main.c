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

    /*** Read contacts from files ***/
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

    int state = 0;
    while (state == 0)
    {
        system("clear");
        if (head == NULL)
        {
            printf("No contacts saved.\n");
            printf("[ 1 ]   create new contact\n");
            printf("[ 2 ]   quit program\n");
            printf("Option: ");
            int choice;
            scanf("%d", &choice);
            switch (choice)
            {
            case 1:
                create_new_contact(contactsDir, head);
                break;
            case 2:
                state = 1;
                break;
            default:
                printf("Invalid choice\n");
                break;
            }
        }
        else
        {
            printf("Current contacts are: \n");
            printf("Index |     contact name \n");
            Contact *current = head;
            int index = 1;
            while(current != NULL)
            {
                printf("  %d   |    %s %s\n", index, current->firstName, current->lastName);
                index++;
                current = current->next;
            }
            
            int choice;
            printf("Choose option: \n");
            printf("[ 1 ]    create new contact\n");
            printf("[ 2 ]    show contact details\n");
            printf("[ 3 ]    change existing contact\n");
            printf("[ 4 ]    remove existing contact\n");
            printf("[ 5 ]    search contact\n");
            printf("[ 6 ]    quit program\n");
            printf("Option: ");
            scanf("%d", &choice);
            int choice_index;

            switch (choice)
            {
                case 1:
                    create_new_contact(contactsDir, head);
                    break;
                case 2:
                    printf("Enter index of contact to show details: ");
                    scanf("%d", &choice_index);
                    Contact *temp = find_contact_by_index(head, choice_index);
                    if (temp != NULL) {
                        show_contact_detail(temp);
                    } else {
                        printf("Contact not found.\n");
                    }
                    break;
                case 3:
                    printf("Enter index of contact to update: ");
                    scanf("%d", &choice_index);
                    temp = find_contact_by_index(head, choice_index);
                    if (temp != NULL) {
                        update_contact(temp);
                        show_contact_detail(temp);
                    } else {
                        printf("Contact not found.\n");
                    }
                    break;

                // Inside the main function, when the user wants to remove a contact
                case 4:
                    printf("Enter index of contact to remove: ");
                    scanf("%d", &choice_index);
                    temp = find_contact_by_index(head, choice_index);
                    if (temp != NULL) {
                        remove_contact(temp);
                    } else {
                        printf("Contact not found.\n");
                    }
                    break;
                case 6:
                    state = 1;
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
        }
    }
    
    
    closedir(contactsDir);
    // Free memory
    free_contacts(head);
    return 0;
}
