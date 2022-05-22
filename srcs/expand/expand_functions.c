/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 12:39:00 by rpottier          #+#    #+#             */
/*   Updated: 2022/05/22 15:57:11 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void	expand_command(t_lst_token *token, t_lst_env *env_list)
{
	t_split	*split;

	split = NULL;
	while (token)
	{
		if (is_heredoc_token(token->type))
			token = skip_two_token(token);
		if (token && token->type == TOK_WORD)
		{
			token = expand_token(token, env_list);
			split = split_post_expand(token);
			insert_split_in_token_list(token, split);
			token = move_to_last_new_token(token, split->size_2d_array);
		}
		if (token)
			token = token->next;
	}
}


t_lst_token	*expand_token(t_lst_token *token, t_lst_env *env_list)
{
	if (ft_strlen(token->str) == 0)
		return (token);
	token = expand_all_variables(token, env_list);

	// printf("[%s]\n", token->str);
	// print_lstquote(token->lst_quote);
	token->str = remove_quotes(token, token->lst_quote);
	return (token);
}


t_lst_token	*expand_all_variables(t_lst_token *token, t_lst_env *env_list)
{
	int				i;
	t_quote_index	quote;
	t_lst_quote		*elem;
int tmp = -2;
//	int max = 15;
	i = 0;
	while (i != -1 && token && token->str[i] /*&& max*/)
	{
	tmp = -2;
		quote.open = -1;
		quote.close = - 1;
//		 printf("i = %d\n", i);
		// printf("%s\n", token->str);
		//max--;
		if (i != -1 && token->str[i] && is_quote(token->str[i]))
		{
			quote.open = i;
			quote.close = expand_in_quotes(&token->str, &i, env_list);
			elem = create_quote_index(quote.open, quote.close);
			ft_lstquote_add_back(&token->lst_quote, elem);
		}
		if (i != -1 && token->str[i] && is_dollar(token->str[i]))
		{
		tmp = i;
			token->str = expand_variable(token->str, &i, env_list);
			if (i != -1 && token->str[i] && is_dollar(token->str[i]) && !token->str[i + 1])
				tmp = -2;
			else if (i != -1 && token->str[i] && is_dollar(token->str[i]) && token->str[i + 1])
				i++;
 		// printf("\n\n%s\n", token->str);
		// printf("i -> %d\n", i);
		// printf("tmp -> %d\n", tmp);
		// printf("quote.open = %d \n quote.close =%d\n\n", quote.open, quote.close);
	
		}

		if (i != -1 && token && token->str && token->str[i] && !is_quote(token->str[i]) && tmp != i)
		{
			move_foward_expanding_var(quote.open, token->str, &i);
		}
		else if (i != -1 && token && token->str && token->str[i]
			&& ((quote.open != -1 && quote.close >= i) || is_wildcard(token->str[i])))
			i++;

	}
	//  if (max == 0)
	//  	exit(1);
	return (token);
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
	if (!variable_name)
	{
		var_content = ft_strdup("$");
		var_content_len = 1;
	}
	else
	{
		var_content = get_var_to_expand_content(variable_name, env_list);
		if (!var_content)
		{
			var_content = __ft_calloc(sizeof(char) * 1);
//			var_content_len = 1;
		}
		else
			var_content_len = ft_strlen(var_content);
	}

	insert_var_content_to_token(&token, var_content, *index);

	if((*index + (var_content_len - 1)) >= 0)
		*index = *index + (var_content_len - 1);
//	else
//		*index = *index + (var_content_len);
//	printf("index = [%d]\n", *index);
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
