#pragma once

#include "ASTNode.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace driftcalc::expr
{
    class ExpressionError : public std::runtime_error
    {
    public:
        explicit ExpressionError(const std::string& msg)
            : std::runtime_error(msg) {}
    };

    class ExpressionEngine
    {
    public:
        ExpressionEngine() = default;

        // 把字符串解析成 AST 根节点
        ExprNodePtr parse(const std::string& input);

    private:
        // 简单 tokenizer + 递归下降 parser
        struct Token
        {
            enum class Type
            {
                End,
                Number,
                Plus,
                Minus,
                Mul,
                Div,
                Pow,
                LParen,
                RParen
            } type;

            double numberValue = 0.0;
        };

        class Lexer
        {
        public:
            explicit Lexer(const std::string& s);

            Token next();
            const Token& current() const { return currentToken; }

        private:
            void skipSpaces();
            Token readNumber();

            const std::string& src;
            std::size_t        pos = 0;
            Token              currentToken{Token::Type::End, 0.0};
        };

        // 递归下降：expr -> term (+/- term)*
        ExprNodePtr parseExpression(Lexer& lex);
        ExprNodePtr parseTerm(Lexer& lex);
        ExprNodePtr parseFactor(Lexer& lex);     // 处理 *, /, ^ 右结合
        ExprNodePtr parseUnary(Lexer& lex);
        ExprNodePtr parsePrimary(Lexer& lex);
    };

} // namespace driftcalc::expr