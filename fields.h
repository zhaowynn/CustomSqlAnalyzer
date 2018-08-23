#ifndef _FIELDS_H_
#define _FIELDS_H_

#include <string>
#include <vector>
#include <map>
#include "boost/any.hpp"
#include "aggregate.h"
#include "def.h"

class DocumentI;
class AggregateI;


class FieldsI {
public:
    virtual ~FieldsI() {}
    virtual bool Handle(DocumentI& doc) = 0;
    virtual bool Handle(DocumentI& doc, const std::vector<std::string>& columns, std::vector<tData> values ) = 0;
    virtual FunType GetFunType() = 0;
    virtual bool IsGroupByType() = 0;
    virtual const std::vector<std::string>& GetColumns() = 0;
    virtual boost::any GetValue() = 0;
    virtual void SetValue(boost::any value) = 0;
    virtual void SetValue(const std::vector<tData>& key, boost::any value) = 0;
};


class Fields: public FieldsI {
public:
    Fields(FunType t, const std::vector<std::string>& columns, bool groupby = false);
    ~Fields();
    bool Handle(DocumentI& doc);
    bool Handle(DocumentI& doc, const std::vector<std::string>& columns, std::vector<tData> values);
    inline FunType GetFunType() { return type_; }
    inline bool IsGroupByType() { return isgroupby_; }
    inline const std::vector<std::string>& GetColumns() { return columns_; }
    boost::any GetValue();
    void SetValue(boost::any value);
    void SetValue(const std::vector<tData>& key, boost::any value);

private:
    AggregateI* getAggregate(FunType t);
    FunType type_;
    std::vector<std::string> columns_;
    bool isgroupby_;
    AggregateI* aggregate_;
    std::map<std::vector<tData>, AggregateI*>* groupbyaggregate_;
};

#endif
