#ifndef INDEXOFHTMLTEMPLATE
#define INDEXOFHTMLTEMPLATE
#include <dirent.h>
#include <sys/stat.h>
#include <string>

class IndexOfHtmlTemplate
{
private:
    std::string _content;
public:
    IndexOfHtmlTemplate(std::string const &root, std::string const &path);
   
    ~IndexOfHtmlTemplate();
    std::string const &getTable() {return _content;}
};


#endif