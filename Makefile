.DEFAULT_GOAL: all
.DELETE_ON_ERROR:
.PHONY: all bonus clean fclean re

#------------------------------------------------------------------------------#
#                                VARIABLES                                     #
#------------------------------------------------------------------------------#

# Compiler and flags
CC		=	c++
CFLAGS	=	-Wall -Werror -Wextra -std=c++98
RM		=	rm -f
MAKE	=	make

# Output file name
NAME	=	ircserv
# Sources are all .cpp files
SRCS	=	main.cpp \
			ACommand.cpp \
			Channel.cpp \
			Client.cpp \
			Invite.cpp \
			Kick.cpp \
			Mode.cpp \
			Server.cpp \
			Topic.cpp \
			utility.cpp
SRCS_DIR = src
# Objects are all .o files
OBJS_DIR	=	bin
OBJS	:= 	$(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))

#colors
GREEN	=	\033[38;5;46m
RESET	=	\033[0m


#------------------------------------------------------------------------------#
#                                 TARGETS                                      #
#------------------------------------------------------------------------------#

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(shell mkdir -p $(OBJS_DIR))
	$(CC) $(CFLAGS) -c -o $@ $< -MMD

# Generates output file
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Removes objects
clean:
	$(RM) $(OBJS) $(OBJS:.o=.d)

# Removes objects and executables
fclean: clean
	$(RM) $(NAME)
	rm -rf bin

# Removes objects and executables and remakes
re: fclean all

leaks: $(NAME)
	leaks -atExit -- ./$(NAME)

run: $(NAME)
	./$(NAME) 6667 PASSWORD123

nc:
	nc 127.0.0.1 6667

weechat: $(NAME)
	docker pull weechat/weechat

wee: $(NAME)
	docker run -it --rm --name weechat-container weechat/weechat /bin/sh -c \
		"weechat --run-command '/server add irc host.docker.internal/6667 -notls -password=PASSWORD123'"

	

-include $(OBJS:.o=.d)