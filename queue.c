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
    if (!head || head->next == head || head->next->next == head)
        return;
    struct list_head *group_prev = head;
    while (true) {
        struct list_head *kth = group_prev;
        int i;
        for (i = 0; i < k; i++) {
            kth = kth->next;
            if (kth == head)
                return;
        }
        struct list_head *group_next = kth->next;
        struct list_head *cur = group_prev->next;
        while (cur->next != group_next) {
            list_del(cur->next);
            list_add(cur->next, group_prev);
        }
        group_prev = cur;
    }
}

static struct list_head *merge_lists(struct list_head *l1,
                                     struct list_head *l2,
                                     bool descend)
{
    struct list_head tmp;
    tmp.next = NULL;
    struct list_head *tail = &tmp;
    while (l1 && l2) {
        element_t *element1 = list_entry(l1, element_t, list);
        element_t *element2 = list_entry(l2, element_t, list);
        int cmp = strcmp(element1->value, element2->value);
        if (descend)
            cmp = -cmp;
        if (cmp <= 0) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = l1 ? l1 : l2;
    return tmp.next;
}
static struct list_head *merge_sort_list(struct list_head *head, bool descend)
{
    if (!head || head->next == head || head->next->next == head)
        return head;
    struct list_head *slow = head, *fast = head->next;
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    struct list_head *mid = slow->next;
    slow->next = NULL;
    struct list_head *left = merge_sort_list(head, descend);
    struct list_head *right = merge_sort_list(mid, descend);
    return merge_lists(left, right, descend);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || head->next == head || head->next->next == head) {
        return;
    }
    struct list_head *first = head->next;
    head->prev->next = NULL;
    struct list_head *sorted = merge_sort_list(first, descend);
    head->next = sorted;
    struct list_head *prev = head;
    struct list_head *cur = sorted;
    while (cur) {
        cur->prev = prev;
        prev = cur;
        cur = cur->next;
    }
    prev->next = head;
    head->prev = prev;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || head->next == head || head->next->next == head)
        return 0;
    int removed = 0;
    struct list_head *cur = head->prev;
    element_t *tail_element = list_entry(cur, element_t, list);
    char *min_value = tail_element->value;
    cur = cur->prev;
    while (cur != head) {
        element_t *tmp_element = list_entry(cur, element_t, list);
        if (strcmp(tmp_element->value, min_value) > 0) {
            struct list_head *tmp = cur->prev;
            list_del(cur);
            q_release_element(tmp_element);
            removed++;
            cur = tmp;
        } else {
            if (strcmp(tmp_element->value, min_value) < 0) {
                min_value = tmp_element->value;
            }
            cur = cur->prev;
        }
    }
    return removed;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || head->next == head || head->next->next == head)
        return 0;
    int removed = 0 struct list_head *cur = head->prev;
    element_t *tail_element = list_entry(cur, element_t, list);
    char *max_value = tail_element->value;
    cur = cur->prev;
    while (cur != head) {
        element_t *tmp_element = list_entry(cur, element_t, list);
        if (strcmp(tmp_element->value, max_value) < 0) {
            struct list_head *tmp = cur->prev;
            list_del(cur);
            q_release_element(tmp_element);
            removed++;
            cur = tmp;
        } else {
            if (strcmp(tmp_element->value, max_value) > 0) {
                max_value = tmp_element->value;
            }
            cur = cur->prev;
        }
    }
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || head->next == head || head->next->next == head)
        return 0;
    return 0;
}
