# SS GitPush

An intelligent Git commit automation tool written in C++98 that generates commit messages based on special comments in your code.

## 📋 Description

`ss_git_push` is a command-line utility that automates the Git commit process by:
- Automatically staging all changes (`git add -A`, including new files, modifications and deletions)
- Scanning modified files for special `SS_COMMIT` comments
- Generating status-aware commit messages (new, updated and removed files)
- Showing the current branch and the generated message before committing
- Creating commits with detailed file-by-file change descriptions
- Pushing to the remote repository (`git push`), unless skipped
- Offering a simulation mode (`--dry-run`) that changes nothing

## 🚀 Features

- **Complete Staging**: Stages everything with `git add -A`, dotfiles and deletions included
- **Smart Comment Detection**: Searches for `// SS_COMMIT:`, `#// SS_COMMIT:`, `{/* SS_COMMIT:` (JSX) and `/* SS_COMMIT:` (CSS/block) markers in your code
- **Status-Aware Messages**: Groups files into `NEW FILES`, `UPDATED FILES` and `REMOVED FILES` sections instead of a single fixed header
- **Branch Display**: Always prints the current branch with the generated message
- **Simulation Mode**: `--dry-run` shows what would be committed without touching the index, history or remote
- **Optional Push**: `--no-push` commits without pushing, for later review
- **Multiple Files Support**: Handles multiple modified files in a single commit
- **Optional Comment Removal**: `-rm` flag removes `SS_COMMIT` comments after pushing, then commits the cleanup so the tree ends clean
- **Checked Execution**: Staging, commit and push faults abort the run with a clear error; a failed push never triggers marker removal
- **C++98 Compatible**: Written in standard C++98 for maximum compatibility

## 📧 Installation

### Prerequisites
- Git installed and configured
- C++ compiler with C++98 support (g++, clang++)
- Make

### Build from Source

```bash
# Clone the repository
git clone <repository-url>
cd ss_git_push

# Build the project
make

# The binary will be created in the current directory
```

### Makefile Targets

```bash
make        # Build the project
make clean  # Remove object files
make fclean # Remove object files and binary
make re     # Rebuild the project from scratch
```

### Optional: Add to PATH

```bash
# You might need to grant permission to the binary
chmod +x ss_git_push

# Copy to a directory in your PATH
sudo cp ss_git_push /usr/local/bin/

# Or add an alias to your shell configuration
echo 'alias ss_git_push="/path/to/ss_git_push"' >> ~/.bashrc
```

## 📁 Project Structure

```
.
├── include/
│   └── ss_git_push.hpp              # Header file with function declarations
├── src/
│   ├── build_commit_message.cpp     # Generates status-aware commit messages
│   ├── extract_commits_from_file.cpp # Extracts SS_COMMIT comments
│   ├── get_staged_files.cpp         # Gets staged files, working-tree entries and branch
│   ├── main.cpp                     # Main programme logic and option parsing
│   ├── remove_commit_lines.cpp      # Removes SS_COMMIT lines (with -rm)
│   └── run_command.cpp              # Checked shell-command execution
├── Makefile                          # Build configuration
├── README.md                         # Documentation (English)
└── README.pt_ao.md                   # Documentation (Portuguese)
```

## 📖 Usage

### Basic Usage

```bash
./ss_git_push
```

This command will:
1. Execute `git add -A` (stage everything, including deletions and dotfiles)
2. Retrieve the list of staged files with their status (new / updated / removed)
3. Scan each file for `SS_COMMIT` comments
4. Print the current branch and the generated message
5. Create the commit with the generated message
6. Execute `git push` to push the changes

### Usage with `-rm` Flag

```bash
./ss_git_push -rm
```

With the `-rm` flag, the programme will:
1. Execute the entire normal commit and push process
2. **Remove all lines** containing `SS_COMMIT` markers from the committed files
3. If a line contains only whitespace/tabs followed by the marker, the entire line is removed
4. If a line contains code before the marker, only the marker and text after it are removed
5. Stage the cleanup and create a second commit (`chore: remove SS_COMMIT markers`)
6. Push the cleanup, so the working tree ends clean

### Usage with `--no-push`

```bash
./ss_git_push --no-push
# or: ./ss_git_push -n
```

Commits normally but skips both pushes. Useful when you want to review the
commit locally first, or batch several commits before pushing. Combines with
`-rm` (the cleanup commit is also kept local).

### Usage with `--dry-run`

```bash
./ss_git_push --dry-run
# or: ./ss_git_push -d
```

Simulation mode. Reads the working tree without staging anything and prints
the branch plus the message that would be generated. Nothing is staged,
committed or pushed. Cannot be combined with `-rm`.

### Usage with `--help`

```bash
./ss_git_push --help
# or: ./ss_git_push -h
```

Prints the usage summary.

### Adding SS_COMMIT Comments

Add special comments in your modified files to describe the changes:

**For C/C++ files:**
```cpp
// SS_COMMIT: Added user authentication function
void	authenticate_user(void)
{
    // implementation
}
```

**For Python/Shell scripts:**
```python
#// SS_COMMIT: Fixed bug in data validation
def	validate_data(input):
    # implementation
```

**For JSX/React components (valid inside markup):**
```jsx
{/* SS_COMMIT: Aligned hero CTA icons */}
<button>Play</button>
```

**For CSS/stylesheets:**
```css
/* SS_COMMIT: Centred footer layout */
.footer { display: flex; }
```

**Code on the same line (will be preserved without marker with `-rm`):**
```cpp
int x = 42;  // SS_COMMIT: Initialised variable x
```

### Example Workflow

