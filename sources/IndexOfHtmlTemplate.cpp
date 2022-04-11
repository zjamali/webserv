#include "../headers/IndexOfHtmlTemplate.hpp"

IndexOfHtmlTemplate::IndexOfHtmlTemplate(std::string const &root, std::string const &path)
{
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir((root + path).c_str())))
    {
        struct stat sb;

        _content += "<!DOCTYPE html><html><head> <title>";
        _content += "index of " + path;
        _content += "</title></head><body>";
        _content += "<h1> index of " + path;
        _content += "</h1><hr>";
        _content += "<table style=\"width:100%\">";
        char buffer[100];
        struct tm t;
        int i = 0;
        while ((entry = readdir(dir)) != NULL)
        {
            if (i >= 1) // skip  current directory link ./
            {
                memset(buffer, 0, 100);
                _content += "<tr>";
                if (stat((root + path + entry->d_name).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
                {
                    _content += "<td>";
                    _content += "<a href=";
                    _content += "\"" + path + std::string(entry->d_name) + "\">";
                    _content += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-folder-fill\" viewBox=\"0 0 16 16\"><path d=\"M9.828 3h3.982a2 2 0 0 1 1.992 2.181l-.637 7A2 2 0 0 1 13.174 14H2.825a2 2 0 0 1-1.991-1.819l-.637-7a1.99 1.99 0 0 1 .342-1.31L.5 3a2 2 0 0 1 2-2h3.672a2 2 0 0 1 1.414.586l.828.828A2 2 0 0 0 9.828 3zm-8.322.12C1.72 3.042 1.95 3 2.19 3h5.396l-.707-.707A1 1 0 0 0 6.172 2H2.5a1 1 0 0 0-1 .981l.006.139z\"/></svg>";
                    _content += "&nbsp;";
                    _content += entry->d_name;
                    _content += "/";
                    _content += "</a>";
                    _content += "</td>";
                    if (i > 1)
                    {
                        _content += "<td>";
                        localtime_r(&(sb.st_mtimespec.tv_sec), &t);
                        strftime(buffer, 80, "%d-%h-%Y  %T", &t);
                        _content += buffer;
                        _content += "</td>";

                        _content += "<td>";
                        _content += "-";
                        _content += "</td>";
                    }
                }
                else
                {
                    _content += "<td>";
                    _content += "<a href=";
                    _content += "\"" + path + std::string(entry->d_name) + "\">";
                    _content += "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-file-earmark-fill\" viewBox=\"0 0 16 16\"><path d=\"M4 0h5.293A1 1 0 0 1 10 .293L13.707 4a1 1 0 0 1 .293.707V14a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2V2a2 2 0 0 1 2-2zm5.5 1.5v2a1 1 0 0 0 1 1h2l-3-3z\"/></svg>";
                    _content += "&nbsp;";
                    _content += entry->d_name;
                    _content += "</a>";
                    _content += "</td>";

                    _content += "<td>";
                    localtime_r(&(sb.st_mtimespec.tv_sec), &t);
                    strftime(buffer, 80, "%d-%h-%Y  %T", &t);
                    _content += buffer;
                    _content += "</td>";

                    _content += "<td>";
                    _content += std::to_string(sb.st_size);
                    _content += "</td>";
                }
                _content += "</tr>";
            }
            i++;
        }
        _content += "</table>";
        closedir(dir);
        _content += "</body></html>";
    }
}

IndexOfHtmlTemplate::~IndexOfHtmlTemplate()
{
}