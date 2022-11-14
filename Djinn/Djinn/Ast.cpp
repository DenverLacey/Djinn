#include "Ast.h"

#include "Error.h"

NodeIndex Ast::add_node(
    AstKind kind,
    CodeLocation loc,
    NodeIndex left,
    NodeIndex right)
{
    AstNode node;
    node.kind = kind;
    node.me = nodes.size();
    node.left = left != -1 ? static_cast<ChildOffset>(left - node.me) : 0;
    node.right = right != -1 ? static_cast<ChildOffset>(right - node.me) : 0;

    locations.push_back(loc);
    nodes.push_back(node);

    return node.me;
}

NodeIndex Ast::add_node_with_info(
    AstKind kind,
    CodeLocation loc,
    InfoIndex info)
{
    AstNode node;
    node.kind = kind;
    node.me = nodes.size();
    node.info = info;

    locations.push_back(loc);
    nodes.push_back(node);

    return node.me;
}

NodeIndex Ast::add_ident_node(std::string_view ident, CodeLocation loc)
{
    AstNode node;
    node.kind = AstKind_Ident;
    node.me = nodes.size();
    node.ident = strings.size();

    strings.push_back(ident);
    locations.push_back(loc);
    nodes.push_back(node);

    return node.me;
}

NodeIndex Ast::add_str_node(std::string_view str, CodeLocation loc)
{
    AstNode node;
    node.kind = AstKind_Str;
    node.me = nodes.size();
    node.ident = strings.size();

    strings.push_back(str);
    locations.push_back(loc);
    nodes.push_back(node);

    return node.me;
}

void Ast::print() const
{
    for (auto root : roots)
    {
        print_node(root);
    }
}

void Ast::print_node(NodeIndex idx) const
{
    auto node = nodes[idx];
    
    switch (node.kind) 
    {
        case AstKind_None:
        default:
            break;
    }
}
