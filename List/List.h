#ifndef __SYM_CLASSIC_LIST__
#define __SYM_CLASSIC_LIST__

#include <stdlib.h>

#include "..\Libs\Logging\Logging.h"
#include "..\Libs\Errors.h"

const void*       POISON_PTR         = (void*)13;
const ListElem_t  POISON             = 0X7FFFFFFF;
const char        COMAND_PROTOTYPE[] = "Dot GraphicDumps/dump%d -o GraphicDumps/Dump%d.png -T png";
      int         GRAPHIC_DUMP_CNT   = 0;

typedef struct ListElem 
{
    ListElem_t val  = 0;
    ListElem*  next = nullptr;
    ListElem*  prev = nullptr;
} ListElem;

typedef struct LogInfo 
{
    const char* name     = "(null)";
    const char* function = "(null)";
    const char* file     = "(null)";
    int         line     = POISON;
    bool        status   = false;
} LogInfo;

typedef struct List 
{
    size_t    size      = 0;
    ListElem* null_elem = nullptr;
    LogInfo   debug     = {};
}List;

int  ListCheck(List* list);

int  ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file);

int  ListDtor(List* list);

void DumpList(List* list, const char* function, const char* file, int line);

void GraphicDump(List* list);

int ListInsert(List* list, ListElem_t value, ListElem* after_which, ListElem** index = nullptr);

int ListRemove(List* list, ListElem* index);

int FindFree(List* list, ListElem** index);

int ListIterate(List* list, ListElem** index);

int ListBegin(List* list, ListElem** index);

int ListEnd(List* list, ListElem** index);

int LogicalAddresToPhys(List* list, int logic_index, ListElem** physic_index);

int LogicalAddresToPhys(List* list, int logic_index, ListElem** physic_index)
{
    ReturnIfError(ListCheck(list));

    CHECK(physic_index == nullptr, "Pointer to physic index = nullptr", -1);

    ListElem* index = list->null_elem;
    *physic_index = nullptr;

    for(int i = 0; i < logic_index; i++)
    {
        ListIterate(list, &index);
        CHECK(index == nullptr, "Element not found\n", 0);
    }

    *physic_index = index;
    
    return 0;
}

int ListIterate(List* list, ListElem** index)
{
    ReturnIfError(ListCheck(list));
    CHECK(index == nullptr || index == POISON_PTR, "index = nullptr", -1);
    
    if ((**index).next == nullptr)
        return -1;

    if ((**index).next != list->null_elem)
        *index = (**index).next;

    return 0;
}

int ListBegin(List* list, ListElem** index)
{
    ReturnIfError(ListCheck(list));

    if (list->size > 0)
        *index = list->null_elem->next;
    else
        *index = nullptr;

    return 0;
}

int ListEnd(List* list, ListElem** index)
{
    ReturnIfError(ListCheck(list));

    if (list->size > 0)
        *index = list->null_elem->prev;
    else
        *index = nullptr;

    return 0;    
}

void GraphicDump(List* list)
{
    char name[20] = "";
    sprintf(name, "GraphicDumps/dump%d", GRAPHIC_DUMP_CNT);
    FILE* fp = fopen(name, "w");

    fprintf(fp, "digraph G{\n");
    fprintf(fp, "node[shape = record, fontsize = 14];\n");
    fprintf(fp, "splines = ortho\n");

    if (list == nullptr || list == POISON_PTR) 
        return;
    if (list->null_elem == nullptr || list->null_elem == POISON_PTR)
        return;

    fprintf(fp, "info[label = \"size = %d\\n | null_elem = %X\"]\n", list->size, list->null_elem);

    ListElem* index = list->null_elem;
    for(int i = 0; i <= list->size; i++)
    {
        if (i == 0)
            fprintf(fp, "Node%X[label = \"<i>%X \\n | <v>%d \\n | {<p> %X |<n>  %X}\"]\n",
                              index,      index,  index->val,   index->prev, index->next);
        else
            fprintf(fp, "Node%X[style=filled, fillcolor = \"#8F9EFF\", label = \"<i>%X \\n |<v>%d \\n | {<p> %X |<n>  %X}\"]\n",
                              index,                                              index,   index->val,   index->prev, index->next);
        index = index->next;
    }

    fprintf(fp, "edge [style=\"invis\", color = \"white\"]\n");
    fprintf(fp, "info:n -> Node%X:w\n", list->null_elem);

    fprintf(fp, "edge [style=\"solid\", color = \"red\"]\n");
    index = list->null_elem;
    for(int i = 0; i <= list->size; i++)
    {
        ListElem* prev = index->prev;
        fprintf(fp, "Node%X -> Node%X\n", index, prev);
        index = prev;
        if (index == nullptr || index == list->null_elem)
            break;
    }

    fprintf(fp, "edge [color = \"blue\"]\n");
    index = list->null_elem;
    for(int i = 0; i <= list->size; i++)
    {
        ListElem* next = index->next;
        fprintf(fp, "Node%X -> Node%X\n", index, next);
        index = next;
        if (index == nullptr || index == list->null_elem)
            break;
    }

    fprintf(fp, "}");

    fclose(fp);

    char comand[50] = "";
    sprintf(comand, COMAND_PROTOTYPE, GRAPHIC_DUMP_CNT, GRAPHIC_DUMP_CNT);
    system(comand);
    
    char path[50] = "";
    LogPrintf("<hr>");
    LogPrintf("<img src=\"GraphicDumps/Dump%d.png\">\n", GRAPHIC_DUMP_CNT);
    LogPrintf("<hr>");

    GRAPHIC_DUMP_CNT++;
}

