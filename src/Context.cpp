#include "Context.hpp"

void Context::name(std::string const& name) {
    m_name = name;
}
std::string Context::name() const {
    return m_name;
}

void Context::gdPath(std::filesystem::path const& path) {
    m_gdPath = path;
}
std::filesystem::path Context::gdPath() const {
    return m_gdPath;
}

Context::Context(std::filesystem::path const& path) {
    m_gdPath = path;
}

Contexts::~Contexts() {
    for (auto& c : m_contexts) {
        delete c;
    }
}

Contexts* Contexts::get() {
    static auto inst = new Contexts();
    return inst;
}

std::vector<Context*> Contexts::all() const {
    return m_contexts;
}
