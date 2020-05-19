/*
** EPITECH PROJECT, 2017
** my_ftp
** File description:
** header
*/

#include "../../../includes/ftp.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int asprintf(char **strp, const char *fmt, ...);

int command_usr(cli_ctrl_t *ctrl, char **toks)
{
    if (toks[1] == NULL) {
        write_response_socket(ctrl->client, SYNTAX_ERR, \
                "Wrong command or syntax error.");
        return (0);
    }
    printf("%s\n\n\n", toks[1]);
    if (ctrl->log == USER || ctrl->log == LOGGED) {
        if (strcmp(toks[1], "Anonymous") == 0) {
            ctrl->log = PASS;
            write_response_socket(ctrl->client, USR_OK_PWD, \
                    "User name okay, need password.");
        } else {
            write_response_socket(ctrl->client, USR_OK_PWD, \
                    "Need account for login.");
        }
    }
    return (0);
}

int command_pass(cli_ctrl_t *ctrl, char **toks)
{
    (void)(toks);
    if (ctrl->log == USER) {
        write_response_socket(ctrl->client, NEED_ACC, \
            "Need account for login.");
        return (0);
    } if (ctrl->log == PASS) {
        ctrl->log = LOGGED;
        write_response_socket(ctrl->client, USR_LOGGED_IN, \
            "User logged in, proceed.");
    } else {
        write_response_socket(ctrl->client, NOT_LOGGED_IN, "Not logged in.");
    }
    return (0);
}

int command_quit(cli_ctrl_t *ctrl, char **toks)
{
    if (toks[1] != NULL) {
        write_response_socket(ctrl->client, SYNTAX_ERR, \
            "Wrong command or syntax error.");
    } else {
        write_response_socket(ctrl->client, SERVICE_CLOSING_CONNECT, \
            "Service closing control connection.");
        close(ctrl->client);
        ctrl->client = 0;
        ctrl->log = USER;
        ctrl->cwd = NULL;
    }
    return (0);
}

int command_noop(cli_ctrl_t *ctrl, char **toks)
{
    (void)(toks);
    write_response_socket(ctrl->client, COMMAND_OK, "Done.");
    return (0);
}

int command_help(cli_ctrl_t *ctrl, char **toks)
{
    char *str = "";

    if (ctrl->log != LOGGED) {
        write_response_socket(ctrl->client, NOT_LOGGED_IN, \
            "Please login with USER and PASS.");
        return (0);
    }
    (void)(toks);
    asprintf(&str, "%sThe following commands are recgnized.\n", str);
    for (int i = 0; commands[i].ptr != NULL; i++)
        asprintf(&str, "%s %s", str, commands[i].str);
    write_response_socket(ctrl->client, HELP, str);
    write_response_socket(ctrl->client, HELP, "Help ok");
    return (0);
}
