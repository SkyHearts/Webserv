NAME		= webserv
CPP			= c++
FLAGS		= -Wall -Werror -Wextra
INCLUDES	= -I includes

RM			= rm -rf

FILES		= main server parse request responseBase responseGet responsePost responseDelete autoindex cgi_handler

SRCS		= $(addsuffix .cpp, $(addprefix src/, $(FILES)))

OBJS_DIR	= objs/
OBJS		= $(addprefix $(OBJS_DIR), $(SRCS:cpp=o))

$(OBJS_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@printf "\033[33mCompiling $<...\033[0m\n"
	@$(CPP) $(FLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(FLAGS) $(OBJS) -o $(NAME)
	@printf "\033[32m$(NAME) compiled sucessfully!\033[0m\n"

clean:
	@$(RM) $(OBJS_DIR)
	@printf "\033[34mCleaned sucessfully!\033[0m\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "\033[34m$(NAME) removed sucessfully!\033[0m\n\n"

re: fclean all