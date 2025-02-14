/**
 * @brief Qlang v2
 * @author QuantVasa
*/

#include "qlang.h"

int main(int argc, char const *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "Error: Not enough arguments.\nProper usage: qlang <IN.Q> <OUT>\n");
		return EXIT_FAILURE;
	};

	return EXIT_SUCCESS;
};