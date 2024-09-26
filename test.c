# include "minishell.h"

int ft_is_separator(char c)
{
	if(c == '>' || c == '<' || c == '\n' || c == '\t' ||c == '|' || c == ' ' || c == '\t')
	    return 1;
	return (0);
}

// char *handle_quote(char *str, char c)
// {
// 	int		i;
//     char	*word;

// 	i = 1;
// 	if (strchr(str + 1, c))
// 	{
// 		while (str[i] && str[i] != c)
// 			i++;
// 		while (!ft_is_separator(str[i]))
// 			i++;
// 		word = strndup(str, i + 1);
// 	}
// 	else
// 	{
// 		while (!ft_is_separator(str[i]))
// 			i++;
// 		word = strndup(str, i + 1);
// 	}
//     return word;
// }

char *handle_quote(char *str)
{
    int		i;
	int     flag ;
    char	*word;

    i = 0;
	flag = 1;
    while (flag)
    {
        if (str[i] == '"')
        {
			i++;
            while (str[i] && str[i] != '"')
                i++;
            if (str[i] == '"')
                i++;
        }
        else if (str[i] == '\'')
        {
			i++;
            while (str[i] && str[i] != '\'')
                i++;
            if (str[i] == '\'')
                i++; 
        }
        else
        {
			while (str[i])
			{
				if (!ft_is_separator(str[i]))
					i++;
                else
                {
					flag = 0;
                    break;
                }
            }
        }
    }
    word = strndup(str, i);
    return word;
}
int main()
{
	char *str;
	while (1)
	{
		str = readline("=> ");
		printf("result = %s\n" , handle_quote(str));
	}
}
// ""e"'c'ho 'b'"o"nj"o"'u'r"
