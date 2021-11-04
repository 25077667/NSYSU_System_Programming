#include "dict.hpp"
#include <unordered_map>

namespace
{
std::unordered_map<FILE *, int> dict;
}  // namespace

void insert(int pid, FILE *fp)
{
    dict.insert({fp, pid});
}

void drop()
{
    for (auto &[fp, _] : dict)
        ::fclose(fp);

    dict.clear();
}

void erase(FILE *fp)
{
    dict.erase(fp);
}

int get_pid(FILE *fp)
{
    return dict.find(fp)->second;
}