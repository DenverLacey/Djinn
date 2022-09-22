#pragma once

#include <stdint.h>
#include <vector>

/* @DOCS
* 
*/

enum AstKind : uint32_t
{
	AstKind_None = 0,
};

// @TEMP
// This will be replaced with the proper Type structure.
// The important thing is that it is a relativly small integer behind the
// scenes both for better packing in the AstNode struct but also to escape the
// need to do lots of copying of types during the semantic checking phase and
// as a bonus, optimize checking for equality of types since they'll interned.
//
struct Type
{
	enum : uint16_t
	{
		Unknown = 0,
	} kind;
	uint16_t complex_index;
};

struct AstNode
{
	AstKind kind = AstKind_None;   // This would be a uint32_t behind the scenes.
	Type type = { Type::Unknown };
	uint32_t me;                   // This node's index in the Ast.
	union
	{
		struct
		{
			int16_t left;   // Relative jump to left child node.
			int16_t right;  // Relative jump to right child node.
		};
		uint32_t info;      // Index of the extra info for this node.
	};
};

struct Ast
{
	// === Data =======================
	std::vector<AstNode> nodes;
};
