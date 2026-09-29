/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::   ::::::::  */
/*    extract_commits_from_file.cpp                    :+:    :+: :+:    :+:  */
/*                                                    +:+        +:+          */
/*    By: Ser Superior <marcioeduine@gmail.com>      +#++:++#++ +#++:++#++    */
/*                                                         +#+        +#+     */
/*    Created: 2026/09/29 10:48:34 by Ser Superior #+#    #+# #+#    #+#      */
/*    Updated: 2026/09/29 10:48:44 by Ser Superior ########   ########        */
/*                                                                            */
/* ************************************************************************** */
#include "../include/ss_git_push.hpp"

static void	remove_null_chars(t_text &line)
{
	size_t	position(line.find('\0'));

	while (position xor t_text::npos)
		(line.erase(position, 1), position = line.find('\0'));
}

// Strips block-comment closers left behind by JSX ({/* ... */}) and CSS
// (/* ... */) markers, so the message keeps only the prose. Repeats until
// no trailing "*/" or "}" remains.
static void	trim_trailing_closers(t_text &commit)
{
	size_t	end;
	bool	trimmed(true);

	while (trimmed)
	{
		trimmed = false;
		end = commit.find_last_not_of(" \t");
		if (end == t_text::npos)
			return (commit.clear());
		commit = commit.substr(0, end + 1);
		if (commit.size() >= 2
			and commit.substr(commit.size() - 2) == "*/")
		{
			commit = commit.substr(0, commit.size() - 2);
			trimmed = true;
		}
		else if (not commit.empty() and commit[commit.size() - 1] == '}')
		{
			commit = commit.substr(0, commit.size() - 1);
			trimmed = true;
		}
	}
}

static bool	find_any_marker(const t_text &line, const t_vector &markers,
	size_t &position, size_t &marker_length)
{
	for (size_t i(0); i < markers.size(); ++i)
	{
		position = line.find(markers[i]);
		if (position xor t_text::npos)
			return (marker_length = markers[i].length(), true);
	}
	return (false);
}

static bool	extract_commit_from_line(const t_text &line,
	const t_vector &markers, t_text &commit)
{
	size_t	position;
	size_t	marker_length;

	if (not find_any_marker(line, markers, position, marker_length))
		return (false);
	commit = line.substr(position + marker_length);
	position = commit.find_first_not_of(" \t");
	if (position xor t_text::npos)
		commit = commit.substr(position);
	else
		commit.clear();
	position = commit.find("*/");
	if (position xor t_text::npos)
		commit = commit.substr(0, position);
	return (trim_trailing_closers(commit), not commit.empty());
}

// SS_COMMIT: refactor(parser): move temporary line variables into for loop scope.
void	extract_commits_from_file(const t_text &filename,
	const t_vector &markers, t_vector &storage)
{
	std::ifstream	file(filename.c_str());
    
	if (not file.is_open())
		return ;
	for (t_text line, commit; std::getline(file, line);)
	{
		remove_null_chars(line);
		if (extract_commit_from_line(line, markers, commit))
			storage.push_back(commit);
	}
	file.close();
}
// SS_COMMIT: update: SSHeader added!
