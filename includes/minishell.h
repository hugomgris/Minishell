/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/27 18:22:52 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"
# include "../libs/libft/ft_printf/includes/ft_printf.h"
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define TRUE 1
# define FALSE 0

//MAIN and LOOP functions
void	ms_main_loop(void);
char	*ms_check_empty_input(char *input);
void	ms_exit_handler(const char *msg);

//TOKENIZER & SYNTAX CHECK
void	ms_tokenizer(char *input);
void	ms_syntax_checker(char *str);
void	ms_checkquotes(char *str, char c);
void	ms_checkspecialchar(char *str);
void	ms_checkpipes(char *str);

//ERROR HANDLER functions
void	ms_error_handler(char *msg);

//SIGNAL HANDLER functions
void	ms_signal_handler(int signal);
void	ms_sigtstp_handler(void);
void	ms_sigint_handler(void);

#endif