// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "LazyFile.h"
#include <fstream>
#include <cstdio>

void LazyFile::reset() {
    m_data.clear();
    m_data.shrink_to_fit();
    m_loaded = false;
}

void LazyFile::validate() {
    try {
        m_validation_result.set_value(true);
        if (!std::filesystem::exists(m_path)) {
            m_validation_result.set_error("file {} does not exist", m_path);
        }
        if (m_validation_result) {
            m_path = std::filesystem::absolute(m_path);
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
    } catch (std::exception& e) {
        m_validation_result.set_error("(fatal) exception validating file {}: {}", m_path, e.what());
        report_error(m_validation_result.message());
    }
}

Result<bool> LazyFile::restat() {
    Result<bool> result;
    struct stat new_stat;
    auto ret = stat(m_path.c_str(), &new_stat);
    if (ret != 0) {
        result.set_error("stat \"{}\" failed with error: {}", m_path, std::strerror(errno));
        report_error(result.message());
        return result;
    }
    m_stat = new_stat;
    return result.set_value(true);
}

void LazyFile::force_unload() {
    reset();
}

LazyFile::LazyFile(const std::string& path)
    : m_path(path) {
    validate();
}

Result<bool> LazyFile::force_reload() {
    reset();
    Result<bool> result;
    auto res = restat();
    if (!res) {
        result.set_error("force_reload {} failed: {}", m_path, res.message());
        report(result.message());
        return result;
    }
    // if this fails, m_validation_result.error() will be true
    validate();
    if (m_validation_result.error()) {
        result.set_error("force_reload {} failed: {}", m_path, m_validation_result.message());
        report(result.message());
        return result;
    }

    // reload
    m_loaded = false;
    auto load_result = load();
    if (!load_result) {
        result.set_error("force_reload {} failed.", m_path);
        report(result.message());
        return result;
    }

    return result.set_value(true);
}

std::vector<uint8_t>* LazyFile::load() {
    // if we loaded before, just return the data
    if (m_validation_result && m_loaded) {
        report("already loaded {}", m_path);
        return &m_data;
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
        report("loading {}!", m_path);
        if (!m_validation_result) {
            report_error(m_validation_result.message());
            return nullptr;
        }

        auto file_size = size_on_disk();
        m_data.resize(file_size);

        // open
        FILE* c_file = std::fopen(m_path.c_str(), "rb");

        if (!c_file) {
            report_error("fopen {} failed with error: {}", m_path, std::strerror(errno));
            reset();
            return nullptr;
        }

        // read
        auto read_size = std::fread(reinterpret_cast<void*>(m_data.data()), sizeof(std::uint8_t), file_size, c_file);

        if (read_size != file_size) {
            report_error("fread {} did not return expected value {}, instead returned {}", m_path, file_size, read_size);
            reset();
            return nullptr;
        }

        // close
        auto ret = std::fclose(c_file);

        if (ret != 0) {
            report_warning("fclose {} failed, might be bad");
        }

        // return
        m_loaded = true;
        return &m_data;

    } catch (std::exception& e) {
        report_error(e.what());
        reset();
        return nullptr;
    }
}

bool LazyFile::has_changed_on_disk() const {
    struct stat new_stat;
    auto ret = stat(m_path.c_str(), &new_stat);
    if (ret != 0) {
        return true;
    }
    return new_stat.st_mtim.tv_sec != m_stat.st_mtim.tv_sec || new_stat.st_ctim.tv_sec != m_stat.st_ctim.tv_sec;
}

std::stringstream LazyFile::to_stream() const {
    TS_BEGIN(Object);
    TS_PROP_M(is_valid());
    TS_PROP_M(m_path);
    TS_PROP_M(m_loaded);
    TS_END();
}
