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

static char *replace_char(char *str, char f, char r)
{
    char *curr = strchr(str, f);

    while (curr) {
        *curr = r;
        curr = strchr(curr, f);
    }
    return (str);
}

static char **sep_tok(char buffer[256])
{
    char **tok = malloc(sizeof(char *) * 6);
    char *ptr;
    int  i = 0;

    if (tok == NULL)
        return (NULL);
    ptr = strtok(buffer, " \n\r\t");
    tok[0] = ptr;
    while ((ptr = strtok(NULL, " \n\r\t")) != NULL) {
        i++;
        tok[i] = ptr;
        if (i == 4)
            break;
    }
    tok[++i] = NULL;
    return (tok);
}

static void parse_command(char buffer[256], cli_ctrl_t *client)
{
    char **tok_array = sep_tok(buffer);
    int i = 0;

    if (tok_array == NULL || tok_array[0] == NULL) {
        write_response_socket(client->client, SYNTAX_ERR, "Syntax err.");
        return;
    }
    for (int i = 0; tok_array[i] != NULL; i++) {
        replace_char(tok_array[i], '\r', '\0');
        replace_char(tok_array[i], '\n', '\0');
    }
    for (; commands[i].ptr != NULL; i++) {
        if (!strcmp(commands[i].str, tok_array[0])) {
            commands[i].ptr(client, tok_array);
            break;
        }
    }
    if (commands[i].ptr == NULL)
        write_response_socket(client->client, SYNTAX_ERR, "Syntax err.");
}

static void reset_client_handle(cli_ctrl_t *client)
{
    shutdown(client->client, SHUT_RDWR);
    close(client->client);
    client->client = 0;
    client->log = USER;
}

int interpret_clients_commands(server_ftp_t *ftp, \
        struct sockaddr_in adc, int clen)
{
    int valread;
    char buff[256];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (FD_ISSET(ftp->pi.clients[i].client, &ftp->readfs)) {
            if ((valread = read(ftp->pi.clients[i].client, buff, 255)) == 0) {
                getpeername(ftp->pi.clients[i].client, \
                        (struct sockaddr *)&adc, (socklen_t *)&clen);
                printf("Client disconnected\tip:port= %s:%d \n", \
                        inet_ntoa(adc.sin_addr), ntohs(adc.sin_port));
                reset_client_handle(&ftp->pi.clients[i]);
            } else {
                buff[valread] = '\0';
                parse_command(buff, &ftp->pi.clients[i]);
            }
        }
    }
    return (0);
}
