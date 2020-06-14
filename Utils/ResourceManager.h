#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Core/Object.h"
#include "DebugTools.h"
#include "LazyFile.h"
#include <map>

class ResourceManager
    : Object
{
    OBJNAME(ResourceManager)
private:
    /// File holding a list of all resources
    LazyFile m_res_file;
    /// All resources should be listed and referred to relative to this path
    std::filesystem::path m_res_base_path;
    /// Filename, LazyFile
    std::map<std::string, LazyFile> m_resources;

public:
    ResourceManager(const std::filesystem::path& res_file_path);

    void reload_resfile();

    void operator()() {
    }
};

#endif // RESOURCEMANAGER_H
