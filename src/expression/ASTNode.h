#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <cmath>

namespace driftcalc::expr
{

//=============================
// Base Expression Node
//=============================
class ExprNode
{
public:
    virtual ~ExprNode() = default;
    virtual double evaluate() const = 0;
};

using ExprNodePtr = std::unique_ptr<ExprNode>;


//=============================
// Number Node
//=============================
class NumberNode : public ExprNode
{
public:
    explicit NumberNode(double v) : value(v) {}
    double evaluate() const override { return value; }

private:
    double value;
};


//=============================
// Binary Node Base
//=============================
class BinaryNode : public ExprNode
{
public:
    BinaryNode(ExprNodePtr lhs, ExprNodePtr rhs)
        : left(std::move(lhs)), right(std::move(rhs)) {}

protected:
    ExprNodePtr left;
    ExprNodePtr right;

public:
    // ★★ 必须提供 getter，让外部可以访问 ★★
    const ExprNode& getLeft()  const { return *left; }
    const ExprNode& getRight() const { return *right; }
};


//=============================
// Add
//=============================
class AddNode : public BinaryNode
{
public:
    using BinaryNode::BinaryNode;

    double evaluate() const override
    {
        return getLeft().evaluate() + getRight().evaluate();
    }
};


//=============================
// Sub
//=============================
class SubNode : public BinaryNode
{
public:
    using BinaryNode::BinaryNode;

    double evaluate() const override
    {
        return getLeft().evaluate() - getRight().evaluate();
    }
};


//=============================
// Mul
//=============================
class MulNode : public BinaryNode
{
public:
    using BinaryNode::BinaryNode;

    double evaluate() const override
    {
        return getLeft().evaluate() * getRight().evaluate();
    }
};


//=============================
// Div
//=============================
class DivNode : public BinaryNode
{
public:
    using BinaryNode::BinaryNode;

    double evaluate() const override
    {
        double denom = getRight().evaluate();
        if (denom == 0.0)
            throw std::runtime_error("division by zero");
        return getLeft().evaluate() / denom;
    }
};


//=============================
// Pow
//=============================
class PowNode : public BinaryNode
{
public:
    using BinaryNode::BinaryNode;

    double evaluate() const override
    {
        return std::pow(getLeft().evaluate(), getRight().evaluate());
    }
};


//=============================
// Unary Minus
//=============================
class UnaryMinusNode : public ExprNode
{
public:
    explicit UnaryMinusNode(ExprNodePtr c)
        : child(std::move(c)) {}

    double evaluate() const override
    {
        return -child->evaluate();
    }

private:
    ExprNodePtr child;

public:
    // ★★ getter ★★
    const ExprNode& getChild() const { return *child; }
};

} // namespace driftcalc::expr