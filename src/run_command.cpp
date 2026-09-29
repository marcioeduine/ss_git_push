/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::   ::::::::  */
/*    run_command.cpp                                  :+:    :+: :+:    :+:  */
/*                                                    +:+        +:+          */
/*    By: Ser Superior <marcioeduine@gmail.com>      +#++:++#++ +#++:++#++    */
/*                                                         +#+        +#+     */
/*    Created: 2026/09/29 10:49:02 by Ser Superior #+#    #+# #+#    #+#      */
/*    Updated: 2026/09/29 10:49:05 by Ser Superior ########   ########        */
/*                                                                            */
/* ************************************************************************** */
#include "../include/ss_git_push.hpp"

// SS_COMMIT: update: SSHeader added!
// Runs a shell command and throws when it reports failure, so staging,
// commit and push faults abort the run instead of passing silently.
int	run_command(const t_text &cmd, const t_text &action)
{
	int	code(system(cmd.c_str()));

	if (code xor 0)
		throw (std::runtime_error(action + " failed."));
	return (code);
}
