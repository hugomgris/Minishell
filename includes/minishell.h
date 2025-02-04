/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/16 09:25:46 by hmunoz-g         ###   ########.fr       */
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
# include <termios.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define TRUE 1
# define FALSE 0
# define S_QUOTE '\''
# define D_QUOTE '\"'
# define GET 0
# define SET 1

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
	T_SQUOTE,
	T_EXPANDED
}	t_token_type;

enum e_shell_state
{
	SHELL_NORMAL = 0,
	SHELL_CHILD_PROCESS = 1,
	SHELL_HEREDOC = 2,
	SHELL_HEREDOC_INTERRUPTED = 3,
	SHELL_CHILD_INTERRUPTED = 4
};

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_match_data
{
	int	i;
	int	j;
	int	start;
	int	match;
}	t_match_data;

typedef struct s_expr
{
	t_token			*tokens;
	struct s_expr	*next;
	struct s_expr	*child;
	char			*separator;
}	t_expr;

typedef struct s_ms
{
	char	*home;
	char	*user;
	char	*prompt;
	char	*input;
	int		exit_status;
	t_list	*ms_env;
	t_list	*export_only;
	t_list	*gc;
	t_token	*tok;
	t_token	*wc;
	t_expr	*expr_tree;
	t_token	*paren_content;
	t_token	*chain_tokens;
	t_token	**exec_chunks;
	char	**cmd_args;
	char	**filt_args;
	int		heredoc_fd;
	int		**pipe_fds;
	int		pipe_count;
	int		chunk_count;
}	t_ms;

typedef struct s_paren_group
{
	t_list	*tokens;
	t_list	*redirections;
	int		start;
	int		end;
}	t_paren_group;

//MAIN and LOOP functions
void	ms_initialise_minishell(t_ms *ms, char **env);
void	ms_main_loop(t_ms *ms);
void	ms_handle_input(t_ms *ms);
char	*ms_check_empty_input(t_ms *ms, char *input);
char	*ms_build_prompt(t_ms *ms);
void	ms_set_shlvl(t_ms *ms);
void	ms_set_custom_colors(t_ms *ms);

//EXPR functions
t_expr	*ms_build_expression_tree(t_ms *ms, t_token *tokens);
t_token	*ms_extract_paren_content(t_token *start, int len);
void	ms_process_token(t_ms *ms, t_expr *expr, t_token **current);
t_expr	*ms_init_expr(void);
t_token	*ms_extract_paren_content(t_token *start, int len);
void	ms_handle_parentheses(t_ms *ms, t_expr *expr, t_token **current);
int		ms_execute_expression(t_ms *ms, t_expr *expr);
int		ms_handle_or(t_ms *ms, t_expr *expr, int result);
int		ms_handle_and(t_ms *ms, t_expr *expr, int result);
void	ms_skip_similar_operators(t_expr **expr, char *op);
int		ms_find_matching_paren(t_token *start);
t_token	*ms_token_dup(t_token *token);
void	ms_free_expression_tree(t_expr *expr);

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
int		ms_toksize(t_token *lst);
t_token	*ms_tokcpy(t_token *original);
void	ms_process_token_content(t_ms *ms, char *tmp, t_token **subtok);
void	ms_process_unquoted(t_ms *ms, char **tmp, t_token **subtok);
void	ms_process_quotes(t_ms *ms, char **tmp, t_token **subtok, char quote);
void	ms_expand_subtoken(t_ms *ms, t_token *lst);
char	*ms_merge_subtoken(t_ms *ms, t_token *subtok);
void	ms_tokinsert(t_token **lst, t_token *current, t_token *new);
void	ms_tokinsert_list(t_token **lst, t_token *current, t_token *new);
t_token	*ms_toksub(t_token *lst, int start, int count);
int		ms_add_node_to_sublist(t_token **sub_list, t_token *current);
void	ms_remove_token(t_token **lst, t_token *prev, \
	t_token *cur, void (*del)(void *));

//PARSER
int		ms_parser(t_ms *ms);
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
char	*ms_trim_spaces(t_ms *ms, char *str);

//WILDCARDS
void	ms_expand_wildcards(t_ms *ms);
void	ms_get_wildcards(t_ms *ms, char *pat, t_token *sub);
int		ms_match_pattern(char *pattern, char *entry, int m, int n);
int		ms_process_dir_entry(t_ms *ms, char *pat, struct dirent *ent);
void	ms_init_match_data(t_match_data *data);
int		ms_retry_star(t_match_data *data, int *start);
void	ms_handle_star(t_match_data *data, int *start);
int		ms_is_hidden_entry(char *entry);
t_token	*ms_tokensort(t_token *tok);
void	ms_add_wc(t_ms *ms, t_token *sub);

