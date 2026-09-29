/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::   ::::::::  */
/*    get_staged_files.cpp                             :+:    :+: :+:    :+:  */
/*                                                    +:+        +:+          */
/*    By: Ser Superior <marcioeduine@gmail.com>      +#++:++#++ +#++:++#++    */
/*                                                         +#+        +#+     */
/*    Created: 2026/09/29 10:48:24 by Ser Superior #+#    #+# #+#    #+#      */
/*    Updated: 2026/09/29 10:48:26 by Ser Superior ########   ########        */
/*                                                                            */
/* ************************************************************************** */
#include "../include/ss_git_push.hpp"

// Lists files already present in the Git index (staged changes only).
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

// Maps a porcelain status letter to a message section key.
static char	classify_status(char code)
{
	if (code == 'A')
		return ('A');
	if (code == 'D')
		return ('D');
	return ('M');
}

// Lists every changed file in the working tree (staged and unstaged),
// each paired with its section key: 'A' (new), 'M' (updated), 'D' (removed).
// Used by --dry-run, which must not touch the index.
t_status_list	get_working_entries(void)
{
	FILE			*file(popen("git status --porcelain", "r"));
	t_status_list	entries;
	char			buffer[4096];
	t_text			line;
	t_text			path;
	size_t			arrow;
	char			code;

	if (not file)
		return (entries);
	while (fgets(buffer, sizeof(buffer), file))
	{
		line = buffer;
		if (not line.empty() and line[line.size() - 1] == '\n')
			line.erase(line.size() - 1, 1);
		if (line.size() < 4)
			continue ;
		arrow = line.find(" -> ");
		if (arrow != t_text::npos)
			path = line.substr(arrow + 4);
		else
			path = line.substr(3);
		if (path.size() >= 2 and path[0] == '"' and path[path.size() - 1] == '"')
			path = path.substr(1, path.size() - 2);
		code = line[0];
		if (code == '?')
			entries.push_back(std::make_pair('A', path));
		else
		{
			if (code == ' ')
				code = line[1];
			entries.push_back(std::make_pair(classify_status(code), path));
		}
	}
	return (pclose(file), entries);
}

// Returns the section key of a file already in the index.
char	staged_file_status(const t_text &filename)
{
	t_text		cmd("git status --porcelain -- ");
	FILE		*file;
	char		buffer[16];
	t_text		out;

	cmd += filename;
	file = popen(cmd.c_str(), "r");
	if (not file)
		return ('M');
	if (fgets(buffer, sizeof(buffer), file))
		out = buffer;
	pclose(file);
	if (out.size() < 2)
		return ('M');
	if (out[0] == 'A' or out[1] == 'A')
		return ('A');
	if (out[0] == 'D' or out[1] == 'D')
		return ('D');
	return ('M');
}

// Returns the current branch name, or "unknown" when it cannot be read.
t_text	get_current_branch(void)
{
	FILE	*file(popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r"));
	char	buffer[256];
	t_text	branch;

	if (not file)
		return ("unknown");
	if (fgets(buffer, sizeof(buffer), file))
		branch = buffer;
	pclose(file);
	if (not branch.empty() and branch[branch.size() - 1] == '\n')
		branch.erase(branch.size() - 1, 1);
	if (branch.empty())
		return ("unknown");
	return (branch);
}