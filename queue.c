#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new = malloc(sizeof(struct list_head));
    if (!new)
        return NULL;
    INIT_LIST_HEAD(new);
    return new;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    element_t *entry, *tmp = NULL;
    list_for_each_entry_safe (entry, tmp, head, list) {
        list_del(&entry->list);
        q_release_element(entry);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element)
        return false;
    size_t length = strlen(s) + 1;
    char *new_str = malloc(length);
    if (!new_str) {
        free(new_element);
        return false;
    }
    strncpy(new_str, s, length);
    new_element->value = new_str;
    list_add(&new_element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element)
        return false;
    size_t length = strlen(s) + 1;
    char *new_str = malloc(length);
    if (!new_str) {
        free(new_element);
        return false;
    }
    strncpy(new_str, s, length);
    new_element->value = new_str;
    list_add_tail(&new_element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *node;
    if (!head)
        return NULL;
    node = list_first_entry(head, element_t, list);
    list_del(&node->list);
    if (sp) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    element_t *node;
    if (!head)
        return NULL;
    node = list_last_entry(head, element_t, list);
    list_del(&node->list);
    if (sp) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;
    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || head->next == head) {
        return false;
    }
    struct list_head *slow = head->next;
    struct list_head *fast = head->next;
    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    element_t *node = list_entry(slow, element_t, list);
    list_del(&node->list);
    q_release_element(node);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;
    struct list_head *tmp = head->next;
    while (tmp != head) {
        element_t *cur = list_entry(tmp, element_t, list);
        if (tmp->next != head) {
            element_t *next = list_entry(tmp->next, element_t, list);
            if (strcmp(cur->value, next->value)) {
                char *dup_val = cur->value;
                while (tmp != head &&
                       strcmp(list_entry(tmp->next, element_t, list)->value,
                              dup_val)) {
                    element_t *del_node = list_entry(tmp, element_t, list);
                    struct list_head *tmp_next = tmp->next;
                    list_del(&del_node->list);
                    q_release_element(del_node);
                    tmp = tmp_next;
                }
            }
        }
        tmp = tmp->next;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || head->next == head || head->next->next == head)
        return;
    struct list_head *tmp = head->next;
    while (tmp != head && tmp->next != head) {
        struct list_head *first = tmp;
        struct list_head *second = tmp->next;
        list_del(second);
        list_add(second, first);
        tmp = first->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || head->next == head || head->next->next == head)
        return;
    struct list_head *tmp = head->next->next;
    while (tmp != head) {
        struct list_head *next = tmp->next;
        list_del(tmp);
        list_add(tmp, head);
        tmp = next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
