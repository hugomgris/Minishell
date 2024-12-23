/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/23 16:34:49 by nponchon         ###   ########.fr       */
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
# include <sys/wait.h>
# include <errno.h>
# include <dirent.h>

# define TRUE 1
# define FALSE 0
# define S_QUOTE '\''
# define D_QUOTE '\"'

static int		g_var;

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
	T_NL,
	T_SUBPRO,
	T_SQUOTE
}	t_token_type;

typedef struct s_token
{
	void			*content;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_ms
{
	t_list	*ms_env;
	t_list	*gc;
	t_token	*tok;
	t_list	*tokens;
	t_list	*filtered_tokens;
	char	*home;
	char	*user;
	char	*prompt;
	char	*input;
	char	**cmd_table;
	int		exit_status;
	int		heredoc;
}	t_ms;

typedef void	(*t_builtin_func)(t_ms *);

typedef struct s_builtin_map
{
	const char		*name;
	t_builtin_func	func;
}	t_builtin_map;

//MAIN and LOOP functions
void	ms_initialise_minishell(t_ms *ms, char **env);
void	ms_main_loop(t_ms *ms);
char	*ms_check_empty_input(t_ms *ms, char *input);
char	*ms_build_prompt(t_ms *ms);
void	ms_set_shlvl(t_ms *ms);

//TOKENIZER and UTILS
int		ms_tokenizer(t_ms *ms, char *str);
int		ms_is_operator(char *c);
int		ms_is_quote(char c);
int		ms_check_operator(t_ms *ms, char **str);
void	ms_skip_space(char **str);
int		ms_skip_quotes(t_ms *ms, char	*str, int *i);
int		ms_extract_atom(t_ms *ms, char **str);
int		ms_extract_atom_as_token(t_ms *ms, t_token *lst, char **str);
int		ms_extract_quote(t_ms *ms, t_token *lst, char **str);
int		ms_extract_operator(t_ms *ms, t_token_type type, char **str);
int		ms_handle_operator(t_ms *ms, char **str);
void	ms_remove_empty_tokens(t_token **lst, void (*del)(void *));
int		ms_is_empty_token(void *content);
t_token	*ms_new_token(void *content, t_token_type type);
t_token	*ms_toklast(t_token *lst);
void	ms_tokadd_back(t_token **lst, t_token *new);
void	ms_tokclear(t_token **lst, void (*del)(void *));
void	ms_process_token_content(t_ms *ms, char *tmp, t_token **subtok);
void	ms_process_unquoted(t_ms *ms, char **tmp, t_token **subtok);
void	ms_process_quotes(t_ms *ms, char **tmp, t_token **subtok, char quote);
void	ms_expand_subtoken(t_ms *ms, t_token *lst);
char	*ms_merge_subtoken(t_ms *ms, t_token *subtok);
void	ms_tokinsert(t_token **lst, t_token *current, t_token *new);
void	ms_remove_token(t_token **lst, t_token *prev, \
	t_token *cur, void (*del)(void *));

//PARSER
int		ms_parser(t_ms *ms, char *str);
void	ms_expand_variable(t_ms *ms);
int		ms_key_checker(char *key, const char *var);
char	*ms_get_key(t_ms *ms, char *str);
char	*ms_replace_expanded(t_ms *ms, char *str, char *key, int mark);
char	*ms_replace_null_value(t_ms *ms, char *str, char *key);
char	*ms_replace_exit_status(t_ms *ms, char *str, char *status);
char	*ms_search_env(t_ms *ms, char *str, int start);
int		ms_ignore_squote(char *str, int *i);
void	ms_remove_quotes(t_ms *ms);
int		ms_count_quotes(char *str);
char	*ms_trim_quotes(char *str, char *new, int len);
void	ms_sort_toks(t_token *toks);

//WILDCARDS
void	ms_expand_wildcards(t_ms *ms);
void	ms_get_wildcards(t_ms *ms, char *pattern, t_token *subtoken);
int		ms_match_count(char *pattern);

//SYNTAX CHECK
int		ms_syntax_checker(t_ms *ms, char *str);
int		ms_checkspecialchar(t_ms *ms, char *str);
int		ms_checkpipes(t_ms *ms, char *str);
int		ms_check_empty_pipe(t_ms *ms, char *str);
int		ms_checkredirections(t_ms *ms, char *str);
int		ms_checkoutfile(t_ms *ms, char *str);
int		ms_checkinfile(t_ms *ms, char *str);

//ERROR and EXIT HANDLER functions
void	ms_error_handler(t_ms *ms, char *msg, int critical);
void	ms_exit_handler(t_ms *ms, const char *msg, int code);
void	ms_exit(t_ms *ms);

//SIGNAL HANDLER functions
void	ms_signal_handler(int signal);
void	ms_sigtstp_handler(void);
void	ms_sigint_handler(void);
void	ms_sigquit_handler(void);
int		ms_get_set(int flag, int val);

//ENVIRONMENT COPY and MANAGEMENT functions
t_list	*ms_copy_env(t_ms *ms, char **env);
char	*ms_build_entry(char *key, char *value);
char	*ms_get_env_variable(t_ms *ms, char *key);
void	ms_set_env_variable(t_ms *ms, char *key, char *value);
void	ms_add_env_variable(t_ms *ms, char *key, char *value);
char	*ms_create_user_entry(t_ms *ms);
char	*ms_create_pwd_entry(t_ms *ms, char *cwd);
char	*ms_get_prompt_user(t_ms *ms);
char	*ms_username_from_psswd(t_ms *ms);
char	*ms_get_cwd(t_ms *ms);
char	*ms_get_hostname(char *session_manager, t_ms *ms);
char	*ms_get_username(t_ms *ms);
char	*ms_make_home_ref(t_ms *ms, char **env);
char	*ms_get_parent_path(t_ms *ms, char *cwd);

//EXECUTOR functions
void	ms_executor(t_ms *ms);
int		ms_handle_system_command(t_ms *ms, char **arr);
char	**ms_prepare_execution(t_ms *ms, char **arr);
void	execute_parent(t_ms *ms, pid_t pid, int *status);
void	execute_child(t_ms *ms, char **arr);
char	*ms_validate_command(t_ms *ms);
char	*ms_get_command_path(t_ms *ms, char *cmd);
char	*ms_get_env_path_or_def(t_ms *ms);
char	**ms_make_argv(t_ms *ms, t_list *tokens);
void	ms_execute_builtin(t_ms *ms);
int		ms_is_builtin(char *cmd);
void	ms_child_process(t_ms *ms, char **arr);
void	ms_parent_process(t_ms *ms, pid_t pid, int *status);
void	ms_executor_cleanup(t_ms *ms, char **arr, int stdout_b, int stdin_b);
char	**ms_list_to_array(t_ms *ms, char **arr);
char	**ms_allocate_env_array(t_list *list);
void	ms_free_partial_array(char **arr, int index);

//REDIRECTION functions
int		ms_redirection(t_ms *ms);
int		ms_setup_redirects(t_list *token, int *fds, t_ms *ms);
int		ms_open(char *file, int flags, int *fd);
int		ms_has_redirection(t_ms *ms);
int		ms_detect_redirector(char *token);
t_list	*ms_filter_tokens(t_list *tokens);
void	ms_close_redirect_fds(int input, int output, int append, int stderr_fd);
int		ms_handle_open_error(t_ms *ms, char *filename);
int		ms_handle_heredoc(char *delimiter, int *fd);
int		ms_finalize_heredoc(int tmp_fd, int *fd);
int		ms_write_heredoc_lines(int tmp_fd, char *delimiter);
int		ms_handle_heredoc_signal(int tmp_fd, int *fd);
int		ms_open_tmp_heredoc(void);

//BUILTIN CD functions
void	ms_cd(t_ms *ms);
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
char	*ms_cd_initial_path(t_ms *ms);

//ENV, PWD, UNSET, ECHO and EXPORT builtin functions
void	ms_env(t_ms *ms);
void	ms_pwd(t_ms *ms);
void	ms_unset(t_ms *ms);
void	ms_echo(t_ms *ms);
void	ms_export(t_ms *ms);
void	ms_export_ex(t_ms *ms, char *key, char *value);
void	ms_export_error(t_ms *ms, char *entry);
int		ms_export_check(const char *var);
void	ms_export_print(t_ms *ms);
char	*ms_build_export_output(t_ms *ms, char *content, char *sym);
void	ms_process_export_arg(t_ms *ms, char *arg);
void	ms_export_with_value(t_ms *ms, char *arg, char *sign);
void	ms_export_without_value(t_ms *ms, char *arg);
t_list	*ms_sort(t_list *lst, int (*cmp)(const void *, const void *, size_t));
int		ms_key_exists(t_ms *ms, char *key);

//GARBAGE COLLECTOR functions
void	gc_add(void *ptr, t_list **gc);
void	ms_print_list(t_list *list);
void	ms_print_toks(t_token *list);

#endif