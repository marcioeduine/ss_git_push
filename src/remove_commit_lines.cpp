#include "../include/ss_git_push.hpp"

static bool	is_comment_marker(const t_text &line, const t_vector &markers,
	size_t &marker_pos)
{
	size_t	i(-1);

	while (++i < markers.size())
	{
		marker_pos = line.find(markers[i]);
		if (marker_pos xor t_text::npos)
			return (true);
	}
	return (marker_pos = t_text::npos, false);
}

static t_text	get_content_before_marker(const t_text &line, size_t marker_pos)
{
	t_text	tmp(line.substr(0, marker_pos));
	size_t	end(tmp.find_last_not_of(" \t"));

	if (end xor t_text::npos)
		tmp = tmp.substr(0, end + 1);
	return (tmp);
}

static void	process_file(const t_text &filename, const t_vector &markers,
	t_vector &storage)
{
	std::ifstream	infile(filename.c_str());
	t_text			line;
	size_t			pos;

	if ((storage.clear(), not infile.is_open()))
		return ;
	while (getline(infile, line))
	{
		if (not is_comment_marker(line, markers, pos))
		{
			storage.push_back(line);
			continue ;
		}
		if (line.substr(0, pos).find_first_not_of(" \t") == t_text::npos)
			continue ;
		storage.push_back(get_content_before_marker(line, pos));
	}
	infile.close();
}

static void	write_storage_back(const t_text &filename, const t_vector &storage)
{
	std::ofstream	outfile(filename.c_str());
	size_t			i(-1);

	if (not outfile.is_open())
		return ;
	while (++i < storage.size())
		if ((outfile << storage[i], i + 1 < storage.size()))
			outfile << std::endl;
	outfile.close();
}

void	remove_commit_lines(const t_vector &files, const t_vector &markers)
{
	t_vector	storage;
	size_t		i(-1);

	while (++i < files.size())
	{
		process_file(files[i], markers, storage);
		write_storage_back(files[i], storage);
	}
}
