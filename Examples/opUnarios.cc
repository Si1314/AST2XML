#include "BuiltinsIO.h"

// Not lógico
bool notA(bool a){
	return !a;
}

// Menos unario
int invA(int a){
	return -a;
}

// Más unario
int retA(int a){
	return +a;
}

// Incremento
int incrA(int a){
	int b = a;
	b++;
	return b;
}

// Decremento
int decrA(int a){
	int b = a;
	b--;
	return b;
}

