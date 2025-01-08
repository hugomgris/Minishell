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

SRC 		:= 	main/minishell.c 				\
				main/loop.c 					\
				main/prompt_utils.c				\
				builtins/cd.c 					\
				builtins/cd_utils1.c			\
				builtins/cd_utils2.c			\
				builtins/cd_utils3.c			\
				builtins/cd_utils4.c			\
				builtins/echo.c 				\
				builtins/env.c 					\
				builtins/exit.c 				\
				builtins/export_utils1.c 		\
				builtins/export_utils2.c 		\
				builtins/export.c 				\
				builtins/unset.c				\
				executor/executor.c				\
				executor/executor_utils1.c		\
				executor/executor_utils2.c		\
				executor/executor_utils3.c		\
				executor/executor_utils4.c		\
				executor/executor_utils5.c		\
				executor/executor_utils6.c		\
				executor/executor_utils7.c		\
				executor/executor_utils8.c		\
				executor/piping.c 				\
				executor/piping_utils1.c		\
				executor/redirection.c 			\
				executor/redirection_utils1.c 	\
				executor/redirection_utils2.c 	\
				executor/redirection_utils3.c 	\
				executor/rerouter.c				\
				parser/parser.c 				\
				parser/parser_utils.c			\
				parser/tokenizer.c 				\
				parser/tokenizer_utils1.c		\
				parser/tokenizer_utils2.c		\
				parser/syntax_checker.c 		\
				parser/expand_variable.c		\
				parser/redirection_checker.c	\
				parser/wildcards.c				\
				parser/wildcards_utils.c		\
				signals/signals.c 				\
				env/env_manager.c 				\
				env/env_utils.c 				\
				utils/string_utils.c 			\
				utils/error_handler.c 			\
				utils/garbage_collector.c		\
				utils/token_utils1.c			\
				utils/token_utils2.c			\
				utils/token_utils3.c			\
				executor/pre_executor.c

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
