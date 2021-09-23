#include "mysqlConnection.h"

int isUserExisting(MYSQL* con, User* user) {
	char* query;
	if ((query = (char*) malloc(strlen(user->username)+strlen(user->password)+strlen("SELECT * FROM users WHERE users.username =  AND users.password = ;")+1))==NULL) return -1;
	*query = '\0';
	sprintf(query, "SELECT * FROM users WHERE users.username = '%s' AND users.password = '%s';", user->username, user->password);
	if (mysql_query(con, query)) {free(query);return -2;}
	free(query);
	MYSQL_RES* result = mysql_store_result(con);
	if (result == NULL) return -3;
	MYSQL_ROW row = mysql_fetch_row(result);
	if (row != NULL) {
		return 1; // has
	} else return 0; // hasn't
}

int isUsernameExisting(MYSQL* con, char* username) {
	char* query;
	if ((query = (char*) malloc(strlen(username)+strlen("SELECT * FROM users WHERE users.username = '';")+1))==NULL) return -1;
	*query = '\0';
	sprintf(query, "SELECT * FROM users WHERE users.username = '%s';", username);
	if (mysql_query(con, query)) {free(query);return -2;}
	free(query);
	MYSQL_RES* result = mysql_store_result(con);
	if (result == NULL) return -3;
	MYSQL_ROW row = mysql_fetch_row(result);
	if (row != NULL) {
		return 1; // has
	} else return 0; // hasn't
}

int insertUser(MYSQL* con, User* user) {
	if (strlen(user->username) == 0) return -80;
	char* query;
	if ((query = (char*) malloc(strlen(user->username)+strlen(user->password)+strlen("INSERT INTO users(username, password) VALUES('', '');")+1))==NULL) return -1;
	*query = '\0';
	sprintf(query, "INSERT INTO users(username, password) VALUES('%s', '%s');", user->username, user->password);
	if (mysql_query(con, query)) {free(query);return -2;}
	free(query);
	return 1;
}

int removeUser(MYSQL* con, char* username) {
	char* query;
	if ((query = (char*) malloc(strlen(username)+strlen("DELETE FROM users WHERE users.username = '';")+1))==NULL) return -1;
	*query = '\0';
	sprintf(query, "DELETE FROM users WHERE users.username = '%s';", username);
	if (mysql_query(con, query)) {free(query);return -2;}
	free(query);
	return 1;	
}

User* getUsers(MYSQL* con) {
	if (mysql_query(con, "SELECT users.username, users.password FROM users;")) return NULL;
	MYSQL_RES* result = mysql_store_result(con);
	if (result == NULL) return NULL;
	MYSQL_ROW row;
	int counter = 0;
	User* arr = (User*) malloc(sizeof(User)*counter);
	while ((row = mysql_fetch_row(result))) {
		counter++;
		arr = (User*) realloc(arr, sizeof(User)*counter);
		strcpy((arr+counter-1)->username, row[0]);
		strcpy((arr+counter-1)->password, row[1]);		
	}
	counter++;
	arr = (User*) realloc(arr, sizeof(User)*counter);
	strcpy((arr+counter-1)->username, "\0");
	strcpy((arr+counter-1)->password, "\0");
	return arr;

}

int hasTable(MYSQL* con) {
	if (mysql_query(con, "SHOW TABLES LIKE 'users';")) return -1;
	MYSQL_RES* result = mysql_store_result(con);
	if (result == NULL) return -2;
	MYSQL_ROW row = mysql_fetch_row(result);
	if (row != NULL) {
		return 1; // has
	} else return 0; // hasn't
}

int cretthtable(MYSQL* con) {
	if(mysql_query(con, "CREATE TABLE users (ID INT NOT NULL AUTO_INCREMENT UNIQUE,username VARCHAR(225) NOT NULL,password VARCHAR(225) NOT NULL);")) return 0;
	return 1;
}
