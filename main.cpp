#include <stdio.h>
#include <string.h>

typedef int ListElem_t;

#include "Libs\Logging\Logging.h"

void PrintElemInLog(ListElem_t val);

#include "List\List.h"

const char logs[] = "ListLogs.html";

#define DEBUG

int main()
{
    OpenHtmlLogFile(logs);

    printf("Start\n");

    List list = {};
    ListCtor(&list, 0);
    GraphicDump(&list);
    
    ListElem* index = nullptr;
    ListInsert(&list, 100, list.null_elem, &index);
    GraphicDump(&list);

    ListInsert(&list, 200, index);
    
    ListInsert(&list, 300, index);

    ListInsert(&list, 400, index);

    ListInsert(&list, 500, index);

    ListInsert(&list, 600, index);

    GraphicDump(&list);

    ListRemove(&list, index);
    
    ListRemove(&list, list.null_elem->next);

    GraphicDump(&list);
    
    ListRemove(&list, list.null_elem->next);

    GraphicDump(&list);
    
    ListRemove(&list, list.null_elem->next);

    GraphicDump(&list);

    ListRemove(&list, list.null_elem->next);

    GraphicDump(&list);
    
    ListRemove(&list, list.null_elem->next);

    GraphicDump(&list);

    printf("End\n");
    CloseHtmlLogFile();
}

void PrintElemInLog(ListElem_t val)
{
    LogPrintf("%10d", val);
}