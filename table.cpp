#include "table.h"

Name* find_name(std::string str, const std::string& table_name, const int& ins,
    NameType type, int parenth_cnt) {
    // Функция поиска в таблице table_name (kword или name)
    // Если ins!=0, то происходит добавление элемента в таблицу

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

    // хэш-индекс
    int hi = 0;
    std::string::iterator pp = str.begin();
    // суммируем по исключающему ИЛИ каждый символ строки.
    // далее сдвигаем для того, чтобы индекс был лучше
    // (исключаем использование только одного байта)
    while (pp != str.end())
    {
        hi <<= 1;
        hi ^= *pp++;
    }
    if (hi < 0)
        hi = -hi;
    // берем остаток
    hi %= size;

    // поиск, используем метод линейного рехеширования
    for (Name* n = (*table)[hi]; n != nullptr; n = n->m_next)
        // если нашли, возвращаем хэш-индекс
        if (!n->m_str.compare(str))
            return n;

    // ничего не нашли
    if (ins == 0)
        return nullptr;
    // добавляем имя
    Name* nn = new Name(str, type, parenth_cnt);
    nn->m_next = (*table)[hi];
    (*table)[hi] = nn;
    return nn;
}

void set_tables() {
    kword_table = std::vector<Name*>(KWORDS_TABLE_SIZE, nullptr);
    name_table = std::vector<Name*>(NAME_TABLE_SIZE, nullptr);

    find_name("t", "name", 1, NameType::IDENT);
    find_name("nil", "name", 1, NameType::IDENT);
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
    find_name("and", "kword", 1);
    find_name("or", "kword", 1);
    find_name("not", "kword", 1);
    find_name("set", "kword", 1);
    find_name("print", "kword", 1);
    find_name("cond", "kword", 1);
    find_name("progn", "kword", 1);

    /*find_name("car", "kword", 1);
    find_name("cdr", "kword", 1);
    find_name("cons", "kword", 1);
    find_name("eval", "kword", 1);
    find_name("quote", "kword", 1);
    find_name("list", "kword", 1);*/
}