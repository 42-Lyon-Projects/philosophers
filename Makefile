NAME = philo

FILES =	main.c								\
		parsing/check_arguments.c			\
		tasks/philosopher_actions.c			\
		tasks/philosopher_task.c			\
		tasks/watchdog_checks.c				\
		tasks/watchdog_task.c				\
		utils/threads_utils.c				\
		utils/time_utils.c					\
		utils/utils.c						\
		init.c

BUILD_DIRECTORY = ./build/

CC = cc

FLAGS = -Wall -Wextra -Werror -c -g3

SOURCES = $(addprefix "sources/", $(SRCS:.c=.o))
OBJS = $(addprefix $(BUILD_DIRECTORY), $(FILES:.c=.o))

$(NAME): $(BUILD_DIRECTORY) $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(BUILD_DIRECTORY)%.o: ./sources/%.c Makefile ./includes/philosophers.h
	$(CC) $(FLAGS) -I ./includes/ $< -o $@

$(BUILD_DIRECTORY):
	mkdir -p $(BUILD_DIRECTORY)parsing	\
	$(BUILD_DIRECTORY)utils				\
	$(BUILD_DIRECTORY)tasks				\

all : $(NAME)

clean :
	$(RM) $(NAME)
	rm -rf $(BUILD_DIRECTORY)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re force philo
