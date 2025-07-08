#ifndef HEADER
#define HEADER

#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <langinfo.h>
#include <inttypes.h>
#include <ctype.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

#define MAX_LEN 4100

#define min(a, b) (((a) < (b)) ? (a) : (b))

extern pid_t fg_pid;

#endif