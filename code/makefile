CC = gcc
OBJS = GUI.o MainWindow.o MemoryWindow.o SettingsWindow.o GameWindow.o Minimax.o Game.o CLI.o Parser.o History.o main.o
EXEC = chessprog

HISTORY_TEST_OBJS = HistoryUnitTest.o History.o
PARSER_TEST_OBJS = ParserUnitTest.o Parser.o
GAME_TEST_OBJS = GameUnitTest.o Game.o History.o

UNIT_TESTS = ParserUnitTest HistoryUnitTest

CC_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
	
HistoryUnitTest: $(HISTORY_TEST_OBJS)
	$(CC) $(HISTORY_TEST_OBJS) -o $@
ParserUnitTest: $(PARSER_TEST_OBJS)
	$(CC) $(PARSER_TEST_OBJS) -o $@
GameUnitTest: $(GAME_TEST_OBJS)
	$(CC) $(GAME_TEST_OBJS) -o $@

	
HistoryUnitTest.o: HistoryUnitTest.c History.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
ParserUnitTest.o: ParserUnitTest.c Parser.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
GameUnitTest.o: GameUnitTest.c Game.h History.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c


History.o: History.h History.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Parser.o: Parser.h Parser.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Game.o: Game.h Game.c History.c History.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CLI.o: CLI.c CLI.h Game.h Game.c History.h History.c Parser.h Parser.c Minimax.c Minimax.h
	$(CC) $(COMP_FLAG) -c $*.c
Minimax.o: Minimax.c Minimax.h Game.c Game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
MainWindow.o: MainWindow.c MainWindow.h Minimax.c Minimax.h Game.c Game.h History.c History.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GameWindow.o: GameWindow.c GameWindow.h Minimax.c Minimax.h Game.c Game.h History.c History.h MainWindow.c MainWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SettingsWindow.o: SettingsWindow.c SettingsWindow.h GameWindow.c GameWindow.h Minimax.c Minimax.h Game.c Game.h History.c History.h MainWindow.c MainWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
MemoryWindow.o: MemoryWindow.c MemoryWindow.h SettingsWindow.c SettingsWindow.h GameWindow.c GameWindow.h Minimax.c Minimax.h Game.c Game.h History.c History.h MainWindow.c MainWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI.o: GUI.c GUI.h MemoryWindow.c MemoryWindow.h SettingsWindow.c SettingsWindow.h GameWindow.c GameWindow.h Minimax.c Minimax.h Game.c Game.h History.c History.h MainWindow.c MainWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c


main.o: main.c Minimax.h Game.h CLI.h GUI.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
