# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/12 15:58:11 by dadmendo          #+#    #+#              #
#    Updated: 2025/12/11 12:43:30 by dadmendo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC= cc
CFLAGS=-g -Wall -Wextra -Werror -I ./libft/ -I .

LIBFT_HEADER=./libft/libft.h
LIBFT= ./libft/libft.a

NAME= minishell
HEADER= minishell.h
LIBS= -l readline -L ./libft/ -l ft
OBJS= process_input/lexical.o process_input/process_ast.o \
	process_input/syntax_checker_1.o process_input/syntax_checker_2.o \
	process_input/syntax_checker_utils.o utils/lib_utils.o utils/readline_utils.o \
	builtin/cd_cmd.o builtin/echo_cmd.o builtin/env_cmd.o builtin/export_cmd.o \
	builtin/pwd_cmd.o utils/ft_envs.o utils/ft_getenv_utils.o  utils/ft_clean_data.o \
	test_main.o process_input/cmd_utils.o utils/quick_sort.o

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

$(LIBFT):
	make bonus -C ./libft/

clean:
	rm -fr $(OBJS)
	make clean -C ./libft/

fclean: clean
	rm -fr $(NAME)
	make fclean -C ./libft/

re: fclean all

.PHONY: all clean fclean re
