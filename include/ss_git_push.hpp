#ifndef SS_GIT_PUSH_HPP
# define SS_GIT_PUSH_HPP

# define ERROR_MANY_ARGS "Too many arguments.\n[ USAGE ]: ./ss_git_push [-rm] [-n|--no-push] [-d|--dry-run]"
# define ERROR_INVALID_ARG "Invalid argument.\n[ USAGE ]: ./ss_git_push [-rm] [-n|--no-push] [-d|--dry-run]"
# define ERROR_RM_DRY_RUN "Options -rm and --dry-run cannot be combined."
# define CLEANUP_MESSAGE "chore: remove SS_COMMIT markers\n"

# include <algorithm>
# include <cstdlib>
# include <fstream>
# include <iostream>
# include <stdexcept>
# include <unistd.h>
# include <utility>
# include <vector>

typedef std::string				t_text;
typedef std::vector<t_text>		t_vector;
typedef std::pair<char, t_text>	t_status_entry;
typedef std::vector<t_status_entry>	t_status_list;

// Command-line options parsed from argv.
struct t_options
{
	bool	rm_flag;
	bool	no_push;
	bool	dry_run;
};

t_vector		get_staged_files(void);
t_status_list	get_working_entries(void);
char			staged_file_status(const t_text &filename);
t_text			get_current_branch(void);
void			extract_commits_from_file(const t_text &filename,
	const t_vector &markers, t_vector &storage);
t_text			build_commit_message(const t_status_list &entries,
	const t_vector &markers);
void			remove_commit_lines(const t_vector &files, const t_vector &markers);
int				run_command(const t_text &cmd, const t_text &action);

#endif
