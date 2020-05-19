/*
** EPITECH PROJECT, 2017
** my_ftp
** File description:
** header
*/

//                                       -------------
//                                       |/---------\|
//                                       ||   User  ||    --------
//                                       ||Interface|<--->| User |
//                                       |\----^----/|    --------
//             ----------                |     |     |
//             |/------\|  FTP Commands  |/----V----\|
//             ||Server|<---------------->|   User  ||
//             ||  PI  ||   FTP Replies  ||    PI   ||
//             |\--^---/|                |\----^----/|
//             |   |    |                |     |     |
// --------    |/--V---\|      Data      |/----V----\|    --------
// | File |<--->|Server|<---------------->|  User   |<--->| File |
// |System|    || DTP  ||   Connection   ||   DTP   ||    |System|
// --------    |\------/|                |\---------/|

// User Interface:
// Space <SP>, Telnet end-of-line code <CRLF>

// USER <SP> <username> <CRLF>   : Specify user for authentication
// PASS <SP> <password> <CRLF>   : Specify password for authentication
// CWD  <SP> <pathname> <CRLF>   : Change working directory
// CDUP <CRLF>                   : Change working directory to parentdir
// QUIT <CRLF>                   : Disconnection
// DELE <SP> <pathname> <CRLF>   : Delete file on the server
// PWD  <CRLF>                   : Print working directory
// PASV <CRLF>                   : Enable "passive" mode for data transfer
// PORT <SP> <host-port> <CRLF>  : Enable "active" mode for data transfer
// HELP [<SP> <string>] <CRLF>   : List available commands
// NOOP <CRLF>                   : Do nothing
// Dtp:
// RETR <SP> <pathname> <CRLF>   : Download file from server to client
// STOR <SP> <pathname> <CRLF>   : Upload file from client to server
// LIST [<SP> <pathname>] <CRLF> : List files in the current working dir

#ifndef MY_FTP_
#define MY_FTP_

#define SERVICE_READY_NMINUTS 120 // Service ready in nnn minutes.
#define DATA_CONNECT_OPEN 125 // Data connection already open; transfer starts
#define FILE_STATUS_OK 150 // File status okay; about to open data connection.
#define COMMAND_OK 200 // Command okay.
#define HELP 214 // Help message.
#define SERVICE_READY_NEW_USR 220 // Service ready for new user.
#define SERVICE_CLOSING_CONNECT 221 // Service closing control connection.
#define DATA_CONNECT_CLOSE 226 // Closing data connection.
#define ENTERING_PASSIVE_MODE 227 // Entering Passive Mode (h1,h2,h3,h4,p1,p2)
#define USR_LOGGED_IN 230 // User logged in, proceed.
#define REQ_FILE_OK 250 // Requested file action okay, completed.
#define CREATED 257 // "PATHNAME" created.
#define USR_OK_PWD 331
#define NEED_ACC 332

#define DT_COMPLETED 226
#define SYNTAX_ERR 500
#define NOT_LOGGED_IN 530
#define ACTION_NOT_PERFORMED 550

#define DEFAULT_USR "Anonymous"
#define DEFAULT_PASS ""
#define HELP_MSG "USER\tPASS\tCWD\tCDUP\nQUIT\tDELE\tPWD\tPASV\nPORT\tHELP\t\
    NOOP\tSTOR\tRETR\nLIST"
#define MAX_CLIENTS 10
#define MAX_REQ 5

#include <sys/select.h>
#include <stdbool.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

// Default values for transfer parameters:
// TYPE: ASCII Non-print
// MODE: Stream
// STRU: File


// control connection
enum log_type {
    USER,
    PASS,
    LOGGED
};

typedef struct server_client_control_s {
    int client;
    int data;
    enum log_type log;
    char *cwd;
} cli_ctrl_t;

typedef struct server_protocol_interpreter_s {
    // Master socket for new connections.
    int sock_master;
    int sock_client;
    cli_ctrl_t clients[MAX_CLIENTS];
    int max_sockd;
} server_pi_t;

// data connection
typedef struct server_data_transfer_process_s {

} server_dtp_t;

typedef struct server_ftp_s {
    server_pi_t pi;
    unsigned short port;
    char *path;
    fd_set readfs;
    bool run;
} server_ftp_t;

// control connection
typedef struct user_protocol_interpreter_s {

} user_pi_t;

// data connection
typedef struct user_data_transfer_process_s {

} user_dtp_t;

typedef struct user_interface_s {
    user_pi_t pi;
    user_dtp_t dtp;
} ui_t;

typedef int (*fptr_commands_t)(cli_ctrl_t *, char **);

typedef struct command_func_arr_s {
    char *str;
    fptr_commands_t ptr;
} cf_arr_t;

extern cf_arr_t commands[];
int command_usr(cli_ctrl_t *, char **);
int command_pass(cli_ctrl_t *, char **);
int command_cwd(cli_ctrl_t *, char **);
int command_cdup(cli_ctrl_t *, char **);
int command_quit(cli_ctrl_t *, char **);
int command_dele(cli_ctrl_t *, char **);
int command_pwd(cli_ctrl_t *, char **);
int command_pasv(cli_ctrl_t *, char **);
int command_port(cli_ctrl_t *, char **);
int command_help(cli_ctrl_t *, char **);
int command_noop(cli_ctrl_t *, char **);
int command_retr(cli_ctrl_t *, char **);
int command_stor(cli_ctrl_t *, char **);
int command_list(cli_ctrl_t *, char **);
int start(unsigned short port, char *path);
int write_response_socket(int socket, int code, char *msg);
void left_operand_void();
int interpret_clients_commands(server_ftp_t *ftp, struct sockaddr_in csock, \
        int clen);
char *get_new_path(cli_ctrl_t *ctrl, char *path);
#endif