//SYNTAX CHECK
int		ms_syntax_checker(t_ms *ms, char *str);
int		ms_checkspecialchar(t_ms *ms, char *str);
int		ms_checkpipes(t_ms *ms, char *str);
int		ms_check_empty_pipe(t_ms *ms, char *str);
int		ms_check_parenthesis(t_ms *ms, char *str);
int		ms_check_or(t_ms *ms, char *str);
int		ms_check_and(t_ms *ms, char *str);
int		ms_is_doubleoperator(t_ms *ms, char *str, int i);
int		ms_check_redirparenthesis(t_ms *ms);
int		ms_is_redirection(char c);
int		ms_check_tokens(t_ms *ms);
int		ms_check_multipleredir(t_ms *ms);

//ERROR and EXIT HANDLER functions
void	ms_error_handler(t_ms *ms, char *msg, int critical);
void	ms_exit_handler(t_ms *ms, const char *msg, int code);
int		ms_exit(t_ms *ms);

//SIGNAL HANDLER functions
void	ms_signal_handler(int signal);
void	ms_reset_signal_handlers(t_ms *ms);
void	ms_sigint_handler(void);
void	ms_sigquit_handler(void);
int		ms_get_set(int flag, int val);
void	ms_setup_signal_handlers(t_ms *ms);

//ENVIRONMENT COPY and MANAGEMENT functions
t_list	*ms_copy_env(t_ms *ms, char **env);
char	*ms_build_entry(char *key, char *value);
char	*ms_get_env_variable(t_ms *ms, char *key);
void	ms_set_env_variable(t_ms *ms, char *key, char *value);
void	ms_add_env_variable(t_ms *ms, char *key, char *value);
char	*ms_create_user_entry(t_ms *ms);
char	*ms_create_pwd_entry(t_ms *ms, char *cwd);
char	*ms_get_prompt_user(t_ms *ms);
char	*ms_username_from_utmp(t_ms *ms);
char	*ms_find_utmp(void);
char	*ms_get_cwd(t_ms *ms);
char	*ms_get_hostname(char *session_manager, t_ms *ms);
char	*ms_get_username(t_ms *ms);
char	*ms_make_home_ref(t_ms *ms, char **env);
char	*ms_get_parent_path(t_ms *ms, char *cwd);

//EXECUTOR functions
int		ms_executor(t_ms *ms);
void	ms_initialize_execution(t_ms *ms, char ***env);
int		ms_execute_chunk(t_ms *ms, char **env, int i);
int		ms_process_command(t_ms *ms, char **env, int i);
int		ms_exec_command(t_ms *ms, char **env);
void	ms_extract_chunks(t_ms *ms, t_token **tokens);
char	*ms_process_chunk(t_ms *ms, t_token **current);
int		ms_count_chunks(t_ms *ms, t_token *tokens);
char	**ms_env_to_array(t_ms *ms, char **arr);
char	**ms_rebuild_env(t_ms *ms);
int		ms_handle_parent_process(t_ms *ms);
int		ms_handle_child_process(t_ms *ms, char **env, int i);
int		ms_is_builtin(const char *cmd);
int		ms_reroute_builtins(t_ms *ms, char **env);
int		ms_handle_builtin(t_ms *ms, char **env, int saved_fds[3]);
int		ms_handle_system_cmd(t_ms *ms, char **env);
int		ms_ex_check_file_in_dir(char *cmd);
int		ms_handle_absolute_path(t_ms *ms, char **env);
int		ms_handle_relative_path(t_ms *ms, char **env);
int		ms_handle_path_search(t_ms *ms, char **env);
int		ms_exec_relative_path(t_ms *ms, char **cmd_args, char **env);
char	*ms_create_error_message(t_ms *ms, char *cmd);
void	ms_save_std_fds(int *saved_fds);
void	ms_restore_std_fds(int *saved_fds);
void	ms_executor_cleanup(t_ms *ms, char **env);
void	ms_close_used_pipes(int **pipe_fds, int i);
void	ms_cleanup_args(t_ms *ms);
void	ms_cleanup_heredoc(t_ms *ms);

//PIPING functions
void	ms_free_pipes(int **pipe_fds, int pipe_count);
int		ms_wait_children(t_ms *ms, int count);
void	ms_create_pipes(t_ms *ms, int ***pipe_fds, int pipe_count);
void	ms_close_parent_pipes(int **pipe_fds, int pipe_count);
void	ms_close_child_pipes(int **pipe_fds, int pipe_count);
void	ms_setup_child_pipes(t_ms *ms, int cmd_index, int pipe_count);
char	**ms_parse_args(t_ms *ms, t_token *exec_chunk, int *arg_count);
char	*ms_process_space_args_in(char *chunk);
char	**ms_process_space_args_out(char **args);
int		ms_exec_direct_path(t_ms *ms, char **cmd_args, char **env);
int		ms_try_path_execution(char *cmd_path, char **cmd_args, char **env);
char	*ms_build_cmd_path(t_ms *ms, char *dir, char *cmd);
int		ms_search_in_path(t_ms *ms, char **cmd_args, char **env);
char	*ms_duplicate_path(t_ms *ms);
int		ms_try_path_execution(char *cmd_path, char **cmd_args, char **env);
int		ms_exec_direct_path(t_ms *ms, char **cmd_args, char **env);

