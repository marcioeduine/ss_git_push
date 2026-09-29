#include "../include/ss_git_push.hpp"

// Appends one message section (NEW / UPDATED / REMOVED FILES) for entries
// carrying the given status key. Files holding SS_COMMIT markers are listed
// with their bullet points; files without markers are listed plainly.
static void	append_section(t_text &message, const t_text &title,
	const t_status_list &entries, char status, const t_vector &markers)
{
	bool		section_open(false);
	size_t		i(-1);
	t_vector	storage;
	size_t		j;

	while (++i < entries.size())
	{
		if (entries[i].first != status)
			continue ;
		if (not section_open)
			(message += "\n\n" + title + ":", section_open = true);
		storage.clear();
		extract_commits_from_file(entries[i].second, markers, storage);
		if (storage.empty())
			message += "\n - " + entries[i].second;
		else
		{
			message += "\n - " + entries[i].second + ":";
			j = -1;
			while (++j < storage.size())
				message += "\n   • " + storage[j];
		}
	}
}

// Builds a status-aware commit message grouping entries into NEW FILES,
// UPDATED FILES and REMOVED FILES sections. Empty sections are omitted.
t_text	build_commit_message(const t_status_list &entries,
	const t_vector &markers)
{
	t_text	message;
	size_t	lead(0);

	append_section(message, "NEW FILES", entries, 'A', markers);
	append_section(message, "UPDATED FILES", entries, 'M', markers);
	append_section(message, "REMOVED FILES", entries, 'D', markers);
	while (lead < message.size() and message[lead] == '\n')
		++lead;
	message.erase(0, lead);
	lead = message.find('\0');
	while (lead xor t_text::npos)
		(message.erase(lead, 1), lead = message.find('\0'));
	return (message);
}
