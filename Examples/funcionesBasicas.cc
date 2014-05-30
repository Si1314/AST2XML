#include "BuiltinsIO.h"

int potencia(int a, int b){
	if(b>=0){
		int c = b;
		int r = 1;
		while(c>0){
			r *= a;
			c--;
		}
		return r;
	}else{
		return -1;
	}
}

int multDiez(int a){
	int b;
	if(a<0){
		b = -a;
	}else{
		b = a;
	}
	int c = 10;
	int r = 0;
	while(c > 0){
		r += a;
		c--;
	}
	return r;
}

int dimeSiPos(int a){
	int r;
	if(a>=0){
		r = 1;
	}else{
		r = 0;
	}
	return r;
}


int factorial(int a){
	if(a>=0){
		int fact = 1;
		for(int i=2;i<=a;i++){
			fact *=i;
		}
		return fact;
	}else{
		return -1;
	}
}

int fibonacci(int a){
	if(a<0) return -1;
	if(a==0 || a==1) return 1;
	int fibo1 = 1;
	int fibo2 = 1;
	int auxf;
	for(int i=2; i<a; i++){
		auxf = fibo1 + fibo2;
		fibo1 = fibo2;
		fibo2 = auxf;
	}
	return fibo1 + fibo2;
}

int introduceEntero(){
	return ConsoleIn_Int();
}

int reproduceEntero(int a){
	ConsoleOut_Int(a);
	return 0;
}

int llamadaFactorial(int a){
	return factorial(a);
}

int ifLlamada(int a, int b){
	int r;
	if(a<=0){
		r = factorial(b);
	}else{
		r = potencia(b,a);
	}
	return r;
}

int pruebaIf01(int a){
	if (a==0) return 1;
	if (a>0) return 3;
	if (a<0) return -2;
}

int pruebaIf02(int a){
	if(a>=0){
		return 2;
	}else{
		return -2;
	}
}

int pruebaIf03(int a){
	if (a>=0){ 
		return 2;
	}
	return 3;
}

int pruebaIf04(int a){
	if (a==0) return 1;
	else return -1;
}

int pruebaIf05(int a){
	if (a==0) return 1;
	else {return -1;}
}

int pruebaFor01(int a){
	if(a>0){
		int r = 0;
		for(int i=0;i<10;i++){
			r += a;
		}
		return r;
	}else{
		return -1;
	}
}

int pruebaWhile01(int a){
	int r = a;
	while(r>0){
		ConsoleOut_Int(r);
		r--;
	}
	return 0;
}