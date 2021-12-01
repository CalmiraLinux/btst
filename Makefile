all:
	g++ src/main.cpp -o btst
	strip btst

install:
	cp btst /usr/bin/btst

