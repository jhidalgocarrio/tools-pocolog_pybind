#include <base/Time.hpp>
#include <iostream>
#include <pocolog_cpp/Format.hpp>
#include <pocolog_cpp/InputDataStream.hpp>
#include <pocolog_cpp/MultiFileIndex.hpp>
#include <pocolog_cpp/Stream.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <string>
#include <typelib/memory_layout.hh>
#include <typelib/typedisplay.hh>
#include <typelib/typemodel.hh>
#include <typelib/value.hh>
#include <typelib/value_ops.hh>

#include "Converter.cpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;
using namespace pybind11::literals;

Typelib::Value get_sample(pocolog_cpp::InputDataStream & stream, size_t sampleNr) {
    std::vector<uint8_t> data;
    if(!stream.getSampleData(data, sampleNr))
        throw std::runtime_error("Error, sample for stream " + stream.getName() + " could not be loaded");

    const Typelib::Type* m_type = stream.getType();

    std::vector<uint8_t> value_buffer;
    value_buffer.resize(m_type->getSize());
    auto value = Typelib::Value(value_buffer.data(), *m_type);
    //init memory area
    Typelib::init(value);
    Typelib::load(value, data.data(), data.size());

    return value;
}


PYBIND11_MODULE(pocolog_pybind, m) {
    m.doc() = R"pbdoc(
        Pybind11 plugin for rock tools-pocolog component
        -----------------------
        .. currentmodule:: pocolog_pybind
        .. autosummary::
           __init__
    )pbdoc";

    m.def("convert", &convert);

    py::module_ m_std = m.def_submodule("std", "Typelib namespace");
    py::class_<std::vector<uint8_t>>(m_std, "VectorUInt8T")
        .def(py::init())
    ;

    py::module_ m_base = m.def_submodule("base", "Base types namespace");
    py::class_<base::Time>(m_base, "BaseTime")
        .def("is_null", &base::Time::isNull)
        .def("to_time_values", &base::Time::toTimeValues)
        .def("to_string", &base::Time::toString)
        .def("get_timezone_offset", &base::Time::getTimezoneOffset)
        .def("tz_info_to_seconds", &base::Time::tzInfoToSeconds)
        .def("to_seconds", &base::Time::toSeconds)
        .def("to_milliseconds", &base::Time::toMilliseconds)
        .def("to_microseconds", &base::Time::toMicroseconds)
    ;

    py::module_ m_typelib = m.def_submodule("typelib", "Typelib namespace");

    m_typelib.def("init", py::overload_cast<Typelib::Value>(&Typelib::init)); // requires min. C++14
    m_typelib.def("init", py::overload_cast<Typelib::Value, Typelib::MemoryLayout const&>(&Typelib::init)); // requires min. C++14
    m_typelib.def("init", py::overload_cast<uint8_t*, Typelib::MemoryLayout const&>(&Typelib::init)); // requires min. C++14
    m_typelib.def("zero", py::overload_cast<Typelib::Value>(&Typelib::zero)); // requires min. C++14
    m_typelib.def("zero", py::overload_cast<Typelib::Value, Typelib::MemoryLayout const&>(&Typelib::zero)); // requires min. C++14
    m_typelib.def("zero", py::overload_cast<uint8_t*, Typelib::MemoryLayout const&>(&Typelib::zero)); // requires min. C++14
    m_typelib.def("destroy", py::overload_cast<Typelib::Value>(&Typelib::destroy)); // requires min. C++14
    m_typelib.def("destroy", py::overload_cast<Typelib::Value, Typelib::MemoryLayout const&>(&Typelib::destroy)); // requires min. C++14
    m_typelib.def("destroy", py::overload_cast<uint8_t*, Typelib::MemoryLayout const&>(&Typelib::destroy)); // requires min. C++14
    m_typelib.def("copy", py::overload_cast<Typelib::Value, Typelib::Value>(&Typelib::copy)); // requires min. C++14
    m_typelib.def("copy", py::overload_cast<void*, void*, Typelib::Type const&>(&Typelib::copy)); // requires min. C++14
    m_typelib.def("copy", py::overload_cast<void*, void*, Typelib::MemoryLayout const&>(&Typelib::copy)); // requires min. C++14
    m_typelib.def("compare", py::overload_cast<Typelib::Value, Typelib::Value>(&Typelib::compare)); // requires min. C++14
    m_typelib.def("compare", py::overload_cast<void*, void*, Typelib::Type const&>(&Typelib::compare)); // requires min. C++14
    m_typelib.def("load", py::overload_cast<Typelib::Value, std::vector<uint8_t> const&>(&Typelib::load)); // requires min. C++14
    m_typelib.def("load", py::overload_cast<Typelib::Value, std::vector<uint8_t> const&, Typelib::MemoryLayout const&>(&Typelib::load)); // requires min. C++14
    m_typelib.def("load", py::overload_cast<uint8_t*, Typelib::Type const&, std::vector<uint8_t> const&, Typelib::MemoryLayout const&>(&Typelib::load)); // requires min. C++14
    m_typelib.def("load", py::overload_cast<Typelib::Value, uint8_t const*, unsigned int>(&Typelib::load)); // requires min. C++14
    m_typelib.def("load", py::overload_cast<Typelib::Value, uint8_t const*, unsigned int, Typelib::MemoryLayout const&>(&Typelib::load)); // requires min. C++14
    m_typelib.def("load", py::overload_cast<uint8_t*, Typelib::Type const&, uint8_t const*, unsigned int, Typelib::MemoryLayout const&>(&Typelib::load)); // requires min. C++14
    m_typelib.def("value_get_field", py::overload_cast<Typelib::Value, std::string const&>(&Typelib::value_get_field), py::keep_alive<0, 1>()); // requires min. C++14
    m_typelib.def("value_get_field", py::overload_cast<void*, Typelib::Type const&, std::string const&>(&Typelib::value_get_field)); // requires min. C++14
    m_typelib.def("type_display", [](Typelib::Type const& type, std::string const& indent = "") {
        std::cout << Typelib::type_display(type, indent);
    });

    py::enum_<Typelib::Type::Category>(m_typelib, "Category", py::arithmetic())
        .value("NULL_TYPE", Typelib::Type::Category::NullType)
        .value("ARRAY", Typelib::Type::Category::Array)
        .value("POINTER", Typelib::Type::Category::Pointer)
        .value("NUMERIC", Typelib::Type::Category::Numeric)
        .value("ENUM", Typelib::Type::Category::Enum)
        .value("COMPOUND", Typelib::Type::Category::Compound)
        .value("OPAQUE", Typelib::Type::Category::Opaque)
        .value("CONTAINER", Typelib::Type::Category::Container)
        .value("NUMBER_OF_VALID_CATEGORIES", Typelib::Type::Category::NumberOfValidCategories)
    ;

    py::class_<Typelib::MetaData>(m_typelib, "MetaData")
        // .def("get", py::overload_cast(&Typelib::MetaData::get))
        // .def("get", py::overload_cast<std::string const&>(&Typelib::MetaData::get))
        .def("include", &Typelib::MetaData::include, "key"_a)
    ;

    py::class_<Typelib::Type>(m_typelib, "Type")
        .def("get_name", &Typelib::Type::getName)
        .def("get_base_ame", &Typelib::Type::getBasename)
        .def("get_namespace", &Typelib::Type::getNamespace)
        .def("get_size", &Typelib::Type::getSize)
        .def("get_category", &Typelib::Type::getCategory)
        .def("is_null", &Typelib::Type::isNull)
        // .def("get_meta_data", py::overload_cast<std::string const&>(&Typelib::Type::getMetaData))
    ;

    py::class_<Typelib::Field>(m_typelib, "Field")
        .def(py::init<const std::string&, Typelib::Type const&>())
        .def(py::init<Typelib::Field const&>())
        .def("get_name", &Typelib::Field::getName)
        .def("get_type", &Typelib::Field::getType)
        .def("get_offset", &Typelib::Field::getOffset)
        //.def("get_meta_data", py::overload_cast<>(&Typelib::Field::getMetaData))
        //.def("get_meta_data", py::overload_cast<std::string const&>(&Typelib::Field::getMetaData))
    ;

    py::class_<Typelib::Numeric, Typelib::Type>(m_typelib, "Numeric")
    ;

    py::class_<Typelib::Compound, Typelib::Type>(m_typelib, "Compound")
        .def(py::init<std::string const&>())
        .def("get_fields", &Typelib::Compound::getFields)
    ;

    py::class_<Typelib::Value>(m_typelib, "Value")
        .def(py::init())
        .def(py::init<void*, Typelib::Type const&>())
        .def("get_data", &Typelib::Value::getData)
        .def("get_type", &Typelib::Value::getType)
        .def("__getitem__", [](const Typelib::Value &s, std::string const& name){
            return Typelib::value_get_field(s, name);
        })
        .def("cast", [](const Typelib::Value &s){
            py::object obj;

            Typelib::Type const & type = s.getType();
            void* ptr_data = s.getData();
            switch (type.getCategory()) {
                case Typelib::Type::Category::Numeric:
                    std::cout << "basename " << type.getBasename() << std::endl;
                    if (type.getBasename() == "int8_t") {
                        obj = py::cast(static_cast<int8_t*>(ptr_data));
                    } else if (type.getBasename() == "uint8_t") {
                        obj = py::cast(static_cast<uint8_t*>(ptr_data));
                    } else if (type.getBasename() == "int16_t") {
                        obj = py::cast(static_cast<int16_t*>(ptr_data));
                    } else if (type.getBasename() == "uint16_t") {
                        obj = py::cast(static_cast<uint16_t*>(ptr_data));
                    } else if (type.getBasename() == "int32_t") {
                        obj = py::cast(static_cast<int32_t*>(ptr_data));
                    } else if (type.getBasename() == "uint32_t") {
                        obj = py::cast(static_cast<uint32_t*>(ptr_data));
                    } else if (type.getBasename() == "int64_t") {
                        obj = py::cast(static_cast<int64_t*>(ptr_data));
                    } else if (type.getBasename() == "uint64_t") {
                        obj = py::cast(static_cast<uint64_t*>(ptr_data));
                    } else if (type.getBasename() == "ssize_t") {
                        obj = py::cast(static_cast<ssize_t*>(ptr_data));
                    } else if (type.getBasename() == "float") {
                        obj = py::cast(static_cast<float*>(ptr_data));
                    } else if (type.getBasename() == "double") {
                        obj = py::cast(static_cast<double*>(ptr_data));
                    } else if (type.getBasename() == "uint8_t") {
                        obj = py::cast(static_cast<uint8_t*>(ptr_data));
                    } else if (type.getBasename() == "uint8_t") {
                        obj = py::cast(static_cast<uint8_t*>(ptr_data));
                    } else {
                        std::cout << "Encountered type " << type.getBasename() << std::endl;
                        throw std::runtime_error("This type is not implemented");
                    }
                            
                    break;

                default:
                    std::cout << "Encountered type " << type.getCategory() << std::endl;
                    throw std::runtime_error("This category is not implemented");
            }

            return obj;
        })
        .def("__len__", [](const Typelib::Value &s){
            Typelib::Type const & type = s.getType();
            if (type.getCategory() == Typelib::Type::Category::Compound){
                Typelib::Compound const* compound = dynamic_cast<Typelib::Compound const*>(&type);
                return compound->getFields().size();
            } else if(type.getCategory() == Typelib::Type::Category::Array) {
                Typelib::Array const* array = dynamic_cast<Typelib::Array const*>(&type);
                return array->getDimension();
            } else {
                throw std::runtime_error("the __len__ method is only available for Compound and Array categories.");
            }
        })
    ;

    py::module_ m_pocolog = m.def_submodule("pocolog", "Pocolog namespace");

    m_pocolog.def("get_sample", &get_sample);

    py::class_<pocolog_cpp::Stream>(m_pocolog, "Stream")
        .def("get_name", &pocolog_cpp::Stream::getName)
        .def("get_type_name", &pocolog_cpp::Stream::getTypeName)
        .def("get_stream_type", &pocolog_cpp::Stream::getStreamType)
        .def("get_first_sample_time", &pocolog_cpp::Stream::getFistSampleTime)
        .def("get_last_sample_time", &pocolog_cpp::Stream::getLastSampleTime)
        .def("get_index", &pocolog_cpp::Stream::getIndex)
        .def("get_size", &pocolog_cpp::Stream::getSize)
        .def("get_sample_data", &pocolog_cpp::Stream::getSampleData)
    ;

    py::class_<pocolog_cpp::InputDataStream, pocolog_cpp::Stream>(m_pocolog, "InputDataStream")
        .def("get_type", &pocolog_cpp::InputDataStream::getType)
        .def("get_typelib_value", &pocolog_cpp::InputDataStream::getTyplibValue)
        .def("get_cxx_type", &pocolog_cpp::InputDataStream::getCXXType)
        .def("get_type_memory_size", &pocolog_cpp::InputDataStream::getTypeMemorySize)
        // .def("get_sample", &pocolog_cpp::InputDataStream::getSample <Typelib:Value>)
    ;

    py::class_<pocolog_cpp::MultiFileIndex>(m_pocolog, "MultiFileIndex")
        .def(py::init<const std::vector<std::string>, bool>(), "fileNames"_a, "verbose"_a=true)
        .def(py::init<bool>(), "verbose"_a=true)
        .def("get_all_streams", &pocolog_cpp::MultiFileIndex::getAllStreams)
        .def("get_size", &pocolog_cpp::MultiFileIndex::getSize)
        // .def("get_global_stream_idx", py::overload_cast<pocolog_cpp::Stream * &>(&pocolog_cpp::MultiFileIndex::getGlobalStreamIdx)); // requires min. C++14
        .def("get_pos_in_stream", &pocolog_cpp::MultiFileIndex::getPosInStream)
        .def("get_sample_stream", &pocolog_cpp::MultiFileIndex::getSampleStream)
        .def("create_index", py::overload_cast<const std::vector<std::string> &>(&pocolog_cpp::MultiFileIndex::createIndex)) // requires min. C++14
    ;

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}