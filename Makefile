# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dadmendo <dadmendo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/12 15:58:11 by dadmendo          #+#    #+#              #
#    Updated: 2026/03/04 19:27:34 by dadmendo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC= cc
INCDIR= -I exec -I ./libft/ -I extern_libs -I process_input -I signals -I utils -I builtin -I .
CFLAGS= -g -Wall -Wextra -Werror $(INCDIR)


LIBFT_HEADER=./libft/libft.h
HEADERS = minishell.h process_input/ast.h extern_libs/extern_libs.h exec/traversing_ast.h builtin/builtin.h utils/utils.h
LIBFT= ./libft/libft.a

NAME= minishell
LIBS= -l readline -L ./libft/ -l ft
OBJS= process_input/lexical.o process_input/process_ast.o \
	process_input/syntax_checker_1.o process_input/syntax_checker_2.o \
	process_input/syntax_checker_utils.o utils/lib_utils.o utils/readline_utils.o \
	builtin/cd_cmd_utils.o builtin/cd_cmd.o builtin/echo_cmd.o builtin/env_cmd.o builtin/export_cmd.o \
	builtin/unset_cmd.o builtin/pwd_cmd.o builtin/trim.o utils/ft_envs.o utils/ft_getenv_utils.o utils/ft_getenv_utils2.o\
	utils/ft_clean_data.o test_main.o utils/cmd_utils.o utils/cmd_utils2.o builtin/exit_cmd.o utils/quick_sort.o \
	exec/traversing_ast.o exec/traversing_ast_utils.o exec/traversing_ast_utils2.o exec/traversing_ast_utils3.o\
	process_input/heredoc.o utils/get_data.o process_input/expansion.o \
	signals/parent_sig.o process_input/expansion_1.o\
	utils/get_wildcard.o exec/lst_utils.o

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

$(LIBFT):
	make bonus -C ./libft/

$(OBJS): $(HEADERS)

clean:
	rm -fr $(OBJS)
	make clean -C ./libft/

fclean: clean
	rm -fr $(NAME)
	make fclean -C ./libft/

re: fclean all

.PHONY: all clean fclean re
