#include "minishell.h"

void	test_print_token_lst(t_token_lst token) {
	t_token_node	*node = token.nodes[HEAD];
	

	char *token_type;
	while (node) {
		switch (node->type)
		{
		case Name:
			token_type = "Name";
			break;
		case Bcmd:
			token_type = "Bcmd";
			break;
		case Operator:
			token_type = "Operator";
			break;
		case Word:
			token_type = "Word";
			break;
		default:
			token_type = "Undefined";
			break;
		}
		printf("Token type %s\n", token_type);
		printf("Token value: %s\n", (char *)node->token);
		printf("--------------------------------------\n");
		node = node->next;
	}
}
