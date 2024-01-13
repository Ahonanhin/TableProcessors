#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PATH_MAX 4096
#define MAX_LEN 256


typedef struct {
    int per_process; // Flag to indicate if output is per-process
    int system_wide; // Flag to indicate if syswide table should be included in output
    int vnodes; // Flag to indicate if vnodes table should be included in output
    int composite; // Flag to indicate if composite table should be included in output
    int output_txt; // Flag to indicate if output should be text-based
    int output_binary; // Flag to indicate if output should be binary
    int threshold; // Threshold for file descriptors (kinda unneeded)
    int target; // PID to target
} Flags;

typedef struct {
    int pid; // Process ID that opened the file
    char fd[MAX_LEN]; // File descriptor associated with the file
    char link[PATH_MAX]; // Symbolic link associated with the file
    long int inode; // Inode of the file
} File_data;

typedef struct listNode_struct {
    File_data file_d; // File data struct
    struct listNode_struct *next; // Pointer to next node
}ListNode;

/**
 * fill_fd: Modify File_data fields to match specified arguments
 * 
 * @param row - File_data struct to be modified
 *  @param pid - Process ID that opened the file
 *  @param fd - File descriptor associated with the file
 *  @param link - Symbolic link associated with the file
 *  @param inode - Inode of the file
 */
void fill_fd(File_data row, int pid, char *fd, char *link, long int inode){
    row.pid = pid, row.inode = inode;
    strcpy(row.fd, fd), strcpy(row.link, link);
    return;
}

/**
 * newListNode: Create a new ListNode struct with specified data
 * 
 * Parameters:
 *  @param data - File_data struct to be included in the new node
 * 
 *  @return A pointer to the new ListNode
 */
ListNode *newListNode (File_data data){
    ListNode *new = (ListNode *) calloc(1, sizeof(ListNode)); //Allocate memory
    new->file_d = data;
    new->next = NULL;
    return new;
}

/**
 * add_to_list: Add a new ListNode to the end of a ListNode linked list
 * 
 * @param head - Pointer to the head of the linked list
 * @param new - Pointer to the new ListNode to be added
 * 
 *  @return A pointer to the head of the linked list
 */
ListNode *add_to_list(ListNode *head, ListNode *new){
    if(head == NULL) return new;
    ListNode *traversor = head;
    while(traversor->next != NULL) traversor = traversor->next;
    traversor->next = new;
    return head;
}

