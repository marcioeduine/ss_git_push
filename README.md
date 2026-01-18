# SS GitPush

An intelligent Git commit automation tool written in C++98 that generates commit messages based on special comments in your code.

## 📋 Description

`ss_git_push` is a command-line utility that automates the Git commit process by:
- Automatically staging all changes (`git add *`)
- Scanning modified files for special `SS_COMMIT` comments
- Generating structured commit messages based on these comments
- Creating commits with detailed file-by-file change descriptions
- Automatically pushing to the remote repository (`git push`)

## 🚀 Features

- **Automatic Staging**: Stages all changes in the repository
- **Smart Comment Detection**: Searches for `// SS_COMMIT:` or `#// SS_COMMIT:` markers in your code
- **Structured Commit Messages**: Generates organised commit messages with file names and change descriptions
- **Multiple Files Support**: Handles multiple modified files in a single commit
- **Optional Comment Removal**: `-rm` flag to remove `SS_COMMIT` comments after committing
- **Automatic Push**: Automatically pushes changes to the remote repository
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
│   ├── build_commit_message.cpp     # Generates commit messages
│   ├── extract_commits_from_file.cpp # Extracts SS_COMMIT comments
│   ├── get_staged_files.cpp         # Gets list of staged files
│   ├── main.cpp                     # Main programme logic
│   └── remove_commit_lines.cpp      # Removes SS_COMMIT lines (with -rm)
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
1. Execute `git add *` (stage all changes)
2. Retrieve the list of staged files
3. Scan each file for `SS_COMMIT` comments
4. Generate a structured commit message
5. Create the commit with the generated message
6. Execute `git push` to push the changes

### Usage with `-rm` Flag

```bash
./ss_git_push -rm
```

With the `-rm` flag, the programme will:
1. Execute the entire normal commit and push process
2. **Remove all lines** containing `SS_COMMIT` markers from the files
3. If a line contains only whitespace/tabs followed by the marker, the entire line is removed
4. If a line contains code before the marker, only the marker and text after it are removed

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

**Code on the same line (will be preserved without marker with `-rm`):**
```cpp
int x = 42;  // SS_COMMIT: Initialised variable x
```

### Example Workflow

```bash
# 1. Edit your files and add SS_COMMIT comments
vim src/main.cpp
# Add: // SS_COMMIT: Implemented new feature X

vim src/utils.cpp
# Add: // SS_COMMIT: Refactored helper functions

# 2. Run ss_git_push
./ss_git_push

# 3. Or run with -rm to clean up comments after committing
./ss_git_push -rm
```

### Generated Commit Message Example

```
UPDATED FILE:

 - src/main.cpp:
   • Implemented new feature X
   • Fixed memory leak in initialisation

 - src/utils.cpp:
   • Refactored helper functions

 - src/config.h
```

Note: Files without `SS_COMMIT` comments appear listed without bullet points.

## 📝 Comment Syntax

The tool recognises two comment formats:

1. **C/C++ style**: `// SS_COMMIT: Your message here`
2. **Script style**: `#// SS_COMMIT: Your message here`

**Rules:**
- Comments must start with `// SS_COMMIT:` or `#// SS_COMMIT:`
- Text after the colon will be used as the change description
- Leading whitespace is automatically trimmed
- Multiple comments in the same file will all be included
- Null characters (`\0`) are automatically removed from messages

## 🎯 Use Cases

- **Development Workflow**: Quickly commit changes with descriptive messages
- **Code Review**: Document changes directly in the code
- **Team Collaboration**: Ensure consistent commit message formatting
- **Learning Projects**: Track incremental changes with detailed descriptions
- **Code Cleanup**: Use `-rm` to remove temporary comments after documenting changes

## ⚠️ Important Notes

- The tool automatically runs `git add *` (stages all changes)
- If no files are staged, it will output "Nothing to commit!"
- Files without `SS_COMMIT` comments will still be listed in the commit
- Commit messages are generated automatically
- Push is executed automatically after committing
- With `-rm`, comments are removed from files **after** the commit is created
- The `-rm` flag preserves code that appears before `SS_COMMIT` markers

## 📋 Command-Line Arguments

```
Usage: ./ss_git_push [-rm]

Options:
  (none)       Execute commit and push normally, keep SS_COMMIT comments
  -rm          Execute commit and push, then remove all lines with SS_COMMIT
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
UPDATED FILE:

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
- Review generated commits before pushing to remote
- Combine with Git hooks for additional automation
- Consider adding multiple `SS_COMMIT` comments for complex changes
- Use `-rm` when comments are only temporary and shouldn't remain in the code
- Avoid using `-rm` if you want to maintain a history of changes in code comments

## 🐛 Error Handling

The programme handles the following errors:

- **Too many arguments**: Accepts only 0 or 1 argument
- **Invalid argument**: Only `-rm` is accepted as an argument
- **Nothing to commit**: Warns if no files are staged
- **Error creating temporary file**: Checks if it can create the message file
- **Error opening temporary file**: Checks if it can write the message

---

**Made using C++98**