```bash
# 1. Edit your files and add SS_COMMIT comments
vim src/main.cpp
# Add: // SS_COMMIT: Implemented new feature X

vim src/App.jsx
# Add: {/* SS_COMMIT: Aligned header icons */}

# 2. Preview with a dry run
./ss_git_push --dry-run

# 3. Commit and push
./ss_git_push

# 4. Or run with -rm to clean up comments after committing
./ss_git_push -rm
```

### Generated Commit Message Example

```
NEW FILES:
 - src/App.jsx:
   • Aligned header icons

UPDATED FILES:
 - src/main.cpp:
   • Implemented new feature X
   • Fixed memory leak in initialisation

REMOVED FILES:
 - src/legacy.cpp
```

Note: Files without `SS_COMMIT` comments appear listed without bullet points.
Empty sections are omitted.

## 📝 Comment Syntax

The tool recognises four comment formats:

1. **C/C++ style**: `// SS_COMMIT: Your message here`
2. **Script style**: `#// SS_COMMIT: Your message here`
3. **JSX style**: `{/* SS_COMMIT: Your message here */}`
4. **Block style**: `/* SS_COMMIT: Your message here */` (CSS and block comments)

**Rules:**
- The message ends at the first `*/` on the line, so trailing closers never
  leak into the commit text
- Trailing `*/` and `}` characters are stripped automatically
- Text after the colon will be used as the change description
- Leading whitespace is automatically trimmed
- Multiple comments in the same file will all be included
- Null characters (`\0`) are automatically removed from messages
- Inside JSX markup, use the `{/* ... */}` form: a bare `//` marker there is
  a syntax error in React

## 🎯 Use Cases

- **Development Workflow**: Quickly commit changes with descriptive messages
- **Code Review**: Document changes directly in the code
- **Team Collaboration**: Ensure consistent commit message formatting
- **Learning Projects**: Track incremental changes with detailed descriptions
- **Code Cleanup**: Use `-rm` to remove temporary comments after documenting changes

## ⚠️ Important Notes

- The tool runs `git add -A` (stages everything, including deletions and dotfiles)
- The current branch is always printed with the generated message: review it
  with `--dry-run` before pushing, especially on `main`
- If no files are staged, it will output "Nothing to commit!"
- Files without `SS_COMMIT` comments will still be listed in the commit
- Push is executed automatically after committing, unless `--no-push` is given
- With `-rm`, the marker cleanup is committed (`chore: remove SS_COMMIT markers`)
  and pushed, so the tree ends clean
- If staging, commit or push fails, the run aborts with an error and markers
  are kept: a failed push never triggers marker removal
- Paths with spaces or special quoting are not supported

## 📋 Command-Line Arguments

```
Usage: ./ss_git_push [-rm] [-n|--no-push] [-d|--dry-run]

Options:
  (none)       Stage, commit and push; keep SS_COMMIT comments
  -rm          Commit, push, then remove SS_COMMIT lines and commit the cleanup
  -n, --no-push  Commit without pushing
  -d, --dry-run  Show branch and generated message; change nothing
  -h, --help   Show usage help
```

## 🔍 Complete Example

**File: main.cpp (before)**
```cpp
#include <iostream>

// SS_COMMIT: Added hello world function
void	hello(void)
{
    std::cout << "Hello, World!" << std::endl;
}

// SS_COMMIT: Updated main to use new hello function
int	main(void)
{
    return (hello(), 0);
}

int x = 42;  // SS_COMMIT: Initialised global variable
```

**Running ss_git_push with -rm:**
```bash
$ ./ss_git_push -rm
```

**Generated commit:**
```
UPDATED FILES:
 - main.cpp:
   • Added hello world function
   • Updated main to use new hello function
   • Initialised global variable
```

**File: main.cpp (after -rm)**
```cpp
#include <iostream>

void	hello(void)
{
    std::cout << "Hello, World!" << std::endl;
}

int	main(void)
{
    return (hello(), 0);
}

int x = 42;
```

## 🛠️ Technical Details

- **Language**: C++98
- **Dependencies**: Standard C++ library, POSIX system calls
- **Compatibility**: Linux, macOS, Unix-like systems
- **System calls used**: `system()`, `popen()`, `pclose()`, `mkstemp()`, `remove()`
- **Git plumbing used**: `git add -A`, `git diff --name-only --cached`, `git status --porcelain`, `git rev-parse --abbrev-ref HEAD`
- **Temporary File Management**: Creates temporary file in `/tmp/` for commit message
- **Text Processing**: Automatically removes null characters and unnecessary whitespace
- **Compilation Flags**: `-Wall -Wextra -Werror -std=c++98`

## 📄 Licence

This project is open source and available for personal and commercial use.

SS is just a signature and it means Ser Superior (Superior Being) in Portuguese. All my projects have it as prefix.

## 🤝 Contributing

Feel free to fork, modify, and submit pull requests. Suggestions and improvements are welcome!

## 💡 Tips

- Use descriptive `SS_COMMIT` comments for better commit history
- Preview with `--dry-run` before pushing to the remote
- Combine with Git hooks for additional automation
- Consider adding multiple `SS_COMMIT` comments for complex changes
- Use `-rm` when comments are only temporary and shouldn't remain in the code
- Avoid using `-rm` if you want to maintain a history of changes in code comments

## 🐛 Error Handling

The programme handles the following errors:

- **Too many arguments**: Accepts only documented flag combinations
- **Invalid argument**: Only `-rm`, `-n`/`--no-push`, `-d`/`--dry-run`, `-h`/`--help` are accepted
- **Conflicting flags**: `-rm` and `--dry-run` cannot be combined
- **Nothing to commit**: Warns if no files are staged (or none changed, in dry-run)
- **Staging/commit/push faults**: Aborts with a clear error; markers are kept
- **Error creating temporary file**: Checks if it can create the message file
- **Error opening temporary file**: Checks if it can write the message

---

**Made using C++98**
