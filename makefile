main: mysqlConnection.c main.c utils.c
	gcc mysqlConnection.c main.c utils.c -o main -lmysqlclient 
	