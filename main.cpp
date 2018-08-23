#include <stdlib.h>
#include <string>
#include "SQLParser.h"
#include "util/sqlhelper.h"
#include "controller.h"
#include "executor.h"
#include "document.h"
#include "docitem.h"



int main(int argc, char* argv[]) {
    if (argc <= 1) {
        fprintf(stderr,
                "Usage: ./example \"SELECT AVG(InternalLatency) FROM test GROUP BY Procedure;\"\n");
        return -1;
    }
    std::string query = argv[1];

    Controller controller;

    SQLExecutor* executor = controller.Parse(query);

    RecordItem item;


    Document doc(&item);
    controller.Handle(doc);

    executor->DebugDump();
    executor->Dump();
}

