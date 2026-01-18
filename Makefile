NAME		= ss_git_push
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
HEADER		= include/ss_git_push.hpp
OBJ			= src/build_commit_message.o \
			src/get_staged_files.o \
			src/remove_commit_lines.o \
			src/extract_commits_from_file.o \
			src/main.o

all: $(NAME)

$(OBJ): $(HEADER)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
