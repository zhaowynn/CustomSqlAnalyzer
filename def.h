#ifndef _DEF_H_
#define _DEF_H_

#include <string>
#include <vector>
#include <set>

typedef struct tData {
    enum EDataType {
        E_NONE = 0,
        E_INT,
        E_FLOAT,
        E_STRING
    };

    tData(EDataType type=E_NONE): type(type) {}
    EDataType type;
    union {
        int ival;
        float fval;
    };
    std::string sval;

    inline bool operator == (const tData& value) {
        if (this->type != value.type)
            return false;
        else if (E_INT == type)
            return this->ival == value.ival;
        else if (E_FLOAT == type)
            return this->fval == value.fval;
        else if (E_STRING == type)
            return this->sval == value.sval;
        return false;
    }
    inline friend bool operator == (const tData& v1, const tData& v2) {
        if (v1.type != v2.type)
            return false;
        else if (E_INT == v1.type)
            return v1.ival == v2.ival;
        else if (E_FLOAT == v1.type)
            return v1.fval == v2.fval;
        else if (E_STRING == v1.type)
            return v1.sval == v2.sval;
        return false;
    }
    inline bool operator < (const tData& value) {
        if (E_INT == type)
            return this->ival < value.ival;
        else if (E_FLOAT == type)
            return this->fval < value.fval;
        else if (E_STRING == type)
            return this->sval < value.sval;
        return false;
    }
    inline friend bool operator < (const tData& v1, const tData& v2) {
        if (E_INT == v1.type)
            return v1.ival < v2.ival;
        else if (E_FLOAT == v1.type)
            return v1.fval < v2.fval;
        else if (E_STRING == v1.type)
            return v1.sval < v2.sval;
        return false;
    }
    inline bool operator > (const tData& value) {
        if (E_INT == type)
            return this->ival > value.ival;
        else if (E_FLOAT == type)
            return this->fval > value.fval;
        else if (E_STRING == type)
            return this->sval > value.sval;
        return false;
    }
    inline friend bool operator > (const tData& v1, const tData& v2) {
        if (E_INT == v1.type)
            return v1.ival > v2.ival;
        else if (E_FLOAT == v1.type)
            return v1.fval > v2.fval;
        else if (E_STRING == v1.type)
            return v1.sval > v2.sval;
        return false;
    }
} tData;

#endif
