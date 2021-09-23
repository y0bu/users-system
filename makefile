main: mysqlConnection.c main.c
	gcc mysqlConnection.c main.c -o main -lmysqlclient 