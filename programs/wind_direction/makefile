FLAGS = -W
OBJS  = pso_alg/main.c pso_alg/pso.c pso_alg/freq/freq.c ../utils/tools.c pso_alg/params.c
HEAD = pso_alg/freq/freq.h pso_alg/pso.h ../utils/tools.h pso_alg/params.h
NAME = pso 

$(NAME): $(OBJS) $(HEAD)
	gcc $(FLAGS) -o $(NAME) $(OBJS) -lm -lgsl -lgslcblas

