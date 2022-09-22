#include <iostream>

#include "skrive.h"

#include "Interpreter.h"
#include "Error.h"

// @TODOS:
// - Handle `\r` in tokenizer.
//

int main(int argc, const char **argv)
{
	if (argc <= 1)
	{
		sk::eprintln("ERR: No filepaths given to Djinn compiler.");
		return -1;
	}

	Interpreter interp;
	interp.load_program_files(argc - 1, &argv[1]);

	for (size_t i = 0; i < interp.loaded_files.size(); i++)
	{
		auto& loaded_file = interp.loaded_files[i];

		sk::println("[CMD]: Parsing file `{}`", loaded_file.filepath);
		interp.parse_file(i);
	}

	return 0;
}
