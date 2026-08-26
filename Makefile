CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
# CFLAGS	+=	-fsanitize=address -g
RM		=	rm -rf

NAME	=	pipex
SRCS	=	./src/main.c \
			./src/pipex.c \
			./src/file.c \
			./src/utils.c

OBJS	=	$(SRCS:.c=.o)
HEADERS	=	./include/pipex.h

LIBFT	=	./libft/libft.a

TESTER			=       ./pipex_tester
TESTER_URL		=       https://github.com/diggens42/pipex_tester.git

RESET	=	\x1b[0m
RED		=	\x1b[31m
YELLOW	=	\x1b[33m
BLUE		=	\x1b[34m

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@
	@echo "$(BLUE) $(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@ $(RESET)"
	@echo "$(YELLOW)Creating executable finished.$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -I./include -I./libft -c $< -o $@
	@echo "$(BLUE) $(CC) $(CFLAGS) -c $< -o $@ $(RESET)"
	@echo "$(YELLOW)Creating object file finished.$(RESET)"

$(LIBFT):
	@git submodule init
	@git submodule update --remote
	@$(MAKE) -C ./libft

test: all
	@if [ -d $(TESTER) ]; then \
			git -C $(TESTER) pull --quiet; \
	else \
			git clone --quiet $(TESTER_URL) $(TESTER); \
	fi
	@bash $(TESTER)/tester.sh ./$(NAME)

testclean:
	@$(RM) $(TESTER)
	@echo "$(RED) $(RM) $(TESTER) $(RESET)"
	@echo "$(YELLOW)Deleting tester finished.$(RESET)"

clean:
	@$(RM) $(OBJS)
	@echo "$(RED) $(RM) $(OBJS) $(RESET)"
	@echo "$(YELLOW)Deleting object files finished.$(RESET)"
	@$(MAKE) clean -C ./libft

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED) $(RM) $(NAME) $(RESET)"
	@echo "$(YELLOW)Deleting executable finished.$(RESET)"
	@$(MAKE) fclean -C ./libft

re: fclean
	@$(MAKE) all

.PHONY: all clean fclean re test testclean
