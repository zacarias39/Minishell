env - Print out a list of all environment variables.

export - Print out a list of all environment variables, ascending order


typedef struct s_ast
{
	t_token_type	type;
	t_wordlist		*word; // Why, is a pointer,
	char			*token;
	char			**args_token;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

// Na struct t_ast, queres adicionar um campo(ponteiro), para as env vars?


t_ast	*create_node(char *token, t_token_type type) // We must set the args_token as NULL, by default.