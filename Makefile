connect4: connect4.cpp
	g++ -g -lm -o c4 connect4.cpp -lncurses
clean:
	rm -f c4
