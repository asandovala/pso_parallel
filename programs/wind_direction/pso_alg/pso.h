#ifndef PSO_
#define PSO_

#define MAX_ITER 1000
#define PARTICLES 100
#define MIXTURE_AMOUNT 7
#define AMOUNT_VECTORS_SOLUTION 3
#define LEN_SOL MIXTURE_AMOUNT * AMOUNT_VECTORS_SOLUTION

#define PARAM_W 0.4//9.0
#define PARAM_C1 1.0//3.5
#define PARAM_C2 0.0//2.5

#define W_A 0.5 //Alpha weight for inertia param
#define W_B 1.5 //Beta weight for cognitive param
#define W_G 1.0 //Gamma weight for social param

#define EXPECTED_FITNESS 20.0

struct particle {
    double *position;        //solution of the particle.    
    double *velocity;        //vector of movement.    
    double *particle_best;   //best historical position.
    int lenSol;             //lenght of the vector solution.
};

struct swarm {
    struct particle **particles;     //All of the swarm's particles.
    int total_particles;            //Number of the aprticles in the swarm.
    double *global_best;             //Best solution of all particles.
    double w_inertia;                //Inertia factor.
    double c1_cognitive;             //Cognitive factor.
    double c2_social;                //Social factor.
};

/*
 * Instance a swarm, create her particles and set the parameters for
 * the movement's function.
 * total_particles: number of particles for the swarm.
 * w: Inertia factor.
 * c1: Cognitive factor.
 * c2: Social factor.
 * lenSol: Lenght of the solution (or dimension).
 * 
 * @return: A pointer to the swarm struct created. 
 */
struct swarm * initializeSwarm(int particles, char * file_path);

void initializeParticle(struct particle *p, struct swarm *s);

/*
 * Update the particles in the swarm accordly to her params in the
 * struct and the movement's function.
 */
void updateSwarm(struct swarm *s);

/*
 * Make parameters dynamic to avoid fast convergence
 */
void updateParameters(struct swarm *s, double fitness);

/*
 * Search for the best result between all particles and set it 
 * to the global_best pointer.
 */
void saveGlobalBest(struct swarm *s);

/*
 * Update velocity accordly the current values of swarm, a given
 * particle and the velocity function designed. 
 */
void updateParticleVelocity(struct swarm *s, struct particle *p);

/*
 * Update position of the particle accordly to her velocity
 */
void updateParticlePosition(struct particle *p);

/*
 * Update the best own result for all particles
 */
void saveParticleBest(struct particle *p); 

/*
 * Check position to avoid go out of the search space
 */
void checkLimitPosition(double *pos);

/*
 * The objective function.
 * return the fitnes of the given vector.
 */
double objectiveFunction(double *position);

double vonMises(double angle, double u, double k);

double mixtureVonMises(double angle, double *solution);

double probabilityWindDirection(double *position, int sector);

void getInitialSolution(double *global_best);

double bessi0( double x );

#endif 