//REDIRECTION functions
int		ms_redirection(t_ms *ms);
int		ms_has_redirection(t_ms *ms);
int		ms_detect_redirector(char *arg);
int		ms_setup_redirects(char **args, int i, int *fds, t_ms *ms);
int		ms_latest_infile(char **args);
void	ms_filter_args(t_ms *ms);
int		ms_count_non_redirectors(char **cmd_args);
char	**ms_allocate_filtered_args(t_ms *ms, int count);
void	ms_populate_filtered_args(t_ms *ms, int count);
int		ms_open(char *file, int flags, int *fd);
int		ms_handle_open_error(t_ms *ms, char *filename);
int		ms_close_redirect_fds(int input, int output, int append, int stderr_fd);
int		ms_has_heredoc(t_ms *ms);
char	*ms_get_separator(t_ms *ms);
int		ms_handle_heredoc_setup(t_ms *ms);
int		ms_manage_heredoc(t_ms *ms, int *fds);
int		ms_handle_heredoc(const char *delimiter, int *fd);
int		ms_open_tmp_heredoc(void);
int		ms_write_heredoc_lines(int tmp_fd, const char *delimiter);
int		ms_finalize_heredoc(int tmp_fd, int *fd);
int		ms_handle_heredoc_signal(int tmp_fd, int *fd);
int		ms_handle_heredoc_error(t_ms *ms, char *error_msg);
int		ms_heredoc_interruption(t_ms *ms, char **env);

//BUILTIN CD functions
int		ms_cd(t_ms *ms);
int		ms_check_cd_args(t_ms *ms);
int		ms_change_directory(t_ms *ms, char *new_path);
int		ms_join_paths(t_ms *ms, char *cwd, char *path, char **new_path);
void	ms_cd_absolute(t_ms *ms, char *path);
int		ms_cd_parent(t_ms *ms);
int		ms_cd_home(t_ms *ms);
int		ms_cd_back(t_ms *ms);
void	ms_cd_relative(t_ms *ms, char *path);
int		ms_cd_root(t_ms *ms, char *path);
void	ms_update_env_pwd(t_list **env, const char *new_cwd);
char	*ms_getcwd_or_error(t_ms *ms);
char	*ms_expand_tilde(t_ms *ms, char*path);
char	*ms_normalize_path(t_ms *ms, char *path);
char	*ms_pop_from_path(char *path);
int		ms_cd_ascend(t_ms *ms);
int		ms_cd_symlink(t_ms *ms, char *path);
int		ms_is_symlink(char *path);
char	*ms_resolve_symlink(t_ms *ms, char *symlink);
int		ms_cd_isdirectory(t_ms *ms, char *path);
int		ms_check_directory_access(t_ms *ms, char *new_path);
int		ms_update_oldpwd(t_ms *ms, char *current_pwd);
char	*ms_cd_initial_path(t_ms *ms);

//ENV, PWD, UNSET, ECHO and EXPORT builtin functions
int		ms_env(t_ms *ms);
int		ms_pwd(t_ms *ms);
int		ms_unset(t_ms *ms);
void	ms_unset_env_key(t_list **env, char *key);
void	ms_unset_remove_node(t_list **head, t_list *prev, t_list *current);
int		ms_unset_key_match(t_list *node, char *key);
int		ms_echo(char **cmd_args);
int		ms_echo_check_n_flag(char **args, int *i);
void	ms_echo_print_args(char **args, int i);
int		ms_export(t_ms *ms, char **cmd_args, char **env);
int		ms_export_ex(t_ms *ms, char *key, char *value);
int		ms_export_error(t_ms *ms, char *entry);
int		ms_export_check(const char *var);
int		ms_export_print(t_ms *ms, char **env);
void	ms_remove_from_export_only(t_ms *ms, char *key);
int		ms_is_export_only(t_ms *ms, char *key);
void	ms_print_env_var(t_ms *ms, char *env_var);
void	ms_print_export_only(t_list *export_var);
int		ms_key_exists(t_ms *ms, char *key);
char	*ms_build_export_output(t_ms *ms, char *content, char *sym);
int		ms_process_export_arg(t_ms *ms, char *arg);
int		ms_export_with_value(t_ms *ms, char *arg, char *sign);
int		ms_export_without_value(t_ms *ms, char *arg);
char	**ms_sort(char **array, int (*cmp)(const void *, const void *, size_t));
size_t	ft_min_strlen(const char *s1, const char *s2);

//GARBAGE COLLECTOR functions
void	gc_add(void *ptr, t_list **gc);
void	ms_print_list(t_list *list);
void	ms_print_toks(t_token *list);

#endif