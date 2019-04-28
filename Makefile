# дефайн нужен для поддержки wchar
COMPILER = g++ --std=c++11 -Wall -D_XOPEN_SOURCE_EXTENDED `pkg-config --cflags ncursesw`
FLAGS = `pkg-config --libs ncursesw` `pkg-config --libs sqlite3` -lpthread

# При изменении любого .h всё нужно перекомпилить (т.к. инклюды)
HEADERS = src/*.h

launch: vktui.exe
	./vktui.exe

vktui.exe: main.o Backend.o Database.o Frontend.o User_input.o Network.o
	$(COMPILER) -o vktui.exe $^ $(FLAGS)


testfront: vktui_frontend_test.exe
	./vktui_frontend_test.exe

vktui_frontend_test.exe: Frontend.o User_input.o frontend_test.o
	$(COMPILER) -o vktui_frontend_test.exe $^ $(FLAGS)


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
User_input.o: src/User_input.cpp $(HEADERS)
	$(COMPILER) -c $<

# вспомогательные классы


# тесты
frontend_test.o: src/tests/frontend_test.cpp $(HEADERS)
	$(COMPILER) -c $<


clean:
	rm -rf *.o *.exe *.log
