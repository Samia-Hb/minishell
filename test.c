# include "minishell.h"

int ft_is_separator(char c)
{
	if(c == '>' || c == '<' || c == '\n' || c == '\t' ||c == '|' || c == ' ' || c == '\t')
	    return 1;
	return (0);
}

//  char *handle_quote(char *str, char c)
//  {
//  	int		i;
//      char	*word;

//  	i = 1;
//  	if (strchr(str + 1, c))
//  	{
//  		while (str[i] && str[i] != c)
//  			i++;
//  		while (!ft_is_separator(str[i]))
//  			i++;
//  		word = strndup(str, i + 1);
//  	}
//  	else
//  	{
//  		while (!ft_is_separator(str[i]))
//  			i++;
//  		word = strndup(str, i + 1);
//  	}
//      return word;
//  }

char *handle_quote(char *str)
{
    int		i;
    char    c;
    int		j;
    char	*word;

    i = 0;
    j = 0;
    word = malloc(strlen(str) + 1);
    c = str[i];
    while (str[i] && !ft_is_separator(str[i]))
    {
        if (str[i] == c)
        {
            word[j++] = str[i++];
            while (str[i] && str[i] != c)
                word[j++] = str[i++];
            if (str[i] == c)
                word[j++] = str[i++];
        }
        else
            word[j++] = str[i++];
    }
    word[j] = '\0';
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
//  ""e"'c'ho 'b'"o"nj"o"'u'r"

