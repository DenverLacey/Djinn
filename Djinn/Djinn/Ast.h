#pragma once

#include <stdint.h>
#include <vector>
#include <string_view>

#include "CodeLocation.h"

using IdentIndex  = uint32_t;
using InfoIndex   = uint32_t;
using NodeIndex   = uint32_t;
using ChildOffset = int16_t;

enum AstKind : uint32_t
{
	AstKind_None = 0,
	AstKind_Root = 1,

	// Literals
	AstKind_Bool,
	AstKind_Char,
	AstKind_Int,
	AstKind_Float,
	AstKind_Str,
	AstKind_Ident,

	// Unary
	AstKind_Negate,

	// Binary
	AstKind_Add,
	AstKind_Subtract,
	AstKind_Multiply,
	AstKind_Divide,
	AstKind_Modulus,
	AstKind_Invoke,

	// Statements
	AstKind_ImprtStmt,

	// Declarations
	AstKind_ProcDecl,
};

/* === Node Infos =====================
* These structs are used to store extra information for complex nodes
* such as declarations that cannot fit in an ordinary AstNode.
*/

struct AstNodeInfo_ProcDecl
{
	IdentIndex ident;
	InfoIndex params;
	InfoIndex body;
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
	NodeIndex me;                   // This node's index in the Ast.
	union
	{
		struct
		{
			ChildOffset left;   // Relative jump to left child node.
			ChildOffset right;  // Relative jump to right child node.
		};
		InfoIndex info;    // Index of the extra info for this node.
		IdentIndex ident;  // Index of the ident for this node.
	};
};

struct Ast
{
	// === Data =======================
	std::vector<AstNode> nodes;
	std::vector<CodeLocation> locations;

	// Node Infos
	std::vector<std::string_view> idents;
	std::vector<AstNodeInfo_ProcDecl> proc_decl_infos;

	// Associated Functions
	NodeIndex add_node(AstKind kind, CodeLocation loc);
	NodeIndex add_ident_node(std::string_view ident, CodeLocation loc);
};
