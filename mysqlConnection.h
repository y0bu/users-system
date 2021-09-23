#pragma once

#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH_DATA 225

typedef struct {
	char username[MAX_LENGTH_DATA];
	char password[MAX_LENGTH_DATA];
} User;

extern MYSQL* con;

int isUserExisting(MYSQL* con, User* user);
int isUsernameExisting(MYSQL* con, char* username);
int insertUser(MYSQL* con, User* user);
User* getUsers(MYSQL* con);
int removeUser(MYSQL* con, char* username);
int hasTable(MYSQL* con);
int cretthtable(MYSQL* con);
