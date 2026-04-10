#include "database.h"
namespace alive::core {

Database::Database(const std::string &name, DataBaseId id)
    : m_name(name)
    , m_id(id)
{}

Database::~Database() {}
} // namespace alive::core
