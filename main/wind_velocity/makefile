FLAGS = -W
OBJS  = ../../src/wind_velocity/main.c ../../src/wind_velocity/pso.c ../../src/wind_velocity/freq/freq.c ../../src/utils/tools.c
HEAD = ../../src/wind_velocity/freq/freq.h ../../src/wind_velocity/pso.h ../../src/utils/tools.h
NAME = pso 

$(NAME): $(OBJS) $(HEAD)
	gcc $(FLAGS) -o $(NAME) $(OBJS) -lm

