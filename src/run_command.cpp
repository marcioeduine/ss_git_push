#include "../include/ss_git_push.hpp"

// Runs a shell command and throws when it reports failure, so staging,
// commit and push faults abort the run instead of passing silently.
int	run_command(const t_text &cmd, const t_text &action)
{
	int	code(system(cmd.c_str()));

	if (code != 0)
		throw (std::runtime_error(action + " failed."));
	return (code);
}
