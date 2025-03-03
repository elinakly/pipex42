NAME = pipex
SRC_DIR = src

HEADERS	:= -I ./include
CC = cc
CFLAGS = -Wall -Werror -Wextra -g
OBJS_DIR  = objs
LIBS	:= libft/libft.a printf/libftprintf.a 

SRCS = main.c utils.c 

SRCS_BONUS = bonus.c bonus_utils.c

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

OBJS_BONUS = $(SRCS_BONUS:%.c=$(OBJS_DIR)/%.o)
all: $(NAME)

libft/libft.a:
	@make -C libft

printf/libftprintf.a:
	@make -C printf

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)


$(NAME): $(OBJS) libft/libft.a printf/libftprintf.a
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)
	
bonus : $(OBJS_BONUS) libft/libft.a printf/libftprintf.a
	@$(CC) $(OBJS_BONUS) $(LIBS) $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJS_DIR)
	make -C libft clean
	make -C printf clean

fclean: clean
	rm -rf $(NAME)
	make -C libft fclean
	make -C printf fclean

re: fclean all

.PHONY: all re fclean clean libmlx libft/libft.a printf/libftprintf.a