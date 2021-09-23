#include "mysqlConnection.h"
#define PORT 8080

char* passwordChecker(char* password);
void removeNewline(char* str);
int menu(int confid, int sock, User* user);
int start(int confid, int sock);
int login(int confid, int sock);
int signin(int confid, int sock);

MYSQL* con;

int main(int argc, char const *argv[]) {
    con = mysql_init(NULL);
    if (!mysql_real_connect(con, "localhost", "java", "password", "demo", 0, NULL, 0)) return -225;
	if (!hasTable(con)) mysql_query(con, "CREATE TABLE users (ID INT NOT NULL AUTO_INCREMENT UNIQUE,username VARCHAR(225) NOT NULL,password VARCHAR(225) NOT NULL);");

    int sock;
    if ((sock = socket(AF_INET , SOCK_STREAM , 0)) == -1) return -1;

    struct sockaddr_in serverSettings;
    memset(&serverSettings, 0, sizeof(serverSettings));
    serverSettings.sin_family = AF_INET;
    serverSettings.sin_port = htons(PORT);
    serverSettings.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*) &serverSettings, sizeof(serverSettings)) < 0) return -2;
    if (listen(sock, 1) < 0) return -3;
    int confid = accept(sock, (struct sockaddr*) NULL, NULL);

    int n = start(confid, sock);
    close(confid);
    int x = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(int));
    return n;
}

char* passwordChecker(char* password) {
    if (strlen(password) < 8 || strlen(password) > 200) return "Password is either too much small or big\n";
    int countLower = 0;
    int countUpper = 0;
    int countDigits = 0;
    for (int i = 0; i < strlen(password); ++i) {
        if (*(password+i) >= '0' && *(password+i) <= '9') countDigits++;
        else if (*(password+i) >= 'A' && *(password+i) <= 'Z') countUpper++;
        else if (*(password+i) >= 'a' && *(password+i) <= 'z') countLower++;
    }
    if (countLower <= 1) return "password must have atleast two lower case!\n";
    else if (countUpper <= 1) return "password must have atleast two upper case!\n";
    else if (countDigits <= 1) return "password must have atleast two digits!\n";
    else return "\0";
}

void removeNewline(char* str) {
    int length = strlen(str);
    if (*(str+length-1) == '\n') *(str+length-1) = '\0';
}

int menu(int confid, int sock, User* user) {
    char* sendings = "\nWelcome back to Yoav systems!\nchoose the service you want just by sending number:\n1.delete your user\n2.show all users\n3.exit\n";
    write(confid, sendings, strlen(sendings));

    int len = 0;
    ioctl(confid, FIONREAD, &len);
    char receive[len];
    if (len > 0) read(confid, receive, 1);

    char receiveOption[1] = {'\0'};
    int n = read(confid, receiveOption, 1);

    if (n < 0 || (*receiveOption != '1' && *receiveOption != '2' && *receiveOption != '3')) return -1;
    else if (*receiveOption == '1') {
        char username[strlen(user->username)];
        strcpy(username, user->username);
        removeUser(con, username);
        return 0;
    }
    else if (*receiveOption == '2') {
        char* listgret = "\nThe List:\n";
        write(confid, listgret, strlen(listgret));
        User* usersArr = getUsers(con);
        char usernames[226];
        while (*(usersArr->password) != '\0') {
            sprintf(usernames, "%s\n", usersArr->username);
            write(confid, usernames, strlen(usernames));
            memset(usernames, 0, sizeof(usernames));
            *usernames = '\0';
            usersArr++;
        }
        return menu(confid, sock, user);
    }
    else return 0;
}

int start(int confid, int sock) {
    char* sendings = "Welcome to Yoav systems!\nchoose the service you want just by sending number:\n1.login\n2.resgister\n3.exit\n";
    write(confid, sendings, strlen(sendings));
    char* receiveOption;
    *receiveOption = '\0';
    int n = read(confid, receiveOption, 1);
    if (n < 0 || (*receiveOption != '1' && *receiveOption != '2' && *receiveOption != '3')) return -1;
    else if (*receiveOption == '1') return login(confid, sock);
    else if (*receiveOption == '2') return signin(confid, sock);
    else return 0;
}

int login(int confid, int sock) {
    int len = 0;
    ioctl(confid, FIONREAD, &len);
    char receive[len];
    if (len > 0) read(confid, receive, 1);

    char* loginStrUsername = "username: ";
    write(confid, loginStrUsername, strlen(loginStrUsername));

    char receiveUsername[225];
    memset(receiveUsername, 0, sizeof(receiveUsername));
    *receiveUsername = '\0';
    int n = read(confid, receiveUsername, 225-1);
    if (n < 0) return -1;
    removeNewline(receiveUsername);

    char* loginStrPassword = "password: ";
    write(confid, loginStrPassword, strlen(loginStrPassword));

    char receivePassword[225];
    memset(receivePassword, 0, sizeof(receivePassword));
    *receivePassword = '\0';
    n = read(confid, receivePassword, 225-1);
    if (n < 0) return -1;
    removeNewline(receivePassword);

    User user;
    strcpy(user.username, receiveUsername);
    strcpy(user.password, receivePassword);
    if (isUserExisting(con, &user)) {
        return menu(confid, sock, &user);
    } else {
        char* response = "the username or the password are incorrect!\n";
        write(confid, response, strlen(response));
        return login(confid, sock);
    }
}

int signin(int confid, int sock) {
    int len = 0;
    ioctl(confid, FIONREAD, &len);
    char receive[len];
    if (len > 0) read(confid, receive, 1);

    char* loginUsername = "username: ";
    write(confid, loginUsername, strlen(loginUsername));
    
    char receiveUsername[225];
    memset(receiveUsername, 0, sizeof(receiveUsername));
    *receiveUsername = '\0';
    int n = read(confid, receiveUsername, 225);
    if (n < 0) return -1;
    removeNewline(receiveUsername);

    char* loginPassword = "password: ";
    write(confid, loginPassword, strlen(loginPassword));

    char receivePassword[225];
    memset(receivePassword, 0, sizeof(receivePassword));
    *receivePassword = '\0';
    n = read(confid, receivePassword, 225-1);
    if (n < 0) return -1;
    removeNewline(receivePassword);

    char* passwordResponse = passwordChecker(receivePassword);

    if (isUsernameExisting(con, receiveUsername)) {
        char* response = "the username is already existing in our systems!\n";
        write(confid, response, strlen(response));
        return signin(confid, sock);
    }
    else if (strcmp(passwordResponse, "\0") != 0) {
        write(confid, passwordResponse, strlen(passwordResponse));
        return signin(confid, sock);
    }
    else {
        User user;
        strcpy(user.username, receiveUsername);
        strcpy(user.password, receivePassword);
        insertUser(con, &user);
        return start(confid, sock);
    }
}
