#include "Interpreter.h"

#include "Error.h"
#include "Parser.h"

#include "skrive.h"
#include <filesystem>
#include <fstream>

void Interpreter::load_program_files(int n, const char** filepaths)
{
	for (int i = 0; i < n; i++)
	{
		const char* filepath = filepaths[i];

		if (std::filesystem::is_directory(filepath))
		{
			TODO("Implement loading program files from directories.");
		}
		else
		{
			sk::println("[CMD]: Loading file `{}`", filepath);
			load_program_file(filepath);
		}
	}
}

std::string read_entire_file(const char* filepath)
{
	auto file = std::ifstream{ filepath, std::ios::binary | std::ios::ate };
	if (!file.is_open())
	{
		throw sk::format("ERR: Could not open `{}`", filepath);
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	auto source = std::string(size, '\0');

	file.read(source.data(), size);
	if (!file.good())
	{
		//throw sk::format("ERR: Could not read from `{}`", filepath);
		sk::eprintln("ERR: Could not read from `{}`", filepath);
		throw std::exception();
	}

	return source;
}

void Interpreter::load_program_file(const char* filepath)
{
	std::string source = read_entire_file(filepath);
	
	auto loaded_file = LoadedFile{ filepath, std::move(source) };
	loaded_files.push_back(std::move(loaded_file));
}

void Interpreter::parse_file(size_t file_index)
{
	ENSURE(file_index < loaded_files.size(), "Index `{}` is outside bounds of loaded_files array. size = {}", file_index, loaded_files.size());
	auto& file = loaded_files[file_index];

	auto ast = Parser::parse_file(file);

	auto parsed_file = ParsedFile{ file_index, std::move(ast) };
	parsed_files.push_back(std::move(parsed_file));
}
