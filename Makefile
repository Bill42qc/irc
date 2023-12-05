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
			Topic.cpp
SRCS_DIR = src
# Objects are all .o files
OBJS_DIR	=	bin
OBJS	:= 	$(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))

#colors
GREEN	=	\033[38;5;46m
RESET	=	\033[0m


$(shell mkdir -p $(OBJS_DIR))
#------------------------------------------------------------------------------#
#                                 TARGETS                                      #
#------------------------------------------------------------------------------#

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
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
	./$(NAME) 6667 JOIN

nc:
	nc 127.0.0.1 6667
weechat: $(NAME)
	docker pull weechat/weechat
	docker run -it weechat/weechat

-include $(OBJS:.o=.d)
