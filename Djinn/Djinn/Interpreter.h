#pragma once

#include "BucketArray.h"
#include "Scope.h"

#include <vector>

static constexpr size_t SCOPE_BUCKET_SIZE = 16;

struct LoadedFile
{
	const char* filepath;
	std::string source;
};

struct ParsedFile
{
	size_t file_index;
	std::vector<void*> ast;
};

using Pid = size_t;

struct Interpreter
{
	// === Data =======================
	BucketArray<Scope> scopes = { SCOPE_BUCKET_SIZE };
	std::vector<LoadedFile> loaded_files;
	std::vector<ParsedFile> parsed_files;
	Pid current_pid = 0;

	// === Methods ====================
	void load_program_files(int n, const char** filepaths);
	void load_program_file(const char* filepath);

	void parse_file(size_t file_index);
};
