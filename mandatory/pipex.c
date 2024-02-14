/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:02:07 by hbettal           #+#    #+#             */
/*   Updated: 2024/02/14 18:20:07 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fds_closer(int end[])
{
	close(end[0]);
	close(end[1]);
}

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

void	command2(int end[], char **av, char **env)
{
	int		fd;
	char	*path;
	char	**commands;

	// if (!av[4][0] || !av[3][0])
	// {
	// 	write(2, "wrong parameters", 17);
	// 	(fds_closer(end), exit(1));
	// }
	fd = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		exit(1);
	commands = ft_split(av[3], ' ');
	path = path_check(commands[0], env, end);
	if (!commands || !path)
		(close(fd), exit(1));
	(dup2(fd, 1), close(fd));
	(dup2(end[0], 0), fds_closer(end));
	if (execve(path, commands, env) < 0)
		exit(1);
}

void	command1(int end[], char **av, char **env)
{
	int		fd;
	char	*path;
	char	**commands;

	// if (!av[1][0] || !av[2][0])
	// {
	// 	write(2, "wrong parameters", 17);
	// 	(fds_closer(end), exit(1));
	// }
	if (access(av[1], F_OK) < 0)
		write(2, "no such file or directory : file1", 34);
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
	pid_t	id;
	int		end[2];

	if (ac != 5 || pipe(end) < 0)
		return (write(2, "expected : ./pipex infile cmd1 cmd2 outfile", 44), 1);
	id = fork();
	if (id < 0)
		return (1);
	if (id == 0)
		command1(end, av, env);
	id = fork();
	if (id < 0)
		return (1);
	if (id == 0)
		command2(end, av, env);
	fds_closer(end);
	while (wait(NULL) > 0)
		;
}
