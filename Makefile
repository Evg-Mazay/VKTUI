# дефайн нужен для поддержки wchar
COMPILER = g++ --std=c++11 -Wall -D_XOPEN_SOURCE_EXTENDED `pkg-config --cflags ncursesw`
DEBUG = -m32 -g -fno-inline -fno-omit-frame-pointer -O0
FLAGS = `pkg-config --libs ncursesw sqlite3` -lpthread -lcurl

# При изменении любого .h всё нужно перекомпилить (т.к. инклюды)
HEADERS = src/*.h src/classes/*.h

launch: vktui.exe
	./vktui.exe

vktui.exe: main.o Backend.o Database.o Frontend.o User_input.o Network.o \
			Auth_interface.o Curl_wrapper.o Longpoll.o Event_queue.o ScrollableWindow.o
	$(COMPILER) -o vktui.exe $^ $(FLAGS)


memorytest:
	$(COMPILER) $(DEBUG) -o vktui_debug.exe src/*.cpp src/classes/*.cpp $(FLAGS)
	drmemory -- vktui_debug.exe

 
testfront: vktui_frontend_test.exe
	./vktui_frontend_test.exe

vktui_frontend_test.exe: Frontend.o ScrollableWindow.o frontend_test.o
	$(COMPILER) -o vktui_frontend_test.exe $^ $(FLAGS)

testnet: vktui_network_test.exe
	./vktui_network_test.exe

vktui_network_test.exe: Network.o Curl_wrapper.o Database.o network_test.o
	$(COMPILER) -o vktui_network_test.exe $^ $(FLAGS)

testpoll: vktui_longpoll_test.exe
	./vktui_longpoll_test.exe

vktui_longpoll_test.exe: Longpoll.o Curl_wrapper.o Database.o longpoll_test.o
	$(COMPILER) -o vktui_longpoll_test.exe $^ $(FLAGS)



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
Longpoll.o: src/Longpoll.cpp $(HEADERS)
	$(COMPILER) -c $<

# вспомогательные классы
Event_queue.o: src/classes/Event_queue.cpp $(HEADERS)
	$(COMPILER) -c $<
ScrollableWindow.o: src/classes/ScrollableWindow.cpp $(HEADERS)
	$(COMPILER) -c $<
Curl_wrapper.o: src/classes/Curl_wrapper.cpp $(HEADERS)
	$(COMPILER) -c $<
Auth_interface.o: src/classes/Auth_interface.cpp $(HEADERS)
	$(COMPILER) -c $<

# тесты
frontend_test.o: src/tests/frontend_test.cpp $(HEADERS)
	$(COMPILER) -c $<
network_test.o: src/tests/network_test.cpp $(HEADERS)
	$(COMPILER) -c $<
longpoll_test.o: src/tests/longpoll_test.cpp $(HEADERS)
	$(COMPILER) -c $<

clean:
	rm -rfv *.o *.exe *.log *.dSYM
