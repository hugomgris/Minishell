/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:07:08 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/25 17:51:16 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"
#include "../libs/libft/ft_printf/includes/ft_printf.h"

#define TRUE 1
#define FALSE 0

//MAIN and LOOP functions
void	ms_main_loop(void);
char	*ms_check_empty_input(char *input);
void	ms_exit_handler(const char *msg);

//ERROR HANDLER functions
void	ms_error_handler(char *msg);