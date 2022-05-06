all:
	g++ -Iinclude src/*.h -o bin/counter.exe src/*.cpp -mwindows -municode -lgdi32