CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g

NAME = codexion

SRC_DIR = src
OBJ_DIR = .obj

SRC = main.c\
	main_loop.c\
	actions.c\
	dongle_priority.c\
	dongle_acquire.c\
	dongle_release.c\
	routines.c\
	thread_pool.c\
	monitor.c

UTILS_SRC = cleanup.c\
	exitscreen.c\
	exitscreen_data0.c\
	exitscreen_data1.c\
	exitscreen_data2.c\
	exitscreen_data3.c\
	f_time.c\
	fill_sim.c\
	ft_atoll.c\
	ft_calloc.c\
	ft_strcmp.c\
	ft_strlen.c\
	heap.c\
	heaptoo.c\
	info_print.c\
	parsing.c\
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
	-@clear 2>/dev/null
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


ARGS = 20 800 100 100 100 7 100 fifo

test: all
	@resize -s 80 140
	./$(NAME) $(ARGS)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(ARGS)

hellgrind: all
	valgrind --tool=helgrind -s --history-level=approx ./$(NAME) $(ARGS)
