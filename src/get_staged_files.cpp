#include "../include/ss_git_push.hpp"

t_vector	get_staged_files(void)
{
	FILE		*file;
	t_vector	storage;
	t_text		line;
	char		buffer[1024];

	file = popen("git diff --name-only --cached", "r");
	if (not file)
		return (storage);
	while (fgets(buffer, sizeof(buffer), file))
	{
		line = buffer;
		if (not line.empty() and line[line.size() - 1] == '\n')
			line.erase(line.size() - 1, 1);
		if (not line.empty())
			storage.push_back(line);
	}
	return (pclose(file), storage);
}
