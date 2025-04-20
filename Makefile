CC = cc
CFLAGS = -fsanitize=address
LIBFT = libft.a
NAME = minishell
SRC = ./builtins/cd.c ./builtins/pwd.c ./builtins/unset.c ./builtins/env.c ./builtins/export.c ./minishell.c
OBJS = ${SRC:.c=.o}
LIBFT_PATH = ./libft

all: $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline ./libft/$(LIBFT) -o $(NAME)

$(LIBFT):
	make --no-print-directory -C $(LIBFT_PATH)

clean:
	rm -f $(OBJS)
	make clean --no-print-directory -C $(LIBFT_PATH)

fclean: clean
	rm -f $(LIBFT) $(NAME)
	make fclean --no-print-directory -C $(LIBFT_PATH)

re: fclean all