#ifndef SS_GIT_PUSH_HPP
# define SS_GIT_PUSH_HPP

# define ERROR_MANY_ARGS "Too many arguments.\n[ USAGE ]: ./ss_git_push [-rm]"
# define ERROR_INVALID_ARG "Invalid argument.\n[ USAGE ]: ./ss_git_push [-rm]"

# include <algorithm>
# include <cstdlib>
# include <fstream>
# include <iostream>
# include <unistd.h>
# include <vector>

typedef std::string			t_text;
typedef std::vector<t_text>	t_vector;

t_vector	get_staged_files(void);
void		extract_commits_from_file(const t_text &filename,
	const t_vector &markers, t_vector &storage);
t_text		build_commit_message(const t_vector &staged_files,
	const t_vector &markers);
void		remove_commit_lines(const t_vector &files, const t_vector &markers);

#endif
