/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:45:13 by dadmendo          #+#    #+#             */
/*   Updated: 2025/12/03 01:04:08 by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_list(t_wordlist *list, const char *prefix, int is_last)
{
    if (list->list[HEAD]->type == Undefined)
        printf("%s%sArgs:\n", prefix, is_last ? "└── " : "├── ");
    else
        printf("%s%sList:\n", prefix, is_last ? "└── " : "├── ");

    t_word *cur = list->list[HEAD];
    char   *str;
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

void print_ast(t_ast *node, const char *prefix, int is_last)
{
    if (!node)
        return;

    // Print this node's header
    printf("%s%s", prefix, is_last ? "└── " : "├── ");

    // Print type
    switch (node->type) {
        case Word:         printf("Word"); break;
        case WordList:     printf("WordList"); break;
        case Pipeline:     printf("Pipeline"); break;
        case RedirLeft:    printf("RedirLeft"); break;
        case RedirRight:   printf("RedirRight"); break;
        case RedirList:    printf("RedirList"); break;
        case Heredoc:      printf("Heredoc"); break;
        case Append:       printf("Append"); break;
        case Command:      printf("Command"); break;
        case OrCondition:  printf("OrCondition"); break;
        case AndCondition: printf("AndCondition"); break;
        default:           printf("Unknown"); break;
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
    if (node->word) child_count++;
    if (node->left) child_count++;
    if (node->right) child_count++;

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
	char *line;
	t_ast *root;

	(void)ac, (void)av, (void)envp;
    // initializing ft_getenv function with valid pointer to the envs
	ft_getenv(envp, NULL);
	while (true)
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
		if (!ft_strcmp(root->token, "echo"))
			echo_cmd(root->args_token);
		else if (!ft_strcmp(root->token, "cd"))
			cd_cmd(root->args_token[0]);
		else if (!ft_strcmp(root->token, "pwd"))
			pwd_cmd();
		ft_strtok(line, false, true);
	}
	free(line);
	return (0);
}
