#include <iostream>
#include "SQLParser.h"
#include "util/sqlhelper.h"
#include "controller.h"
#include "executor.h"
#include "aggregate.h"
#include "column.h"
#include "condition.h"
#include "document.h"
#include "fields.h"
#include "groupby.h"

static void parseConditionOperatorExpression(hsql::Expr* expr, Condition* node);
static void parseConditionExpression(hsql::Expr* expr, Condition* node, int rl = -1);

static void parseConditionOperatorExpression(hsql::Expr* expr, Condition* node) {
    if (expr == nullptr) {
        return;
    }
    switch (expr->opType) {
    case hsql::kOpAnd:
        node->SetOpType(hsql::kOpAnd);
        node->SetLeft(new Condition());
        node->SetRight(new Condition());
        break;
    case hsql::kOpOr:
        node->SetOpType(hsql::kOpOr);
        node->SetLeft(new Condition());
        node->SetRight(new Condition());
        break;
    case hsql::kOpNot:
        node->SetOpType(hsql::kOpNot);
        node->SetLeft(new Condition());
        break;
    default:
        node->SetOpType(expr->opType);
        break;
    }
    parseConditionExpression(expr->expr, node, 0);
    if (expr->expr2 != nullptr) {
        parseConditionExpression(expr->expr2, node, 1);
    } else if (expr->exprList != nullptr) {
        for (hsql::Expr* e : *expr->exprList) {
            parseConditionExpression(e, node, 2);
        }
    }
}

static void parseConditionExpression(hsql::Expr* expr, Condition* node, int rl) {
    switch (expr->type) {
    case hsql::kExprColumnRef: {
        ColumnI* col = new Column();
        col->SetName(expr->name);
        node->SetColumn(col);
        break;
    }
    case hsql::kExprLiteralFloat: {
        node->GetColumn()->SetType(hsql::kExprLiteralFloat);
        node->GetColumn()->SetValue(expr->fval);
        break;
    }
    case hsql::kExprLiteralInt: {
        node->GetColumn()->SetType(hsql::kExprLiteralInt);
        node->GetColumn()->SetValue(expr->ival);
        break;
    }
    case hsql::kExprLiteralString: {
        node->GetColumn()->SetType(hsql::kExprLiteralString);
        std::string value(expr->name);
        node->GetColumn()->SetValue(value);
        break;
    }
    case hsql::kExprOperator: {
        if (-1 == rl) {
            parseConditionOperatorExpression(expr, node);
        } else if (0 == rl) {
            parseConditionOperatorExpression(expr, node->GetLeft());
        } else if (1 == rl) {
            parseConditionOperatorExpression(expr, node->GetRight());
        } else if (2 == rl) {
            parseConditionOperatorExpression(expr, node);
        }
        break;
    }
    default:
        std::cerr << "Unrecognized expression type " << expr->type << std::endl;
        return;
    }
}

static void parseGroupByExpression(hsql::Expr* expr, GroupByI* groupBy) {
    switch (expr->type) {
    case hsql::kExprColumnRef: {
        groupBy->SetColumn(expr->name);
        break;
    }
    default:
        std::cerr << "Unrecognized expression type " << expr->type << std::endl;
        return;
    }
}


typedef struct tFields {
    std::string name;
    std::vector<std::string> columns;
} tFields;


static void parseFieldsExpression(hsql::Expr* expr, tFields* fields) {
    switch (expr->type) {
    case hsql::kExprColumnRef: {
        fields->columns.push_back(expr->name);
        break;
    }
    case hsql::kExprFunctionRef: {
        fields->name = expr->name;
        for (hsql::Expr* e : *expr->exprList) {
            parseFieldsExpression(e, fields);
        }
        break;
    }
    default:
        std::cerr << "Unrecognized expression type " << expr->type << std::endl;
        return;
    }
}

Controller::Controller() {
}

Controller::~Controller() {
    for (auto it = std::begin(executors_); it != std::end(executors_); ++it) {
        delete it->second;
    }
}

SQLExecutor* Controller::Parse(const std::string& query) {
    {
        ReadLock rLock(rwlock_);
        if (executors_.count(query) > 0) {
            return executors_[query];
        }
    }
    SQLExecutor* executor = this->ParseQuery(query);
    if (executor) {
        WriteLock wLock(rwlock_);
        if (executors_.count(query) > 0) {
            delete executor;
            executor = executors_[query];
        } else {
            executors_[query] = executor;
        }
    }
    return executor;
}

bool Controller::Handle(DocumentI& doc) {
    ReadLock rLock(rwlock_);
    for (auto it = std::begin(executors_); it != std::end(executors_); ++it) {
        it->second->Handle(doc);
    }
    return true;
}

SQLExecutor* Controller::ParseQuery(const std::string& query) {
    SQLExecutor* executor = nullptr;
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);
    if (result.isValid()) {
        FieldsI* fields = nullptr;
        Condition* condition = nullptr;
        GroupByI* groupBy = nullptr;
        for (auto i = 0u; i < result.size(); ++i) {
            const hsql::SQLStatement* stmt = result.getStatement(i);
            if (stmt->type() == hsql::kStmtSelect) {
                const hsql::SelectStatement* selectStatement = (const hsql::SelectStatement*) stmt;
                //FieldsI
                tFields fieldsHelper;
                for (hsql::Expr* expr : *selectStatement->selectList) {
                    parseFieldsExpression(expr, &fieldsHelper);
                    break;
                }
                FunType t = AggFunUtil::Str2FunType(fieldsHelper.name);
                if (E_NONE == t) {
                    continue;
                }
                fields = new Fields(t, fieldsHelper.columns, selectStatement->groupBy != nullptr);
                //Condition
                if (selectStatement->whereClause) {
                    condition = new Condition();
                    parseConditionExpression(selectStatement->whereClause, condition);
                }
                //GroupByI
                if (selectStatement->groupBy) {
                    groupBy = new GroupBy();
                    for (hsql::Expr* expr : *selectStatement->groupBy->columns) {
                        parseGroupByExpression(expr, groupBy);
                    }
                }
                executor = new SQLExecutor(query, fields, condition, groupBy);
                break;
            }
        }
    }
    return executor;
}

