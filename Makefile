COMPILER = g++ --std=c++11 -Wall

# При изменении любого .h всё нужно перекомпилить (т.к. инклюды)
HEADERS = src/*.h src/classes/*.h

launch: vktui.exe
	./vktui.exe

vktui.exe: main.o event_handler.o renderer.o user_input.o \
			state.o event_queue.o B.o events.o messages.o
	$(COMPILER) -o vktui.exe -lncurses $^



# основные классы
B.o: src/B.cpp $(HEADERS)
	$(COMPILER) -c $<
event_queue.o: src/event_queue.cpp $(HEADERS)
	$(COMPILER) -c $<
state.o: src/state.cpp $(HEADERS)
	$(COMPILER) -c $<
user_input.o: src/user_input.cpp $(HEADERS)
	$(COMPILER) -c $<
renderer.o: src/renderer.cpp $(HEADERS)
	$(COMPILER) -c $<
event_handler.o: src/event_handler.cpp $(HEADERS)
	$(COMPILER) -c $<
main.o: src/main.cpp $(HEADERS)
	$(COMPILER) -c $<

# другие классы
events.o: src/classes/events.cpp $(HEADERS)
	$(COMPILER) -c $<
messages.o: src/classes/messages.cpp $(HEADERS)
	$(COMPILER) -c $<


clean:
	rm -rf `cat .gitignore`