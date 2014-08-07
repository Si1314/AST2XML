#include "BuiltinsIO.h"

bool dameValor(bool a){
	return a;
}

int dameValor(int b){
	return b;
}

int suma(int a, int b){
	return a+b;
}

int resta(int a, int b){
	return a-b;
}

int multiplicacion(int a, int b){
	return a*b;
}

int division(int a, int b){
	return a/b;
}

bool ylogica(bool a, bool b){
	return a&&b;
}

bool ologica(bool a, bool b){
	return a||b;
}

bool mayor(int a, int b){
	return a>b;
}

bool mayorIgual(int a, int b){
	return a>=b;
}

bool menor(int a, int b){
	return a<b;
}

bool menorIgual(int a, int b){
	return a<=b;
}

bool igual(int a, int b){
	return a==b;
}

bool distinto(int a, int b){
	return a!=b;
}

int hazWhile(int a){
	int b = a + 1;
	while(b<0){
		b++;
	}
	return b;
}

int hazWhile2(int a){
	int b = 0;
	while(b<5){
		ConsoleOut_Int(a+b);
		b++;
	}
	return b;
}

int forSimple(int a){
	if(a>0){
		for(int i=0;i<a;i++){
			ConsoleOut_Int(i);
		}
		return 0;
	}else{
		return -1;
	}
}