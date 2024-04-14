#include "database.h"
namespace inae::core {

Database::Database(const std::string &name, DataBaseId id)
    : m_name(name)
    , m_id(id)
{}

Database::~Database() {}
} // namespace inae::core
