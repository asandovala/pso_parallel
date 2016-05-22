#ifndef PSO_
#define PSO_

struct particle {
    float *position;
    float *velocity;
    float *particle_best;
};

struct swarm {
    struct particle *particles;
    int total_particles;
    float *global_best;
    float w_inertia;
    float c1_cognitive;
    float c2_social;
};

struct swarm initializeSwarm(int total_particles, int w, int c1, int c2);

void updateSwarm(struct swarm *s);

void saveGlobalBest(struct swarm *s);

void updateParticlePosition(struct particle *p, float movement);

void saveParticleBest(struct particle *p); 

#endif 
