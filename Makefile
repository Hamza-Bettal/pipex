NAME = pipex
NAME_BONUS = pipex_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
SRC = mandatory/ft_split.c mandatory/pipex.c mandatory/utilities.c
BONUS =	bonus/pipex_bonus.c bonus/ft_split_bonus.c \
    bonus/get_next_line.c bonus/here_doc.c bonus/utilities_bonus.c
SRC_OBJ = $(SRC:.c=.o)
BONUS_OBJS = $(BONUS:.c=.o)

all: $(NAME)

bonus: $(NAME_BONUS)

$(NAME): $(SRC_OBJ)
	$(CC) $(CFLAGS) $(SRC_OBJ) -o $(NAME)

$(NAME_BONUS): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(NAME_BONUS)

%.o: %.c mandatory/pipex.h bonus/pipex_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(SRC_OBJ) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: clean