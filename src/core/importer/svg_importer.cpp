#include "svg_importer.h"
#include <core/model/layers/layer.h>

#include <format>
#include <parser_rapidxml_ns.hpp>
#include <rapidxml_ns/rapidxml_ns.hpp>
#include <svgpp/policy/xml/rapidxml_ns.hpp>
#include <svgpp/svgpp.hpp>

namespace alive::importer {

Layers SvgImporter::import(const std::filesystem::path &path, Messages &msgs)
{
    XMLDocument xml_doc;
    try {
        xml_doc.load(path.string().c_str());
    } catch (svgpp::exception_base const &e) {
        typedef boost::error_info<svgpp::tag::error_info::xml_element, XMLElement> element_error_info;
        std::string error = std::format("Error reading file {} ", path.string());
        msgs.emplace_back(MessageType::e_Error, error);
        if (XMLElement const *element = boost::get_error_info<element_error_info>(e)) {
            error = std::format(" in element {}\n",
                                std::string((*element)->name(),
                                            (*element)->name() + (*element)->name_size()));
            msgs.emplace_back(MessageType::e_Error, error);
        }

        msgs.emplace_back(MessageType::e_Error, e.what());
    } catch (std::exception const &e) {
        msgs.emplace_back(MessageType::e_Error,
                          std::format("Error reading file {}: {}\n", path.string(), e.what()));
    }

    return {};
}

} // namespace alive::importer
