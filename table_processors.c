#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>
#include<fcntl.h>

#include "table_makers.c"

#ifndef	_DIRENT_H
#define	_DIRENT_H	1
#endif




/**
* Check if the process with the given PID belongs to the user with the given UID.
* 
* @param target_pid The PID of the process to check.
* @param uid The UID of the user to check.
* 
* @return 1 if the process belongs to the user, 0 otherwise.
Side effects:
 * - If an error occurs (e.g., directory cannot be opened), the function prints an error message to stderr and exits.
*/

int check_if_user_process(int target_pid, uid_t uid){

    // Get the user's password entry
    struct passwd *pw = getpwuid(uid);

    // Check if the password entry was successfully retrieved
    if(pw == NULL){
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
    
    // Open the directory containing the list of processes
    DIR *dir  = opendir("/proc");
    struct dirent * entry;

    // Check if the directory was successfully opened
    if(dir == NULL){
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Read the next entry in the directory
    if((entry =  readdir(dir)) !=  NULL){
        char pathname[PATH_MAX];
        char line[MAX_LEN];
        char *ptr;
        
        //if there is a target
        if(target_pid){

            // Construct the pathname of the status file for the target process
            snprintf(pathname, sizeof(pathname),"/proc/%d/status", target_pid);

            FILE *fp = fopen(pathname, "r"); 

            if(fp == NULL) return 0; //in case file is unseccfully open

            while (fp){
                fgets(line, sizeof(line), fp);
                if(strncmp(line, "Uid:", 4) == 0){ //CHeck every line starting with Uid:
                    ptr = line + 4;
                    while(*ptr == ' ' || *ptr == '\t'){ //Move pointer until first digit of uid
                        ptr++;
                    }

                    uid_t p_uid = atoi(ptr);// convert to string
                    if (p_uid == pw->pw_uid) return 1; //if process uid matches user_uid, then true
                    fclose(fp); //close file
                    return 0;
                    }
            }
        }
    }
    return 0;
}


/**
 * @brief Returns a linked list of File_data structures representing 
 * all open file descriptors for all processes owned by the current user.
 *
 * @return The head of the linked list containing all File_data structures.
 */
ListNode *composite_list(){

    uid_t uid = getuid();
    
    ListNode *head = NULL;
    
    
    // Open the directory containing the list of processes
    DIR *dir  = opendir("/proc");
    
    // Check if the directory was successfully opened
    if(dir == NULL){
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    //Loop through all directory entries
    struct dirent *entry;
    while((entry = readdir(dir)) !=  NULL){

        char *ptr;
        int pid = strtol(entry->d_name, &ptr, 10);

        // Check if the process belongs to the current user and is valid
        if(*ptr == '\0' && check_if_user_process(pid, uid)){

            char pathname[PATH_MAX];
            snprintf(pathname, PATH_MAX,"/proc/%d/fd", pid);

            DIR *fd_dir = opendir(pathname);
            struct dirent *fd_insider;
            if(fd_dir){
                //loop through file descriptors
                while((fd_insider = readdir(fd_dir))){
                    if(fd_insider->d_type == DT_LNK){

                        //create a new filw data object and set its attributes
                        File_data fd;
                        fd.pid = pid, fd.inode = fd_insider->d_ino;
                        strcpy(fd.fd, fd_insider->d_name);

                        char pathname2[PATH_MAX];
                        snprintf(pathname2, PATH_MAX,"/proc/%d/fd/%s", pid, fd_insider->d_name);
                        char link[MAX_LEN];
                        ssize_t length = readlink(pathname2, link, MAX_LEN-1);
                        if(length){ //checks if readlink is successful
                            link[length] = '\0';
                            strcpy(fd.link, link);
                        }
                        //creates a new ListNode and add it to the linked List
                        ListNode *temp = newListNode(fd);
                        // printf("%d\n", temp->file_d.pid);
                        head = add_to_list(head, temp);
                    }
                }

            }
        }     
    }
    return head; //returns head of linked list
}

int main(int argc, char** argv){
    
    char *end_ptr;
    int target = 0;
   
    ListNode *table = composite_list();

    // printf("%d \n", table->file_d.pid);
    Flags cmd_line;
    cmd_line.composite = 0, cmd_line.output_binary = 0, cmd_line.per_process = 0, cmd_line.target = 0, \
    cmd_line.system_wide = 0, cmd_line.threshold = 0, cmd_line.output_txt = 0, cmd_line.vnodes = 0;


    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--per-process") == 0) {
            cmd_line.per_process = 1;
        } else if (strcmp(argv[i], "--systemWide") == 0) {
            cmd_line.system_wide = 1;
        } else if (strcmp(argv[i], "--Vnodes") == 0) {
            cmd_line.vnodes = 1;
        } else if (strcmp(argv[i], "--composite") == 0) {
            cmd_line.composite = 1;
        } else if (strcmp(argv[i], "--output_TXT") == 0) {
            cmd_line.output_txt = 1;
        } else if (strcmp(argv[i], "--output_binary") == 0) {
            cmd_line.output_binary = 1;
        } else if (strncmp(argv[i], "--threshold=", 12) == 0) {
            cmd_line.threshold = atoi(argv[i] + strlen("--threshold="));
        } else {
            target = strtol(argv[1], &end_ptr, 10);
            if (target) {
                uid_t uid = getuid();
                if (!check_if_user_process(target, uid)){
                    fprintf(stderr,"Process is not owned by user");
                return 1;
                }
                if (i == 1) printf(">>> Target PID: %d\n",target);
                if (argc == 2) 
                {
                    composite_table(table, argv[1]);
                    return 0;
                }
            } else {
                fprintf(stderr,"Invalid argument\n");
                return 1;
            }
        }
    }

    if (cmd_line.output_txt) {
        // redirect stdout to file
        remove("compositeTable.txt");
        FILE *fp = fopen("compositeTable.txt", "a");
        stdout = fp;
        if(target)composite_table(table, argv[1]);
        else(composite_table(table, "No PID"));
        fclose(fp);
    }
    if(cmd_line.target){
        composite_table(table, "No args");
    }
    if (cmd_line.per_process) {
       per_process_table(table, argv[1]);
    }

    if (cmd_line.system_wide) {
        systemWide_table(table, argv[1]);
    }

    if (cmd_line.vnodes) {
        vnode_table(table, argv[1]);
    }

    if (cmd_line.composite || (!cmd_line.per_process && !cmd_line.system_wide && !cmd_line.vnodes)) {
        if(target)composite_table(table, argv[1]);
        else(composite_table(table, "No PID"));
    }
    if (cmd_line.threshold){
        threshold_maker(table, cmd_line.threshold);
    }
    if (cmd_line.output_binary) {
    // save composite table to binary file
        composite_table_to_binary(table, argv[1], "compositeTable.bin");
        return 0;
    }
    return 0;
}