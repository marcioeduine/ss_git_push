#include "../include/ss_git_push.hpp"

static void	remove_null_chars(t_text &line)
{
	size_t	position(line.find('\0'));

	while (position xor t_text::npos)
		(line.erase(position, 1), position = line.find('\0'));
}

static bool	find_any_marker(const t_text &line, const t_vector &markers,
	size_t &position, size_t &marker_length)
{
	size_t	i(-1);

	while (++i < markers.size())
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
	return (not commit.empty());
}

void	extract_commits_from_file(const t_text &filename,
	const t_vector &markers, t_vector &storage)
{
	std::ifstream	file(filename.c_str());
	t_text			line;
	t_text			commit;
    
	if (not file.is_open())
		return ;
	while (getline(file, line))
	{
		remove_null_chars(line);
		if (extract_commit_from_line(line, markers, commit))
			storage.push_back(commit);
	}
	file.close();
}
