/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/03 19:52:46 by hmunoz-g         ###   ########.fr       */
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

typedef struct s_ms
{
	t_list	*ms_env;
	t_list	*gc;
	char	*input;
}	t_ms;

//MAIN and LOOP functions
void	ms_main_loop(t_ms *ms);
char	*ms_check_empty_input(t_ms *ms, char *input);
char	*ms_build_prompt(t_ms *ms);

//TOKENIZER & SYNTAX CHECK
void	ms_tokenizer(t_ms *ms, char *str);
void	ms_syntax_checker(t_ms *ms, char *str);
int		ms_checkquotes(char *str, char c);
int		ms_checkspecialchar(char *str);
int		ms_checkpipes(t_ms *ms, char *str);
int		ms_check_empty_pipe(t_ms *ms, char *str);
char	*ms_expand_variable(t_list **ms_env, char *str);
char	*ms_replace_expanded(char *str, char *key, char *var);
char	*ms_replace_null_value(char *str, char *key);
char	*ms_search_env(t_list **ms_env, char *str, int start);

//ERROR and EXIT HANDLER functions
void	ms_error_handler(t_ms *ms, char *msg, int critical);
void	ms_exit_handler(t_ms *ms, const char *msg);

//SIGNAL HANDLER functions
void	ms_signal_handler(int signal);
void	ms_sigtstp_handler(void);
void	ms_sigint_handler(void);
void	ms_sigquit_handler(void);

//ENVIRONMENT COPY and MANAGEMENT functions
t_list	*ms_copy_env(t_ms *ms, char **env);
void	ms_add_env_variable(t_ms *ms, const char *env_var);
char	*ms_create_user_entry(t_ms *ms);
char	*ms_create_pwd_entry(t_ms *ms, char *cwd);
char	*ms_get_prompt_user(t_ms *ms);
char	*ms_username_from_psswd(t_ms *ms);
char	*ms_get_env_variable(t_ms *ms, const char *var_name);
char	*ms_get_cwd(t_ms *ms);
char	*ms_get_hostname(char *session_manager, t_ms *ms);
char	*ms_get_username(t_ms *ms);
void	ms_set_env_variable(t_ms *ms, char *key, char *value);

//EXECUTOR functions
void	ms_executor(t_ms *ms);

//BUILTIN functions
void	ms_cd(t_ms *ms, char *path);
int		ms_change_directory(t_ms *ms, char *new_path);
int		ms_join_paths(char *cwd, char *path, char **new_path);
void	ms_cd_absolute(t_ms *ms, char *path);
void	ms_cd_home(t_ms *ms);
void	ms_cd_back(t_ms *ms);
void	ms_cd_relative(t_ms *ms, char *path);
void	ms_update_env_pwd(t_list **env, const char *new_cwd);
char	*ms_getcwd_or_error(t_ms *ms);

//GARBAGE COLLECTOR functions
void	gc_add(void *ptr, t_list **gc);
void	ms_print_list(t_list *list);

#endif