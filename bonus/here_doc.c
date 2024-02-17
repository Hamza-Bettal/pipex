/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 21:53:45 by hbettal           #+#    #+#             */
/*   Updated: 2024/02/17 03:43:32 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}

void	ft_here_doc(int end[], int ac, char **av)
{
	char	*str;

	str = NULL;
	if (ac < 6)
		(fds_closer(end), exit(1));
	if (fork() == 0)
	{
		while (1)
		{
			printf("hiiiiiiii");
			str = get_next_line(0);
			if (ft_strcmp(str, av[2]) == 0)
			{
				(free(str), fds_closer(end), close(0));
				exit(1);
			}
			write(end[1], str, ft_strlen(str));
			free(str);
		}
	}
}
