/**
 * @brief Qlang
*/

#include "qlang.h"

int main(int argc, char const *argv[])
{
	ASSERT(argc >= 3, "Not enough arguments supplied.\n\tProper usage: qlang <IN.Q> <OUT>\n");
	processFile(argv[1]); // Process main program file

	return EXIT_SUCCESS;
};
