
main : main.cpp node.hpp message.h WNode
	g++ -std=c++11 -o main main.cpp -lzmq

WNode : fornodes.cpp node.hpp message.h
	g++ -std=c++11 -o WNode fornodes.cpp -lzmq

clean:
	rm main WNode