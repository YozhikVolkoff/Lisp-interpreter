//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : Declaration of the name table and the keyword table
//                find_name looks for (and maybe inserts) names in the tables
//-----------------------------------------------------------------------------

#ifndef __TABLE__
#define __TABLE__

#define KWORDS_TABLE_SIZE 128
#define NAME_TABLE_SIZE 512

#include <vector>
#include <cassert>
#include "name.h"

extern std::vector<Name*> kword_table;
extern std::vector<Name*> name_table;

// find name, if not found and ins != 0 insert the name into the table
Name* find_name(std::string str, const std::string& table_name, const int& ins,
    NameType type = NameType::UNDEF, int parenth_cnt = 0);

// allocate memory and set keywords
void set_tables();
#endif