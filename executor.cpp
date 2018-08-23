#include <iostream>
#include <vector>
#include <iterator>
#include "executor.h"
#include "condition.h"
#include "document.h"
#include "fields.h"
#include "groupby.h"
#include "column.h"


SQLExecutor::SQLExecutor(const std::string& query, FieldsI* f, Condition* c, GroupByI* g):
    query_(query), fields_(f), condition_(c), groupby_(g) {
}
SQLExecutor::~SQLExecutor() {
    if (condition_) {
        delete condition_;
    }
    if (groupby_) {
        delete groupby_;
    }
    delete fields_;
}

bool SQLExecutor::Handle(DocumentI& doc) {
    if (condition_ && (!condition_->Match(doc))) {
        return false;
    }
    if (groupby_) {
        std::set<std::vector<tData> > groupByV = groupby_->Handle(doc);
        const std::vector<std::string>& groupByC = groupby_->GetColumns();
        std::lock_guard<std::mutex> lk(lock_);
        for (auto it = std::begin(groupByV); it != std::end(groupByV); it++) {
            fields_->Handle(doc, groupByC, *it);
        }
    } else {
        std::lock_guard<std::mutex> lk(lock_);
        fields_->Handle(doc);
    }
    return true;
}

void debugCondition(Condition* condition) {
    if (nullptr == condition) {
        return;
    }
    std::cout << "    " << "OpType:" << condition->GetOpType() << std::endl;
    if (nullptr != condition->GetColumn()) {
        ColumnI* column = condition->GetColumn();
        std::cout << "    " << "column name: " << column->GetName() << std::endl;
        std::cout << "    " << "column type: " << column->GetType() << std::endl;
        std::cout << "    " << "column value: " << std::endl;
        std::vector<boost::any>& values = column->GetValue();
        for (auto it = std::begin(values); it != std::end(values); it++) {
            if (it->type() == typeid(int)) {
                std::cout <<  "            " << boost::any_cast<int>(*it) << std::endl;
            } else if (it->type() == typeid(float)) {
                std::cout <<  "            " << boost::any_cast<float>(*it) << std::endl;
            } else if (it->type() == typeid(int64_t)) {
                std::cout <<  "            " << boost::any_cast<int64_t>(*it) << std::endl;
            } else if (it->type() == typeid(std::string)) {
                std::cout <<  "            " << boost::any_cast<std::string>(*it) << std::endl;
            } else {
                std::cout <<  "            " << "Error" << std::endl;
            }
        }
    }

    std::cout << "\n" << std::endl;

    if (condition->GetLeft())
        debugCondition(condition->GetLeft());
    if (condition->GetRight())
        debugCondition(condition->GetRight());
}


void SQLExecutor::DebugDump() {
    {
        std::cout << "==========================Fields======================" << std::endl;
        std::vector<std::string> columns = fields_->GetColumns();
        std::cout << "columns:" << std::endl;
        for (auto it = std::begin(columns); it != std::end(columns); ++it)
            std::cout << "    " << (*it) << std::endl;
        std::cout << "aggregate function:" << std::endl;
        std::cout << "    " << (fields_->GetFunType()) << std::endl;
    }

    {
        std::cout << "\n==========================GroupBy======================" << std::endl;
        if (groupby_) {
            std::vector<std::string> columns = groupby_->GetColumns();
            std::cout << "columns:" << std::endl;
            for (auto it = std::begin(columns); it != std::end(columns); ++it)
                std::cout << "    " << (*it) << std::endl;
        }
    }

    {
        std::cout << "\n==========================Condition======================" << std::endl;
        if (condition_) {
            debugCondition(condition_);
        }
        std::cout << "\n" << std::endl;
    }

}

void SQLExecutor::Dump() {
    std::cout << "\n==========================ParseResult======================" << std::endl;

    auto printLd = [](boost::any& value) {
        if (AggFunUtil::IsInt(value)) {
            std::cout << "value: " << (boost::any_cast<int>(value)) << std::endl;
        } else if (AggFunUtil::IsFloat(value)) {
            std::cout << "value: " << (boost::any_cast<float>(value)) << std::endl;
        } else if (AggFunUtil::IsInt64(value)) {
            std::cout << "value: " << (boost::any_cast<int64_t>(value)) << std::endl;
        } else if (value.type() == typeid(std::map<std::vector<tData>, boost::any>)) {
            std::cout << "value: ";
            std::map<std::vector<tData>, boost::any> m = boost::any_cast<std::map<std::vector<tData>, boost::any> >(value);
            for (auto iter = std::begin(m); iter != std::end(m); iter++) {
                std::cout << " " << "{";
                bool isFirst = true;
                for (auto it = std::begin(iter->first); it != std::end(iter->first); it++) {
                    if (isFirst) {
                        isFirst = false;
                    } else {
                        std::cout << "|";
                    }
                    if (tData::E_INT == it->type)
                        std::cout << it->ival;
                    else if (tData::E_FLOAT == it->type)
                        std::cout << it->fval;
                    else if (tData::E_STRING == it->type)
                        std::cout << it->sval;
                }
                std::cout << ": ";
                if (iter->second.type() == typeid(int)) {
                    std::cout << boost::any_cast<int>(iter->second) << "}";
                } else if (iter->second.type() == typeid(float)) {
                    std::cout << boost::any_cast<float>(iter->second) << "}";
                } else if (iter->second.type() == typeid(unsigned char)) {
                    std::cout << boost::any_cast<unsigned char>(iter->second) << "}";
                } else if (iter->second.type() == typeid(unsigned short)) {
                    std::cout << boost::any_cast<unsigned short>(iter->second) << "}";
                } else if (iter->second.type() == typeid(int64_t)) {
                    std::cout << boost::any_cast<int64_t>(iter->second) << "}";
                } else if (iter->second.type() == typeid(std::string)) {
                    std::cout << boost::any_cast<std::string>(iter->second) << "}";
                }
            }
        } else if (value.type() == typeid(std::string)) {
            std::cout <<  "            " << boost::any_cast<std::string>(value) << std::endl;
        } else {
            std::cout << "    value: " << "Error)" << std::endl;
        }
    };
    boost::any value;
    {
        std::lock_guard<std::mutex> lk(lock_);
        value = fields_->GetValue();
    }
    std::cout << "\nsql query: " << query_ << std::endl;
    if (value.empty()) {
        std::cout << "=========no matched data=========" << std::endl;
    } else {
        printLd(value);
    }

    std::cout << "\n" << std::endl;
}


