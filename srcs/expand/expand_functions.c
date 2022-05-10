/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 12:39:00 by rpottier          #+#    #+#             */
/*   Updated: 2022/05/11 01:24:07 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_command(t_lst_token *token, t_lst_env *env_list)
{
	char	**split;
	int		size_2d_array;
	int		i;

	while (token && token->type != TOK_PIPE)
	{
		if (is_heredoc_token(token->type))
			token = skip_two_token(token);
		if (token && token->type == TOK_WORD)
		{
			if (ft_strlen(token->str) > 0)
				token = expand_token(token, env_list);
			split = split_post_expand(token);
			size_2d_array = ft_size_2d_array(split);
			if (size_2d_array > 1)
				insert_split_in_token_list(token, split);
			i = -1;
			while (token && ++i < (size_2d_array - 1))
				token = token->next;
		}
		if (token)
			token = token->next;
	}
}

char	*expand_variable(char *token, int *index, t_lst_env *env_list)
{
	char	*var_content;
	char	*variable_name;
	int		var_content_len;

	var_content = NULL;
	variable_name = NULL;
	var_content_len = 0;
	variable_name = get_variable_to_expand_name(&token[*index + 1]);
	var_content = get_var_to_expand_content(variable_name, env_list);
	if (!var_content)
	{
		var_content = __ft_calloc(sizeof(char) * 1);
		var_content_len = 0;
	}
	else
		var_content_len = ft_strlen(var_content);
	insert_var_content_to_token(&token, var_content, *index);
	*index = *index + (var_content_len - 1);
	return (token);
}

int	expand_in_quotes(char **token, int *i, t_lst_env *env_list)
{
	int	closing_quote_position;

	if (is_simple_quote((*token)[*i]))
	{
		closing_quote_position = skip_quote(*token, i);
		closing_quote_position--;
	}
	else
	{
		*i = *i + 1;
		while ((*token)[*i] && !is_double_quote((*token)[*i]))
		{
			if (is_dollar((*token)[*i]))
				*token = expand_variable(*token, i, env_list);
			*i = *i + 1;
		}
		closing_quote_position = *i;
	}
	return (closing_quote_position);
}

t_lst_token	*expand_token(t_lst_token *token, t_lst_env *env_list)
{
	int				i;
	t_quote_index	quote;
	t_lst_quote		*lst_quote;
	t_lst_quote		*elem;

	lst_quote = NULL;
	i = 0;
	while (i != -1 && token && token->str[i])
	{
		quote.open = -1;
		if (token->str[i] && is_quote(token->str[i]))
		{
			quote.open = i;
			quote.close = expand_in_quotes(&token->str, &i, env_list);
			elem = create_quote_index(quote.open, quote.close);
			ft_lstquote_add_back(&lst_quote, elem);
		}
		if (token->str[i] && is_dollar(token->str[i]))
			token->str = expand_variable(token->str, &i, env_list);
		if (i != -1 && token && token->str && token->str[i])
			move_foward(quote.open, token->str, &i);
	}
	token->str = remove_quotes(token, lst_quote);
	return (token);
}

void	move_foward(int open_quote, char *token_str, int *index)
{
	if (open_quote != -1)
	{
		if (!is_simple_quote(token_str[open_quote]))
			(*index)++;
	}
	else
		(*index)++;
}