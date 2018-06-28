#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXLINE 4096
#define MAXARG 30
#define MAX_NAME_LEN 256
#define MAX_PATH_LEN 1024
#define MAX_SYS_PATH 8

#define IS_INTERNAL_CMD 0
#define IS_PIPE 1
#define IS_IN_REDIRECT 2
#define IS_OUT_REDIRECT 3
#define IS_OUT_REDIRECT_APPEND 4
#define IS_NORMAL_CMD 5
#define IS_BACKGROUND true

struct parse_info
{
    int flag;
    bool run_flag; 
    char* in_file;
    char* out_file;
    char* command2;
    char** parameters2;
};

char *path;
char *gpath[MAX_SYS_PATH+1];
bool path_flag;
char* command;
char** parameters;
int paraNum;
struct parse_info info;
pid_t pid;
int status;
int fd[2];
pid_t chd_pid;
int chd_status;


void init();
void type_prompt();
int read_command();
void parsing_command(struct parse_info *);
void is_initernal_cmd();
void is_pipe();
void is_in_redirect();
void is_out_redirect();
void is_out_redirect_append();
void is_normal_cmd();
bool find_command();

