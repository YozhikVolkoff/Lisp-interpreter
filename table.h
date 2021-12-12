#ifndef __TABLE__
#define __TABLE__

#define KWORDS_TABLE_SIZE 128
#define NAME_TABLE_SIZE 512

#include <vector>
#include <cassert>
#include "name.h"

extern std::vector<Name*> kword_table;
extern std::vector<Name*> name_table;

Name* find_name(std::string str, const std::string& table_name, const int& ins,
    NameType type = NameType::UNDEF, int parenth_cnt = 0);

void set_tables();
#endif