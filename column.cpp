#include "column.h"


Column::Column(): type_(hsql::kExprLiteralInt), name_("") {
}

void Column::SetValue(boost::any value) {
	this->value_.push_back(boost::any(value));
}
