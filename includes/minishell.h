/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/02 18:29:37 by hmunoz-g         ###   ########.fr       */
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
	t_list	*input;
	char	**cmd_table;
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
	T_OR,
	T_NL
}	t_token_type;

//MAIN and LOOP functions
void	ms_main_loop(t_ms *ms);
char	*ms_check_empty_input(t_ms *ms, char *input);
char	*ms_build_prompt(t_ms *ms);

//TOKENIZER
void	ms_tokenizer(t_ms *ms, char *str);
int		ms_count_tokens(char *str);
void	ms_allocate_tokens(t_ms *ms, char *str);
void	ms_get_tokens(t_ms *ms, char *str);
void	ms_trim_tokens(t_ms *ms, char	**array);

//SYNTAX CHECK
int		ms_syntax_checker(t_ms *ms, char *str);
int		ms_checkquotes(char *str, char c);
int		ms_checkspecialchar(char *str);
int		ms_checkpipes(t_ms *ms, char *str);
int		ms_check_empty_pipe(t_ms *ms, char *str);
char	*ms_expand_variable(t_ms *ms, char *str);
int		ms_checkredirections(char *str);
char	*ms_replace_expanded(char *str, char *key, char *var);
char	*ms_replace_null_value(char *str, char *key);
char	*ms_search_env(t_ms *ms, char *str, int start);

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
char	*ms_get_prompt_user(t_ms *ms);
char	*ms_username_from_psswd(t_list **gc);
char	*ms_get_env_variable(t_list *ms_env, const char *var_name);
char	*ms_get_cwd(t_ms *ms);
char	*ms_get_hostname(char *session_manager, t_list **gc);

//GARBAGE COLLECTOR functions
void	gc_add(void *ptr, t_list **gc);
void	ms_print_gc(t_list *gc, int index);

#endif