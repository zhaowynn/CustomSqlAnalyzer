#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <string>
#include <unordered_map>
#include "boost/any.hpp"
#include "rwlock.h"

class SQLExecutor;
class DocumentI;


class Controller {
public:
    Controller();
    ~Controller();
    SQLExecutor* Parse(const std::string& query);
    bool Handle(DocumentI& doc);

protected:
    SQLExecutor* ParseQuery(const std::string& query);

    RWLock rwlock_;
    std::unordered_map<std::string, SQLExecutor*> executors_;
};

#endif
