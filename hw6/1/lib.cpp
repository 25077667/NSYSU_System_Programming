#include "lib.h"

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace
{
std::unordered_map<int, std::string> wd_map_dir;
std::unordered_map<std::string, int> path_map_wd(100);
std::string buf;

inline std::string get_dir(const std::string &path, int base)
{
    return path.substr(0, base + 2);
}
}  // namespace

void add_watch_file(const char *path, int base, int wd)
{
    const std::string dir = get_dir(path, base);
    wd_map_dir[wd] = dir;
    path_map_wd[path] = wd;
}

void erase_watch_file(const char *path)
{
    path_map_wd.erase(path);
}

const char *concat(const char *s1, const char *s2)
{
    if (s1 == NULL && s2 == NULL)
        return "";
    else if (s1 == NULL && s2)
        return s2;
    else if (s2 == NULL)
        return s1;
    else
        buf = std::string(s1) + "/" + std::string(s2);
    return buf.c_str();
}

const char *wd2dir(int wd)
{
    return wd_map_dir[wd].c_str();
}
