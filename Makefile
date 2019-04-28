# дефайн нужен для поддержки wchar
COMPILER = g++ --std=c++11 -Wall -D_XOPEN_SOURCE_EXTENDED `pkg-config --cflags ncursesw`
FLAGS = `pkg-config --libs ncursesw` `pkg-config --libs sqlite3` -lpthread

# При изменении любого .h всё нужно перекомпилить (т.к. инклюды)
HEADERS = src/*.h

launch: vktui.exe
	./vktui.exe

vktui.exe: main.o Backend.o Database.o Frontend.o Network.o
	$(COMPILER) -o vktui.exe $^ $(FLAGS)


# основные классы
main.o: src/main.cpp $(HEADERS)
	$(COMPILER) -c $<
Backend.o: src/Backend.cpp $(HEADERS)
	$(COMPILER) -c $<
Database.o: src/Database.cpp $(HEADERS)
	$(COMPILER) -c $<
Frontend.o: src/Frontend.cpp $(HEADERS)
	$(COMPILER) -c $<
Network.o: src/Network.cpp $(HEADERS)
	$(COMPILER) -c $<


clean:
	rm -rf *.o *.exe
