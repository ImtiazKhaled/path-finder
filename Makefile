all:
	g++ find_route.cpp -o find_route
	./find_route input1.txt Munich Kassel
	./find_route input1.txt Munich Kassel h_kassel.txt
	./find_route input1.txt Berlin Kassel
	./find_route input1.txt Berlin Kassel h_kassel.txt
	./find_route input1.txt Luebeck Kassel
	./find_route input1.txt Luebeck Kassel h_kassel.txt
	./find_route input1.txt London Kassel