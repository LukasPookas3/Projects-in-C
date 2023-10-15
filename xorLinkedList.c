#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct node {
    char* name;
	struct node* xor_value;
} Node;

Node* calculate_xor_value(Node* before, Node* after) {
    return (Node*)((intptr_t)before ^ (intptr_t)after);
}

// Inserts the string at the beginning of the XOR linked list.
void insert_string(Node** head, const char* newObj) {
    if ((strlen(newObj) + 1) > 64)
    {
        return;
    }

    Node *inserted_node = (Node *)malloc(sizeof(Node));

    inserted_node->name = (char *)newObj;
    inserted_node->xor_value = calculate_xor_value(*head, NULL);

    if (*head != NULL)
    {
        Node *next_node = calculate_xor_value((*head)->xor_value, NULL);
        (*head)->xor_value = calculate_xor_value(inserted_node, next_node);
    }

    *head = inserted_node;
}

// If possible, inserts before the string "before" and returns true. Returns false if not possible (e.g., the before string is not in the list).
int insert_before(Node** head, const char* before, const char* newObj) {
    Node *current_node = *head;
    Node *previous_node = NULL;
    Node *next_node;
 
    if ((*head == NULL) || ((strlen(newObj) + 1) > 64))
    {
        return 0;
    }

    while (current_node != NULL)
    {
        next_node = calculate_xor_value(previous_node, current_node->xor_value);

        if (current_node->name == before)
        {
            // Creating and assigning space to new node
            Node *new_node = (Node *)malloc(sizeof(Node));
            new_node->name = (char *)newObj;
            new_node->xor_value = calculate_xor_value(previous_node, current_node);

            if (current_node == *head)
            {
                // Assign new head
                current_node->xor_value = calculate_xor_value(new_node, next_node);
                *head = new_node;
            }
            else
            {
                // Store the node previous of previous_node to create the new link
                Node *previous_of_previous_node = calculate_xor_value(previous_node->xor_value, current_node);

                current_node->xor_value = calculate_xor_value(new_node, next_node);
                previous_node->xor_value = calculate_xor_value(previous_of_previous_node, new_node);
            }

            return 1;
        }
 
        previous_node = current_node;
        current_node = next_node;
    }
    
    return 0;
}

// If possible, inserts after the string "after" and returns true. Returns false if not possible (e.g., the after string is not in the list).
int insert_after(Node** head, const char* after, const char* newObj) {
    Node *current_node = *head;
    Node *previous_node = NULL;
    Node *next_node;

    if ((*head == NULL) || ((strlen(newObj) + 1) > 64))
    {
        return 0;
    }

    while (current_node != NULL)
    {
        next_node = calculate_xor_value(previous_node, current_node->xor_value);

        if (current_node->name == after)
        {
            previous_node = current_node;
            current_node = next_node;

            // Creating and assigning space to new node
            Node *new_node = (Node *)malloc(sizeof(Node));
            new_node->name = (char *)newObj;
            new_node->xor_value = calculate_xor_value(previous_node, current_node);

            // Store the node previous of previous_node to create the new link
            Node *previous_of_previous_node = calculate_xor_value(previous_node->xor_value, current_node);

            if (current_node != NULL)
            {
                next_node = calculate_xor_value(previous_node, current_node->xor_value);

                current_node->xor_value = calculate_xor_value(new_node, next_node);
                previous_node->xor_value = calculate_xor_value(previous_of_previous_node, new_node);
            }
            else
            {
                // To avoid an error as the next node to a NULL node cannot be fetched
                previous_node->xor_value = calculate_xor_value(previous_of_previous_node, new_node);
            }

            return 1;
        }
 
        previous_node = current_node;
        current_node = next_node;
    }
    
    return 0;
}

// If possible removes the string at the beginning of the XOR Linked list and returns its value in result. If successful return true, otherwise returns false
int remove_string(Node** head, char* result) {
    Node *temp_node = *head;

    if (*head != NULL)
    {
        int i;
        for (i = 0; i < (strlen((*head)->name) + 1); i++)
        {
            result[i] = ((*head)->name)[i];
        }

        Node *new_head = calculate_xor_value(NULL, (*head)->xor_value);
        if (new_head == NULL)
        {
            *head = NULL;
        }
        else
        {
            new_head->xor_value = calculate_xor_value(*head, calculate_xor_value(NULL, new_head->xor_value));
            *head = new_head;
        }

        free(temp_node);
    }
    else
    {
        return 0;
    }

    return 1;
}

