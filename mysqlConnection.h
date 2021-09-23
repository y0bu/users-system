#pragma once
#include <sys/ioctl.h> 
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
	char username[225];
	char password[225];
} User;

extern MYSQL* con;

int isUserExisting(MYSQL* con, User* user);

int isUsernameExisting(MYSQL* con, char* username);

int insertUser(MYSQL* con, User* user);

User* getUsers(MYSQL* con);

int removeUser(MYSQL* con, char* username);

int hasTable(MYSQL* con);
