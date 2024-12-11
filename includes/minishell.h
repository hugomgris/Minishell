/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/11 16:39:40 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"
# include "../libs/libft/ft_printf/includes/ft_printf.h"
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <errno.h>

# define TRUE 1
# define FALSE 0

typedef struct s_ms
{
	t_list	*ms_env;
	t_list	*gc;
	t_list	*tokens;
	char	*home;
	char	*user;
	char	*prompt;
	char	*input;
	char	**cmd_table;
	int		exit_status;
	int		heredoc;
}	t_ms;

typedef enum e_type_tokens
{
	T_ATOM,
	T_LESS,
	T_GREATER,
	T_DBLESS,
	T_DBGREATER,
	T_PIPE,
	T_LPARENTH,
	T_RPARENTH,
	T_AND,
	T_AMPERSAND,
	T_OR,
	T_NL
}	t_token_type;

//MAIN and LOOP functions
void	ms_initialise_minishell(t_ms *ms, char **env);
void	ms_main_loop(t_ms *ms);
char	*ms_check_empty_input(t_ms *ms, char *input);
char	*ms_build_prompt(t_ms *ms);

//TOKENIZER and UTILS
int		ms_tokenizer(t_ms *ms, char *str);
int		is_operator(char *c);
int		is_quote(char c);
int		ms_check_operator(t_ms *ms, char **str);
void	ms_skip_space(char **str);
int		ms_skip_quotes(t_ms *ms, char	*str, int *i);
int		ms_extract_atom(t_ms *ms, char **str);
int		ms_extract_quote(t_ms *ms, char **str);
int		ms_extract_operator(t_ms *ms, t_token_type type, char **str);
int		ms_handle_operator(t_ms *ms, char **str);

//PARSER
int		ms_parser(t_ms *ms, char *str);
void	ms_expand_variable(t_ms *ms);
int		ms_key_checker(char *key, const char *var);
char	*ms_replace_expanded(t_ms *ms, char *str, char *key, char *var);
char	*ms_replace_null_value(t_ms *ms, char *str, char *key);
char	*ms_replace_exit_status(t_ms *ms, char *str, char *status);
char	*ms_search_env(t_ms *ms, char *str, int start);
void	ms_remove_quotes(t_ms *ms);

//SYNTAX CHECK
int		ms_syntax_checker(t_ms *ms, char *str);
int		ms_checkspecialchar(char *str);
int		ms_checkpipes(t_ms *ms, char *str);
int		ms_check_empty_pipe(t_ms *ms, char *str);
int		ms_checkredirections(t_ms *ms, char *str);
int		ms_checkoutfile(t_ms *ms, char *str);
int		ms_checkinfile(t_ms *ms, char *str);

//ERROR and EXIT HANDLER functions
void	ms_error_handler(t_ms *ms, char *msg, int critical);
void	ms_exit_handler(t_ms *ms, const char *msg, int code);
void	ms_exit(t_ms *ms, char **args);

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
char	*ms_make_home_ref(t_ms *ms, char **env);
char	*ms_get_parent_path(t_ms *ms, char *cwd);

//EXECUTOR functions
void	ms_executor(t_ms *ms);

//BUILTIN CD functions
void	ms_cd(t_ms *ms, char *path);
int		ms_change_directory(t_ms *ms, char *new_path);
int		ms_join_paths(t_ms *ms, char *cwd, char *path, char **new_path);
void	ms_cd_absolute(t_ms *ms, char *path);
void	ms_cd_home(t_ms *ms);
void	ms_cd_back(t_ms *ms);
void	ms_cd_relative(t_ms *ms, char *path);
void	ms_cd_root(t_ms *ms, char *path);
void	ms_update_env_pwd(t_list **env, const char *new_cwd);
char	*ms_getcwd_or_error(t_ms *ms);
char	*ms_expand_tilde(t_ms *ms, char*path);
char	*ms_normalize_path(t_ms *ms, char *path);
char	*ms_pop_from_path(char *path);
void	ms_cd_ascend(t_ms *ms);
void	ms_cd_symlink(t_ms *ms, char *path);
int		ms_is_symlink(char *path);
char	*ms_resolve_symlink(t_ms *ms, char *symlink);
int		ms_cd_isdirectory(t_ms *ms, char *path);
int		ms_check_directory_access(t_ms *ms, char *new_path);
int		ms_update_oldpwd(t_ms *ms, char *current_pwd);

//ENV, PWD, UNSET, ECHO and EXPORT builtin functions
void	ms_env(t_ms *ms);
void	ms_pwd(t_ms *ms);
void	ms_unset(t_ms *ms, char **args);
void	ms_echo(t_ms *ms, char **args);
void	ms_export(t_ms *ms, char **input);
void	ms_export_ex(t_ms *ms, char *key, char *value);
void	ms_export_error(t_ms *ms, char *entry);
int		ms_export_check(const char *var);
void	ms_export_print(t_ms *ms);
void	ms_process_export_arg(t_ms *ms, char *arg);
void	ms_export_with_value(t_ms *ms, char *arg, char *sign);
void	ms_export_without_value(t_ms *ms, char *arg);
t_list	*ms_sort(t_list *lst, int (*cmp)(const void *, const void *, size_t));

//GARBAGE COLLECTOR functions
void	gc_add(void *ptr, t_list **gc);
void	ms_print_list(t_list *list);

#endif