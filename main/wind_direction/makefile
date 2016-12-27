FLAGS = -W
OBJS  = ../../src/wind_direction/main.c ../../src/wind_direction/pso.c ../../src/wind_direction/freq/freq.c ../../src/utils/tools.c ../../src/wind_direction/params.c
HEAD = ../../src/wind_direction/freq/freq.h ../../src/wind_direction/pso.h ../../src/utils/tools.h ../../src/wind_direction/params.h
NAME = pso 

$(NAME): $(OBJS) $(HEAD)
	gcc $(FLAGS) -o $(NAME) $(OBJS) -lm -lgsl -lgslcblas

