#include "ExpressionEngine.h"

#include <cctype>
#include <cmath>

namespace driftcalc::expr
{

// ---------- PowNode::evaluate ----------

// ========== Lexer 实现 ==========

ExpressionEngine::Lexer::Lexer(const std::string& s)
    : src(s)
{
    currentToken = next();
}

void ExpressionEngine::Lexer::skipSpaces()
{
    while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos])))
        ++pos;
}

ExpressionEngine::Token ExpressionEngine::Lexer::readNumber()
{
    std::size_t start = pos;
    bool        hasDot = false;

    while (pos < src.size())
    {
        char c = src[pos];
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            ++pos;
        }
        else if (c == '.' && !hasDot)
        {
            hasDot = true;
            ++pos;
        }
        else
        {
            break;
        }
    }

    const std::string numStr = src.substr(start, pos - start);
    if (numStr.empty())
        throw ExpressionError("Invalid number at position " + std::to_string(start));

    Token t;
    t.type = Token::Type::Number;
    t.numberValue = std::stod(numStr);
    return t;
}

ExpressionEngine::Token ExpressionEngine::Lexer::next()
{
    skipSpaces();

    if (pos >= src.size())
    {
        currentToken = {Token::Type::End, 0.0};
        return currentToken;
    }

    char c = src[pos];

    switch (c)
    {
    case '+':
        ++pos;
        currentToken = {Token::Type::Plus, 0.0};
        return currentToken;
    case '-':
        ++pos;
        currentToken = {Token::Type::Minus, 0.0};
        return currentToken;
    case '*':
        ++pos;
        currentToken = {Token::Type::Mul, 0.0};
        return currentToken;
    case '/':
        ++pos;
        currentToken = {Token::Type::Div, 0.0};
        return currentToken;
    case '^':
        ++pos;
        currentToken = {Token::Type::Pow, 0.0};
        return currentToken;
    case '(':
        ++pos;
        currentToken = {Token::Type::LParen, 0.0};
        return currentToken;
    case ')':
        ++pos;
        currentToken = {Token::Type::RParen, 0.0};
        return currentToken;
    default:
        break;
    }

    if (std::isdigit(static_cast<unsigned char>(c)) || c == '.')
    {
        currentToken = readNumber();
        return currentToken;
    }

    throw ExpressionError(std::string("Unexpected character '") + c + "' at position " +
                          std::to_string(pos));
}

// ========== ExpressionEngine::parse ==========

ExprNodePtr ExpressionEngine::parse(const std::string& input)
{
    Lexer lex(input);
    auto  root = parseExpression(lex);

    if (lex.current().type != Token::Type::End)
        throw ExpressionError("Unexpected trailing characters in expression");

    return root;
}

// expr := term ( ('+' | '-') term )*
ExprNodePtr ExpressionEngine::parseExpression(Lexer& lex)
{
    auto node = parseTerm(lex);

    while (true)
    {
        auto t = lex.current().type;
        if (t == Token::Type::Plus)
        {
            lex.next();
            auto rhs = parseTerm(lex);
            node = std::make_unique<AddNode>(std::move(node), std::move(rhs));
        }
        else if (t == Token::Type::Minus)
        {
            lex.next();
            auto rhs = parseTerm(lex);
            node = std::make_unique<SubNode>(std::move(node), std::move(rhs));
        }
        else
        {
            break;
        }
    }

    return node;
}

// term := factor ( ('*' | '/') factor )*
ExprNodePtr ExpressionEngine::parseTerm(Lexer& lex)
{
    auto node = parseFactor(lex);

    while (true)
    {
        auto t = lex.current().type;
        if (t == Token::Type::Mul)
        {
            lex.next();
            auto rhs = parseFactor(lex);
            node = std::make_unique<MulNode>(std::move(node), std::move(rhs));
        }
        else if (t == Token::Type::Div)
        {
            lex.next();
            auto rhs = parseFactor(lex);
            node = std::make_unique<DivNode>(std::move(node), std::move(rhs));
        }
        else
        {
            break;
        }
    }

    return node;
}

// factor := unary ( '^' factor )?     // 右结合
ExprNodePtr ExpressionEngine::parseFactor(Lexer& lex)
{
    auto base = parseUnary(lex);

    if (lex.current().type == Token::Type::Pow)
    {
        lex.next();
        auto exponent = parseFactor(lex); // 注意：右结合
        base = std::make_unique<PowNode>(std::move(base), std::move(exponent));
    }

    return base;
}

// unary := '-' unary | primary
ExprNodePtr ExpressionEngine::parseUnary(Lexer& lex)
{
    if (lex.current().type == Token::Type::Minus)
    {
        lex.next();
        auto child = parseUnary(lex);
        return std::make_unique<UnaryMinusNode>(std::move(child));
    }

    return parsePrimary(lex);
}

// primary := Number | '(' expr ')'
ExprNodePtr ExpressionEngine::parsePrimary(Lexer& lex)
{
    const auto& t = lex.current();

    if (t.type == Token::Type::Number)
    {
        double val = t.numberValue;
        lex.next();
        return std::make_unique<NumberNode>(val);
    }

    if (t.type == Token::Type::LParen)
    {
        lex.next();
        auto node = parseExpression(lex);
        if (lex.current().type != Token::Type::RParen)
            throw ExpressionError("Missing closing parenthesis");
        lex.next();
        return node;
    }

    throw ExpressionError("Expected number or '('");
}

} // namespace driftcalc::expr