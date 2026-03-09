/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_sig.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 14:51:46 by zcasimir          #+#    #+#             */
/*   Updated: 2026/03/04 16:41:20 by zcasimir        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	handle_parent_sigint2(int sig)
{
	(void)sig;
	if (sig == SIGINT)
		ft_putchar_fd('\n', STDOUT_FILENO);
	else if (sig == SIGQUIT)
		printf("Quit (core dumped)\n");
	else if (sig == SIGTSTP)
		printf("Stopped execution: cannot continue after the signal!\n");
	else if (sig == SIGKILL)
		printf("Killed\n");
	else if (sig == SIGTERM)
		printf("Terminated\n");
	else if (sig == SIGABRT)
		printf("Aborted (core dumped)\n");
	else if (sig == SIGILL)
		printf("Illegal Instruction (core dumped)\n");
	else if (sig == SIGFPE)
		printf("Floating point exception (core dumped)\n");
	else if (sig == SIGSEGV)
		printf("Segmetation fault (core dumped)\n");
}

void	handle_busy(int sig)
{
	if (sig == SIGTSTP)
		printf("Stopped execution: cannot continue after the signal!\n");
}

void	handle_parent_sigint(int sig)
{
	(void)sig;
	last_cmd_status(SIGINT + 128, UPDATE_DATA);
	if (g_sig_re != SIGINT)
		ft_putchar_fd('\n', STDIN_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_heredoc(int sig)
{
	(void)sig;
	g_sig_re = SIGINT;
	on_error(NULL, RAISE);
	ft_putchar_fd('\n', STDIN_FILENO);
	last_cmd_status(SIGINT + 128, UPDATE_DATA);
	close(STDIN_FILENO);
}

void	parent_signal(int op)
{
	if (op == IDLE)
	{
		if (g_sig_re == SIGINT)
			set_default_std_fd(NO, STDIN_FILENO);
		set_default_std_fd(NO, STDERR_FILENO);
		signal(SIGINT, handle_parent_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (op == BUSY)
	{
		set_default_std_fd(NO, STDOUT_FILENO);
		signal(SIGINT, handle_busy);
		signal(SIGQUIT, handle_busy);
		signal(SIGINT, handle_busy);
		signal(SIGQUIT, handle_busy);
		signal(SIGTERM, handle_busy);
		signal(SIGABRT, handle_busy);
		signal(SIGSEGV, handle_busy);
		signal(SIGFPE, handle_busy);
		signal(SIGILL, handle_busy);
		signal(SIGTSTP, handle_busy);
	}
}
