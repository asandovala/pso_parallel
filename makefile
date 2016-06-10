FLAGS = -W
OBJS  = src/main.c src/pso.c src/freq/freq.c 
HEAD = src/freq/freq.h src/pso.h
NAME = pso 

$(NAME): $(OBJS) $(HEAD)
	gcc $(FLAGS) -o $(NAME) $(OBJS) -lm

