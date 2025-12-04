/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dadivaldo <dadivaldo@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:45:13 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/04 11:08:09 by dadivaldo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_list(t_wordlist *list, const char *prefix, int is_last)
{
	printf("%s%sArgs:\n", prefix, is_last ? "└── " : "├── ");

	t_word *cur = list->list[HEAD];
	while (cur)
	{
		printf("%s         %s\n", prefix, cur->token);
		cur = cur->next;
	}
}

void print_ast(t_ast *node, const char *prefix, int is_last)
{
	if (!node)
		return;

	// Print this node's header
	printf("%s%s", prefix, is_last ? "└── " : "├── ");

	// Print type
	switch (node->type)
	{
	case Word:
		printf("Word");
		break;
	case WordList:
		printf("WordList");
		break;
	case Pipeline:
		printf("Pipeline");
		break;
	case RedirLeft:
		printf("RedirLeft");
		break;
	case RedirRight:
		printf("RedirRight");
		break;
	case RedirList:
		printf("RedirList");
		break;
	case Heredoc:
		printf("Heredoc");
		break;
	case Append:
		printf("Append");
		break;
	case Command:
		printf("Command");
		break;
	case OrCondition:
		printf("OrCondition");
		break;
	case AndCondition:
		printf("AndCondition");
		break;
	default:
		printf("Unknown");
		break;
	}

	// Token (if any)
	if (node->token)
		printf("  (%s)", node->token);

	printf("\n");

	// Build new prefix
	char new_prefix[256];
	snprintf(new_prefix, sizeof(new_prefix), "%s%s",
			 prefix, is_last ? "    " : "│   ");

	// Determine children
	int child_count = 0;
	if (node->word)
		child_count++;
	if (node->left)
		child_count++;
	if (node->right)
		child_count++;

	int idx = 0;

	// Print word list
	if (node->word)
		print_list(node->word, new_prefix, ++idx == child_count);

	// Print left
	if (node->left)
		print_ast(node->left, new_prefix, ++idx == child_count);

	// Print right
	if (node->right)
		print_ast(node->right, new_prefix, ++idx == child_count);
}

int main(int ac, char **av, char **envp)
{
//	char *line;
//	t_ast *root;

	(void)ac;

	t_envp	metadata = ft_initialize(envp);
	
	ft_putendl_fd(ft_getenv(metadata, av[1]), STDOUT_FILENO);
/*	while (true)
	{
		line = d_get_input();
		if (!ft_strcmp(line, "exit"))
			break;
		if (!line)
			continue;
		pid_t pid = fork();
		if (pid == 0)
		{
			ft_strtok(line, false, false);
			root = parse_expression(0);
			printf("\n");
			print_ast(root, "", 1);
			printf("\n");
			printf("Syntax: OK!\n");

			exit(0); // could be a return, and we must free the line var, also the root
		}
		else
			waitpid(pid, 0, 0);
		ft_strtok(line, false, false);
		root = parse_expression(0);
		if (root->word)
			root->args_token = ft_get_args(root->word->list[0], root->word->list_len);
		if (!strcmp(root->token, "echo"))
			echo_cmd(root->args_token);
		else if (!strcmp(root->token, "cd"))
			cd_cmd(root->args_token[0]);
		else if (!strcmp(root->token, "pwd"))
			pwd_cmd();
		ft_strtok(line, false, true);
	}
	free(line);*/
	ft_free_t_envp(&metadata);
	return (0);
}
