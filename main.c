/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcasimir <zcasimir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:55:39 by zcasimir          #+#    #+#             */
/*   Updated: 2025/12/06 06:45:27by zcasimir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char *str = "5 2 1 8 4 3 7 11";
int sum = 12;
int value = 0;
char se[100];

int is_in(char *str, char c)
{
    int i = 0;
    
    while (str[i])
    {
        if (c == str[i] && str[i + 1] != c)
            return 1;
        i++;
    }
    return 0;
}

void    backtrack(char *s)
{
    int len = strlen(str);
    if (value == sum)
    {
        printf("%s\n", s);
        return ;
    }
    for (int i = 0; i < len; i++)
    {
        int n = strlen(s);
        if (!isspace(str[i]) && strstr(s, "11"))
            continue ;
        else if (is_in(s, str[i]) || isspace(str[i]))
            continue ;
        s[n] = str[i];
        value += atoi(&str[i]);
        backtrack(s);
        s[n] = '\0';
        value -= atoi(&str[i]);
    }
}

int main() {
    // Write C code here
    char set[199];
    for (int i = 0; i < 199; i++)
        set[i] = '\0';
    backtrack(set);
    return 0;
}
