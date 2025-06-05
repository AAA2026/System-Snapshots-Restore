# xv6 File System Snapshot & Restore

## Description

This project introduces a file system snapshot and restore capability to the xv6 operating system, a simple, Unix-like teaching operating system inspired by MIT's 6.828 course. Inspired by utilities like Timeshift in modern Linux distributions, this feature allows users to capture the state of the xv6 file system at a specific point in time and revert back to that state later. This provides a basic mechanism for system recovery in case of accidental data loss, configuration errors, or other unwanted changes within the xv6 environment. The implementation consists of two user-level programs, `snapshot` and `restore`, which are integrated into the standard xv6 build process and are callable directly from the xv6 shell.

## Features

*   **File System Snapshots:** The `snapshot` command enables users to create a complete snapshot of the current file system. Each snapshot represents the state of all files and directories at the moment the command is executed.
*   **Timestamped Storage:** Snapshots are automatically organized within a hidden directory named `.snapshots` located at the root of the file system. Each individual snapshot is stored in its own subdirectory, named using a unique timestamp (e.g., `snapshot_YYYY-MM-DD_HH-MM-SS`), ensuring clear identification and chronological ordering.
*   **Self-Exclusion:** To prevent recursive snapshots and unnecessary storage consumption, the snapshot process intelligently excludes the `.snapshots` directory itself from being included in any snapshot.
*   **Snapshot Listing:** The `restore` command provides a user-friendly interface to view all available snapshots currently stored within the `.snapshots` directory. It presents a numbered list of snapshots for easy selection.
*   **System Restoration:** Users can select a specific snapshot from the list presented by the `restore` command. Upon confirmation, the utility will revert the entire file system (excluding the `.snapshots` directory) back to the state captured in the chosen snapshot. This process involves removing the current file system contents and replacing them with the contents of the selected snapshot.
*   **Seamless xv6 Integration:** The `snapshot` and `restore` programs are compiled as standard xv6 user applications and are readily available for execution within the xv6 shell after building the modified operating system.

## Installation and Setup

To use the snapshot and restore functionality, you need to integrate the provided source files (`snapshot.c`, `restore.c`) into the xv6 source tree and rebuild the operating system. The necessary modifications typically involve:

1.  **Place Source Files:** Copy `snapshot.c` and `restore.c` into the `user/` directory of your xv6 source code.
2.  **Update Makefile:** Edit the main `Makefile` in the root of the xv6 source directory. Add `_snapshot` and `_restore` to the `UPROGS` list. This ensures that the new programs are compiled and included in the file system image.
    ```makefile
    UPROGS=\
	_cat\
	_echo\
	_forktest\
	_grep\
	_init\
	_kill\
	_ln\
	_ls\
	_mkdir\
	_rm\
	_sh\
	_stressfs\
	_usertests\
	_wc\
	_zombie\
	_snapshot\
	_restore
    ```
3.  **Update User Headers/Syscalls (if necessary):** Depending on the specific implementation details within `snapshot.c` and `restore.c` (e.g., if new system calls were added, though the provided documentation suggests these are user programs using existing calls), you might need to update `user/usys.pl` (to generate syscall stubs) and `user/user.h` (to add function prototypes). The provided `OSdoc.docx` indicates prototypes were added to `user.h`.
    ```c
    // Example addition in user/user.h
    // ... other prototypes ...
    int snapshot(void);
    int restore(void);
    ```
4.  **Build xv6:** Navigate to the root directory of your xv6 source code in your terminal and run the standard build commands:
    ```sh
    make clean
    make qemu
    ```
    This will compile the modified kernel and user programs, create a new file system image, and launch the xv6 operating system in the QEMU emulator.

## Usage

Once xv6 is running with the snapshot/restore programs installed, you can use them from the xv6 shell:

1.  **Creating a Snapshot:**
    To capture the current state of the file system, simply run the `snapshot` command:
    ```sh
    $ snapshot
    ```
    Upon successful completion, a message will be displayed indicating the name of the snapshot directory created within `.snapshots` (e.g., `? Snapshot saved as .snapshots/snapshot_2025-06-03_21-42-00`).

2.  **Restoring from a Snapshot:**
    To restore the file system to a previously saved state, run the `restore` command:
    ```sh
    $ restore
    ```
    The program will list all available snapshots found in the `.snapshots` directory, each with a corresponding number:
    ```
    Available snapshots:
    [1] snapshot_2025-06-03_21-42-00
    [2] snapshot_2025-06-03_21-45-15
    Enter the number of the snapshot to restore: 
    ```
    Enter the number corresponding to the snapshot you wish to restore and press Enter. The system will then overwrite the current file system state with the contents of the selected snapshot. A confirmation message will indicate the completion of the restore process (e.g., `? Restored to snapshot: snapshot_2025-06-03_21-42-00`).

    **Caution:** Restoring a snapshot is a destructive operation. All changes made to the file system after the selected snapshot was taken will be lost.

## Dependencies

*   **xv6 Operating System:** This utility is designed specifically for the xv6 environment and relies on its system calls and build system.
*   **Standard C Library (within xv6):** The programs are written in C and use standard library functions available in xv6.
*   **Build Tools:** A standard C compiler (like GCC) and `make` are required to build the modified xv6 system.
*   **QEMU:** Required to run the xv6 operating system if you don't have alternative hardware support.

