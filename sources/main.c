/*
** EPITECH PROJECT, 2018
** idk
** File description:
** idk
*/


#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../includes/ftp.h"
#include <unistd.h>

static int help(int ret)
{
    printf("USAGE: ./myftp port path\n\t");
    printf("port is the port number on which the server socket listens\n\t");
    printf("path is the path to the home directory for the Anonymous user\n");
    return (ret);
}

int main(int ac, char **av)
{
    errno = 0;
    char *endptr;
    long port;

    if (ac != 3)
        return (help(0));
    port = strtol(av[1], &endptr, 0);
    switch (errno) {
        case 0: NULL;
            break;
        default: return (help(84));
    }
    if (port < 0 || port > USHRT_MAX)
        return (help(84));
    if (chdir(av[2]) == -1)
        return (help(84));
    return (start((unsigned short)port, av[2]));
}
