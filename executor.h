#ifndef _SQL_EXECUTOR_H_
#define _SQL_EXECUTOR_H_

#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include "boost/any.hpp"
#include "def.h"

class DocumentI;
class FieldsI;
class Condition;
class GroupByI;

class SQLExecutor {
public:
    SQLExecutor(const std::string& query, FieldsI* f, Condition* c = nullptr, GroupByI* g = nullptr);
    ~SQLExecutor();

    bool Handle(DocumentI& doc);
    void DebugDump();
    void Dump();

private:
    std::string query_;
    std::mutex lock_;
    FieldsI* fields_;
    Condition* condition_;
    GroupByI* groupby_;
};

#endif
