# sbunix (with SBUsh and sblibc)

## Intro

sbunix is a simple Unix implementation, SBUsh and sblibc are built-in [shell](http://en.wikipedia.org/wiki/Unix_shell) and [libc](http://www.gnu.org/software/libc/) implementation. They are licensed under [GPLv3](http://www.gnu.org/copyleft/gpl.html). 


## Source Codes and Compile

- The source code is avaliable on [GitHub](https://github.com/waigx/sbunix).
 
- To compile the code, run

   ```
   $ make
   ```
- To run the **sbunix**, use

   ```
   qemu-system-x86_64 -curses -cdrom $USER.iso -drive id=disk,file=$USER.img,if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0 -net nic -net user,hostfwd=tcp::10080-:80 -net user,hostfwd=tcp::10023-:23 --no-reboot -gdb tcp::9998
   ```

## About sbunix

- With RTC support, the default timezone is **TIMEZONE_EST_S** (-4), change default timezone in **sys/time/echotime.c**, add new timezone in **rtc.h**.

- Initialization system setting (environment variables) in **/etc/rc**;

- Preemptive Operating System;

- Simple file system implementation: TARFS;

- With virtual memory, user ring3 processes, COW fork(), auto-growing stack;

- With Shell Script files support;

- Provided binaries

  ```
  - cd [Note: Built-in SBUsh]
  - cat
  - echo
  - hello
  - init
  - kill
  - ls
  - printenv
  - ps
  - pwd
  - sleep
  ```
  
- Implemented **syscalls**
  
  ```
  - sys_exit
  - sys_brk
  - sys_fork
  - sys_getpid
  - sys_getppid
  - sys_execve
  - sys_wait4
  - sys_kill [Note: Only **kill -9** supported]
  - sys_nanosleep
  - sys_getcwd
  - sys_chdir
  - sys_open
  - sys_read
  - sys_write [Note: Only Standard Output supported]
  - sys_lseek
  - sys_close
  - sys_getdents
  - sys_yield
  - sys_gettask
  ```
  

## About SBUsh

- **Not** supported export/setenv with variable($) and quotation mark currently.

- **Not** supported escape sequence and quotation mark yet.
 

- Supported escape sequences of PS1 variable:
  * \b a space character
  * \h hostname before first dot
  * \H full hostname
  * \u user name
  * \w current working directory with replacement of home by '~'
  * \W full working directory
  * \s shell name (SHELL_NAME)
  
- Changing environment variables example:
  
  ```
  $ export PATH=/bin:/usr/bin:/usr/local/bin
  ```
  or
  
  ```
  $ setenv PATH /bin:/usr/bin:/usr/local/bin
  ```  
- cd command, execute scripts should work as expected, examples:

  ```
  $ cd ../../  
 
  ```
- & suffix to set process to background, example:

  ```
  $ ls&
 
  ```  


## About sblibc

- Comments of non-standard functions are in the C files.

- **ls** and **hello** are for test purpose, they are not fully functional binary, and will improve in future.

- **malloc** and **free** function can just ensure a runnable status, they will impove by [Buddy memory allocation](http://en.wikipedia.org/wiki/Buddy_memory_allocation) in future.

- Supported escape sequences of printf now:
  * \c char
  * \s string
  * \d integer
  * \u unsigned integer

- Full function list:
  
  ```
  - void exit(int status);
  - void *malloc(size_t size);
  - void free(void *ptr);
  - int brk(void *end_data_segment);
  - pid_t fork(void);
  - pid_t getpid(void);
  - pid_t getppid(void);
  - int execve(const char *filename, char *const argv[], char *const envp[]);
  - pid_t waitpid(pid_t pid, int *status, int options);
  - unsigned int sleep(unsigned int seconds);
  - unsigned int alarm(unsigned int seconds);
  - char *getcwd(char *buf, size_t size);
  - int chdir(const char *path);
  - int open(const char *pathname, int flags);
  - ssize_t read(int fd, void *buf, size_t count);
  - ssize_t write(int fd, const void *buf, size_t count);
  - off_t lseek(int fildes, off_t offset, int whence);
  - int close(int fd);
  - int pipe(int filedes[2]);
  - int dup(int oldfd);
  - int dup2(int oldfd, int newfd);
  - void *opendir(const char *name);
  - struct dirent *readdir(void *dir);
  - int closedir(void *dir);
  - uint64_t syscall_0(uint64_t n);
  - uint64_t syscall_1(uint64_t n, uint64_t a1);
  - uint64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2);
  - uint64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3);
  - uint64_t syscall_4(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4);
  - int printf(const char *format, ...);
  - int min(int n1, int n2);
  - void *sbrk(intptr_t increment);
  - char *itoa(char *buf, int64_t num);
  - char *utoa(char *buf, uint64_t num);
  - char ** setopt(const char *, const char *, char **);
  - char * getopt(char *, const char *, char **);
  - void echoerr(const char *, const char *, const char *);
  - char * readline(char *, int); 
  - char * writeline(const char *, int);
  - int pathtype(const char *);
  - int strlistprintf(char *buf, const char *format, va_list val);
  - char ** splitstr(char *, const char *);
  - char * joinstrlst(char *, char **, const char *);
  - char * freestrarr(char **, int);
  - size_t lenstrarr(char **);
  - char ** cpystrarr(char **, char **);
  - char ** cpynstrarr(char **, char **, size_t);
  - size_t strlen(const char *str);
  - int strcmp(const char *s1, const char *s2);
  - int strncmp(const char *s1, const char *s2, size_t n);
  - char *strcpy(char *dest, const char *src);
  - char *strncpy(char *dest, const char *src, size_t n);
  - char *strstr(const char *haystack, const char *needle);
  - char *strrev(char *dest, const char *src);
  - int nanosleep(const struct timespec *req, struct timespec *rem);
  ```
