#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Core/Object.h"
#include "DebugTools.h"
#include "LazyFile.h"

class ResourceManager
    : Object
{
    OBJECT(ResourceManager)
private:
    /// File holding a list of all resources
    LazyFile m_res_file;

public:
    ResourceManager(const std::filesystem::path& res_file_path);

    void operator()() {
    }
};

#endif // RESOURCEMANAGER_H
