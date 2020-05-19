/*
** EPITECH PROJECT, 2020
** my_ftp
** File description:
** header
*/

#include "../../../includes/ftp.h"
#include <netinet/ip.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int configure_master_socket(server_ftp_t *ftp, struct sockaddr_in \
        *sockaddr, unsigned short port)
{
    int op = 1;
    struct in_addr inaddr = {
        htonl(INADDR_ANY)
    };
    if ((ftp->pi.sock_master = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket()");
        exit(84);
    } if (setsockopt(ftp->pi.sock_master, SOL_SOCKET, \
                (SO_REUSEADDR | SO_REUSEPORT ), (char *)&op, sizeof(op)) < 0) {
        perror("setsockopt()");
        exit(84);
    }
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    sockaddr->sin_addr = inaddr;
    if (bind(ftp->pi.sock_master, (const struct sockaddr *)sockaddr, \
                sizeof(*sockaddr)) < 0) {
        perror("bind()");
        exit(84);
    }
    return (0);
}

static void get_active_socket_set(server_ftp_t *ftp)
{
    int sockd;

    errno = 0;
    ftp->pi.max_sockd = ftp->pi.sock_master;
    FD_ZERO(&ftp->readfs);
    FD_SET(ftp->pi.sock_master, &ftp->readfs);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        ((sockd = ftp->pi.clients[i].client) > 0 ? \
        FD_SET(sockd, &ftp->readfs) : left_operand_void());
        (sockd > ftp->pi.max_sockd ? ftp->pi.max_sockd = sockd: 0);
    }
    if (select(ftp->pi.max_sockd + 1, &ftp->readfs, NULL, NULL, NULL) < 0 && \
            (errno != EINTR)) {
        perror("select()");
        exit(84);
    }
}

static void accept_new_clients(server_ftp_t *ftp, \
        struct sockaddr_in saddr_client, int clen)
{
    if (FD_ISSET(ftp->pi.sock_master, &ftp->readfs))
    {
        if ((ftp->pi.sock_client = accept(ftp->pi.sock_master, \
                        (struct sockaddr *)&saddr_client, \
                        (socklen_t *)&clen)) < 0) {
            perror("accept()");
        }
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (ftp->pi.clients[i].client == 0) {
                ftp->pi.clients[i].client = ftp->pi.sock_client;
                ftp->pi.clients[i].log = USER;
                ftp->pi.clients[i].cwd = strdup(ftp->path);
                break;
            }
        }
        printf("New client accepted\tip:port= %s:%d\n", \
        inet_ntoa(saddr_client.sin_addr), ntohs(saddr_client.sin_port));
        write_response_socket(ftp->pi.sock_client, SERVICE_READY_NEW_USR, \
                "Service ready for new user.");
    }
}

static int listening(struct sockaddr_in saddr_client, \
        int clen, server_ftp_t ftp)
{
    if (listen(ftp.pi.sock_master, MAX_REQ) < 0) {
        perror("listen()");
        exit(84);
    }
    while (ftp.run == true) {
        get_active_socket_set(&ftp);
        accept_new_clients(&ftp, saddr_client, clen);
        interpret_clients_commands(&ftp, saddr_client, clen);
    }
    return (0);
}

int start(unsigned short port, char *path)
{
    struct sockaddr_in sockaddr = { 0 };
    struct sockaddr_in sockaddr_clients = { 0 };
    server_ftp_t ftp;

    ftp.run = true;
    if ((ftp.path = strdup(path)) == NULL)
        exit(84);
    configure_master_socket(&ftp, &sockaddr, port);
    memset(&ftp.pi.clients, 0, sizeof(cli_ctrl_t) * MAX_CLIENTS);
    listening(sockaddr_clients, sizeof(struct sockaddr_in), ftp);
    if (close(ftp.pi.sock_master) == -1) {
        perror("close()");
        exit(84);
    }
    return (0);
}
