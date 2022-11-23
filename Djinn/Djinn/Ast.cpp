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
        print_node(root, 0);
    }
}

void Ast::print_node(NodeIndex idx, size_t indent) const
{
    auto node = nodes[idx];
    auto loc = locations[idx];

    std::string indents(indent + 1, '\t');
    std::string indents1(indent, '\t');
    
    switch (node.kind) 
    {
        // Literals
        case AstKind_Bool:
        {
            sk::println("Bool {{");
            sk::println("{}loc: {}", indents, loc);
            sk::println("{}value: {}", indents, static_cast<bool>(node.info));
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Char:
        {
            sk::println("Char {{");
            sk::println("{}loc: {}", indents, loc);
            sk::println("{}value: '{}'", indents, static_cast<char32_t>(node.info));
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Int:
        {
            sk::println("Int {{");
            sk::println("{}loc: {}", indents, loc);
            sk::println("{}value: {}", indents, static_cast<int32_t>(node.info));
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Float:
        {
            sk::println("Float {{");
            sk::println("{}loc: {}", indents, loc);
            sk::println("{}value: {}", indents, *reinterpret_cast<float*>(&node.info));
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Str:
        {
            auto s = strings[node.str];
            sk::println("Str {{");
            sk::println("{}loc: {}", indents, loc);
            sk::println("{}value: \"{}\"", indents, s);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Ident:
        {
            auto s = strings[node.ident];
            sk::println("Ident {{");
            sk::println("{}loc: {}", indents, loc);
            sk::println("{}value: {}", indents, s);
            sk::println("{}}}", indents1);
            break;
        }

        // Unary
        case AstKind_Negate:
        {
            sk::println("Negate {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}sub: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }

        // Binary
        case AstKind_Assign:
        {
            sk::println("Assign {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Add:
        {
            sk::println("Add {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Subtract:
        {
            sk::println("Subtract {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Multiply:
        {
            sk::println("Multiply {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Divide:
        {
            sk::println("Divide {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Modulus:
        {
            sk::println("Modulus {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Invoke:
        {
            sk::println("Invoke {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Subscript:
        {
            sk::println("Subscript {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }
        case AstKind_Binding:
        {
            sk::println("Binding {{");
            sk::println("{}loc: {}", indents, loc);
            sk::print("{}lhs: ", indents);
            print_node(node.me + node.left, indent + 1);
            sk::print("{}rhs: ", indents);
            print_node(node.me + node.right, indent + 1);
            sk::println("{}}}", indents1);
            break;
        }

        // Blocks
        case AstKind_Block: 
        {
            auto& block = block_infos[node.info];
            
            sk::println("Block {{");
            sk::println("{}loc: {}", indents, loc);
            
            sk::println("{}nodes:", indents);
            for (size_t i = 0; i < block.nodes.size(); i++)
            {
                sk::print("{}[{}]: ", indents, i);
                print_node(block.nodes[i], indent + 1);
            }
            
            sk::println("{}}}", indents1);
            break;
        }

        // Statements
        case AstKind_ImportStmt:
        {
            sk::println("Import {{");
            sk::println("{}loc: {}", indents, loc);
            
            sk::print("{}path: ", indents);
            print_node(node.me + node.left, indent + 1);
            
            sk::println("{}}}", indents1);
            break;
        }

        // Declarations
        case AstKind_ProcDecl:
        {
            auto& info = proc_decl_infos[node.info];

            sk::println("ProcDecl {{");
            sk::println("{}loc: {}", indents, loc);
            
            sk::print("{}ident: ", indents);
            print_node(info.ident, indent + 1);

            sk::print("{}params: ", indents);
            print_node(info.params, indent + 1);

            sk::print("{}body: ", indents);
            print_node(info.body, indent + 1);

            sk::println("{}}}", indents1);
            break;
        }

        // Other
        case AstKind_Params:
        {
            auto& block = block_infos[node.info];

            sk::println("Params {{");
            sk::println("{}loc: {}", indents, loc);

            sk::println("{}params:", indents);
            for (size_t i = 0; i < block.nodes.size(); i++)
            {
                sk::print("{}[{}]: ", indents, i);
                print_node(block.nodes[i], indent + 1);
            }

            sk::println("{}}}", indents1);
            break;
        }

        case AstKind_None:
        default:
        {
            sk::println("<AstKind: {}> {{", static_cast<int>(node.kind));
            sk::println("{}loc: {}", indents, loc);
            sk::println("{}}}", indents1);
            break;
        }
    }
}
