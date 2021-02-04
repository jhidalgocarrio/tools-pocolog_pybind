#include <base/Time.hpp>
#include <pocolog_cpp/Format.hpp>
#include <pocolog_cpp/InputDataStream.hpp>
#include <pocolog_cpp/MultiFileIndex.hpp>
#include <pocolog_cpp/Stream.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <typelib/typemodel.hh>
#include <typelib/value.hh>
#include <typelib/value_ops.hh>

#include "Converter.cpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;
using namespace pybind11::literals;


PYBIND11_MODULE(pocolog_pybind, m) {
    m.doc() = R"pbdoc(
        Pybind11 plugin for rock tools-pocolog component
        -----------------------
        .. currentmodule:: pocolog_pybind
        .. autosummary::
           __init__
    )pbdoc";

    m.def("convert", &convert);

    py::class_<base::Time>(m, "BaseTime")
        .def("is_null", &base::Time::isNull)
        .def("to_time_values", &base::Time::toTimeValues)
        .def("to_string", &base::Time::toString)
        .def("get_timezone_offset", &base::Time::getTimezoneOffset)
        .def("tz_info_to_seconds", &base::Time::tzInfoToSeconds)
        .def("to_seconds", &base::Time::toSeconds)
        .def("to_milliseconds", &base::Time::toMilliseconds)
        .def("to_microseconds", &base::Time::toMicroseconds)
    ;

    py::enum_<Typelib::Type::Category>(m, "TypelibCategory", py::arithmetic())
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

    py::class_<Typelib::Type>(m, "TypelibType")
        .def("get_name", &Typelib::Type::getName)
        .def("get_base_ame", &Typelib::Type::getBasename)
        .def("get_namespace", &Typelib::Type::getNamespace)
        .def("get_size", &Typelib::Type::getSize)
        .def("get_category", &Typelib::Type::getCategory)
        .def("is_null", &Typelib::Type::isNull)
    ;

    py::class_<Typelib::Value>(m, "TypelibValue")
        .def(py::init())
        .def(py::init<void*, Typelib::Type const&>())
        .def("get_data", &Typelib::Value::getData)
        .def("get_type", &Typelib::Value::getType)
    ;

    py::class_<pocolog_cpp::Stream>(m, "PocologStream")
        .def("get_name", &pocolog_cpp::Stream::getName)
        .def("get_type_name", &pocolog_cpp::Stream::getTypeName)
        .def("get_stream_type", &pocolog_cpp::Stream::getStreamType)
        .def("get_first_sample_time", &pocolog_cpp::Stream::getFistSampleTime)
        .def("get_last_sample_time", &pocolog_cpp::Stream::getLastSampleTime)
        .def("get_index", &pocolog_cpp::Stream::getIndex)
        .def("get_size", &pocolog_cpp::Stream::getSize)
        .def("get_sample_data", &pocolog_cpp::Stream::getSampleData)
    ;

    py::class_<pocolog_cpp::InputDataStream, pocolog_cpp::Stream>(m, "PocologInputDataStream")
        .def("get_cxx_type", &pocolog_cpp::InputDataStream::getCXXType)
        .def("get_type_memory_size", &pocolog_cpp::InputDataStream::getTypeMemorySize)
        // .def("get_sample", &pocolog_cpp::InputDataStream::getSample <std::vector<int>>)
    ;

    py::class_<pocolog_cpp::MultiFileIndex>(m, "PocologMultiFileIndex")
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