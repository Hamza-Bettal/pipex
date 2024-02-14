/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:40:58 by hbettal           #+#    #+#             */
/*   Updated: 2024/02/14 17:24:43 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*path_check(char *command, char **env, int end[])
{
	int		i;
	char	*cmnd;
	char	*path;
	char	**paths;

	if (access(command, F_OK) != -1)
		return (command);
	i = 0;
	paths = ft_split(env[4] + 5, ':');
	if (!paths)
		exit(1);
	cmnd = ft_strjoin("/", command);
	while (paths[i])
	{
		path = ft_strjoin(paths[i], cmnd);
		if (access(path, F_OK) != -1)
			return (free_handler(paths), free(cmnd), path);
		free(path);
		i++;
	}
	free_handler(paths);
	free(cmnd);
	fds_closer(end);
	write(2, "command not found", 18);
	return (NULL);
}

void	command1(int end[], char **av, char **env)
{
	int		fd;
	char	*path;
	char	**commands;

	if (!av[1][0] || !av[2][0])
	{
		write(2, "wrong parameters", 17);
		(fds_closer(end), exit(1));
	}
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		exit(1);
	commands = ft_split(av[2], ' ');
	path = path_check(commands[0], env, end);
	if (!commands || !path)
		(close(fd), exit(1));
	dup2(fd, 0);
	close(fd);
	dup2(end[1], 1);
	fds_closer(end);
	if (execve(path, commands, env) < 0)
		exit(1);
}

int	main(int ac, char **av, char **env)
{
	int	end[2];

	if ()
}


O1zxcvbnm,./