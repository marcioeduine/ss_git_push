#include "../include/ss_git_push.hpp"

static void	ss_git_push(const t_vector &markers, bool flag)
{
	char			tmp[] = "/tmp/ss_commit_XXXXXX";
	int				fd(mkstemp(tmp));
	std::ofstream	os(tmp);
	t_vector		staged_files((system("git add *"), get_staged_files()));
	t_text			message;

	if (staged_files.empty())
		throw (std::runtime_error("Nothing to commit!"));
	message = build_commit_message(staged_files, markers);
	if (fd == -1)
		throw (std::runtime_error("Creating the tmp file!"));
	close(fd);
	if (not os.is_open())
		throw ((remove(tmp), std::runtime_error("Opening tmp file!")));
	(os << "UPDATED FILE:\n" << message, os.close());
	system((t_text("git commit -F ") + tmp).c_str());
	if (flag)
		remove_commit_lines(staged_files, markers);
	(remove(tmp), system("git push"));
}

static int	init(int ac, char **av)
{
	t_vector	markers;
	bool		flag;

	try
	{
		if (ac > 2)
			throw (std::invalid_argument(ERROR_MANY_ARGS));
		flag = (ac == 2 and t_text(av[1]) == "-rm");
		if (ac == 2 and not flag)
			throw (std::invalid_argument(ERROR_INVALID_ARG));
		markers.push_back("// SS_COMMIT:");
		markers.push_back("#// SS_COMMIT:");
		ss_git_push(markers, flag);
	}
	catch (const std::exception &e)
	{
		return (std::cerr << "[ ERROR ]: " << e.what() << std::endl, 1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	return (init(ac, av));
}
