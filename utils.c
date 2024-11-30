/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:34:04 by shebaz            #+#    #+#             */
/*   Updated: 2024/11/30 14:34:33 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void print_env(t_envi *envi)
{
	while(envi)
	{		
		printf("name = %s\n",envi->name);
		printf("value = %s\n",envi->vale);
		envi = envi->next;
	}
}

