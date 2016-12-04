#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pso.h"
#include "freq/freq.h"
#include "../utils/tools.h"
#include "params.h"
//#include <gsl/gsl_sf_bessel.h>
//#include "bessel.c"

#define PI 3.14159265358979323846

double range_u[] = {0.0, 2 * PI};
double range_k[] = {0.0, 700.0};

double limit_u[] = {0.0, 1.0};
double limit_k[] = {0.0, 1.0};
double limit_w[] = {0.0, 1.0};

//double w_range[] = {0.89, 0.89};
double w_range[] = {0.4, 0.9};
//double c1_range[] = {0.5, 0.5};
double c1_range[] = {0.5, 2.5};
//double c2_range[] = {0.7, 0.7};
double c2_range[] = {0.5, 2.5};

struct swarm * initializeSwarm(int particles, char * file_path) {
    int i, j;
    struct swarm *s;
    struct particle *p;

    
    //printf("\n FILE: %s\n", file_path);
    loadFreqData(file_path);

    s = (struct swarm *) malloc(sizeof(struct swarm));
    s->particles = malloc(sizeof(struct particle) * particles);

    s->total_particles = particles;
    s->w_inertia = w_range[0];
    s->c1_cognitive = c1_range[0];
    s->c2_social = c2_range[1];

    //init global best
    s->global_best = malloc(sizeof(double) * LEN_SOL);
    getInitialSolution(s->global_best);

    //set initial global_best
    for (i = 0; i < particles; i++) {
        p = malloc(sizeof(struct particle));
        initializeParticle(p, s);
        s->particles[i] = p;        
    }

    return s;
}

void initializeParticle(struct particle *p, struct swarm *s) {
    double *p_best = malloc(sizeof(double) * LEN_SOL); 
    double *pos = malloc(sizeof(double) * LEN_SOL); 
    double *vel = malloc(sizeof(double) * LEN_SOL); 
    //double *data;
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

        p->position[i] = s->global_best[i] + rand01() * 0.01; 
        p->position[i + MIXTURE_AMOUNT] = s->global_best[i + MIXTURE_AMOUNT] + rand01() * 0.01;
        p->position[i + MIXTURE_AMOUNT * 2] = s->global_best[i + MIXTURE_AMOUNT * 2] + rand01() * 0.01;

        p->particle_best[i] = p->position[i];
        p->particle_best[i + MIXTURE_AMOUNT] = p->position[i + MIXTURE_AMOUNT];
        p->particle_best[i + MIXTURE_AMOUNT * 2] = p->position[i + MIXTURE_AMOUNT * 2];
    
    }

    checkLimitPosition(p->position);
    checkLimitPosition(p->particle_best);

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

    int j;

    for (i = 0; i < MIXTURE_AMOUNT; i++) {

        from = i * sectorData;
        to = (i + 1) * sectorData;
        if (i == MIXTURE_AMOUNT - 1) {
            to = (i + 1) * sectorData + allData % MIXTURE_AMOUNT;
        }
         
        data = getRangeOfData(DATA_DIRECTION.rawData, from, to);  

        global_best[i] = getPrevailingDirection(data, to - from) / range_u[1]; 
       
        global_best[i + MIXTURE_AMOUNT] = getConcentration(data, to - from) / range_k[1]; 

        from = i * sectors;
        to = (i + 1) * sectors;
        if (i == MIXTURE_AMOUNT - 1) {
            to = (i + 1) * sectors + NUMBER_OF_CLASSES % MIXTURE_AMOUNT;
        }

        global_best[i + MIXTURE_AMOUNT * 2] = getWeightAproximation(from, to);
    }
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

void updateParameters(struct swarm *s, double fitness) {
    double percent = (EXPECTED_FITNESS / fitness);
    if (percent > 1) {
        percent = 0;
    }

    //s->w_inertia = pow((1.0 - (double)time/MAX_ITER), W_A) * (w_range[1] - w_range[0]) + w_range[0];
    s->w_inertia = (w_range[1] - s->w_inertia) * percent + s->w_inertia;
    //s->c1_cognitive = pow((1.0 - (double)time/MAX_ITER), W_B) * (c1_range[1] - c1_range[0]) + c1_range[0];
    s->c1_cognitive = (c1_range[1] - s->c1_cognitive) * percent + s->c1_cognitive;
    //s->c2_social = pow((1.0 - (double)time/MAX_ITER), W_G) * (c2_range[1] - c2_range[0]) + c2_range[1];
    s->c2_social = (c2_range[0] - s->c2_social) * percent + s->c2_social;
}

void saveGlobalBest(struct swarm *s) {
    double best_result = objectiveFunction(s->global_best);
    double fitness;
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

            updateParameters(s, best_result);    

            //printf("\n New best: %d \n", i);
            //for ( j = 0; j < LEN_SOL; j++) {
                //printf(" %.5f,", s->global_best[j]);        
            //}

            //printf("\n Fitness: %lf \n", best_result);
        }
    }
}

void updateParticleVelocity(struct swarm *s, struct particle *p) {
    double w = s->w_inertia;
    double c1 = s->c1_cognitive;
    double c2 = s->c2_social;
    double rnd1 = rand01(); //0 to 1
    double rnd2 = rand01();
    double vel;
    double diff_p_best;
    double diff_g_best;
    int i;

    for (i = 0; i < LEN_SOL; i++) {
        vel = p->velocity[i];
        diff_p_best = p->particle_best[i] - p->position[i];
        diff_g_best = p->particle_best[i] - s->global_best[i];
        p->velocity[i] = w * vel + c1 * rnd1 * diff_p_best + c2 * rnd2 * diff_g_best; 
    }
}

