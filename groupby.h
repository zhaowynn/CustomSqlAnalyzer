#ifndef _GROUPBY_H_
#define _GROUPBY_H_

#include <string>
#include <vector>
#include <set>
#include "def.h"

class DocumentI;


class GroupByI {
public:
    virtual ~GroupByI() {}
    virtual std::set<std::vector<tData> > Handle(DocumentI& doc) = 0;
    virtual void SetColumn(const std::string& column) = 0;
    virtual const std::vector<std::string>& GetColumns() = 0;
};


class GroupBy: public GroupByI {
public:
    GroupBy();
    GroupBy(const std::string& column);
    GroupBy(const std::vector<std::string>& columns);
    ~GroupBy();

    std::set<std::vector<tData> > Handle(DocumentI& doc);
    void SetColumn(const std::string& column);
    inline const std::vector<std::string>& GetColumns() { return columns_; }

private:
    std::vector<std::string> columns_;
};

#endif
