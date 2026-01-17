# SS GitPush

A smart Git commit automation tool written in C++98 that generates commit messages based on special comments in your code.

## 📋 Description

`ss_git_push` is a command-line utility that automates the Git commit process by: 
- Automatically staging all changes (`git add .`)
- Scanning modified files for special `SS_COMMIT` comments
- Generating structured commit messages based on these comments
- Creating commits with detailed file-by-file change descriptions

## 🚀 Features

- **Automatic Staging**: Stages all changes in the repository
- **Smart Comment Detection**: Searches for `// SS_COMMIT:` or `#// SS_COMMIT:` markers in your code
- **Structured Commit Messages**: Generates organized commit messages with file names and change descriptions
- **Multiple Files Support**: Handles multiple modified files in a single commit
- **C++98 Compatible**: Written in standard C++98 for maximum compatibility

## 🔧 Installation

### Prerequisites
- Git installed and configured

### Optional:  Add to PATH

```bash
# You might need to grant permission to the binary
chmod +x /path/to/ss_git_push

# Copy to a directory in your PATH
sudo cp ss_git_push /usr/local/bin/

# Or add an alias to your shell configuration
echo 'alias ss_git_push="/path/to/ss_git_push"' >> ~/.bashrc
```

## 📖 Usage

### Basic Usage

1. Make changes to your files
2. Add `SS_COMMIT` comments to describe your changes
3. Run the binary:

```bash
./ss_git_push
```

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

### Example Workflow

```bash
# 1. Edit your files and add SS_COMMIT comments
vim src/main.cpp
# Add:  // SS_COMMIT:  Implemented new feature X

vim src/utils.cpp
# Add: // SS_COMMIT:  Refactored helper functions

# 2. Run ss_git_push
./ss_git_push
```

### Generated Commit Message Example

```
UPDATED FILE:

 - src/main.cpp:
   • Implemented new feature X
   • Fixed memory leak in initialization

 - src/utils.cpp:
   • Refactored helper functions
```

## 📝 Comment Syntax

The tool recognizes two comment formats:

1. **C/C++ style**:  `// SS_COMMIT: Your message here`
2. **Script style**: `#// SS_COMMIT: Your message here`

**Rules:**
- Comments must start with `// SS_COMMIT:` or `#// SS_COMMIT:`
- Text after the colon will be used as the change description
- Leading whitespace is automatically trimmed
- Multiple comments in the same file will all be included

## 🎯 Use Cases

- **Development Workflow**: Quickly commit changes with descriptive messages
- **Code Review**: Document changes directly in the code
- **Team Collaboration**: Ensure consistent commit message formatting
- **Learning Projects**: Track incremental changes with detailed descriptions

## ⚠️ Important Notes

- The tool automatically runs `git add .` (stages all changes)
- If no files are staged, it will output "Nothing to commit!"
- Files without `SS_COMMIT` comments will still be listed in the commit
- Commit messages are generated automatically - review before pushing

## 🔍 Example

**File:  main.cpp**
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
```

**Running ss_git_push:**
```bash
$ ./ss_git_push
```

**Generated commit:**
```
UPDATED FILE:

 - main.cpp:
   • Added hello world function
   • Updated main to use new hello function
```

## 🛠️ Technical Details

- **Language**: C++98
- **Dependencies**: Standard C++ library, POSIX system calls
- **Compatibility**: Linux, macOS, Unix-like systems
- **System calls used**: `system()`, `popen()`, `pclose()`

## 📄 License

This project is open source and available for personal and commercial use.

SS is just a signature and it means Ser Superior (Superior Being) in portuguese. All my projects have it as prefix.

## 🤝 Contributing

Feel free to fork, modify, and submit pull requests.  Suggestions and improvements are welcome!

## 💡 Tips

- Use descriptive `SS_COMMIT` comments for better commit history
- Review generated commits before pushing to remote
- Combine with Git hooks for additional automation
- Consider adding multiple `SS_COMMIT` comments for complex changes

---

**Made using C++98**
