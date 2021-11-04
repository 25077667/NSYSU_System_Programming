#include "fmtcmd.hpp"
#include <cstring>
#include <string>

void format_cmd(char *cmd, const char *tok)
{
    const std::string &s =
        std::string(" ") + std::string(tok) + std::string(" ");
    ::strncat(cmd, s.c_str(), s.length());
}