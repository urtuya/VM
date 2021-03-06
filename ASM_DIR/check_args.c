/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydavis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 02:28:03 by ydavis            #+#    #+#             */
/*   Updated: 2020/03/01 21:33:29 by ydavis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	check_direct(t_core *core, t_token *token, char *string, int arg)
{
	token->args[arg].size = (token->op.tdir ? 2 : 4);
	if (!string[1])
		parse_error(core, "Error with direct argument", token->str);
	if (string[1] == LABEL_CHAR)
		token->args[arg].type = 4;
	else
		token->args[arg].type = 1;
}

void	check_register(t_token *token, int arg)
{
	token->args[arg].size = 1;
	token->args[arg].type = 2;
}

void	check_indirect(t_token *token, char *string, int arg)
{
	token->args[arg].size = 2;
	if (string[0] == LABEL_CHAR)
		token->args[arg].type = 5;
	else
		token->args[arg].type = 3;
}

void	check_args(t_core *core, char **split, t_token *token, int arg)
{
	char	*string;
	int		*exp;

	if (arg == 0)
		exp = token->op.first;
	else if (arg == 1)
		exp = token->op.second;
	else
		exp = token->op.third;
	token->args[arg].str = NULL;
	check_malloc(string = ft_strtrim(split[arg]));
	if (string[0] == '\0')
		parse_error(core, "Error with args", token->str);
	if (exp[0] && string[0] == DIRECT_CHAR)
		check_direct(core, token, string, arg);
	else if (exp[2] && string[0] == 'r')
		check_register(token, arg);
	else if (exp[1])
		check_indirect(token, string, arg);
	else
		parse_error(core, "Error with args", token->str);
	token->size += token->args[arg].size;
	check_malloc(token->args[arg].str = ft_strdup(string));
	free(string);
}
