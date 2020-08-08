// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "ResourceManager.h"
#include "stl_ext.h"

ResourceManager::ResourceManager(const std::filesystem::path& res_file_path)
    : m_res_file(res_file_path)
    , m_res_base_path(m_res_file.path().parent_path()) {
    if (res_file_path.empty()) {
        report_warning("resfile name empty, might cause confusing errors");
    } else {
        reload_resfile();
    }
}

void ResourceManager::reload_resfile() {
    static const std::string error_fmt = "A fatal occured trying to load/reload the res file: {}";
    auto result = m_res_file.force_reload();
    if (result.error()) { // FIXME: Handle all these asserts properly?
        report_error(error_fmt, result.message());
        return;
    }
    auto contents = m_res_file.load();
    if (!contents) {
        ASSERT_NOT_REACHABLE();
    }
    // clear previously loaded resources
    m_resources.clear();
    // parsing one resource per line

    // do a line count for debugging
    report("found {} lines in {}", std::count(contents->begin(), contents->end(), '\n'), m_res_file.filename());

    // read all lines
    auto previous_newline = contents->begin();
    for (;;) {
        auto next_newline = std::find(previous_newline, contents->end(), '\n');
        if (next_newline == contents->end()) {
            report("done");
            break;
        }
        std::string filename(previous_newline, next_newline);
        if (!filename.empty()) {
            LazyFile file(m_res_base_path / filename);
            if (file.is_valid()) {
                m_resources.insert_or_assign(filename, std::move(file));
                report("loaded {}", filename);
            } else {
                report_error("the file \"{}\" in {} is not valid: {}", filename, m_res_file.filename(), file.validation_error_message());
            }
        } else {
            report("skipping a line");
        }
        previous_newline = next_newline + 1;
    }

    // unload as not to waste space (this is what i made LazyFile for, come on!)
    m_res_file.force_unload();
}

Result<Ref<LazyFile>> ResourceManager::get_resource_by_name(const std::string& name) {
    Result<Ref<LazyFile>> result;
    if (m_resources.find(name) == m_resources.end()) {
        result.set_error(fmt::format("resource \"{}\" not found", name));
        return result;
    }
    result.set_value(m_resources.at(name));
    return result;
}

Managed<sf::Texture> ResourceManager::load_texture(const std::string& name) {
    if (m_resources.contains(name)) {
        Managed<sf::Texture> texture = make_managed<sf::Texture>();
        LazyFile& file = m_resources.at(name);
        if (file.is_valid()) {
            auto data = file.load();
            if (texture->loadFromMemory(data->data(), data->size())) {
                return texture;
            } else {
                report_error("an error occured loading texture from memory");
            }
        } else {
            report_error("error: {}", file.validation_error_message());
            // expensive bailout, as we dealloc the texture
        }
    } else {
        report_error("resource \"{}\" not found", name);
    }
    return nullptr;
}
