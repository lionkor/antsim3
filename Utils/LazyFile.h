#ifndef LAZYFILE_H
#define LAZYFILE_H

#include <filesystem>
#include <string>
#include <vector>
#include <sys/stat.h>

#include "DebugTools.h"
#include "Core/Object.h"
#include "Result.h"

class LazyFile
    : public Object
{
    OBJECT(LazyFile)

private:
    Result<bool>              m_validation_result;
    std::vector<std::uint8_t> m_data;
    std::filesystem::path     m_path;
    struct stat               m_stat;
    bool                      m_loaded { false };

    void reset();
    void validate();

public:
    LazyFile(const std::filesystem::path&);
    LazyFile(LazyFile&&) = default;
    LazyFile& operator=(LazyFile&&) = default;

    LazyFile(const LazyFile&) = delete;
    LazyFile& operator=(const LazyFile&) = delete;


    [[nodiscard]] Result<std::reference_wrapper<std::vector<std::uint8_t>>> get();

    bool        is_valid() const { return m_validation_result.ok(); }
    std::string validation_error_message() const { return m_validation_result.message(); }
    std::size_t size_on_disk() const { return m_stat.st_size; }

    [[nodiscard]] Result<bool> force_reload();
    [[nodiscard]] Result<bool> restat();
    void                       force_unload();

    const std::filesystem::path& path() const { return m_path; }
    std::filesystem::path        filename() const { return m_path.filename(); }

    /// Whether the file has changed on disk. update the LazyFile with restat()
    bool has_changed_on_disk() const;

    // Object interface
public:
    virtual std::stringstream to_stream() const override;
};

#endif // LAZYFILE_H
