NAME		= webserv
CPP			= c++
FLAGS		= -Wall -Werror -Wextra -fsanitize=address -g3
INCLUDES	= -I includes

RM			= rm -rf

FILES		= main server

SRCS		= $(addsuffix .cpp, $(addprefix src/, $(FILES)))

OBJS_DIR	= objs/
OBJS		= $(addprefix $(OBJS_DIR), $(SRCS:cpp=o))

$(OBJS_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@printf "\033[38;5;121mCompiling $<...\033[0m"
	$(CPP) $(FLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(FLAGS) $(OBJS) -o $(NAME)
	@printf "\033[38;5;121m$(NAME) compiled sucessfully!\033[0m\n"

clean:
	$(RM) $(OBJS_DIR)
	@printf "\033[38;5;121mCleaned sucessfully!\033[0m\n"

fclean: clean
	$(RM) $(NAME)
	@printf "\033[38;5;121m$(NAME) removed sucessfully!\033[0m\n"

re: fclean all