all: historico.o paciente.o fila_por_prioridade.o relacao_de_pacientes.o IO.o main.o
	gcc historico.o paciente.o fila_por_prioridade.o relacao_de_pacientes.o IO.o main.c -o main -std=c99 -Wall -lm
historico.o:
	gcc -c historico.c -o historico.o
paciente.o:
	gcc -c paciente.c -o paciente.o
fila_por_prioridade.o:
	gcc -c fila_por_prioridade.c -o fila_por_prioridade.o
relacao_de_pacientes.o:
	gcc -c relacao_de_pacientes.c -o relacao_de_pacientes.o
IO.o:
	gcc -c IO.c -o IO.o
run:
	./main
clean:
	rm *.o
memory:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main
