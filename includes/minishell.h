/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 18:02:20 by hmunoz-g         ###   ########.fr       */
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
void	ms_main_loop(t_list *ms_env);
char	*ms_check_empty_input(char *input);
void	ms_exit_handler(const char *msg, t_list *ms_env);
char	*ms_build_prompt(t_list *ms_env);
char	*ms_get_cwd(t_list *ms_env);
char	*ms_get_prompt_user(t_list *ms_env);
char	*ms_get_hostname(char *session_manager);
char	*ms_get_env_variable(t_list *ms_env, const char *var_name);
char	*ms_check_empty_input(char *input);
char	*ms_username_from_psswd(void);

//TOKENIZER & SYNTAX CHECK
void	ms_tokenizer(char *input);
void	ms_syntax_checker(char *str);
void	ms_checkquotes(char *str, char c);
void	ms_checkspecialchar(char *str);
void	ms_checkpipes(char *str);

//ERROR HANDLER functions
void	ms_error_handler(char *msg, int is_critical);

//SIGNAL HANDLER functions
void	ms_signal_handler(int signal);
void	ms_sigtstp_handler(void);
void	ms_sigint_handler(void);
void	ms_sigquit_handler(void);

//ENVIRONMENT COPY functions
t_list	*ms_copy_env(t_list *ms_env, char	**env);
void	ms_add_env_variable(t_list **ms_env, const char *env_var);
char	*ms_create_user_entry(void);
char	*get_prompt_user(void);

//GARBAGE COLLECTOR functions
void	gc_cleanup(void);
t_list	**gc_instance(void);
void	gc_add(void	*ptr);

#endif