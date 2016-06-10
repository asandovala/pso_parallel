#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pso.h"
#include "freq/freq.h"

#define LEN_SOL 2
int sol_max[] = {20, 20};
int sol_min[] = {0, 0};

struct swarm * initializeSwarm(int particles, int w, int c1, int c2) {
    int i;
    struct swarm *s;
    struct particle *p;

    s = malloc(sizeof(struct swarm));
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

    loadFreqData();

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
        p->position[i] = rand() % sol_max[i]; 
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

void saveGlobalBest(struct swarm *s) {
    float best_result = objectiveFunction(s->global_best);
    float fitness;
    int particles = s->total_particles;
    int i;
    struct particle *p;

    for (i = 0; i < particles; i++) {
        p = s->particles[i];
        fitness = objectiveFunction(p->position);
        if (fitness < best_result) { 
            best_result = fitness;
            s->global_best = p->position;         
        }
    }
}

void updateParticleVelocity(struct swarm *s, struct particle *p) {
    float w = s->w_inertia;
    float c1 = s->c1_cognitive;
    float c2 = s->c2_social;
    float rnd1 = (rand() % 101)/100.0; //0 to 1
    float rnd2 = (rand() % 101)/100.0;
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
    }
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
        sum += pow(vel_freq.data[i][1] - fWeibull(position, vel), 2);
    }

    return 0.5 * sum;
}
