all:
	g++ -Iinclude src/*.h -o bin/main.exe src/*.cpp -municode -lgdi32