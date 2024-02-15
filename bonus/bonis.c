/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonis.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:40:58 by hbettal           #+#    #+#             */
/*   Updated: 2024/02/15 15:24:17 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// void	fds_closer(int end[])
// {
// 	close(end[0]);
// 	close(end[1]);
// }

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

// void	parent_command(int end[], char **av, char **env, int i)
// {
// 	char	*path;
// 	char	**commands;

// 	if (!av[i][0])
// 		(write(2, "wrong parameters", 17), fds_closer(end), exit(1));
// 	commands = ft_split(av[i], ' ');
// 	path = path_check(commands[0], env, end);
// 	if (!commands || !path)
// 		(fds_closer(end), exit(1));
// 	dup2(end[0], 0);
// 	fds_closer(end);
// 	// if (execve(path, commands, env) < 0)
// 	// 	exit(1);
// }

void	child_command(int end[], char **av, char **env, int i)
{
	char	*path;
	char	**commands;

	if (!av[i][0])
		(write(2, "wrong parameters", 17), fds_closer(end), exit(1));
	commands = ft_split(av[i], ' ');
	path = path_check(commands[0], env, end);
	if (!commands || !path)
		(fds_closer(end), exit(1));
	dup2(end[1], 1);
	fds_closer(end);
	execve(path, commands, env);
	exit(1);
}

int	main(int ac, char **av, char **env)
{
	pid_t	id;
	int		fd;
	int		fd2;
	int		end[2];
	int		i;

	fd = open(av[1], O_RDONLY);
	fd2 = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		exit(1);
	dup2(fd, 0);
	i = 2;
	while (i < ac - 1)
	{
		printf("cmd = %s\n", av[i]);
		if (i < ac - 2)
			pipe(end);
		else
			dup2(fd2, 1);
		id = fork();
		if (id == 0)
		{
			close(fd);
			close(fd2);
			child_command(end, av, env, i - 1);
		}
		else
		{
			close(fd);
			close(fd2);
		}
		dup2(end[0], 0);
		++i;
	}
	dup2(fd2, 1);
	(fds_closer(end), close(fd), close(fd2));
	while (wait(NULL) > 0)
		;
}
