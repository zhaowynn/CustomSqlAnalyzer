#include "aggregate.h"


FunType AggFunUtil::Str2FunType(const std::string& name) {
    if (name == "SUM")
        return E_SUM;
    else if (name == "MAX")
        return E_MAX;
    else if (name == "MIN")
        return E_MIN;
    else if (name == "COUNT")
        return E_COUNT;
    else if (name == "RATE")
        return E_RATE;
    else if (name == "AVG")
        return E_AVG;
    return E_NONE;
}


void SUM::Set(boost::any& val) {
    if (value_.empty()) {
        value_ = val;
        return;
    }
    if (AggFunUtil::IsInt(val)) {
        value_ = boost::any_cast<int>(value_) + boost::any_cast<int>(val);
    } else if (AggFunUtil::IsFloat(val)) {
        value_ = boost::any_cast<float>(value_) + boost::any_cast<float>(val);
    } else if (AggFunUtil::IsInt64(val)) {
        value_ = boost::any_cast<int64_t>(value_) + boost::any_cast<int64_t>(val);
    }
}


void MAX::Set(boost::any& val) {
    if (value_.empty()) {
        value_ = val;
        return;
    }
    if (AggFunUtil::IsInt(val)) {
        if (boost::any_cast<int>(val) > boost::any_cast<int>(value_)) {
            value_ = val;
        }
    } else if (AggFunUtil::IsFloat(val)) {
        if (boost::any_cast<float>(val) > boost::any_cast<float>(value_)) {
            value_ = val;
        }
    } else if (AggFunUtil::IsInt64(val)) {
        if (boost::any_cast<int64_t>(val) > boost::any_cast<int64_t>(value_)) {
            value_ = val;
        }
    }
}


void MIN::Set(boost::any& val) {
    if (value_.empty()) {
        value_ = val;
        return;
    }
    if (AggFunUtil::IsInt(val)) {
        if (boost::any_cast<int>(val) < boost::any_cast<int>(value_)) {
            value_ = val;
        }
    } else if (AggFunUtil::IsFloat(val)) {
        if (boost::any_cast<float>(val) < boost::any_cast<float>(value_)) {
            value_ = val;
        }
    } else if (AggFunUtil::IsInt64(val)) {
        if (boost::any_cast<int64_t>(val) < boost::any_cast<int64_t>(value_)) {
            value_ = val;
        }
    }
}


void COUNT::Set(boost::any& val) {
    if (value_.empty()) {
        value_ = val;
        return;
    }
    if (AggFunUtil::IsInt(val)) {
        value_ = boost::any_cast<int>(value_) + boost::any_cast<int>(val);
    } else if (AggFunUtil::IsFloat(val)) {
        value_ = boost::any_cast<float>(value_) + boost::any_cast<float>(val);
    } else if (AggFunUtil::IsInt64(val)) {
        value_ = boost::any_cast<int64_t>(value_) + boost::any_cast<int64_t>(val);
    }
}


void RATE::Set(boost::any& val) {
    bool ret = false;
    if (AggFunUtil::IsInt(val)) {
        ret = boost::any_cast<int>(val) > 0 ? true : false;
    } else if (AggFunUtil::IsFloat(val)) {
        ret = boost::any_cast<bool>(val);
    } else if (AggFunUtil::IsFloat(val)) {
        ret = boost::any_cast<float>(val) > 0 ? true : false;
    }
    if (ret) {
        ++sucess_;
    } else {
        ++fail_;
    }
}

boost::any RATE::Get() {
    if(sucess_ == 0 && fail_ == 0) {
        return (float)0;
    }
    return (float)sucess_ / (sucess_ + fail_);
}


void AVG::Set(boost::any& val) {
    ++count_;
    if (value_.empty()) {
        value_ = val;
        return;
    }
    if (AggFunUtil::IsInt(val)) {
        value_ = boost::any_cast<int>(value_) + boost::any_cast<int>(val);
    } else if (AggFunUtil::IsFloat(val)) {
        value_ = boost::any_cast<float>(value_) + boost::any_cast<float>(val);
    } else if (AggFunUtil::IsInt64(val)) {
        value_ = boost::any_cast<int64_t>(value_) + boost::any_cast<int64_t>(val);
    }
}

boost::any AVG::Get() {
    if (0 == count_)
        return (float)0;
    if (AggFunUtil::IsInt(value_)) {
         return (count_ == 0) ? 0 : (float)boost::any_cast<int>(value_) / count_;
    } else if (AggFunUtil::IsFloat(value_)) {
        return (count_ == 0) ? 0 : (float)boost::any_cast<float>(value_) / count_;
    } else if (AggFunUtil::IsInt64(value_)) {
        return (count_ == 0) ? 0 : (float)boost::any_cast<int64_t>(value_) / count_;
    }
    return (float)0;
}

