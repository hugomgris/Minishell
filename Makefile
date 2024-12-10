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
DFLAGS		= -MT $@ -MMD -MP

# -=-=-=-=-    PATH -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

LIBFTDIR	= libs/libft/
PRINTFDIR	= libs/libft/ft_printf/
RM			= rm -fr

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

<<<<<<< HEAD
SRCS 		:= 	src/main/minishell.c 			\
				src/main/loop.c 				\
				src/main/prompt_utils.c			\
				src/builtins/cd.c 				\
				src/builtins/cd_utils1.c		\
				src/builtins/cd_utils2.c		\
				src/builtins/cd_utils3.c		\
				src/builtins/cd_utils4.c		\
				src/builtins/echo.c 			\
				src/builtins/env.c 				\
				src/builtins/exit.c 			\
				src/builtins/export.c			\
				src/builtins/export_utils.c		\
				src/executor/executor.c			\
				src/executor/piping.c 			\
				src/executor/redirection.c 		\
				src/parser/parser.c 			\
				src/parser/tokenizer.c 			\
				src/parser/tokenizer_utils.c	\
				src/parser/syntax_checker.c 	\
				src/parser/expand_variable.c	\
				src/parser/redirection_checker.c\
				src/signals/signals.c 			\
				src/env/env_manager.c 			\
				src/env/env_utils.c 			\
				src/utils/string_utils.c 		\
				src/utils/error_handler.c 		\
				src/utils/garbage_collector.c	
=======
SRC 		:= 	main/minishell.c 			\
				main/loop.c 				\
				main/prompt_utils.c			\
				builtins/cd.c 				\
				builtins/cd_utils1.c		\
				builtins/cd_utils2.c		\
				builtins/echo.c 			\
				builtins/env.c 				\
				builtins/exit.c 			\
				executor/executor.c			\
				executor/piping.c 			\
				executor/redirection.c 		\
				parser/parser.c 			\
				parser/parser_utils.c		\
				parser/tokenizer.c 			\
				parser/tokenizer_utils.c	\
				parser/syntax_checker.c 	\
				parser/expand_variable.c	\
				parser/redirection_checker.c\
				signals/signals.c 			\
				env/env_manager.c 			\
				env/env_utils1.c 			\
				env/env_utils2.c 			\
				utils/string_utils.c 		\
				utils/error_handler.c 		\
				utils/exit_handler.c		\
				utils/garbage_collector.c	
>>>>>>> develop3

SRCDIR		= src
SRCS		= $(addprefix $(SRCDIR)/, $(SRC))

OBJDIR		= .obj
OBJS		= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

DEPDIR		= .dep/
DEPS		= $(addprefix $(DEPDIR), $(SRC:.c=.d))
DEPDIRS		= $(DEPDIR)builtins/ 	\
			$(DEPDIR)env/ 			\
			$(DEPDIR)executor/ 		\
			$(DEPDIR)main/ 			\
			$(DEPDIR)parser/ 		\
			$(DEPDIR)signals/ 		\
			$(DEPDIR)utils/

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: make_libft make_printf $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c Makefile
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(DFLAGS) -Ilibft -Ilibft/ft_printf -c $< -o $@
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"
	@mkdir -p $(DEPDIR) $(DEPDIRS)
	@mv $(patsubst %.o,%.d,$@) $(subst $(OBJDIR),$(DEPDIR),$(@D))/

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
	@$(RM) $(OBJDIR) $(DEPDIR) 
	@echo "$(RED)Cleaned object files and dependencies$(DEF_COLOR)"

fclean: clean
	@$(RM) minishell $(PRINTFDIR)libftprintf.a $(LIBFTDIR)libft.a
	@echo "$(RED)Cleaned all binaries$(DEF_COLOR)"

-include $(DEPS)

re: fclean all

.PHONY: all clean fclean re make_libft make_printf
