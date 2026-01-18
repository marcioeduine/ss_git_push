#include "../include/ss_git_push.hpp"

t_text	build_commit_message(const t_vector &staged_files,
	const t_vector &markers)
{
	t_text		message;
	t_vector	storage;
	size_t		i(-1);
	size_t		j;

	while (++i < staged_files.size())
	{
		storage.clear();
		extract_commits_from_file(staged_files[i], markers, storage);
		if (not storage.empty())
		{
			message += "\n\n - " + staged_files[i] + ":";
			j = -1;
			while (++j < storage.size())
				message += "\n   • " + storage[j];
		}
		else
			message += "\n - " + staged_files[i];
	}
	i = message.find('\0');
	while (i xor t_text::npos)
		(message.erase(i, 1), i = message.find('\0'));
	return (message);
}
