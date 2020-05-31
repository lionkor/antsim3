#include "LazyFile.h"
#include <fstream>
#include <cstdio>

void LazyFile::reset() {
    m_data.clear();
}

Result<bool> LazyFile::restat() {
    Result<bool> result;
    struct stat  new_stat;
    auto         ret = stat(m_path.c_str(), &new_stat);
    if (ret != 0) {
        result.set_error("stat \"{}\" failed with error: {}", m_path, std::strerror(errno));
        report_error(result.message());
        return result;
    }
    m_stat = new_stat;
    return result.set_value(true);
}

LazyFile::LazyFile(const std::filesystem::path& path)
    : m_path(path) {
    m_validation_result.set_value(true);
    if (!std::filesystem::exists(path)) {
        m_validation_result.set_error("file {} does not exist", path);
    }
    if (m_validation_result) {
        m_path = std::filesystem::absolute(path);
        if (!std::filesystem::is_regular_file(m_path)) {
            m_validation_result.set_error("file {} is not a regular file", m_path);
        } else {
            auto res = restat();
            if (!res) {
                m_validation_result.set_error("restat for {} failed: {}", m_path, res.message());
            }
        }
    } else {
        report_error("Validating file {} failed: {}", m_path, m_validation_result.message());
    }
}

Result<bool> LazyFile::force_reload() {
    report_error("not implemented");
    return Result<bool>().set_error("ERROR");
}

Result<std::reference_wrapper<std::vector<uint8_t>>> LazyFile::load() {
    Result<std::reference_wrapper<std::vector<uint8_t>>> result;

    // if we loaded before, just return the data
    if (m_validation_result && m_loaded) {
        return result.set_value(m_data);
    }

    // loading is retty straight forward:
    // 1. open
    // 2. read
    // 3. close
    // 3.5 set m_loaded
    // 4. return
    // all of the steps have to be labelled and verified for success
    // on any critical error, report_error, set result.set_error and
    // call reset(), then return.

    try {
        if (!m_validation_result) {
            result.set_error(m_validation_result.message());
            report_error(result.message());
            reset();
            return result;
        }

        auto file_size = size_on_disk();
        m_data.resize(file_size);

        // open
        FILE* c_file = std::fopen(m_path.c_str(), "rb");

        if (!c_file) {
            result.set_error("fopen {} failed with error: {}", m_path, std::strerror(errno));
            report_error(result.message());
            reset();
            return result;
        }

        // read
        auto read_size = std::fread(reinterpret_cast<void*>(m_data.data()), sizeof(std::uint8_t), file_size, c_file);

        if (read_size != file_size) {
            result.set_error("fread {} did not return expected value {}, instead returned {}", m_path, file_size, read_size);
            report_error(result.message());
            reset();
            return result;
        }

        // close
        auto ret = std::fclose(c_file);

        if (ret != 0) {
            report_warning("fclose {} failed, might be bad");
        }

        // return
        m_loaded = true;
        return result.set_value(std::ref(m_data));

    } catch (std::exception& e) {
        report_error(e.what());
        reset();
        return result.set_error("An exception occurred in {}: {}", HERE(), e.what());
    }
}

std::stringstream LazyFile::to_stream() const {
    auto ss = Object::to_stream();
    ss << "valid=" << bool(m_validation_result) << ";"
       << "path=" << m_path << ";"
       << "loaded=" << m_loaded << ";"
       << "validation_error=\"" << m_validation_result.message() << "\";";
    return ss;
}
