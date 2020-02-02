all:
	g++ find_route.cpp -o find_route
	./find_route input1.txt Bremen Kassel
	./find_route input1.txt Karlsruhe Duesseldorf
	./find_route input1.txt London Kassel
	./find_route input1.txt Saarbruecken Kassel h_kassel.txt
	./find_route input1.txt Bremen Kassel h_kassel.txt