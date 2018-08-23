#include <iostream>
#include <vector>
#include <list>
#include "SQLParser.h"
#include "util/sqlhelper.h"
#include "document.h"
#include "condition.h"
#include "fields.h"
#include "groupby.h"
#include "column.h"
#include "docitem.h"
#include "log.h"
#include "document.h"


Document::Document(ItemI* item): item_(item) {
}

bool Document::HandleCondition(const Condition& condition) {
    hsql::OperatorType opType = condition.GetOpType();
    ColumnI* column = condition.GetColumn();
    std::string& name = column->GetName();
    std::vector<boost::any> values = column->GetValue();

    bool matched = false;
    switch(opType) {
        case hsql::kOpEquals: {
            matched = item_->IsOpEquals(name, values[0]);
            break;
        }
        case hsql::kOpNotEquals: {
            matched = item_->IsOpNotEquals(name, values[0]);
            break;
        }
        case hsql::kOpLess: {
            matched = item_->IsOpLess(name, values[0]);
            break;
        }
        case hsql::kOpLessEq: {
            matched = item_->IsOpLessEq(name, values[0]);
            break;
        }
        case hsql::kOpGreater: {
            matched = item_->IsOpGreater(name, values[0]);
            break;
        }
        case hsql::kOpGreaterEq: {
            matched = item_->IsOpGreaterEq(name, values[0]);
            break;
        }
        case hsql::kOpIn: {
            matched = item_->IsOpIn(name, values);
            break;
        }
        default: {
            break;
        }
    }
    return matched;
}

std::set<std::vector<tData> > Document::HandleGroupBy(GroupByI* groupby) {
    const std::vector<std::string>& columns = groupby->GetColumns();
    return item_->getGroupBy(columns);
}

bool Document::HandleFields(FieldsI* fields) {
    const std::vector<std::string>& columns = fields->GetColumns();
    switch(fields->GetFunType()) {
        case E_SUM: {
            fields->SetValue(item_->GetSum(columns[0]));
            break;
        }
        case E_MAX: {
            fields->SetValue(item_->GetMax(columns[0]));
            break;
        }
        case E_MIN: {
            fields->SetValue(item_->GetMin(columns[0]));
            break;
        }
        case E_COUNT: {
            int values = item_->GetCount(columns);
            fields->SetValue(values);
            break;
        }
        case E_RATE: {
            fields->SetValue(item_->GetRate(columns[0]));
            break;
        }
        case E_AVG: {
            fields->SetValue(item_->GetAvg(columns[0]));
            break;
        }
        default: {
            break;
        }
    }
    return false;
}

bool Document::HandleFields(FieldsI* fields, const std::vector<std::string>& columns, std::vector<tData> values) {
    const std::vector<std::string>& fieldsColumns = fields->GetColumns();
    switch(fields->GetFunType()) {
        case E_SUM: {
            fields->SetValue(item_->GetSum(fieldsColumns[0]));
            break;
        }
        case E_MAX: {
            fields->SetValue(item_->GetSum(fieldsColumns[0]));
            break;
        }
        case E_MIN: {
            fields->SetValue(item_->GetMin(fieldsColumns[0]));
            break;
        }
        case E_COUNT: {
            int count = item_->GetCount(fieldsColumns, columns, values);
            fields->SetValue(values, count);
            break;
        }
        case E_RATE: {
            fields->SetValue(item_->GetRate(fieldsColumns[0]));
            break;
        }
        case E_AVG: {
            fields->SetValue(item_->GetAvg(columns[0]));
            break;
        }
        default: {
            break;
        }
    }
    return false;
}
