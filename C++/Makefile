all: main clean run


main: main.cpp
	echo "Creating a game file" 
	gcc -c main.cpp
	g++ main.o -o game -lsfml-graphics -lsfml-window -lsfml-system

clean: 
	rm -f ./main.o
	echo "Game File ready!!!!"
	
run: 
	clear
	./game
