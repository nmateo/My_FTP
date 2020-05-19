NAME	= myftp

CC	= gcc

RM	= rm -f

SRCS	= ./sources/main.c \
		  ./sources/server_ftp/server_pi/start.c \
		  ./sources/server_ftp/server_pi/utils.c \
		  ./sources/server_ftp/server_pi/interpreter.c \
		  ./sources/server_ftp/server_pi/commands.c \
		  ./sources/server_ftp/server_pi/commands_usr_pass.c \
		  ./sources/server_ftp/server_pi/nav.c

OBJS	= $(SRCS:.c=.o)


CFLAGS = -I ./includes/
CFLAGS += -Wall -Wextra -g

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) 

re: fclean all 

client: fclean_client client

.PHONY: all clean fclean re
