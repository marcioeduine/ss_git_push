/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::   ::::::::  */
/*    main.cpp                                         :+:    :+: :+:    :+:  */
/*                                                    +:+        +:+          */
/*    By: Ser Superior <marcioeduine@gmail.com>      +#++:++#++ +#++:++#++    */
/*                                                         +#+        +#+     */
/*    Created: 2026/09/29 10:49:10 by Ser Superior #+#    #+# #+#    #+#      */
/*    Updated: 2026/09/29 10:49:14 by Ser Superior ########   ########        */
/*                                                                            */
/* ************************************************************************** */
#include "../include/ss_git_push.hpp"

// Writes text to a fresh temporary file and returns its path.
static t_text	write_temp_message(const t_text &body)
{
	char			tmp[] = "/tmp/ss_commit_XXXXXX";
	int				fd(mkstemp(tmp));
	std::ofstream	os;

	if (fd == -1)
		throw (std::runtime_error("Creating the tmp file!"));
	close(fd);
	os.open(tmp);
	if (not os.is_open())
		throw ((remove(tmp), std::runtime_error("Opening tmp file!")));
	return (os << body, os.close(), tmp);
}

// Prints the generated message with its branch, then commits it.
static void	commit_message(const t_text &body, const t_text &branch)
{
	t_text	tmp(write_temp_message(body));

	std::cout << "Branch: " << branch << "\n" << body << std::endl;
	run_command(t_text("git commit -F ") + tmp, "Creating commit");
	remove(tmp.c_str());
}

// Simulation mode: shows what would be committed without touching
// the index, the history or the remote.
static void	preview(const t_vector &markers)
{
	t_status_list	entries(get_working_entries());

	if (entries.empty())
		throw (std::runtime_error("Nothing to commit!"));
	std::cout << "Branch: " << get_current_branch() << "\n"
		<< "[DRY RUN] No changes were staged, committed or pushed.\n"
		<< build_commit_message(entries, markers) << std::endl;
}

static void	ss_git_push(const t_vector &markers, const t_options &opts)
{
	t_vector		files;
	t_status_list	entries;
	t_text			message;
	t_text			branch;

	if (opts.preview)
		return (preview(markers));
	run_command("git add -A", "Staging changes");
	files = get_staged_files();
	if (files.empty())
		throw (std::runtime_error("Nothing to commit!"));
	branch = get_current_branch();
	{
		for (size_t	i(0); i < files.size(); ++i)
			entries.push_back(std::make_pair(staged_file_status(files[i]),
				files[i]));
	}
	message = build_commit_message(entries, markers);
	commit_message(message, branch);
	if (opts.no_push)
		std::cout << "Skipped push (--no-push)." << std::endl;
	else
		run_command("git push", "Pushing to remote");
	if (not opts.rm_flag)
		return ;
	remove_commit_lines(files, markers);
	run_command("git add -A", "Staging marker cleanup");
	if (get_staged_files().empty())
	{
		std::cout << "No SS_COMMIT markers found; nothing to clean."
			<< std::endl;
		return ;
	}
	commit_message(CLEANUP_MESSAGE, get_current_branch());
	if (opts.no_push)
		std::cout << "Skipped push (--no-push)." << std::endl;
	else
		run_command("git push", "Pushing marker cleanup");
}

static void	print_usage(void)
{
	std::cout << "Usage: ./ss_git_push [-rm] [-n|--no-push] [-p|--preview]\n"
		<< "\nOptions:\n"
		<< "  (none)         Stage, commit and push; keep SS_COMMIT comments\n"
		<< "  -rm            After pushing, remove SS_COMMIT lines and commit the cleanup\n"
		<< "  -n, --no-push  Commit without pushing\n"
		<< "  -p, --preview  Show branch and generated message; change nothing\n"
		<< "  -h, --help     Show this help\n";
}

static void	parse_options(int ac, char **av, t_options &opts)
{
	int	i(0);

	opts.rm_flag = false;
	opts.no_push = false;
	opts.preview = false;
	while (++i < ac)
	{
		t_text	arg(av[i]);

		if (arg == "-rm")
			opts.rm_flag = true;
		else if (arg == "-n" or arg == "--no-push")
			opts.no_push = true;
		else if (arg == "-p" or arg == "--preview")
			opts.preview = true;
		else if (arg == "-h" or arg == "--help")
		{
			print_usage();
			std::exit(0);
		}
		else
			throw (std::invalid_argument(ERROR_INVALID_ARG));
	}
	if (opts.rm_flag and opts.preview)
		throw (std::invalid_argument(ERROR_RM_PREVIEW));
}

static int	init(int ac, char **av)
{
	t_vector	markers;
	t_options	opts;

	try
	{
		parse_options(ac, av, opts);
		markers.push_back("// SS_" "COMMIT: ");
		markers.push_back("# SS_" "COMMIT: ");
		markers.push_back("{/* SS_" "COMMIT: ");
		markers.push_back("/* SS_" "COMMIT: ");
		ss_git_push(markers, opts);
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