// If possible, removes the string after the string "after" and fills in the result buffer with its value. If successful return true, otherwise returns false
int remove_after(Node** head, const char *after, char *result) {
    Node *current_node = *head;
    Node *previous_node = NULL;
    Node *next_node;
 
    if (*head == NULL)
    {
        return 0;
    }

    while (current_node != NULL)
    {
        next_node = calculate_xor_value(previous_node, current_node->xor_value);

        if (current_node->name == after)
        {
            if (next_node == NULL)
            {
                return 0;
            }
            else
            {
                Node *prev_of_prev_node = previous_node;
                previous_node = current_node;
                current_node = next_node;
                next_node = calculate_xor_value(previous_node, current_node->xor_value);

                int i;
                for (i = 0; i < (strlen(current_node->name) + 1); i++)
                {
                    result[i] = (current_node->name)[i];
                }

                // To avoid an error as next_node does not have a XOR value
                if (next_node == NULL)
                {
                    previous_node->xor_value = calculate_xor_value(prev_of_prev_node, next_node);

                    free(current_node);
                }
                else
                {
                    // Store the node next of the next node to create link
                    Node *next_of_next_node = calculate_xor_value(current_node, next_node->xor_value);

                    previous_node->xor_value = calculate_xor_value(prev_of_prev_node, next_node);
                    next_node->xor_value = calculate_xor_value(previous_node, next_of_next_node);

                    free(current_node);
                }
            }

            return 1;
        }
 
        previous_node = current_node;
        current_node = next_node;
    }
    
    return 0;
}

// If possible, removes the string before the string "before" and fills in the result buffer with its value. If successful return true, otherwise returns false.
int remove_before(Node** head, const char *before, char *result) {
    Node *current_node = *head;
    Node *previous_node = NULL;
    Node *next_node;
 
    if (*head == NULL)
    {
        return 0;
    }

    while (current_node != NULL)
    {
        next_node = calculate_xor_value(previous_node, current_node->xor_value);

        if (current_node->name == before)
        {
            if (current_node == *head)
            {
                return 0;
            }
            else
            {
                if (previous_node == *head)
                {
                    // Removing head node
                    remove_string(&(*head), result);
                }
                else
                {
                    int i;
                    for (i = 0; i < (strlen(previous_node->name) + 1); i++)
                    {
                        result[i] = (previous_node->name)[i];
                    }
                    // Store the node previous of previous_node and previous of previous_of_previous_node to create the new link
                    Node *prev_of_prev_node = calculate_xor_value(previous_node->xor_value, current_node);
                    Node *prev_prev_of_prev_node = calculate_xor_value(prev_of_prev_node->xor_value, previous_node);

                    prev_of_prev_node->xor_value = calculate_xor_value(prev_prev_of_prev_node, current_node);
                    current_node->xor_value = calculate_xor_value(prev_of_prev_node, next_node);

                    free(previous_node);
                }
            }

            return 1;
        }
 
        previous_node = current_node;
        current_node = next_node;
    }
    
    return 0;
}

// Removes all nodes and releases any memory allocated to the linked list
void free_all(Node** head) {
    Node *current_node = *head;
    Node *previous_node = NULL;
    Node *next_node;
 
    while (current_node != NULL)
    {
        free(current_node);

        next_node = calculate_xor_value(previous_node, current_node->xor_value);

        previous_node = current_node;
        current_node = next_node;
    }
}

void printList (Node *head)
{
    Node *curr = head;
    Node *prev = NULL;
    Node *next;
 
    printf ("Following are the nodes of Linked List: \n");
 
    while (curr != NULL)
    {
        // print current node
        printf ("%s \n", curr->name);
 
        // get address of next node: curr->npx is
        // next^prev, so curr->npx^prev will be
        // next^prev^prev which is next
        next = calculate_xor_value(prev, curr->xor_value);
 
        // update prev and curr for next iteration
        prev = curr;
        curr = next;
    }
}
 
// Driver program to test above functions
int main ()
{
    /* Create following Doubly Linked List
    head-->40<-->30<-->20<-->10 */
    Node *head = NULL;
    char result[64];

    insert_string(&head, "Luke");
    insert_string(&head, "Sean");
    insert_string(&head, "Aki");
    insert_string(&head, "Mark");
 
    printList(head);

    remove_before(&head, "Aki", result);

    printList(head);

    remove_after(&head, "Sean", result);

    printList(head);

    insert_after(&head, "Sean", "Luke");

    printList(head);

    insert_before(&head, "Aki", "Mark");

    printList(head);

    // print the created list

    /*if (remove_string(&head, result))
    {
        printList(head);
    }*/
 
    return (0);
}
