#include <iostream>
#include <map>
#include "fields.h"
#include "document.h"


Fields::Fields(FunType t, const std::vector<std::string>& columns, bool groupby):
    type_(t), columns_(columns), isgroupby_(groupby), aggregate_(nullptr), groupbyaggregate_(nullptr) {
    if (isgroupby_) {
        this->groupbyaggregate_ = new std::map<std::vector<tData>, AggregateI*>();
    } else {
        this->aggregate_ = getAggregate(t);
    }
}

Fields::~Fields() {
    if (aggregate_) {
        delete aggregate_;
    }
    if (groupbyaggregate_) {
        for (auto it = std::begin(*groupbyaggregate_); it != std::end(*groupbyaggregate_); ++it) {
            delete it->second;
        }
        delete groupbyaggregate_;
    }
}

bool Fields::Handle(DocumentI& doc) {
    return doc.HandleFields(this);
}

bool Fields::Handle(DocumentI& doc, const std::vector<std::string>& columns, std::vector<tData> values) {
    return doc.HandleFields(this, columns, values);
}

boost::any Fields::GetValue() {
    if (isgroupby_) {
        std::map<std::vector<tData>, boost::any> values;
        for (auto it = std::begin(*groupbyaggregate_); it != std::end(*groupbyaggregate_); ++it) {
            values[it->first] = it->second->Get();
        }
        return values;
    } else {
        return aggregate_->Get();
    }
}

void Fields::SetValue(boost::any value) {
    if (! isgroupby_) {
    	aggregate_->Set(value);
    }
}

void Fields::SetValue(const std::vector<tData>& key, boost::any value) {
    if (isgroupby_) {
        if (groupbyaggregate_->count(key) == 0) {
            (*groupbyaggregate_)[key] = new COUNT();
        }
        (*groupbyaggregate_)[key]->Set(value);
    }
}

AggregateI* Fields::getAggregate(FunType t) {
    AggregateI* p = nullptr;
    switch (t) {
        case E_SUM: {
            p = new SUM();
            break;
        }
        case E_MAX: {
            p = new MAX();
            break;
        }
        case E_MIN: {
            p = new MIN();
            break;
        }
        case E_COUNT: {
            p = new COUNT();
            break;
        }
        case E_RATE: {
            p = new RATE();
            break;
        }
        case E_AVG: {
            p = new AVG();
            break;
        }
        default: {
            break;
        }
    }
    return p;
}

