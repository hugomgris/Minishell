/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 17:07:26 by hmunoz-g         ###   ########.fr       */
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
void	ms_main_loop(t_list **ms_env, t_list **gc);
char	*ms_check_empty_input(char *input, t_list **ms_env, t_list **gc);
char	*ms_build_prompt(t_list **ms_env, t_list **gc);

//TOKENIZER & SYNTAX CHECK
void	ms_tokenizer(t_list **ms_env, char *str, t_list **gc);
void	ms_syntax_checker(char *str, t_list **ms_env, t_list **gc);
int		ms_checkquotes(char *str, char c);
int		ms_checkspecialchar(char *str);
int		ms_checkpipes(char *str, t_list **ms_env, t_list **gc);
int		ms_check_empty_pipe(char *str, t_list **ms_env, t_list **gc);
char	*ms_expand_variable(t_list **ms_env, char *str, t_list **gc);
char	*ms_replace_expanded(char *str, char *key, char *var);
char	*ms_replace_null_value(char *str, char *key);
char	*ms_search_env(t_list **ms_env, char *str, int start, t_list **gc);

//ERROR and EXIT HANDLER functions
void	ms_error_handler(t_list **ms_env, t_list **gc, char *msg, int critical);
void	ms_exit_handler(t_list **ms_env, t_list **gc, const char *msg);

//SIGNAL HANDLER functions
void	ms_signal_handler(int signal);
void	ms_sigtstp_handler(void);
void	ms_sigint_handler(void);
void	ms_sigquit_handler(void);

//ENVIRONMENT COPY and MANAGEMENT functions
t_list	*ms_copy_env(t_list **ms_env, char **env, t_list **gc);
void	ms_add_env_variable(t_list **ms_env, const char *env_var, t_list **gc);
char	*ms_create_user_entry(t_list **ms_env, t_list **gc);
char	*ms_get_prompt_user(t_list **ms_env, t_list **gc);
char	*ms_username_from_psswd(t_list **gc);
char	*ms_get_env_variable(t_list *ms_env, const char *var_name);
char	*ms_get_cwd(t_list **ms_env, t_list **gc);
char	*ms_get_hostname(char *session_manager, t_list **gc);

//GARBAGE COLLECTOR functions
void	gc_add(void *ptr, t_list **gc);
void	ms_print_gc(t_list *gc, int index);

#endif