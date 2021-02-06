#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#define numthreads 3

typedef struct{
	double x, y, z;
} Cord;

typedef struct{
	Cord *vetor1, *vetor2, *vetorRes;
} ArgCross;

void inicia(), *calccampo(void *k), *calccampoeff(), *calcvect(void *arg), calcTudo(Cord* desc_, Cord* aux_), reseta();
void Calcdsdt(), *Resetx(), *Resety(), *Resetz(), Reset(), *attx(), *atty(), *attz(), att(), escreve(int *i), Integra();
void Checa(int qnt, char*argsv[]), Dados();
ArgCross *SetParam(Cord* vetor1, Cord* vetor2, Cord* vetorRes);

Cord *s0, *s, *H, *dsdt, *saida, *Heff;
ArgCross* saida_;
FILE *fp, *ani, *dados, *field;
Cord *vects[numthreads * 2];
Cord *aux, *r1, *r2, *desc;
pthread_t threads[numthreads];
int n = 1000;
double tmax = 10.0;
double tmin = 0.0;
double lambda = 0.0;
double t, *taux = NULL, h;
int corte = 10;

void inicia(){
	saida_ = malloc(sizeof(ArgCross));
	aux = malloc(sizeof(Cord));
	Heff = malloc(sizeof(Cord));
	saida = malloc(sizeof(Cord));
	r1 = malloc(sizeof(Cord));
	r2 = malloc(sizeof(Cord));
	desc = malloc(sizeof(Cord));
	taux = malloc(sizeof(double));
	desc->x = desc->y = desc->z = 0;
	h = (tmax - tmin) / n;
	fp = fopen("out.dat", "w+");
	field = fopen("field.dat", "w+");
	s0 = malloc(sizeof(Cord));
	s0->x = 1.0 / sqrt(3.0);
	s0->y = 1.0 / sqrt(3.0);
	s0->z = 1.0 / sqrt(3.0);
	s = malloc(sizeof(Cord));
	H = malloc(sizeof(Cord));
	dsdt = malloc(sizeof(Cord));
	calccampo((void*)taux);
	for(int i = 0; i < numthreads * 2; i++){
		vects[i] = malloc(sizeof(Cord));
	}
	reseta();
}

void* calccampo(void *k){
	double k_ = *(double*) k;
	H->x = 0.0;
	H->y = 0.0;
	H->z = 1.0;
	return NULL;
}

void* calccampoeff(){
	calcvect((void*)SetParam(aux, H, saida));
	Heff->x = H->x + lambda * saida->x;
	Heff->y = H->y + lambda * saida->y;
	Heff->z = H->z + lambda * saida->z;
}

void* calcvect(void *arg){
	ArgCross* k = (ArgCross*) arg;
	k->vetorRes->x = k->vetor1->y * k->vetor2->z - k->vetor1->z * k->vetor2->y;
	k->vetorRes->y = -(k->vetor1->x * k->vetor2->z - k->vetor1->z * k->vetor2->x);
	k->vetorRes->z = k->vetor1->x * k->vetor2->y - k->vetor1->y * k->vetor2->x;
}

ArgCross *SetParam(Cord* vetor1, Cord* vetor2, Cord* vetorRes){
	saida_->vetor1 = vetor1;
	saida_->vetor2 = vetor2;
	saida_->vetorRes = vetorRes;
	return saida_;
}
void calcTudo(Cord* desc_, Cord* aux_){
	aux_->x += desc_->x;
	aux_->y += desc_->y;
	aux_->z += desc_->z;
	calccampoeff();
	calcvect((void*)SetParam(aux_, Heff, vects[0]));
	calcvect((void*)SetParam(aux_, Heff, vects[1]));
	calcvect((void*)SetParam(aux_, Heff, vects[2]));
}

void reseta(){
	aux->x = s0->x;
	aux->y = s0->y;
	aux->z = s0->z;
}

void Calcdsdt(){
	reseta();
	calcTudo(desc, aux);
	r1->x = (vects[0]->x) * h;
	r1->y = (vects[1]->y) * h;
	r1->z = (vects[2]->z) * h;
	reseta();
	calcTudo(r1, aux);
	r2->x = (vects[0]->x) * h;
	r2->y = (vects[1]->y) * h;
	r2->z = (vects[2]->z) * h;
	reseta();
	dsdt->x = (r1->x + r2->x) / 2.0;
	dsdt->y = (r1->y + r2->y) / 2.0;
	dsdt->z = (r1->z + r2->z) / 2.0;
}


void* Resetx(){
	s0->x = s->x;
}
void* Resety(){
	s0->y = s->y;
}
void* Resetz(){
	s0->z = s->z;
}

void Reset(){
	pthread_create(&threads[0], NULL, Resetx, NULL);
	pthread_create(&threads[1], NULL, Resety, NULL);
	pthread_create(&threads[2], NULL, Resetz, NULL);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
}

void* attx(){
	s->x = s0->x + dsdt->x;
}
void* atty(){
	s->y = s0->y + dsdt->y;
}
void* attz(){
	s->z = s0->z + dsdt->z;
}

void att(){
	pthread_create(&threads[0], NULL, attx, NULL);
	pthread_create(&threads[1], NULL, atty, NULL);
	pthread_create(&threads[2], NULL, attz, NULL);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
}

void escreve(int *i){
	if(*i % corte == 0){
	fprintf(fp, "%.12f \t %.12f \t %.12f\n", s->x, s->y, s->z);
	fprintf(field, "%.12f \t %.12f \t %.12f\n", H->x, H->y, H->z);
	}
	//fprintf(fp, "0.0 \t %.12f \t %.12f \t %.12f\n", s->x, s->y, s->z);
}

void Integra(){
	for(int i = 0; i <= n; i++){
		double t = i * h;
		calccampo((void*)&t);
		Calcdsdt();
		att();
		escreve(&i);
		Reset();
	}
}

void Checa(int qnt, char*argsv[]){
	if (qnt == 6){
		n = atoi(argsv[1]);
		tmin = atof(argsv[2]);
		tmax = atof(argsv[3]);
		lambda = atof(argsv[4]);
		corte = atoi(argsv[5]);
	}else if(qnt == 5){
		n = atoi(argsv[1]);
		tmin = atof(argsv[2]);
		tmax = atof(argsv[3]);
		lambda = atof(argsv[4]);
	}else if(qnt == 4){
		n = atoi(argsv[1]);
		tmin = atof(argsv[2]);
		tmax = atof(argsv[3]);
	}else if(qnt == 3){
		n = atoi(argsv[1]);
		tmax = atof(argsv[2]);
	}else if(qnt == 2){
		n = atoi(argsv[1]);
	}
}

void Dados(){
	dados = fopen("data.dat", "w+");
	printf("Starting system:\nn = %d\nlambda = %.5f\n(tmin, tmax) = (%.5f, %.5f)\nh = %.5f\ncut = %d\nRunge-Kutta 2\n", n, lambda, tmin, tmax, h, corte);
	fprintf(dados, "Starting system:\nn = %d\nlambda = %.5f\n(tmin, tmax) = (%.5f, %.5f)\nh = %.5f\ncut = %d\nRunge-Kutta 2\n", n, lambda, tmin, tmax, h, corte);
}