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

int command_cwd(cli_ctrl_t *ctrl, char **toks)
{
    char *path = NULL;

    if (toks[1] == NULL) {
        write_response_socket(ctrl->client, SYNTAX_ERR, \
                "Wrong command or syntax error.");
        return (0);
    } if ((path = get_new_path(ctrl, toks[1])) == NULL) {
        write_response_socket(ctrl->client, ACTION_NOT_PERFORMED, \
                "Failed to change directory.");
        return (0);
    }
    ctrl->cwd = path;
    write_response_socket(ctrl->client, REQ_FILE_OK, \
            "Requested file action okay, completed.");
    return (0);
}

int command_cdup(cli_ctrl_t *ctrl, char **toks)
{
    (void)(toks);
    char *path = NULL;

    if ((path = get_new_path(ctrl, "..")) == NULL) {
        write_response_socket(ctrl->client, ACTION_NOT_PERFORMED, \
                "Failed to change directory.");
        return (0);
    }
    ctrl->cwd = path;
    write_response_socket(ctrl->client, REQ_FILE_OK, \
            "Requested file action okay, completed.");
    return (0);
}

int command_pwd(cli_ctrl_t *ctrl, char **toks)
{
    char *str = "\"";

    if (toks[1] != NULL) {
        write_response_socket(ctrl->client, SYNTAX_ERR, \
                "Wrong command or syntax error.");
        return (0);
    }
    asprintf(&str, "%s%s\"", str, ctrl->cwd);
    write_response_socket(ctrl->client, CREATED, str);
    return (0);
}

int command_dele(cli_ctrl_t *ctrl, char **toks)
{
    char *path;

    if (toks[1] == NULL) {
        write_response_socket(ctrl->client, SYNTAX_ERR, \
                "Wrong command or syntax error.");
        return (0);
    } if ((path = get_new_path(ctrl, toks[1])) == NULL) {
        write_response_socket(ctrl->client, ACTION_NOT_PERFORMED, \
            "Failed to find file.");
        return (0);
    }
    remove(path);
    write_response_socket(ctrl->client, REQ_FILE_OK, \
        "Requested file action okay, completed.");
    return (0);
}
