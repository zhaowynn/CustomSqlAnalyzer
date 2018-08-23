#include "groupby.h"
#include "document.h"

GroupBy::GroupBy() {
}

GroupBy::GroupBy(const std::string& column) {
    this->columns_.push_back(column);
}

GroupBy::GroupBy(const std::vector<std::string>& columns): columns_(columns) {
}

GroupBy::~GroupBy() {
}

std::set<std::vector<tData> > GroupBy::Handle(DocumentI& doc) {
    return doc.HandleGroupBy(this);
}

void GroupBy::SetColumn(const std::string& column) {
    this->columns_.push_back(column);
}
