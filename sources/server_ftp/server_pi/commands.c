/*
** EPITECH PROJECT, 2017
** my_ftp
** File description:
** header
*/

#include "../../../includes/ftp.h"
#include <string.h>
#include <stdio.h>

cf_arr_t commands[] = {
    {
        .str = "USER",
        .ptr = command_usr
    },
    {
        .str = "PASS",
        .ptr = command_pass
    },
    {
        .str = "NOOP",
        .ptr = command_noop
    },
    {
        .str = "QUIT",
        .ptr = command_quit
    },
    {
        .str = "HELP",
        .ptr = command_help
    },
    {
        .str = "CWD",
        .ptr = command_cwd
    },
    {
        .str = "CDUP",
        .ptr = command_cdup
    },
    {
        .str = "PWD",
        .ptr = command_pwd
    },
    {
        .str = "DELE",
        .ptr = command_dele
    },
    {
        .str = NULL,
        .ptr = (fptr_commands_t)0
    }
};
