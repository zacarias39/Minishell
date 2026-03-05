/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:49:01 by zcasimir          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:22 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "ast.h"
# include "extern_libs.h"

# define IDLE 1
# define BUSY 2

void	parent_signal(int op);
void	handle_parent_sigint(int sig);
void	handle_heredoc(int sig);
void	handle_busy(int sig);
void	handle_parent_sigint2(int sig);

#endif
