/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 16:07:02 by rpottier          #+#    #+#             */
/*   Updated: 2022/04/20 06:09:20 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENISATION
# define TOKENISATION

# include "minishell.h"

char	**ft_split_pipe_seq(char *s, char *sep);

int		is_separator(char c, char *separator);

char	*insert_token_separator(char *str, char *sep);
char	*insert_token_redirect_in(void);
char	*insert_token_redirect_out(void);
char	*insert_token_pipe(void);

#endif