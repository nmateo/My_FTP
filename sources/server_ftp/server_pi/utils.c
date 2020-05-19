/*
** EPITECH PROJECT, 2020
** my_ftp
** File description:
** header
*/

#include "../../../includes/ftp.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

char *get_new_path(cli_ctrl_t *ctrl, char *path)
{
    char *fullpath;
    char *resolved;

    if ((fullpath = malloc(sizeof(char) * PATH_MAX)) == NULL)
        return (NULL);
    sprintf(fullpath, "%s/%s", ctrl->cwd, path);
    if ((resolved = realpath(fullpath, NULL)) == NULL)
        return (NULL);
    free(fullpath);
    return (resolved);
}

int write_response_socket(int socket, int code, char *msg)
{
    dprintf(socket, "%d %s\r\n", code, msg);
    return (0);
}

void left_operand_void(void)
{

}
