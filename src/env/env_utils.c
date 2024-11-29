/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/29 14:30:28 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Instance maker for the ms_env copy of env. 
Returns a pointer to the static container of ms_env.
!Must discuss this implementation!
*/
t_list	**ms_env_instance(void)
{
	static t_list	*ms_env = NULL;

	return (&ms_env);
}

/*
Helper function that looks for an entry in the ms_env copy of env.
It can be paired with ms_add_env_variable to create an env entry if not found.
	Something wich is helpful in unset env case, as the ms_env
	can be built during execution (adding a USER=, HOME=, etc).
*/
char	*ms_get_env_variable(t_list *ms_env, const char *var_name)
{
	t_list	*current;
	char	*value;

	current = ms_env;
	while (current)
	{
		value = current->content;
		if (ft_strncmp(value, var_name, ft_strlen(var_name)) == 0)
			return (value + ft_strlen(var_name));
		current = current->next;
	}
	return (NULL);
}

/*
Helper function that adds an env variable to the ms_env copy of env.
Protection from double entries is handled by calling ms_get_env_variable first,
	with entry key as string to search:
		 e.g.: ms_get_env(ms_env, "HOME=").
If no entry is found, we should create it and add it.
		For an example of this case, check ms_get_cwd @ prompt_utils.c.
*/
void	ms_add_env_variable(const char *env_var)
{
	t_list	*new_node;
	t_list	**ms_env;

	ms_env = ms_env_instance();
	new_node = ft_lstnew(ft_strdup(env_var));
	if (!new_node)
		ms_error_handler("Error: env copy failed during node creation", 0);
	ft_lstadd_back(ms_env, new_node);
}

/*
Helper function that creates an entry for "HOME=" if env is unset.
Maybe this could be a general entry maker in the future that
	accepts any entry key, but right now it's specific for home.
*/
char	*ms_make_home(char *cwd)
{
	char	*aux;
	char	*home;
	int		tmp;

	aux = cwd;
	tmp = 3;
	while (tmp--)
	{
		aux = ft_strchr(aux, '/');
		aux++;
	}
	aux--;
	home = ft_substr(cwd, 0, aux - cwd);
	gc_add(home);
	return (home);
}

/*
Helper function that substitutes the home part of a dir with '~'.
There's no explicit need for this, but bash does it, so 🤷
*/
char	*ms_short_dir(char *cwd)
{
	int		aux;

	aux = 3;
	while (aux--)
		cwd = ft_strchr(cwd, '/') + 1;
	cwd--;
	cwd = ft_strjoin("~", cwd);
	gc_add(cwd);
	return (cwd);
}
