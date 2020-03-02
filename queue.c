#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q) {
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    if (q->size > 0) {
        list_ele_t *curr = q->head;
        list_ele_t *prev = NULL;
        while (curr) {
            prev = curr;
            curr = curr->next;
            free(prev->value);
            free(prev);
        }
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));

    if (!q->head) {
        q->tail = newh;
    }
    newh->next = q->head;
    q->head = newh;

    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));

    if (!q->head) {
        q->head = newh;
    } else {
        q->tail->next = newh;
    }
    q->tail = newh;

    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }

    if (sp != NULL) {
        memset(sp, '\0', bufsize);
        strncpy(sp, q->head->value, bufsize - 1);
    }

    if (!q->head->next) {
        free(q->head->value);
        free(q->head);

        q->head = NULL;
        q->tail = NULL;
        q->size--;
        return true;
    }

    list_ele_t *tmp;
    tmp = q->head;
    q->head = q->head->next;
    q->size--;

    free(tmp->value);
    free(tmp);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }

    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }

    list_ele_t *curr = q->head;
    list_ele_t *prev = NULL;
    list_ele_t *next = curr->next;

    q->tail = q->head;
    while (next) {
        curr->next = prev;
        prev = curr;
        curr = next;
        next = next->next;
    }

    curr->next = prev;
    q->head = curr;
}


list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    if (!left) {
        return right;
    }
    if (!right) {
        return left;
    }

    list_ele_t *result_head;
    list_ele_t *curr;

    // Initialize result_head and curr
    if (strcmp(left->value, right->value) < 0) {
        result_head = curr = left;
        left = left->next;
    } else {
        result_head = curr = right;
        right = right->next;
    }

    while (left != NULL && right != NULL) {
        if (strcmp(left->value, right->value) < 0) {
            curr->next = left;
            left = left->next;
        } else {
            curr->next = right;
            right = right->next;
        }
    }

    // If there is still node left in 'left list'
    if (left) {
        curr->next = left;
    }

    // If there is still node left in 'right list'
    if (right) {
        curr->next = right;
    }

    return result_head;
}

list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }

    /*
     * Divide nodes of linked list into two halves by slow-fast pointer.
     * Forward 'fast' by two nodes, and forward 'slow' by one node.
     */
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        slow = slow->next;
    }

    list_ele_t *left = head;
    list_ele_t *right = slow->next;
    slow->next = NULL;  // To let 'right' know where to end

    left = merge_sort(left);
    right = merge_sort(right);

    return merge(left, right);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size == 0 || q->size == 1) {
        return;
    }

    q->head = merge_sort(q->head);

    // Update list tail
    list_ele_t *tmp = q->head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    q->tail = tmp;
}
