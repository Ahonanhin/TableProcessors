#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs_def.c"

/**
* Generates a table of file descriptors used by the process(es) specified in target_pid.
* @param head Pointer to the head of a linked list containing information about 
* file descriptors used by various processes.
* @param target_pid A string specifying the process ID(s) for 
* which to generate the table. If set to 0, the table will include all 
* processes.
* @return Returns 1 upon successful completion of the function.
*/
int per_process_table(ListNode *head, char *target_pid){

    // Convert the input parameter `target_pid` to an integer value.
    int pid = atoi(target_pid);

    // Print the header of the table.
    printf("%-4s %-7s \t %-5s \n"," ","PID", "FD");
    printf("%-4s =======================================\n", " ");


    ListNode *traversor = head;

    if (pid) { //If target PID is specified
        while (traversor != NULL) {
            if (traversor->file_d.pid == pid){
                printf("%-4s %-5d \t %-5s\n"," ",pid, traversor->file_d.fd);
            }
            traversor = traversor->next;
        } 
    }
    else { //if there is no target PID
        int i = 0;
        while (traversor != NULL)
        {
            printf("%-4d %-7d \t %-5s\n",i, traversor->file_d.pid, traversor->file_d.fd);
            i++;
            traversor = traversor->next;
        }
    }
    printf("%-4s =======================================\n", " ");
    return 1;
}

/**
*Displays a table of all file descriptors for processes.
*If a PID is specified, only file descriptors for that process are displayed.
*Otherwise, all file descriptors for all user owned processes are displayed.

*@param head a pointer to the head of a singly linked list of all file information
*@param target_pid a string representing the target process ID to display file descriptors for. If NULL, all processes are displayed.

* @return 1 on success
*/
int systemWide_table(ListNode *head, char *target_pid) {
    // Convert the input parameter `target_pid` to an integer value.
    int pid = atoi(target_pid);

    // Print the header of the table.
    printf("%-4s %-7s \t %-5s \t %-10s\n"," ", "PID", "FD", "Filename");
    printf("%-4s =========================================================\n"," ");

    ListNode *traversor = head;

    if (pid) {  //If target PID is specified
        while (traversor != NULL) {
            if (traversor->file_d.pid == pid){
                printf("%-4s %-7d \t %-5s \t %-10s\n"," ", pid, traversor->file_d.fd, traversor->file_d.link);
            }
            traversor = traversor->next;
        } 
    }
    else { //if there is no target PID
        int i = 0;
        while (traversor != NULL)
        {
            printf("%-4d %-7d \t %-5s \t %-10s\n",i, traversor->file_d.pid, traversor->file_d.fd, traversor->file_d.link);
            i++;
            traversor = traversor->next;
        }
    }
    printf("%-4s =======================================\n", " ");
    return 1;
}

/**
 * prints a table of file descriptors and their corresponding inodes 
 * for a given process or for all processes if the target_pid parameter is not provided.
 * 
 * @param head A pointer to the head of a singly linked list of all file information
 * @param target_pid a string representing the target process ID to display file descriptors for. If NULL, all processes are displayed.
 * 
 * @return 1 on sucess
*/
int vnode_table(ListNode *head, char *target_pid){
    // Convert the input parameter `target_pid` to an integer value.
    int pid = atoi(target_pid);

    // Print the header of the table.
    printf("%-4s %-5s \t %-7s \n"," ","FD", "Inode");
    printf("%-4s =======================================\n", " ");

    ListNode *traversor = head;

    if (pid) {
        while (traversor != NULL) {
            if (traversor->file_d.pid == pid){
                printf("%-4s %-5s \t %-7ld\n"," ", traversor->file_d.fd, traversor->file_d.inode);
            }
            traversor = traversor->next;
        } 
    }
    else {
        int i = 0;
        while (traversor != NULL)
        {
            printf("%-4d %-5s \t %-7ld\n", i, traversor->file_d.fd, traversor->file_d.inode);
            i++;
            traversor = traversor->next;
        }
    }
    printf("%-4s =======================================\n", " ");
    return 1;
}


