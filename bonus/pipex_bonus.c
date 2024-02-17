/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:40:58 by hbettal           #+#    #+#             */
/*   Updated: 2024/02/17 03:49:53 by hbettal          ###   ########.fr       */
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
	paths = ft_split(where_path(env), ':');
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

void	middle_commands(int end[], char **av, char **env, int i)
{
	char	*path;
	char	**commands;

	if (fork() == 0)
	{
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
}

void	last_cmd(int end[], char **av, char **env, int ac)
{
	int		fd;
	char	*path;
	char	**commands;

	if (fork() == 0)
	{
		fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd < 0)
			exit(1);
		dup2(fd, 1);
		close(fd);
		if (!av[ac - 2][0])
			(write(2, "wrong parameters", 17), fds_closer(end), exit(1));
		commands = ft_split(av[ac - 2], ' ');
		path = path_check(commands[0], env, end);
		if (!commands || !path)
			exit(1);
		execve(path, commands, env);
		exit(1);
	}
}

void	first_cmd(int end[], char **av, char **env)
{
	int		fd;
	char	*path;
	char	**commands;

	if (fork() == 0)
	{
		fd = open(av[1], O_RDONLY);
		if (fd < 0)
			exit(1);
		if (dup2(fd, 0) < 0)
			exit(1);
		close(fd);
		if (!av[2][0])
			(write(2, "wrong parameters", 17), fds_closer(end), exit(1));
		commands = ft_split(av[2], ' ');
		path = path_check(commands[0], env, end);
		if (!commands || !path)
			(fds_closer(end), exit(1));
		dup2(end[1], 1);
		fds_closer(end);
		execve(path, commands, env);
		exit(1);
	}
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	pipex.i = 2;
	if (ac < 5 || pipe(pipex.end))
		return (write(2, "missing parameters", 17), 1);
	if (!ft_strcmp(av[1], "here_doc"))
		(ft_here_doc(pipex.end, ac, av), close(pipex.end[1]), pipex.i++);
	else
		(first_cmd(pipex.end, av, env), close(pipex.end[1]));
	pipex.input = pipex.end[0];
	while (pipex.i++ < ac - 3)
	{
		if (dup2(pipex.input, 0) < 0)
			return (write (2, "dup2 failed", 12), 1);
		close(pipex.input);
		if (pipe(pipex.end) < 0)
			return (write(2, "pipe failed", 10), 1);
		(middle_commands(pipex.end, av, env, pipex.i), close(pipex.end[1]));
		pipex.input = pipex.end[0];
	}
	if (dup2(pipex.input, 0) < 0)
		return (write (2, "dup2 failed", 12), 1);
	(close(pipex.input), last_cmd(pipex.end, av, env, ac), close(0));
	while (wait(NULL) != -1)
		;
}
