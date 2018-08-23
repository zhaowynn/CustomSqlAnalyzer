#include "boost/any.hpp"
#include "condition.h"
#include "column.h"
#include "document.h"


Condition::Condition() :
    optype_(hsql::kOpNone), column_(nullptr), left_(nullptr), right_(nullptr) {
}

Condition::~Condition() {
    if (column_) {
        delete column_;
    }
    if (left_) {
        delete left_;
    }
    if (right_) {
        delete right_;
    }
}

ColumnI* Condition::GetColumn() const {
    return column_;
}
void Condition::SetColumn(ColumnI* column) {
    this->column_ = column;
}
Condition* Condition::GetLeft() const {
    return left_;
}
void Condition::SetLeft(Condition* left) {
    this->left_ = left;
}
hsql::OperatorType Condition::GetOpType() const {
    return optype_;
}
void Condition::SetOpType(hsql::OperatorType optype) {
    this->optype_ = optype;
}
Condition* Condition::GetRight() const {
    return right_;
}
void Condition::SetRight(Condition* right) {
    this->right_ = right;
}

bool Condition::Match(DocumentI& doc) {
    bool matched = false;
    switch (optype_) {
    case hsql::kOpEquals: {
        matched = doc.HandleCondition(*this);
        break;
    }
    case hsql::kOpNotEquals: {
        matched = doc.HandleCondition(*this);
        break;
    }
    case hsql::kOpLess: {
        matched = doc.HandleCondition(*this);
        break;
    }
    case hsql::kOpLessEq: {
        matched = doc.HandleCondition(*this);
        break;
    }
    case hsql::kOpGreater: {
        matched = doc.HandleCondition(*this);
        break;
    }
    case hsql::kOpGreaterEq: {
        matched = doc.HandleCondition(*this);
        break;
    }
    case hsql::kOpAnd: {
        matched = left_->Match(doc) && right_->Match(doc);
        break;
    }
    case hsql::kOpOr: {
        matched = left_->Match(doc) || right_->Match(doc);
        break;
    }
    case hsql::kOpIn: {
        matched = doc.HandleCondition(*this);
        break;
    }
    default: {
        break;
    }
    }
    return matched;
}

