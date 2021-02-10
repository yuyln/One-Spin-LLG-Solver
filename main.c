#include <stdio.h>
#include <stdlib.h>
#include "rk4.c"


int main(int qnt, char*argsv[]){
	Checa(qnt, argsv);
	inicia();
	Dados();
	Integra();
	return 0;
}