#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include <string>
#include <vector>
#include <set>
#include "boost/any.hpp"
#include "SQLParser.h"
#include "util/sqlhelper.h"
#include "aggregate.h"
#include "def.h"


class Condition;
class GroupByI;
class FieldsI;
class ItemI;


class DocumentI {
public:
    ~DocumentI() {}
    virtual bool HandleCondition(const Condition& condition) = 0;
    virtual std::set<std::vector<tData> > HandleGroupBy(GroupByI* groupby) = 0;
    virtual bool HandleFields(FieldsI* fields) = 0;
    virtual bool HandleFields(FieldsI* fields, const std::vector<std::string>& columns, std::vector<tData> values) = 0;
};


class Document: public DocumentI {
public:
    Document(ItemI* item);
    bool HandleCondition(const Condition& condition);
    std::set<std::vector<tData> > HandleGroupBy(GroupByI* groupby);
    bool HandleFields(FieldsI* fields);
    bool HandleFields(FieldsI* fields, const std::vector<std::string>& columns, std::vector<tData> values);
private:
    ItemI* item_;
};

#endif
