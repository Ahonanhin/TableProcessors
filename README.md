# Table Processors

This program displays tables which can allow users to keep track of open user-owned files, the assignation of File Descriptors and Processes.

## Installation

To compile the program, you can use the provided makefile. The makefile specifies the compiler to use, the source files to compile, and the target program to build.

1. Open a terminal or command prompt.
2. Navigate to the directory where the program files are located.
3. Type `make` and press Enter. This will compile the program and create an executable file named `table_processors`.
4. To run the program, type `./table_processors` and press Enter.
5. If you want to remove the object files and the executable, you can use the clean target. Simply type `make clean` and press Enter.

## Description

### File: table_processors.c

This file processes file data in a data structure `file_data` and controls the operations called on it.

#### Functions

- `check_if_user_process(int target_pid, uid_t uid)`: Check if the process with the given PID belongs to the user with the given UID.
- `composite_list()`: Returns a linked list of `File_data` structures representing all open file descriptors for all processes owned by the current user.

### File: table_makers.c

This file defines all the different function calls that print the various tables to standard output.

#### Functions

- `per_process_table(ListNode head, char *target_pid)`: Generates a table of file descriptors used by the process(es) specified in `target_pid`.
- `systemWide_table(ListNode head, char *target_pid)`: Displays a table of all file descriptors for processes. If a PID is specified, only file descriptors for that process are displayed.
- `vnode_table(ListNode *head, char *target_pid)`: Prints a table of file descriptors and their corresponding inodes for a given process or for all processes if the `target_pid` parameter is not provided.
- `composite_table (ListNode *head, char *target_pid)`: Prints a table of file descriptors, filenames, and inodes associated with the specified process or with all processes if no process ID is provided.
- `composite_table_to_binary(ListNode *head, char *target_pid, const char *filename)`: Prints a table of file descriptors, filenames, and inodes associated with the specified process to a text file or with all processes if no process ID is provided.
- `int threshold_maker(ListNode *head, int threshold)`: `threshold_maker` - finds processes that have opened a number of files above a given threshold and prints the PIDs of the processes that have opened a number of files greater than the specified threshold.

### File: structs_def.c

This file defines all the different data structures used to process the arguments and the file information

#### Structs

- `Flags`: Processes the different command line arguments
  - `int per_process`: indicate if per-process table is printed
  - `int system_wide`: indicate if systemWide table is printed
  - `int vnodes`: indicate if vnode table is printed
  - `int composite`: indicate if output should be in ASCII format
  - `int output_txt`: indicate if output should be in binary format
  - `int threshold`: Threshold for FDs
  - `int targerPID`: PID to target
- `File_Data`: Stores FD data
  - `int PID`: Process ID of file
  - `char fd [MAX_LEN]`: FD associated with file
  - `char link[PATH_MAX]`: Symbolic link associated with the file
  - `long int inode`: inode of the file
- `ListNode`: Stores all FD data in a list
  - `File_data file_d`: `File_data` struct
  - `struct listNode_struct *next`: pointer to next file data

#### Functions

- `fill_fd(File_data row, int pid, char *fd, char *link, long int inode)`: Modify `File_data` fields to match specified arguments
- `newListNode (File_data data)`: Create a new `ListNode` struct with specified data
- `add_to_list(ListNode *head, ListNode *new)`: Add a new `ListNode` to the end of a `ListNode` linked list

## Usage

The program accepts multiple command-line arguments in (almost) any combination as follows:

- `--per-process`: indicates that only the process FD table will be displayed.
- `--systemWide`: indicates that only the system-wide FD table will be displayed.
- `--Vnodes`: indicates that the Vnodes FD table will be displayed.
- `--composite`: indicates that only the composed table will be displayed.
- `--threshold=X`: where X >0 denotes an integer, indicating that processes which have a number of FDs assigned larger than X should be flagged in the output. For this, it will list the PID and number of assigned FDs, e.g. PID (FD).
- `--output_TXT`: when this flag is used, the program will save the "composite" table in text (ASCII) format into a file named `compositeTable.txt`.
- `--output_binary`: when this flag is used, the program will save the "composite" table in binary format into a file named `compositeTable.bin`.
- Positional argument: (Should be first argument in line) only one positional argument indicating a particular process ID number (PID). If not specified, the program will process all the currently running processes for the user executing the program.

### Default behavior

If no argument is passed to the program, it will display the composite table. Alternatively, the program will display the composite table: `--composite`.

### Notes

- `--output_binary` and `--output_TXT` may not be called with other arguments. The only outputs will go to the `.bin` file or `.txt` file. Calling `--output.bin` will still print the `--composite` data, since no arguments are technically called.