#define DUMP_L(list) DumpList(list, __PRETTY_FUNCTION__, __FILE__, __LINE__)

void DumpList(List* list, const char* function, const char* file, int line)
{
    LogPrintf("\nDump in %s(%d) in function %s\n", file, line, function);

    ListCheck(list);
    
    if (list == nullptr || list == POISON_PTR) 
        return;

    LogPrintf("Stack[%p] ", list);

    if (list->debug.name != nullptr && list->debug.name != POISON_PTR)
        LogPrintf("\"%s\"", list->debug.name);
    if (list->debug.function != nullptr && list->debug.function != POISON_PTR)
        LogPrintf("created at %s ", list->debug.function);
    if (list->debug.file != nullptr && list->debug.file != POISON_PTR)
        LogPrintf("at %s(%d):", list->debug.file);
    LogPrintf("\n");

    LogPrintf("Status: ");
    if (list->debug.status)
        LogPrintf("enable\n");
    else
        LogPrintf("disable\n");

    LogPrintf("{\n");
    LogPrintf("\tsize     = %zu\n", list->size);

    if (list->null_elem != nullptr && list->null_elem != POISON_PTR)
    {
        LogPrintf("\tdata:");
        ListElem* index = list->null_elem;
        for(int i = 0; i <= list->size; i++)
        {
            LogPrintf("|");
            PrintElemInLog(index->val);
            index = index->next;
        }
        LogPrintf("|\n");

        LogPrintf("\tnext:");
        index = list->null_elem;
        for(int i = 0; i <= list->size; i++)
        {
            LogPrintf("|%10p", index->next);  
            index = index->next;  
        }
        LogPrintf("|\n");
            
        LogPrintf("\tprev:");
        index = list->null_elem;
        for(int i = 0; i <= list->size; i++)
        {
            LogPrintf("|%10p", index->prev);  
            index = index->next;  
        }
            
        LogPrintf("|\n");
    }

    LogPrintf("}\n\n");
}

int ListCheck(List* list)
{
    int error = 0;
    if (list == nullptr || list == POISON_PTR)
        error |= NULL_LIST_POINTER;
    else
    {
        if (list->size      == POISON)     error |= WRONG_SIZE;
        if (list->null_elem == nullptr || list->null_elem == POISON_PTR)
            error != DAMAGED_POINTER_TO_NULL_ELEM;
        
        if (list->debug.file     == nullptr ||
            list->debug.file     == POISON_PTR) error |= DEBUG_FILE_DAMAGED;
        if (list->debug.function == nullptr ||
            list->debug.function == POISON_PTR) error |= DEBUG_FUNCTION_DAMAGED;
        if (list->debug.name     == nullptr ||
            list->debug.name     == POISON_PTR) error |= DEBUG_NAME_DAMAGED;
        if (list->debug.line     == POISON)     error |= DEBUG_LINE_DAMAGED;
    }

    LogAndParseErr(error != 0, error);
    
    return error;
}

#define ListCtor(list, capacity)  ListConstructor(list, capacity, __LINE__, #list, __PRETTY_FUNCTION__, __FILE__)

int ListConstructor(List* list, int capacity, int line, const char* name, const char* function, const char* file)
{
    LogAndParseErr(list == nullptr, NULL_LIST_POINTER);

    list->size      = 0;
    list->null_elem = (ListElem*)calloc(1, sizeof(ListElem));
    list->null_elem->next = list->null_elem;
    list->null_elem->prev = list->null_elem;

    list->debug.name     = name;
    list->debug.function = function;
    list->debug.file     = file;
    list->debug.line     = line;
    list->debug.status   = true;

    return ListCheck(list);
}

int ListDtor(List* list)
{
    ListCheck(list);

    list->size      = POISON;

    ListElem* index = list->null_elem;
    for(int i = 0; i <= list->size; i++)
    {
        ListElem* new_index = index->next;
        free(index);
        index = new_index;
    }
    list->null_elem = (ListElem*)POISON_PTR;

    list->debug.file     = (const char*)POISON_PTR;
    list->debug.function = (const char*)POISON_PTR;
    list->debug.name     = (const char*)POISON_PTR;
    list->debug.line     = POISON;

    return 0;
}

int FindFree(List* list, ListElem** index)
{
    ReturnIfError(ListCheck(list));

    *index = (ListElem*)calloc(1, sizeof(ListElem));

    return 0;
}

int ListRemove(List* list, ListElem* index)
{
    ReturnIfError(ListCheck(list));

    CHECK(index == nullptr || index == nullptr, "Error index", -1);

    ListElem* next = index->next;
    ListElem* prev = index->prev;

    CHECK(next == nullptr || prev == nullptr, "Index to not inserted element", -1);
    
    next->prev = prev;
    prev->next = next;

    index->val  = POISON;
    index->prev = nullptr;
    index->next = nullptr;
    free(index);
 
    list->size--;

    return 0;
}


int ListInsert(List* list, ListElem_t value, ListElem* after_which, ListElem** index)
{
    ReturnIfError(ListCheck(list));

    CHECK(after_which == nullptr, "Error index", -1);
    
    CHECK(after_which->next == nullptr || after_which->prev == nullptr, "Index to not inserted element", -1);

    ListElem* new_elem = nullptr;
    ReturnIfError(FindFree(list, &new_elem));

    if (index != nullptr && index != POISON_PTR)
        *index = new_elem;
    
    new_elem->val = value;
    
    ListElem* next = after_which->next;
    new_elem->next = after_which->next;
    new_elem->prev = after_which;

    next->prev        = new_elem; 
    after_which->next = new_elem;

    list->size++;

    return 0;
}

#endif //__SYM_CLASSIC_LIST__