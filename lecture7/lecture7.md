% Lecture 7
% CprE 308
% January 26, 2015

# Intro

## Today's Topics
 - File System Interface

# File System Interface

## The File Abstraction
<!---
Array of bytes - contents don't matter.  No difference between ASCII, binary, etc.
-->
 - A UNIX file is a simple array of bytes
 - Files are made larger by writing beyond their current end
 - Files are grouped into directories

## Directories
![](img/directories.png)

## Directory Representation
<!---
Mention i-nodes.  Draw linux directory like on p791.
Mention open file descriptor table.  How files are used, etc.
-->
![](img/dir_rep.png)

## Interface to the Programmer
<!---
Talk about file descripters, including stdin (0), stdout (1), and stderr (2).
-->
 1. Open a file (read, write, read-write) modes

    `fd = open("file",O_RDONLY);`

 2. Read/Write the file

    `size = read(fd,buffer,n)`

    `size = write(fd,buffer,n);`

 3. Close the file

    `close(fd);`

## File Access Permissions
<!---
Processes get UID/GID of owner.  File gets UID/GID of creating process.
Superuser (root) has UID 0.  Can read/write all files.
x refers to search instead of execute in directories.
-->
Who's allowed to do what?

 - Who?
    - user (owner)
    - group
    - others (rest of the world)
 - What?
    - read
    - write
    - execute

## Permissions Example
<!---
Only file owner and root can change protection
-->
```
% ls -lR
.:
total 2
drwxr-x--x  2 snt    adm    1024 Dec 17 13:34 A
drwxr-----  2 snt    adm    1024 Dec 17 13:34 B

./A:
total 1
-rw-rw-rw-  1 snt    adm     593 Dec 17 13:34 x

./B:
total 2
-r--rw-rw-  1 snt    adm     446 Dec 17 13:34 x
-rw----rw-  1 trina  adm     446 Dec 17 13:45 y
```

## Seetting File Permission
```c
#include <sys/types.h>
#include <sys/stat.h>
int chmod(const char *path, mode_t mode)
```

 - sets the file permissions of the given file to those specified in *mode*
 - only the owner of a file and the superuser may change its permissions
 - nine combinable positions for *mode* (*read/write/execute* for *user/group*, and *others*)
    - S_IRUSR (0400), S_IWUSR (0200), S_IXUSR (0100)
    - S_IRGRP (040), S_IWGRP (020), S_IXGRP (010)
    - S_IROTH (04), S_IWOTH (02), S_IXOTH (01)
