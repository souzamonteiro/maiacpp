#include <iostream>
using namespace std;

int main(void) {
	int a;
	float b;
	char c;
	char d[] = "Alo Mundo!";
	
	int *pi;
	float *pf;
	char *pc;
	
	a = 1;
	b = 2;
	c = 'x';
	
	pi = &a;
	pf = &b;
	pc = &c;
	
	printf("Endereco apontado por pi: %ld, valor contido na posicao indicada por pi: %d\n", pi, *pi);
	printf("Endereco apontado por pf: %ld, valor contido na posicao indicada por pf: %f\n", pf, *pf);
	printf("Endereco apontado por pc: %ld, valor contido na posicao indicada por pc: %c\n", pc, *pc);
	
	pc = d;
	printf("Endereco apontado por pc: %ld, valor contido na posicao indicada por pc: %c\n", pc, *pc);
	pc++;
	printf("Endereco apontado por pc: %ld, valor contido na posicao indicada por pc: %c\n", pc, *pc);
	(*pc)++;
	printf("Endereco apontado por pc: %ld, valor contido na posicao indicada por pc: %c\n", pc, *pc);
	
	cout << d << endl;
	
	return 0;
}