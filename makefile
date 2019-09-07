run: task4
	./task4
task4: main.o lexem.o syntax.o
	g++ main.o lexem.o syntax.o -o task4
main.o: main.cpp lexem.h syntax.h
	g++ -c main.cpp
lemex.o: lexem.cpp lexem.h
	g++ -c lexem.cpp
syntax.o: syntax.cpp syntax.h lexem.h
	g++ -c syntax.cpp
clean:
	rm main.o lexem.o syntax.o task4