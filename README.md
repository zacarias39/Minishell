env - Print out a list of all environment variables.

export - Print out a list of all environment variables, ascending order

I left a lot of comments for you my friend, on utils/ and redirection/word function;


Update:

# By Dadivaldo

cd_cmd
A função **arrlen**, não é necessária, pois só precisamos saber se passa 1, e não ter que percorrer todos os argumentos


export_cmd && env_cmd
Infelizmente, temos que sempre ordenar, ou ter dois arrays para unsorted(env) and sorted(export)


pwd: Teste: mkdir j && cd j; rm -fr ../f; pwd
	dir_path = getcwd(NULL, USHRT_MAX); // é perigoso


test_main.c:
	Precisamos liberar o que foi mallocado, no processo filho


# Bullet point 12/30/2025

- Add 'cmd_name' inside the 'word' function;
- Removal of quotes from the strings;
- String Expansion;
- Execute cmd(Pseudocode)


Here_doc
 -> << eof;
Get all the data and put it inside a file called '.eof', and return the file name;
