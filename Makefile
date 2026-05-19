CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g

# Project Name
NAME = codexion

SRC_DIR = src
OBJ_DIR = .obj

SRC = main.c\
	main_loop.c\
	actions.c\
	routines.c

UTILS_SRC = cleanup.c\
	f_time.c\
	fill_sim.c\
	ft_atoll.c\
	ft_strcmp.c\
	ft_strlen.c\
	info_print.c\
	parsing.c\
	queue.c\
	sim_init.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)/utils/, $(UTILS_SRC:.c=.o))

all: $(NAME) art

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Pattern Rule to Compile .c to .o inside obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Object Files
clean:
	rm -f $(OBJ)

# Clean Object Files and Executable
fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

# Rebuild the Project
re: fclean all

art:
	@clear
	@echo '⠀⠀⠀⠀⠀⠀⠀⣠⣴⡶⠟⠛⠿⠷⠶⣶⣤⣤⣀⠀⠀⠀⠀⠀⠀⠀'
	@echo '⠀⠀⠀⠀⠀⣠⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠻⣦⡀⠀⠀⠀⠀'
	@echo '⠀⠀⠀⠀⢠⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣦⠀⠀⠀'
	@echo '⠀⠀⠀⢀⣼⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣠⣤⡀⠹⣧⠀⠀'
	@echo '⠀⠀⢰⡿⠋⠀⠀⣀⣠⣄⡀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣦⣿⣆⡀'
	@echo '⠀⠀⠈⣿⣦⠀⣿⣟⡉⠉⠛⠗⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧'
	@echo '⢀⣤⡾⠛⠁⠈⠙⠋⠛⠻⠿⠃⠀⠈⣿⣿⣿⣿⠿⠿⣿⣿⣿⣿⣿⡿'
	@echo '⠸⣯⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠈⠻⠋⢁⣴⠶⢨⣿⣿⣿⣿⡇'
	@echo '⠀⣼⣿⣤⣈⠉⠁⣤⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⡏⠀⣼⣿⣿⣿⣿⠁'
	@echo '⠀⠹⣿⠉⣙⣿⣷⣿⡇⠀⠀⠀⠀⠀⢀⣠⡘⠉⣠⣾⠿⣿⣿⡿⠁⠀'
	@echo '⢀⣾⠟⣀⡉⠉⠀⣈⠁⠀⠀⠀⠀⣰⡿⠛⠛⠟⠋⠁⠀⢻⡿⠁⠀⠀'
	@echo '⣿⠃⠀⠿⠁⣤⠀⠙⢷⣤⣤⣠⣾⠟⠀⠀⠀⠀⠀⠀⠀⣾⠃⠀⠀⠀'
	@echo '⠻⢶⣶⣶⣾⣿⣤⣤⣶⡾⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⢸⡟⠀⠀⠀⠀'
	@echo '⠀⠀⠀⠀⠀⠀⠀⠙⢷⣤⣀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣿⠃⠀⠀⠀⠀'
	@echo '⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠷⢶⣦⣴⡶⠿⠛⠉⠀⠀⠀⠀⠀⠀'
	@echo '⠀⠀⠀⠀⠀⠀⠀⠀coder unite⠀⠀⠀⠀⠀⠀⠀⠀'


ARGS = 3 400 100 100 100 7 100 fifo

valgrind: all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME) $(ARGS)
