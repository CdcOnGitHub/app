#pragma once

#include "Manager.hpp"
#include <utils.hpp>
#include <filesystem>
#include <string>

class Context {
protected:
    std::filesystem::path m_gdPath;
    std::string m_name;

public:
    void name(std::string const& name);
    std::string name() const;

    void gdPath(std::filesystem::path const& name);
    std::filesystem::path gdPath() const;

    Context(std::filesystem::path const& gdPath);
};

class Contexts {
protected:
    std::vector<Context*> m_contexts;

public:
    ~Contexts();

    static Contexts* get();

    std::vector<Context*> all() const;
};
