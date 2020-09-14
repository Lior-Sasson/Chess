#include "unit_test_util.h"
#include "Parser.h"
#include <stdbool.h>

static bool test1() {
	Command *cmd = malloc(sizeof(Command));
	ASSERT_TRUE(cmd != NULL);

	ASSERT_TRUE(parseLine(cmd, "helloooo bdikat shfiut")); // no allocation error V
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "\n")); // V
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "start"));
	ASSERT_TRUE(cmd-> cmd == START);
	
	ASSERT_TRUE(parseLine(cmd, "startt"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, " start"));
	ASSERT_TRUE(cmd-> cmd == START);
	
	ASSERT_TRUE(parseLine(cmd, "sta rt"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "game_mode 3"));
	ASSERT_TRUE(cmd-> cmd == GAME_MODE && cmd-> validArgs == false);
	
	ASSERT_TRUE(parseLine(cmd, "game_mode 0"));
	ASSERT_TRUE(cmd-> cmd == GAME_MODE && cmd-> validArgs == false);
	
	ASSERT_TRUE(parseLine(cmd, "game_mode 2"));
	ASSERT_TRUE(cmd-> cmd == GAME_MODE && cmd-> validArgs == true && cmd-> intArg == 2);
	
	ASSERT_TRUE(parseLine(cmd, "gamemode 1"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);
	
	ASSERT_TRUE(parseLine(cmd, "game mode 1"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);
	
	ASSERT_TRUE(parseLine(cmd, "difficulty 3"));
	ASSERT_TRUE(cmd-> cmd == DIFFICULTY && cmd-> validArgs == true && cmd-> intArg == 3);
	
	ASSERT_TRUE(parseLine(cmd, "difficulty 3 3")); // it's ok if there are extra param. p.4
	ASSERT_TRUE(cmd-> cmd == DIFFICULTY && cmd-> validArgs == true && cmd-> intArg == 3);
	
	ASSERT_TRUE(parseLine(cmd, "user_color 3"));
	ASSERT_TRUE(cmd-> cmd == USER_COLOR && cmd-> validArgs == false);
	
	ASSERT_TRUE(parseLine(cmd, "user_color 0"));
	ASSERT_TRUE(cmd-> cmd == USER_COLOR && cmd-> validArgs == true && cmd-> intArg == 0);
	
	ASSERT_TRUE(parseLine(cmd, "load hheeeelllloooo"));
	ASSERT_TRUE(cmd-> cmd == LOAD && cmd-> validArgs == true && cmd-> intArg == 14);
	
	ASSERT_TRUE(parseLine(cmd, "hheeeelllloooo load"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);
	
	ASSERT_TRUE(parseLine(cmd, " default 0"));
	ASSERT_TRUE(cmd-> cmd == DEFAULT);

	ASSERT_TRUE(parseLine(cmd, " default	"));
	ASSERT_TRUE(cmd-> cmd == DEFAULT);

	ASSERT_TRUE(parseLine(cmd, "print_settings 8"));
	ASSERT_TRUE(cmd-> cmd == PRINT_SETTINGS);

	ASSERT_TRUE(parseLine(cmd, "8 print_settings 8"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "print settings "));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "quit !"));
	ASSERT_TRUE(cmd-> cmd == QUIT);

	ASSERT_TRUE(parseLine(cmd, "quit!"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, " quit "));
	ASSERT_TRUE(cmd-> cmd == QUIT);

	ASSERT_TRUE(parseLine(cmd, "move <2,H> to <8,D>"));
	ASSERT_TRUE(cmd-> cmd == MOVE && cmd-> validArgs == true && cmd-> srcPos[0] == 1 && cmd-> srcPos[1] == 7 && cmd-> destPos[0] == 7 && cmd-> destPos[1] == 3);

	ASSERT_TRUE(parseLine(cmd, "move <4,D> to <4,D>"));
	ASSERT_TRUE(cmd-> cmd == MOVE && cmd-> validArgs == true && cmd-> srcPos[0] == 3 && cmd-> srcPos[1] == 3 && cmd-> destPos[0] == 3 && cmd-> destPos[1] == 3);

	ASSERT_TRUE(parseLine(cmd, "move <1,B> to <3,D>"));
	ASSERT_TRUE(cmd-> cmd == MOVE && cmd-> validArgs == true && cmd-> srcPos[0] == 0 && cmd-> srcPos[1] == 1 && cmd-> destPos[0] == 2 && cmd-> destPos[1] == 3);

	ASSERT_TRUE(parseLine(cmd, "move <3,C> to <7,F>"));
	ASSERT_TRUE(cmd-> cmd == MOVE && cmd-> validArgs == true && cmd-> srcPos[0] == 2 && cmd-> srcPos[1] == 2 && cmd-> destPos[0] == 6 && cmd-> destPos[1] == 5);

	ASSERT_TRUE(parseLine(cmd, "move <A,3> to <F,4>"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "move <9,H> to <8,D>"));
	ASSERT_TRUE(cmd-> cmd == MOVE && cmd-> validArgs == false);

	ASSERT_TRUE(parseLine(cmd, "move <2,I> to <8,D>"));
	ASSERT_TRUE(cmd-> cmd == MOVE && cmd-> validArgs == false);

	ASSERT_TRUE(parseLine(cmd, "move <2,H> <8,D>"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "get_moves <2,H>"));
	ASSERT_TRUE(cmd-> cmd == GET_MOVES && cmd-> validArgs == true && cmd-> srcPos[0] == 1 && cmd-> srcPos[1] == 7);

	ASSERT_TRUE(parseLine(cmd, "get_moves <0,H>"));
	ASSERT_TRUE(cmd-> cmd == GET_MOVES && cmd-> validArgs == false);

	ASSERT_TRUE(parseLine(cmd, "get_moves"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "get_moves	 "));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "get_moves < , >"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "get_moves <1,1>"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, " save hheeeelllloooo"));
	ASSERT_TRUE(cmd-> cmd == SAVE && cmd-> validArgs == true && cmd-> intArg == 14);

	ASSERT_TRUE(parseLine(cmd, "save gnfdjkgfjdkghfgjdkfvhdskgfgf "));
	ASSERT_TRUE(cmd-> cmd == SAVE && cmd-> validArgs == true && cmd-> intArg == 28);

	ASSERT_TRUE(parseLine(cmd, "save_gnfdjkgfjdkghfgjdkfvhdskgfgf"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, " undo 8"));
	ASSERT_TRUE(cmd-> cmd == UNDO);

	ASSERT_TRUE(parseLine(cmd, "undoo"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);

	ASSERT_TRUE(parseLine(cmd, "un do"));
	ASSERT_TRUE(cmd-> cmd == INVALID_LINE);
	
	ASSERT_TRUE(parseLine(cmd, "undo"));
	ASSERT_TRUE(cmd-> cmd == UNDO);
	
	ASSERT_TRUE(parseLine(cmd, "reset"));
	ASSERT_TRUE(cmd-> cmd == RESET);

	free(cmd);
	return true;	
}
/*
int main (){
	RUN_TEST(test1);
	return 0;
}
*/
