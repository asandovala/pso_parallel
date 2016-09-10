#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pso.h"
#include "freq/freq.h"
#include "../utils/tools.h"
#include "params.h"

#define PI 3.14159265358979323846

double range_u[] = {0.0, 2 * PI};
double range_k[] = {0.0, 700.0};

double limit_u[] = {0.0, 1.0};
double limit_k[] = {0.0, 1.0};
double limit_w[] = {0.0, 1.0};

double w_range[] = {0.4, 0.9};
double c1_range[] = {1.0, 3.5};
double c2_range[] = {0.0, 2.5};

struct swarm * initializeSwarm(int particles, int w, int c1, int c2) {
    int i, j;
    struct swarm *s;
    struct particle *p;

    loadFreqData();

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
    double best_result = 1000.0;
    double fitness;
    double value;

    s->global_best = malloc(sizeof(double) * LEN_SOL);
    getInitialSolution(s->global_best);

/*    s->global_best = malloc(sizeof(double) * LEN_SOL);
   
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
*/
    return s;
}

void initializeParticle(struct particle *p) {
    double *p_best = malloc(sizeof(double) * LEN_SOL); 
    double *pos = malloc(sizeof(double) * LEN_SOL); 
    double *vel = malloc(sizeof(double) * LEN_SOL); 
    double *data;
    int from;
    int to;
    int i;
    int sectors = NUMBER_OF_CLASSES/MIXTURE_AMOUNT;

    p->position = pos;
    p->velocity = vel;
    p->particle_best = p_best;

    for (i = 0; i < MIXTURE_AMOUNT; i++) {
        p->velocity[i] = 0.0;
        p->velocity[i + MIXTURE_AMOUNT] = 0.0;
        p->velocity[i + 2 * MIXTURE_AMOUNT] = 0.0;

        from = i * MIXTURE_AMOUNT;
        to = (i + 1) * MIXTURE_AMOUNT;
        
        //data = getRangeOfData(DATA_DIRECTION.rawData, from, to);  

        p->position[i] = rand01(); 
        p->position[i + MIXTURE_AMOUNT] = rand01();
        p->position[i + MIXTURE_AMOUNT * 2] = rand01();

        p->particle_best[i] = p->position[i];
        p->particle_best[i + MIXTURE_AMOUNT] = p->position[i + MIXTURE_AMOUNT];
        p->particle_best[i + MIXTURE_AMOUNT * 2] = p->position[i + MIXTURE_AMOUNT * 2];
    
    }

    p->lenSol = LEN_SOL;
}

void getInitialSolution(double *global_best) {
    double *data;
    int from;
    int to;
    int i;
    int sectors = NUMBER_OF_CLASSES/MIXTURE_AMOUNT;
    int allData = DATA_DIRECTION.len;
    int sectorData = (allData / MIXTURE_AMOUNT);

    for (i = 0; i < MIXTURE_AMOUNT; i++) {

        from = i * sectorData;
        to = (i + 1) * sectorData;
        if (i == MIXTURE_AMOUNT - 1) {
            to = (i + 1) * sectorData + allData % MIXTURE_AMOUNT;
        }
         
        printf("From: %d To: %d \n", from, to); 
        data = getRangeOfData(DATA_DIRECTION.rawData, from, to);  

        global_best[i] = getPrevailingDirection(data) / range_u[1]; 
        global_best[i + MIXTURE_AMOUNT] = getConcentration(data) / range_k[1]; 

        from = i * sectors;
        to = (i + 1) * sectors;
        if (i == MIXTURE_AMOUNT - 1) {
            to = (i + 1) * sectors + NUMBER_OF_CLASSES % MIXTURE_AMOUNT;
        }

        global_best[i + MIXTURE_AMOUNT * 2] = getWeightAproximation(DATA_DIRECTION.classesFrequencies, from, to);
    }

}
/*
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

void checkLimitPosition(double *pos) {
    int i;
    for (i = 0; i < LEN_SOL; i++) {
        if (pos[i] > sol_max[i]) {
            pos[i] = sol_min[i];
        } else if (pos[i] < sol_min[i]) {
            pos[i] = sol_max[i];
        }
    }
}

double fWeibull(double *position, double velocity) {
    float k = position[0];
    float c = position[1];
    float e = exp(1.0);
    float v = velocity;

    float factor1 = (k/c);
    float factor2 = pow((v/c), k-1);
    float factor3 = pow(e, -pow((v/c), k));

    return factor1 * factor2 * factor3; 
}

double objectiveFunction(double *position) {
    float sum = 0;
    float rReal;
    float rWeibull;
    float vel;
    int i;

    //TODO Programar la Xi 2

    for (i = 0; i < vel_freq.len; i++) {
        vel = vel_freq.data[i][0];
        rReal = vel_freq.data[i][1];
        rWeibull = fWeibull(position, vel);
        sum += pow(rReal - rWeibull, 2);
    }

    return 0.5 * sum;
}
*/