void updateParticlePosition(struct particle *p) {
    int i;

    for (i = 0; i < LEN_SOL; i++) {
        p->position[i] += p->velocity[i]; 
        checkLimitPosition(p->position);
    }

    //printf("Pos: %.2f, %.2f\n", p->position[0], p->position[1]);
}

void saveParticleBest(struct particle *p) {
    int i;
    double current_best = objectiveFunction(p->particle_best);
    double current_value = objectiveFunction(p->position);
    //double current_best = 0.0;

    if (current_value < current_best) {
        for (i = 0; i < LEN_SOL; i++) {
            p->particle_best[i] = p->position[i]; 
        }   
    }
} 

void checkLimitPosition(double *pos) {
    int i, j;
    double sum = 0.0;
    double diff;

    for (i = 0; i < MIXTURE_AMOUNT; i++) {

        if (pos[i] > 1.0) { 
            pos[i] = 1.0; 
        } else if (pos[i] < 0.0) {
            pos[i] = 0.0;     
        }

        if (pos[i + MIXTURE_AMOUNT] > 1.0) { 
            pos[i + MIXTURE_AMOUNT] = 1.0; 
        } else if (pos[i + MIXTURE_AMOUNT] < 0.0) {
            pos[i + MIXTURE_AMOUNT] = 0.0;     
        }

        if (pos[i + MIXTURE_AMOUNT * 2] > 1) { 
            pos[i + MIXTURE_AMOUNT * 2] = 1.0; 
        } else if (pos[i + MIXTURE_AMOUNT * 2] < 0) {
            pos[i + MIXTURE_AMOUNT * 2] = 0.0;     
        }
            
    }

    for (i = 0; i < MIXTURE_AMOUNT; i++) { //Keep the constraint for the weights
        sum += pos[i + MIXTURE_AMOUNT * 2];
    }

    if (sum > 1.0) {
        for (i = 0; i < MIXTURE_AMOUNT; i++) { //Keep the constraint for the weights
            pos[i + MIXTURE_AMOUNT * 2] = pos[i + MIXTURE_AMOUNT * 2] / sum;
        }  
    } else if (sum < 1.0) {
        diff = 1.0 - sum;
        diff = diff / MIXTURE_AMOUNT;
        for (i = 0; i < MIXTURE_AMOUNT; i++) { //Keep the constraint for the weights
            pos[i + MIXTURE_AMOUNT * 2] += diff;
        }    
    }

    sum = 0.0;
    for (i = 0; i < MIXTURE_AMOUNT; i++) {
        sum += pos[i + MIXTURE_AMOUNT * 2];
    }

    if (sum > 1.1 || sum < 0.9) {
        printf("No se cumplió la restricción \n");
        for (i = 0; i < MIXTURE_AMOUNT; i++) {
            printf("params: %lf \n", pos[i + MIXTURE_AMOUNT * 2]);
        }
    }
}

//VonMises
double vonMises(double angle, double u, double k) {
    u *= range_u[1];
    k *= range_k[1];
    //return exp(k * cos(angle - u)) / (2.0 * PI * gsl_sf_bessel_I0(k));
    double result = exp(k * cos(angle - u)) / (2.0 * PI * bessi0(k));
    if (result < 0) {
        printf("Error, negative probability %lf \n", result);
    }
    return result;
}

double mixtureVonMises(double angle, double *solution) {
    int i;
    double sum = 0.0;
    double w, u, k;

    for (i = 0; i < MIXTURE_AMOUNT; i++) {
        u = solution[i];
        k = solution[i + MIXTURE_AMOUNT];
        w = solution[i + MIXTURE_AMOUNT * 2];
        sum += w * vonMises(angle, u, k);
    }

    return sum;
}

double probabilityWindDirection(double *position, int class) {
    //double precision = 0.017453292; // ~ 1º
    //double precision = 0.17453292; // ~ 1º
    double finalAngle;
    double angle;
    double sum = 0.0;

    double lenClass = ((2.0 * PI) / NUMBER_OF_CLASSES);
    double precision = lenClass/16.0;
    double from = class * lenClass;
    double to = (class + 1) * lenClass;

    angle = from;
    finalAngle = to;


    while (angle < finalAngle) {
        sum += mixtureVonMises(angle, position) * precision;
        //sum += mixtureVonMises(angle, position);
        angle += precision;
    }


    return sum;
    //return sum * lenClass;

    //return mixtureVonMises((to + from) * 0.5, position) * lenClass;
    //return mixtureVonMises(to, position);
}

//La Xi cuadrado
double objectiveFunction(double *position) {
    int i;
    double Oi;
    double pi;
    double sum = 0.0;
    double sumProb = 0.0;
    double n = DATA_DIRECTION.sumAllFreq;
    double lenClass = ((2.0 * PI) / NUMBER_OF_CLASSES);

    for (i = 0; i < NUMBER_OF_CLASSES; i++) {
        Oi = DATA_DIRECTION.classesFrequencies[i];
        pi = probabilityWindDirection(position, i);

        //sum += pow((Oi - n*pi), 2)/(n*pi);
        sum += pow((Oi - n*pi), 2)/(n*pi);
    }

    //printf(" %lf \n", sumProb);
 
    return sum; 
}

double bessi0( double x )
/*------------------------------------------------------------*/
/* PURPOSE: Evaluate modified Bessel function In(x) and n=0.  */
/*------------------------------------------------------------*/
{
   double ax,ans;
   double y;


   if ((ax=fabs(x)) < 3.75) {
      y=x/3.75,y=y*y;
      ans=1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492
         +y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
   } else {
      y=3.75/ax;
      ans=(exp(ax)/sqrt(ax))*(0.39894228+y*(0.1328592e-1
         +y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
         +y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
         +y*0.392377e-2))))))));
   }
   return ans;
}