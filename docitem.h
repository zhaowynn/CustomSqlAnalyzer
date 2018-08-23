#ifndef _DOCITEM_H_
#define _DOCITEM_H_

#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include "boost/any.hpp"
#include "def.h"


class ItemI {
public:
    // handle fields
    virtual boost::any GetSum(const std::string& name) = 0;
    virtual boost::any GetMax(const std::string& name) = 0;
    virtual boost::any GetMin(const std::string& name) = 0;
    virtual bool GetRate(const std::string& name) = 0;
    virtual boost::any GetAvg(const std::string& name) = 0;
    virtual int GetCount(const std::vector<std::string>& names) = 0;
    virtual int GetCount(const std::vector<std::string>& names, const std::vector<std::string>& columns, std::vector<tData> values) = 0;
    virtual std::set<std::vector<tData> > getGroupBy(const std::vector<std::string>& names) = 0;

    // handle condition
    // value: 1.int64_t; 2. float
    virtual bool IsOpEquals(const std::string& name, boost::any& value) = 0;
    virtual bool IsOpNotEquals(const std::string& name, boost::any& value) = 0;
    virtual bool IsOpLess(const std::string& name, boost::any& value) = 0;
    virtual bool IsOpLessEq(const std::string& name, boost::any& value) = 0;
    virtual bool IsOpGreater(const std::string& name, boost::any& value) = 0;
    virtual bool IsOpGreaterEq(const std::string& name, boost::any& value) = 0;
    virtual bool IsOpIn(const std::string& name, std::vector<boost::any>& values) = 0;
};


struct RecordItem : public ItemI {
public:
    // data

    // aggregation
    boost::any GetValue(const std::string& name);
    boost::any GetSum(const std::string& name);
    boost::any GetMax(const std::string& name);
    boost::any GetMin(const std::string& name);
    bool GetRate(const std::string& name);
    boost::any GetAvg(const std::string& name);
    int GetCount(const std::vector<std::string>& names);
    int GetCount(const std::vector<std::string>& names, const std::vector<std::string>& columns, std::vector<tData> values);
    // groupby
    std::set<std::vector<tData> > getGroupBy(const std::vector<std::string>& names);
    // operation
    bool IsOpEquals(const std::string& name, boost::any& value);
    bool IsOpNotEquals(const std::string& name, boost::any& value);
    bool IsOpLess(const std::string& name, boost::any& value);
    bool IsOpLessEq(const std::string& name, boost::any& value);
    bool IsOpGreater(const std::string& name, boost::any& value);
    bool IsOpGreaterEq(const std::string& name, boost::any& value);
    bool IsOpIn(const std::string& name, std::vector<boost::any>& values);
};

#endif
