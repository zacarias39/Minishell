CC     := cc
CFLAGS := -Wall -Wextra -Werror -I.
SRC    :=   ft_isalnum.c ft_isprint.c ft_memcmp.c  ft_putchar_fd.c ft_split.c \
			ft_strlcat.c ft_strncmp.c ft_substr.c ft_atoi.c ft_isalpha.c ft_isspace.c\
			ft_itoa.c ft_memcpy.c  ft_putendl_fd.c ft_strchr.c  ft_strlcpy.c \
			ft_strnstr.c ft_tolower.c ft_bzero.c   ft_isascii.c ft_memcpy.c \
			ft_memmove.c ft_putnbr_fd.c  ft_strdup.c  ft_strlen.c  ft_strrchr.c \
			ft_toupper.c ft_calloc.c  ft_isdigit.c ft_memchr.c  ft_memset.c ft_strcmp.c  \
			ft_putstr_fd.c  ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_striteri.c
SRCB   :=   ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
			ft_lstclear.c ft_lstiter.c ft_lstmap.c
OBJ    := $(SRC:.c=.o)
BOBJ   := $(SRCB:.c=.o)
NAME   := libft.a
AR     := ar rcs
RM     := rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

bonus: $(BOBJ) $(OBJ)
	$(AR) $(NAME) $(OBJ) $(BOBJ)

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(OBJ) $(BOBJ)

fclean: clean
	$(RM)  $(OBJ) $(BOBJ) $(NAME)

re: fclean all
