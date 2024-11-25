# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/29 11:12:41 by hmunoz-g          #+#    #+#              #
#    Updated: 2024/11/25 11:24:46 by hmunoz-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
DEF_COLOR = \033[0;39m
YELLOW = \033[0;93m
CYAN = \033[0;96m
GREEN = \033[0;92m
BLUE = \033[0;94m
RED = \033[0;91m

NAME = minishell
CC = cc
FLAGS = -Werror -Wall -Wextra -g
LIBFTDIR = libs/libft/
PRINTFDIR = libs/libft/ft_printf/
RM = rm -f

SRCS = srcs/main/minishell.c

OBJS = $(SRCS:.c=.o)

all: $(LIBFTDIR)libft.a $(PRINTFDIR)libftprintf.a $(NAME)

%.o: %.c Makefile includes/minishell.h
	$(CC) $(FLAGS) -Ilibft -Ilibft/ft_printf -c $< -o $@
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"

$(NAME): $(LIBFTDIR)libft.a $(PRINTFDIR)libftprintf.a $(OBJS)
	@echo "$(GREEN)Compiling minishell!$(DEF_COLOR)"
	$(CC) $(FLAGS) $(SRCS) $(LIBFTDIR)libft.a $(PRINTFDIR)libftprintf.a -o minishell
	@echo "$(GREEN)Minishell compiled!$(DEF_COLOR)"
	@echo "$(RED)May God have mercy on our souls.$(DEF_COLOR)"

$(LIBFTDIR)libft.a:
	@echo "$(CYAN)Building libft.a!$(DEF_COLOR)"
	@$(MAKE) -C $(LIBFTDIR)

$(PRINTFDIR)libftprintf.a:
	@echo "$(CYAN)Building libftprintf.a!$(DEF_COLOR)"
	@$(MAKE) -C $(PRINTFDIR)

clean:
	@$(MAKE) clean -C $(LIBFTDIR)
	@$(MAKE) clean -C $(PRINTFDIR)
	@$(RM) $(OBJS)
	@echo "$(RED)Cleaned object files$(DEF_COLOR)"

fclean: clean
	@$(MAKE) fclean -C $(LIBFTDIR)
	@$(MAKE) fclean -C $(PRINTFDIR)
	@$(RM) pipex
	@echo "$(RED)Cleaned all binaries$(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re libft ft_printf
