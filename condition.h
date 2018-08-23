#ifndef _CONDITION_H_
#define _CONDITION_H_

#include <string>
#include <list>
#include <algorithm>
#include <iterator>
#include "SQLParser.h"
#include "util/sqlhelper.h"

class ColumnI;
class DocumentI;


class Condition {
public:
    Condition();
    ~Condition();

    ColumnI* GetColumn() const;
    void SetColumn(ColumnI* column);
    Condition* GetLeft() const;
    void SetLeft(Condition* left);
    hsql::OperatorType GetOpType() const;
    void SetOpType(hsql::OperatorType optype);
    Condition* GetRight() const;
    void SetRight(Condition* right);

    bool Match(DocumentI& doc);

protected:
    hsql::OperatorType optype_;
    ColumnI* column_;
    Condition* left_;
    Condition* right_;
};

#endif
