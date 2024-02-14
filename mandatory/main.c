
#include "pipex.h"

// int main(int ac, char **av, char **env)
// {
//     char **command = malloc(sizeof (char *) * 3);
//     command[0] = "ls";
//     command[1] = "-l";
//     command[2] = NULL;
//     execve("/bin/ls", av, env);
// }
// int	main(int ac, char **av, char **env)
// {
// 	int	fds[2];
// 	int	fd1 = open("main.c", O_RDONLY, 0777);
// 	int	fd2 = open("tesst.txt", O_WRONLY | O_CREAT, 0777);
// 	pipe(fds);
//     char **command = malloc(sizeof (char *) * 3);
//     command[0] = "ls";
//     command[1] = "-l";
//     command[2] = NULL;
// 	int id = fork();
// 	if (id == 0)
// 	{
// 		dup2(fd1, 0);
// 		close(fd1);
// 		close(fd2);
// 		dup2(fds[1], 1);
// 		close(fds[0]);
// 		close(fds[1]);
// 		execve("/bin/ls",command, env);
// 	}
// 	else
// 	{
// 		dup2(fd2, 1);
// 		close(fd1);
// 		close(fd2);
// 		dup2(fds[0], 0);
// 		close(fds[0]);
// 		close(fds[1]);
// 		execlp("cat", "cat", NULL);
// 	}
// 	while (wait(NULL) != -1)
// 		;
// }
int main(int ac, char **av)
{
	write(1, av[1], 1);
}