#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pso.h"
#include "freq/freq.h"
#include "../utils/tools.h"

float sol_max[] = {20.0, 20.0};
float sol_min[] = {1.0, 1.0};

float w_range[] = {0.4, 0.9};
float c1_range[] = {1.0, 3.5};
float c2_range[] = {0.0, 2.5};

struct swarm * initializeSwarm(int particles, int w, int c1, int c2, char * file_path) {
    int i, j;
    struct swarm *s;
    struct particle *p;

    loadFreqData(file_path);

    s = (struct swarm *) malloc(sizeof(struct swarm));
    s->particles = malloc(sizeof(struct particle) * particles);

    s->total_particles = particles;
    s->w_inertia = w;
    s->c1_cognitive = c1;
    s->c2_social = c2;

    //set initial global_best
    for (i = 0; i < particles; i++) {
        p = malloc(sizeof(struct particle));
        initializeParticle(p);
        s->particles[i] = p;        
    }

    //init global best
    float best_result = 1000.0;
    float fitness;
    float value;

    s->global_best = malloc(sizeof(float) * LEN_SOL);
    
    for (i = 0; i < particles; i++) {
        p = s->particles[i];
        fitness = objectiveFunction(p->position);
        if (fitness < best_result) { 
            best_result = fitness;
            for (j = 0; j < LEN_SOL; j++) {
                value = p->position[j];
                s->global_best[j] = value;
            }     
        }
    }   

    return s;
}

void initializeParticle(struct particle *p) {
    float *p_best = malloc(sizeof(float) * LEN_SOL); 
    float *pos = malloc(sizeof(float) * LEN_SOL); 
    float *vel = malloc(sizeof(float) * LEN_SOL); 
    int i;
    p->position = pos;
    p->velocity = vel;
    p->particle_best = p_best;
    for (i = 0; i < LEN_SOL; i++) {
        p->velocity[i] = 0.0;
        p->position[i] = rangeDoubleInRange(sol_min[i], sol_max[i]); 
        p->particle_best[i] = p->position[i];
    }
    p->lenSol = LEN_SOL;
}

void updateSwarm(struct swarm *s) {
    int i;
    int particles = s->total_particles;
    struct particle *p;

    for (i = 0; i < particles; i++) {
        p = s->particles[i];
        updateParticleVelocity(s, p);
        updateParticlePosition(p);
        saveParticleBest(p);
    }

    saveGlobalBest(s);
}

void updateParameters(struct swarm *s, int time) {
    s->w_inertia = pow((1.0 - (float)time/MAX_ITER), W_A) * (w_range[1] - w_range[0]) + w_range[0];
    s->c1_cognitive = pow((1.0 - (float)time/MAX_ITER), W_B) * (c1_range[1] - c1_range[0]) + c1_range[0];
    s->c2_social = pow((1.0 - (float)time/MAX_ITER), W_G) * (c2_range[1] - c2_range[0]) + c2_range[1];
}


void saveGlobalBest(struct swarm *s) {
    float best_result = objectiveFunction(s->global_best);
    float fitness;
    int particles = s->total_particles;
    int i, j;
    struct particle *p;

    for (i = 0; i < particles; i++) {
        p = s->particles[i];
        fitness = objectiveFunction(p->position);
        if (fitness < best_result) { 
            //printf("Replace!: %f -> %f | k: %f , c: %f \n", best_result, fitness, p->position[0], p->position[1]);
            best_result = fitness;
            //printf("Replace!: %f -> %f \n", best_result, fitness);
            for (j = 0; j < LEN_SOL; j++) {
                s->global_best[j] = p->position[j];
            }     
        }
    }
}

void updateParticleVelocity(struct swarm *s, struct particle *p) {
    float w = s->w_inertia;
    float c1 = s->c1_cognitive;
    float c2 = s->c2_social;
    float rnd1 = rand01(); //0 to 1
    float rnd2 = rand01();
    float vel;
    float diff_p_best;
    float diff_g_best;
    int i;

    for (i = 0; i < p->lenSol; i++) {
        vel = p->velocity[i];
        diff_p_best = p->particle_best[i] - p->position[i];
        diff_g_best = p->particle_best[i] - s->global_best[i];
        p->velocity[i] = w * vel + c1 * rnd1 * diff_p_best + c2 * rnd2 * diff_g_best; 
    }
}

void updateParticlePosition(struct particle *p) {
    int i;
    for (i = 0; i < p->lenSol; i++) {
        p->position[i] += p->velocity[i]; 
        checkLimitPosition(p->position);
    }

    //printf("Pos: %.2f, %.2f\n", p->position[0], p->position[1]);
}

void saveParticleBest(struct particle *p) {
    int i;
    float current_best = objectiveFunction(p->particle_best);
    float current_value = objectiveFunction(p->position);
    if (current_value < current_best) {
        for (i = 0; i < p->lenSol; i++) {
            p->particle_best[i] = p->position[i]; 
        }   
    }
} 

void checkLimitPosition(float *pos) {
    int i;
    for (i = 0; i < LEN_SOL; i++) {
        if (pos[i] > sol_max[i]) {
            pos[i] = sol_min[i];
        } else if (pos[i] < sol_min[i]) {
            pos[i] = sol_max[i];
        }
    }
}

float fWeibull(float *position, float velocity) {
    float k = position[0];
    float c = position[1];
    float e = exp(1.0);
    float v = velocity;

    float factor1 = (k/c);
    float factor2 = pow((v/c), k-1);
    float factor3 = pow(e, -pow((v/c), k));

    return factor1 * factor2 * factor3; 
}

/*
 * Edit this accordly to the objective function.
 * Note: All comparison between values are thouht as minimization.
 */
float objectiveFunction(float *position) {
    float sum = 0;
    float rReal;
    float rWeibull;
    float vel;
    int i;

    for (i = 0; i < vel_freq.len; i++) {
        vel = vel_freq.data[i][0];
        rReal = vel_freq.data[i][1];
        rWeibull = fWeibull(position, vel);
        sum += pow(rReal - rWeibull, 2);
    }

    return 0.5 * sum;
}
