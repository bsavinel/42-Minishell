/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_sequence_checker.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsavinel <bsavinel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 09:57:55 by rpottier          #+#    #+#             */
/*   Updated: 2022/05/04 14:27:25 by bsavinel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_SEQUENCE_CHECKER_H
# define PIPE_SEQUENCE_CHECKER_H

# include "minishell.h"

int	len_no_whitespace(char *commande);
int	pipe_sequence_checker(char *commande);

#endif