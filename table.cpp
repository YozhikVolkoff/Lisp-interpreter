#include "table.h"

Name* find_name(std::string str, const std::string& table_name, const int& ins,
    NameType type, int parenth_cnt) {
    // ������� ������ � ������� table_name (kword ��� name)
    // ���� ins!=0, �� ���������� ���������� �������� � �������

    assert(table_name == "kword" || table_name == "name");
    std::vector<Name*>* table;
    int size;
    if (table_name == "kword") {
        table = &kword_table;
        size = kword_table.size();
    }
    else {
        table = &name_table;
        size = name_table.size();
    }

    // ���-������
    int hi = 0;
    std::string::iterator pp = str.begin();
    // ��������� �� ������������ ��� ������ ������ ������.
    // ����� �������� ��� ����, ����� ������ ��� �����
    // (��������� ������������� ������ ������ �����)
    while (pp != str.end())
    {
        hi <<= 1;
        hi ^= *pp++;
    }
    if (hi < 0)
        hi = -hi;
    // ����� �������
    hi %= size;

    // �����, ���������� ����� ��������� �������������
    for (Name* n = (*table)[hi]; n != nullptr; n = n->m_next)
        // ���� �����, ���������� ���-������
        if (!n->m_str.compare(str))
            return n;

    // ������ �� �����
    if (ins == 0)
        return nullptr;
    // ��������� ���
    Name* nn = new Name(str, type, parenth_cnt);
    nn->m_next = (*table)[hi];
    (*table)[hi] = nn;
    return nn;
}

void set_tables() {
    kword_table = std::vector<Name*>(KWORDS_TABLE_SIZE, nullptr);
    name_table = std::vector<Name*>(NAME_TABLE_SIZE, nullptr);

    find_name("+", "kword", 1);
    find_name("-", "kword", 1);
    find_name("*", "kword", 1);
    find_name("/", "kword", 1);
    find_name("=", "kword", 1);
    find_name("/=", "kword", 1);
    find_name("<", "kword", 1);
    find_name("<=", "kword", 1);
    find_name(">", "kword", 1);
    find_name(">=", "kword", 1);
    find_name("set", "kword", 1);
    find_name("car", "kword", 1);
    find_name("cdr", "kword", 1);
    find_name("cons", "kword", 1);
    find_name("eval", "kword", 1);
    find_name("quote", "kword", 1);
    find_name("list", "kword", 1);
    find_name("print", "kword", 1);
}