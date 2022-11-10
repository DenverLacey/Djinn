#include "Ast.h"

NodeIndex Ast::add_node(AstKind kind, CodeLocation loc)
{
    AstNode node;
    node.kind = kind;
    node.me = nodes.size();
    node.info = 0;

    locations.push_back(loc);
    nodes.push_back(node);

    return node.me;
}

NodeIndex Ast::add_ident_node(std::string_view ident, CodeLocation loc)
{
    AstNode node;
    node.kind = AstKind_Ident;
    node.me = nodes.size();
    node.ident = idents.size();

    idents.push_back(ident);
    locations.push_back(loc);
    nodes.push_back(node);

    return node.me;
}
