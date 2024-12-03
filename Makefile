# -=-=-=-=-    COLOURS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

DEF_COLOR	= \033[0;39m
YELLOW 		= \033[0;93m
CYAN 		= \033[0;96m
GREEN 		= \033[0;92m
BLUE 		= \033[0;94m
RED 		= \033[0;91m

# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

NAME 		:= minishell

# -=-=-=-=-    FLAG -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

CC			= cc
FLAGS		= -Werror -Wall -Wextra -pthread -g -fsanitize=address

# -=-=-=-=-    PATH -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

LIBFTDIR	= libs/libft/
PRINTFDIR	= libs/libft/ft_printf/
RM			= rm -f

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRCS 		:= 	src/main/minishell.c 			\
				src/main/loop.c 				\
				src/main/prompt_utils.c			\
				src/builtins/cd.c 				\
				src/builtins/echo.c 			\
				src/builtins/env.c 				\
				src/builtins/exit.c 			\
				src/executor/executor.c			\
				src/executor/piping.c 			\
				src/executor/redirection.c 		\
				src/parser/parser.c 			\
				src/parser/tokenizer.c 			\
				src/parser/syntax_checker.c 	\
				src/parser/expand_variable.c	\
				src/parser/redirection_checker.c\
				src/signals/signals.c 			\
				src/env/env_manager.c 			\
				src/env/env_utils.c 			\
				src/utils/string_utils.c 		\
				src/utils/error_handler.c 		\
				src/utils/exit_handler.c		\
				src/utils/garbage_collector.c	

OBJS 		:= $(SRCS:.c=.o)

DEPS		:= $(SRCS:.c=.d)

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: make_libft make_printf $(NAME)

%.o: %.c Makefile
	$(CC) $(FLAGS) -MT $@ -MMD -MP -Ilibft -Ilibft/ft_printf -c $< -o $@
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"

$(NAME): $(LIBFTDIR)libft.a $(PRINTFDIR)libftprintf.a $(OBJS)
	@echo "$(GREEN)Compiling minishell!$(DEF_COLOR)"
	$(CC) $(FLAGS) -lreadline $(SRCS) $(LIBFTDIR)libft.a $(PRINTFDIR)libftprintf.a -o minishell
	@echo "$(GREEN)Minishell compiled!$(DEF_COLOR)"
	@echo "$(RED)May God have mercy on our souls.$(DEF_COLOR)"

make_libft:
	@echo "$(CYAN)Building libft.a!$(DEF_COLOR)"
	@$(MAKE) -C $(LIBFTDIR)

make_printf:
	@echo "$(CYAN)Building libftprintf.a!$(DEF_COLOR)"
	@$(MAKE) -C $(PRINTFDIR)

clean:
	@$(MAKE) clean -C $(LIBFTDIR)
	@$(MAKE) clean -C $(PRINTFDIR)
	@$(RM) $(OBJS) $(DEPS) 
	@echo "$(RED)Cleaned object files and dependencies$(DEF_COLOR)"

fclean: clean
	@$(RM) minishell $(PRINTFDIR)libftprintf.a $(LIBFTDIR)libft.a
	@echo "$(RED)Cleaned all binaries$(DEF_COLOR)"

-include $(DEPS)

re: fclean all

.PHONY: all clean fclean re make_libft make_printf
