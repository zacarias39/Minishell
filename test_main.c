/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:45:13 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/12 12:03:28 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(t_wordlist *list, const char *prefix, int is_last)
{
	t_word	*cur;
	char	*str;

	if (list->list[HEAD]->type == Undefined)
		printf("%s%sArgs:\n", prefix, is_last ? "└── " : "├── ");
	else
		printf("%s%sList:\n", prefix, is_last ? "└── " : "├── ");
	cur = list->list[HEAD];
	while (cur)
	{
		if (cur->type == RedirLeft)
			str = " <";
		else if (cur->type == RedirRight)
			str = " >";
		else if (cur->type == Heredoc)
			str = " <<";
		else if (cur->type == Append)
			str = " >>";
		else
			str = "";
		printf("%s          %s %s\n", prefix, str, cur->token);
		cur = cur->next;
	}
}

void	print_ast(t_ast *node, const char *prefix, int is_last)
{
	char	new_prefix[256];
	int		child_count;
	int		idx;

	if (!node)
		return ;
	// Print this node's header
	printf("%s%s", prefix, is_last ? "└── " : "├── ");
	// Print type
	switch (node->type)
	{
	case Word:
		printf("Word");
		break ;
	case WordList:
		printf("WordList");
		break ;
	case Pipeline:
		printf("Pipeline");
		break ;
	case RedirLeft:
		printf("RedirLeft");
		break ;
	case RedirRight:
		printf("RedirRight");
		break ;
	case RedirList:
		printf("RedirList");
		break ;
	case Heredoc:
		printf("Heredoc");
		break ;
	case Append:
		printf("Append");
		break ;
	case Command:
		printf("Command");
		break ;
	case OrCondition:
		printf("OrCondition");
		break ;
	case AndCondition:
		printf("AndCondition");
		break ;
	default:
		printf("Unknown");
		break ;
	}
	// Token (if any)
	if (node->token)
		printf("  (%s)", node->token);
	printf("\n");
	// Build new prefix
	snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix,
		is_last ? "    " : "│   ");
	// Determine children
	child_count = 0;
	if (node->word)
		child_count++;
	if (node->left)
		child_count++;
	if (node->right)
		child_count++;
	idx = 0;
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

// envars_info is now a pointer because after updating it with new envars the changes weren't persisting
void	builtin_cmd(t_ast *root, t_envars *envars_info)
{
	ssize_t	i;

	root->args_token = matrix_from_list(&root->word);
	// putting root->word on a condition is pointless and can result in unwanted behaviour
	// because it will always be NULL, as it's being freed inside matrix_from_list function
	// that's way I removed it;
	if (!ft_strcmp(root->token, "echo"))
		echo_cmd(root->args_token);
	else if (!ft_strcmp(root->token, "export"))
		export_cmd(envars_info, root->args_token);
	else if (!ft_strcmp(root->token, "cd"))
		cd_cmd(root->args_token);
	else if (!ft_strcmp(root->token, "pwd"))
		pwd_cmd();
	else if (!ft_strcmp(root->token, "env"))
	{
		i = -1;
		while (++i < envars_info->n_items)
			printf("%s\n", envars_info->matrix[i]);
	}
	// Commenting this because, args_token pointers points to the strtok str
	// which can only be freed there and not individualy;
	//ft_free_matrix(&root->args_token);
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_ast		*root;
	t_envars	envars_info;
	pid_t		pid;

	(void)ac, (void)av, (void)envp;
	// initializing ft_getenv function with valid pointer to the envs
	ft_init_envars(envp, &envars_info);
	//TODO: THIS IS A TEST OF MINE, I ADDED THE ENVP ARRAY AGAIN IN OUR VARIABLE;
	//printf("Before Capacity: %zd\n", envars_info.n_items);
	//TODO: TESTING THE EXPORT ENVARS FUNCTION, IT SEEMS TO WORK REALLY WELL;
	//update_envars(&envars_info, envp);
	//printf("After Capacity: %zd\n", envars_info.n_items);*/
	while (true)
	{
		line = d_get_input();
		if (!ft_strcmp(line, "exit"))
			break ;
		if (!line)
			continue ;
		pid = fork();
		if (pid == 0)
		{
			ft_strtok(line, false, false);
			root = parse_expression(0);
			printf("\n");
			print_ast(root, "", 1);
			printf("\n");
			printf("Syntax: OK!\n");
			exit(0); // could be a return, and we must free the line var,also the root
						// No we don't need because we'll use this child process just to check for syntax errors;
		}
		else
			waitpid(pid, 0, 0);
		ft_strtok(line, false, false);
		root = parse_expression(0);
		printf("\nExecution:\n");
		builtin_cmd(root, &envars_info);
		ft_strtok(NULL, false, true);
	}
	free(line);
	ft_free_matrix(&envars_info.matrix);
	return (0);
}
