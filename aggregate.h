#ifndef _AGGREGATE_H_
#define _AGGREGATE_H_

#include <iostream>
#include <string>
#include "boost/any.hpp"


enum FunType {
    E_NONE = 0,
    E_SUM,
    E_MAX,
    E_MIN,
    E_COUNT,
    E_RATE,
    E_AVG
};

class AggFunUtil {
public:
    static FunType Str2FunType(const std::string& name);
    static bool IsBool(const boost::any& value)  { return value.type() == typeid(bool); }
    static bool IsInt(const boost::any& value)   { return value.type() == typeid(int); }
    static bool IsFloat(const boost::any& value) { return value.type() == typeid(float); }
    static bool IsInt64(const boost::any& value) { return value.type() == typeid(int64_t); }
    static bool IsString(const boost::any& value){ return value.type() == typeid(std::string); }
};

class AggregateI {
public:
    virtual ~AggregateI() {}
    virtual void Set(boost::any& val) = 0;
    virtual void Reset() { value_ = 0; }
    virtual boost::any Get() { return value_; }
protected:
    boost::any value_;
};

class SUM : public AggregateI {
public:
    void Set(boost::any& val);
};

class MAX : public AggregateI {
public:
    void Set(boost::any& val);
};

class MIN : public AggregateI {
public:
    void Set(boost::any& val);
};

class COUNT : public AggregateI {
public:
    void Set(boost::any& val);
};

class RATE : public AggregateI {
public:
    RATE(): sucess_(0), fail_(0) {}
    void Set(boost::any& val);
    boost::any Get();
private:
    int sucess_;
    int fail_;
};

class AVG : public AggregateI {
public:
    AVG(): count_(0) {}
    void Set(boost::any& val);
    boost::any Get();
private:
    int count_;
};

#endif