/**
 * Prints a table of file descriptors, filenames, and inodes associated with the 
 * specified process or with all processes if no process ID is provided.
 * 
 * @param head   A pointer to the head of a linked list of file data structures.
 * @param target_pid a string representing the target process ID to display file descriptors for.
 *  If NULL, all processes are displayed.
 * 
 * @return 1 on sucess
*/
int composite_table (ListNode *head, char *target_pid){
    // Convert the input parameter `target_pid` to an integer value.
    int pid = atoi(target_pid);

    // Print the header of the table.
    printf("%-4s %-7s \t %-5s \t%-10s \t %-7s\n"," ", "PID", "FD", "Filename", "Inode");
    printf("%-4s =========================================================\n"," ");

    ListNode *traversor = head;
    int i = 0;
    if (pid) {
        while (traversor != NULL) {
            if (traversor->file_d.pid == pid){
                printf("%-4s %-7d \t %-5s \t %-10s \t %-7ld\n"," ", pid, traversor->file_d.fd, \
                    traversor->file_d.link, traversor->file_d.inode);
            }
            traversor = traversor->next;
        } 
    }
    else {
        
        while (traversor != NULL)
        {   
    
            printf("%-4d %-7d \t %-5s \t %-10s \t %-7ld\n",i , traversor->file_d.pid, traversor->file_d.fd, \
                traversor->file_d.link, traversor->file_d.inode);
            i++;
            traversor = traversor->next;
        } 
    }
    printf("%-4s =======================================\n", " ");
    return 1;
}

/**
 * Prints a table of file descriptors, filenames, and inodes associated with the 
 * specified process to a text file or with all processes if no process ID is provided.
 * 
 * @param head   A pointer to the head of a linked list of file data structures.
 * @param target_pid a string representing the target process ID to display file descriptors for.
 * @param filename string specifying the path of the file to be written
 *  If NULL, all processes are displayed.
 * 
 * @return 1 on sucess
*/
int composite_table_to_binary(ListNode *head, char *target_pid, const char *filename) {
    // Convert the input parameter `target_pid` to an integer value.
    int pid = atoi(target_pid);

    // Open the output file for binary writing.
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    // Write the header to the file.
    char header[] = "    PID      FD      Filename    Inode\n=======================================\n";
    fwrite(header, sizeof(char), sizeof(header), fp);

    ListNode *traversor = head;

    if (pid) {
        while (traversor != NULL) {
            if (traversor->file_d.pid == pid){
                // Write the file data to the file.
                fwrite(&(traversor->file_d), sizeof(File_data), 1, fp);
            }
            traversor = traversor->next;
        } 
    }
    else {
        int i = 0;
        while (traversor != NULL)
        {
            // Write the file data to the file.
            char num[MAX_LEN];
            snprintf(num, sizeof(char)*8, "%d",i);
            fwrite(num, sizeof(int), 1, fp);
            fwrite(&(traversor->file_d), sizeof(File_data), 1, fp);
            i++;
            traversor = traversor->next;
        } 
    }
    // Close the file.
    fclose(fp);

    return 1;
}


/**
* threshold_maker - finds processes that have opened a number of files above a given threshold
* and prints the PIDs of the processes that have opened a number of files greater than
*the specified threshold.

* @head: pointer to the head of the linked list containing the file data
* @threshold: integer value representing the maximum number of files allowed

*@return 1 on success.
*/
int threshold_maker(ListNode *head, int threshold){
    
    //print the header of a file
    printf("## Offending processes --threshold=%d\n", threshold);
    printf("=======================================\n");
    
    ListNode *traversor = head;

    int pid = head->file_d.pid;
    int count = 0;
    while( traversor != NULL){
        if (traversor->file_d.pid != pid){
            if( count > threshold) printf("%d (%d), ", pid, count); //print content results
            pid = traversor->file_d.pid;
            count = 0;
        }
        count++;
        traversor = traversor->next;
    }
    printf("\n");
    return 1;
}


