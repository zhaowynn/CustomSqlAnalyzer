#ifndef _COLUMN_H_
#define _COLUMN_H_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "boost/any.hpp"
#include "SQLParser.h"
#include "util/sqlhelper.h"
#include "document.h"

class ColumnI {
public:
    virtual ~ColumnI() {}
    virtual std::string& GetName() = 0;
    // int64_t, float, std::string
    virtual std::vector<boost::any>& GetValue() = 0;
    virtual void SetName(const std::string& name) = 0;
    virtual void SetValue(boost::any value) = 0;
    virtual hsql::ExprType GetType() = 0;
    virtual void SetType(hsql::ExprType type) = 0;
};

class Column: public ColumnI {
public:
    Column();
    inline std::string& GetName() { return name_; }
    inline std::vector<boost::any>& GetValue() { return value_; }
    inline void SetName(const std::string& name) { this->name_ = name; }
    void SetValue(boost::any value);
    hsql::ExprType GetType() { return type_; }
    void SetType(hsql::ExprType type) { this->type_ = type; }

private:
    hsql::ExprType type_;
    std::string name_;
    std::vector<boost::any> value_;
};

#endif
