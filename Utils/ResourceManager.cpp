#include "ResourceManager.h"

ResourceManager::ResourceManager(const std::filesystem::path& res_file_path)
    : m_res_file(res_file_path) {
    ASSERT(m_res_file.is_valid()); // FIXME: Handle all these asserts properly?
    auto res_file_contents_result = m_res_file.get();
    ASSERT(res_file_contents_result.ok());
    auto& contents = res_file_contents_result.value().get();
    // TODO: CONTINUE
}
