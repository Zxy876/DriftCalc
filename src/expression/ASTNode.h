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

    // ⭐ 添加 getter，供 StepTreeBuilder 使用
    const ExprNode& leftNode() const { return *left; }
    const ExprNode& rightNode() const { return *right; }

protected:
    ExprNodePtr left;
    ExprNodePtr right;
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
        return leftNode().evaluate() + rightNode().evaluate();
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
        return leftNode().evaluate() - rightNode().evaluate();
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
        return leftNode().evaluate() * rightNode().evaluate();
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
        double denom = rightNode().evaluate();
        if (denom == 0.0)
            throw std::runtime_error("division by zero");
        return leftNode().evaluate() / denom;
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
        return std::pow(leftNode().evaluate(), rightNode().evaluate());
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
        return -childNode().evaluate();
    }

    // ⭐ Getter for StepTreeBuilder
    const ExprNode& childNode() const { return *child; }

private:
    ExprNodePtr child;
};

} // namespace driftcalc::expr