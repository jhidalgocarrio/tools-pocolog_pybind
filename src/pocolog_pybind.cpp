#include <base/Time.hpp>
#include <base/Eigen.hpp>
#include <base/samples/Event.hpp>
#include <base/samples/Frame.hpp>
#include <base/samples/RigidBodyState.hpp>
#include <orocos/eds/edsTypes.hpp>
#include <cassert>
#include <iostream>
#include <iterator>
#include <regex>
#include <pocolog_cpp/Format.hpp>
#include <pocolog_cpp/InputDataStream.hpp>
#include <pocolog_cpp/MultiFileIndex.hpp>
#include <pocolog_cpp/Stream.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <sstream>
#include <string>
#include <typelib/memory_layout.hh>
#include <typelib/typedisplay.hh>
#include <typelib/typemodel.hh>
#include <typelib/value.hh>
#include <typelib/value_ops.hh>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;
using namespace pybind11::literals;

py::object cast_typelib_value(const Typelib::Value &s, bool recursive = false){
    py::object obj;

    Typelib::Type const & type = s.getType();
    std::string type_name = type.getName();
    std::string basename = type.getBasename();
    void* ptr_data = s.getData();
    switch (type.getCategory()) {
        case Typelib::Type::Category::Numeric: {
            Typelib::Numeric const* type_numeric = dynamic_cast<Typelib::Numeric const*>(&type);

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
            } else if (type.getBasename() == "size_t") {
                obj = py::cast(static_cast<size_t*>(ptr_data));
            } else if (type.getBasename() == "float") {
                obj = py::cast(static_cast<float*>(ptr_data));
            } else if (type.getBasename() == "double") {
                obj = py::cast(static_cast<double*>(ptr_data));
            } else if (type.getBasename() == "bool") {
                obj = py::cast(static_cast<bool*>(ptr_data));
            } else {
                std::cout << "Encountered type " << type.getName() << std::endl;
                throw std::runtime_error("This type is not implemented");
            }
        } break;

        case Typelib::Type::Category::Array: {
            Typelib::Array const* type_array = dynamic_cast<Typelib::Array const*>(&type);

            std::string numeric_type = basename.substr(0, basename.find("["));
            std::regex r("\\[(\\d+)]");
            size_t array_length = 0;
            for(std::sregex_iterator i = std::sregex_iterator(basename.begin(), basename.end(), r);
                i != std::sregex_iterator(); ++i)
            {
                std::smatch match = *i;
                array_length = std::stoi(match[1].str());
            }
            assert((array_length > 0) && "Could not parse length of array");

            if (numeric_type == "int8_t") {
                int8_t* ptr_array = static_cast<int8_t*>(ptr_data);
                std::vector<int8_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "uint8_t") {
                uint8_t* ptr_array = static_cast<uint8_t*>(ptr_data);
                std::vector<uint8_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "int16_t") {
                int16_t* ptr_array = static_cast<int16_t*>(ptr_data);
                std::vector<int16_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "uint16_t") {
                uint16_t* ptr_array = static_cast<uint16_t*>(ptr_data);
                std::vector<uint16_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "int32_t") {
                int32_t* ptr_array = static_cast<int32_t*>(ptr_data);
                std::vector<int32_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "uint32_t") {
                uint32_t* ptr_array = static_cast<uint32_t*>(ptr_data);
                std::vector<uint32_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "int64_t") {
                int64_t* ptr_array = static_cast<int64_t*>(ptr_data);
                std::vector<int64_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "uint64_t") {
                uint64_t* ptr_array = static_cast<uint64_t*>(ptr_data);
                std::vector<uint64_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "ssize_t") {
                ssize_t* ptr_array = static_cast<ssize_t*>(ptr_data);
                std::vector<ssize_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "size_t") {
                size_t* ptr_array = static_cast<size_t*>(ptr_data);
                std::vector<size_t> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "float") {
                float* ptr_array = static_cast<float*>(ptr_data);
                std::vector<float> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "double") {
                double* ptr_array = static_cast<double*>(ptr_data);
                std::vector<double> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else if (numeric_type == "bool") {
                bool* ptr_array = static_cast<bool*>(ptr_data);
                std::vector<bool> vect(ptr_array, ptr_array + array_length);
                obj = py::cast(vect);
            } else {
                std::cout << "Encountered type " << type.getName() <<" has numeric type "<<numeric_type<< std::endl;
                throw std::runtime_error("This type is not implemented");
            }

            break;
        }

        case Typelib::Type::Category::Container: {
            Typelib::Container const* type_container = dynamic_cast<Typelib::Container const*>(&type);

            std::string cpp_element = type_name.substr(0, type_name.find("<"));
            size_t templ_loc = type_name.find("<");
            std::string cpp_template = type_name.substr(templ_loc+2, type_name.length() - 3 - templ_loc);

            if (cpp_element == "/std/string") {
                obj = py::cast(*(static_cast<std::string*>(ptr_data)));
                // throw std::runtime_error("The continer /std/string is not currently supported.");
            } else if (cpp_element == "/std/vector"){
                size_t array_length = type_container->getElementCount(s.getData());
                std::string numeric_type = cpp_template;

                if (numeric_type == "int8_t") {
                    obj = py::cast(*(static_cast<std::vector<int8_t>*>(ptr_data)));
                } else if (numeric_type == "uint8_t") {
                    obj = py::cast(*(static_cast<std::vector<uint8_t>*>(ptr_data)));
                } else if (numeric_type == "int16_t") {
                    obj = py::cast(*(static_cast<std::vector<int16_t>*>(ptr_data)));
                } else if (numeric_type == "uint16_t") {
                    obj = py::cast(*(static_cast<std::vector<uint16_t>*>(ptr_data)));
                } else if (numeric_type == "int32_t") {
                    obj = py::cast(*(static_cast<std::vector<int32_t>*>(ptr_data)));
                } else if (numeric_type == "uint32_t") {
                    obj = py::cast(*(static_cast<std::vector<uint32_t>*>(ptr_data)));
                } else if (numeric_type == "int64_t") {
                    obj = py::cast(*(static_cast<std::vector<int64_t>*>(ptr_data)));
                } else if (numeric_type == "uint64_t") {
                    obj = py::cast(*(static_cast<std::vector<uint64_t>*>(ptr_data)));
                } else if (numeric_type == "ssize_t") {
                    obj = py::cast(*(static_cast<std::vector<ssize_t>*>(ptr_data)));
                } else if (numeric_type == "size_t") {
                    obj = py::cast(*(static_cast<std::vector<size_t>*>(ptr_data)));
                } else if (numeric_type == "float") {
                    obj = py::cast(*(static_cast<std::vector<float>*>(ptr_data)));
                } else if (numeric_type == "double") {
                    obj = py::cast(*(static_cast<std::vector<double>*>(ptr_data)));
                } else if (numeric_type == "base/samples/Event") {
                    obj = py::cast(*(static_cast<std::vector<::base::samples::Event>*>(ptr_data)));
                } else if (numeric_type == "base/samples/RigidBodyState_m") {
                    obj = py::cast(*(static_cast<std::vector<::base::samples::RigidBodyState>*>(ptr_data)));
                } else if (numeric_type == "wrappers/Matrix</double,3,1>") {
                    obj = py::cast(*(static_cast<std::vector<::base::Vector3d>*>(ptr_data)));
                } else if (numeric_type == "wrappers/Matrix</double,4,1>") {
                    obj = py::cast(*(static_cast<std::vector<::base::Vector4d>*>(ptr_data)));
                } else if (numeric_type == "base/samples/frame/frame_attrib_t") {
                    obj = py::cast(*(static_cast<std::vector<::base::samples::frame::frame_attrib_t>*>(ptr_data)));
                } else {
                    std::cout << "Encountered type " << type.getName()<<" has numeric type "<<numeric_type<< std::endl;
                    throw std::runtime_error("This type is not implemented");
                }

            } else {
                std::cout << "Encountered type " << type.getName() << std::endl;
                throw std::runtime_error("This type is not implemented");
            }

            break;
        }

        case Typelib::Type::Category::Compound: {
            py::dict py_dict;

            Typelib::Compound const* type_compound = dynamic_cast<Typelib::Compound const*>(&type);

            typedef Typelib::Compound::FieldList Fields;
            Fields const& fields(type_compound->getFields());
            Fields::const_iterator const end = fields.end();

            for (Fields::const_iterator it = fields.begin(); it != end; ++it)
            {
                Typelib::Field field = *it;

                Typelib::Value field_value = Typelib::value_get_field(s, field.getName());

                if (recursive) {
                    py_dict[py::cast(field.getName())] = cast_typelib_value(field_value, recursive);
                } else {
                    py_dict[py::cast(field.getName())] = field_value;
                }
            }

            obj = py_dict;

            break;
        }
        case Typelib::Type::Category::Enum: {
            Typelib::Enum const* type_enum = dynamic_cast<Typelib::Enum const*>(&type);

            if (type.getBasename() == "frame_mode_t") {
                obj = py::cast(static_cast<base::samples::frame::frame_mode_t*>(ptr_data));
            } else if (type.getBasename() == "frame_status_t") {
                obj = py::cast(static_cast<base::samples::frame::frame_status_t*>(ptr_data));
             } else {
                std::cout << "Encountered type " << type.getName() << std::endl;
                throw std::runtime_error("This type is not implemented");
            }
        } break;

        default:
            std::cout << "Encountered type " << type.getCategory()<<" type "<< type << std::endl;
            throw std::runtime_error("This category is not implemented");
    }

    return obj;
}


PYBIND11_MODULE(pocolog_pybind, m) {
    m.doc() = R"pbdoc(
        Pybind11 plugin for rock tools-pocolog component
        -----------------------
        .. currentmodule:: pocolog_pybind
        .. autosummary::
           __init__
    )pbdoc";

    py::module_ m_std = m.def_submodule("std", "Typelib namespace");
    py::class_<std::vector<uint8_t>>(m_std, "VectorUInt8T")
        .def(py::init())
    ;

    py::module_ m_base = m.def_submodule("base", "Base types namespace");
    py::class_<base::samples::Event>(m_base, "Event")
        .def(py::init<>())
        .def(py::init<uint16_t, uint16_t, base::Time, uint8_t>())
        .def_readwrite("x", &base::samples::Event::x)
        .def_readwrite("y", &base::samples::Event::y)
        .def_readwrite("ts", &base::samples::Event::ts)
        .def_readwrite("p", &base::samples::Event::polarity)
    ;
    py::class_<base::Time>(m_base, "Time")
        .def(py::init<>())
        .def_readwrite("microseconds", &base::Time::microseconds)
        .def("from_seconds", py::overload_cast<const int64_t> (&base::Time::fromSeconds), "Constructor time in seconds")
        .def("from_seconds", py::overload_cast<const int> (&base::Time::fromSeconds), "Constructor time in seconds")
        .def("from_milliseconds", py::overload_cast<const int64_t>(&base::Time::fromMilliseconds), "Constructor time in milliseconds")
        .def("from_microseconds", py::overload_cast<const int64_t>(&base::Time::fromMicroseconds), "Constructor time in microseconds")
        .def("is_null", &base::Time::isNull)
        .def("to_time_values", &base::Time::toTimeValues)
        .def("to_string", &base::Time::toString)
        .def("get_timezone_offset", &base::Time::getTimezoneOffset)
        .def("tz_info_to_seconds", &base::Time::tzInfoToSeconds)
        .def("to_seconds", &base::Time::toSeconds)
        .def("to_milliseconds", &base::Time::toMilliseconds)
        .def("to_microseconds", &base::Time::toMicroseconds)
    ;
    py::class_<base::samples::RigidBodyState>(m_base, "RigidBodyState")
        .def(py::init<>())
        .def_readwrite("time", &base::samples::RigidBodyState::time)
        .def_readwrite("source_frame", &base::samples::RigidBodyState::sourceFrame)
        .def_readwrite("target_frame", &base::samples::RigidBodyState::targetFrame)
        .def_readwrite("position", &base::samples::RigidBodyState::position)
        .def_readwrite("cov_position", &base::samples::RigidBodyState::cov_position)
        .def_readwrite("orientation", &base::samples::RigidBodyState::orientation)
        .def_readwrite("cov_orientation", &base::samples::RigidBodyState::cov_orientation)
        .def_readwrite("velocity", &base::samples::RigidBodyState::velocity)
        .def_readwrite("cov_velocity", &base::samples::RigidBodyState::cov_velocity)
        .def_readwrite("angular_velocity", &base::samples::RigidBodyState::angular_velocity)
        .def_readwrite("cov_angular_velocity", &base::samples::RigidBodyState::cov_angular_velocity)
    ;
    py::enum_<base::samples::frame::frame_mode_t>(m_base, "FrameMode")
        .value("MODE_UNDEFINED", base::samples::frame::frame_mode_t::MODE_UNDEFINED)
        .value("MODE_GRAYSCALE", base::samples::frame::frame_mode_t::MODE_GRAYSCALE)
        .value("MODE_RGB", base::samples::frame::frame_mode_t::MODE_RGB)
        .value("MODE_UYVY", base::samples::frame::frame_mode_t::MODE_UYVY)
        .value("MODE_BGR", base::samples::frame::frame_mode_t::MODE_BGR)
        .value("MODE_RGB32", base::samples::frame::frame_mode_t::MODE_RGB32)
        .value("RAW_MODES", base::samples::frame::frame_mode_t::RAW_MODES)
        .value("MODE_BAYER", base::samples::frame::frame_mode_t::MODE_BAYER)
        .value("MODE_BAYER_RGGB", base::samples::frame::frame_mode_t::MODE_BAYER_RGGB)
        .value("MODE_BAYER_GRBG", base::samples::frame::frame_mode_t::MODE_BAYER_GRBG)
        .value("MODE_BAYER_BGGR", base::samples::frame::frame_mode_t::MODE_BAYER_BGGR)
        .value("MODE_BAYER_GBRG", base::samples::frame::frame_mode_t::MODE_BAYER_GBRG)
        .value("COMPRESSED_MODES", base::samples::frame::frame_mode_t::COMPRESSED_MODES)
        .value("MODE_PJPG", base::samples::frame::frame_mode_t::MODE_PJPG)
        .value("MODE_JPEG", base::samples::frame::frame_mode_t::MODE_JPEG)
        .value("MODE_PNG", base::samples::frame::frame_mode_t::MODE_PNG)
    ;
    py::enum_<base::samples::frame::frame_status_t>(m_base, "FrameStatus")
        .value("STATUS_EMPTY", base::samples::frame::frame_status_t::STATUS_EMPTY)
        .value("STATUS_VALID", base::samples::frame::frame_status_t::STATUS_VALID)
        .value("STATUS_INVALID", base::samples::frame::frame_status_t::STATUS_INVALID)
    ;
    py::class_<base::Quaterniond>(m_base, "Quaterniond")
        .def(py::init<>())
        .def("x", (double &(Eigen::Quaternion<double,  Eigen::DontAlign>::*)()) (&Eigen::Quaternion<double,  Eigen::DontAlign>::x))
        .def("y", (double &(Eigen::Quaternion<double,  Eigen::DontAlign>::*)()) (&Eigen::Quaternion<double,  Eigen::DontAlign>::y))
        .def("z", (double &(Eigen::Quaternion<double,  Eigen::DontAlign>::*)()) (&Eigen::Quaternion<double,  Eigen::DontAlign>::z))
        .def("w", (double &(Eigen::Quaternion<double,  Eigen::DontAlign>::*)()) (&Eigen::Quaternion<double,  Eigen::DontAlign>::w))
        .def("__repr__",[](const base::Quaterniond &q) {
            return "["+std::to_string(q.x())+" "+ std::to_string(q.y())+" "+ std::to_string(q.z())+" "+ std::to_string(q.w())+"]";
        })
    ;

    py::module_ m_eds = m.def_submodule("eds", "EDS types namespace");
    py::class_<eds::VectorKFs>(m_eds, "VectorKFs")
        .def(py::init<>())
        .def_readwrite("time", &eds::VectorKFs::time)
        .def_readwrite("kfs", &eds::VectorKFs::kfs)
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
        .def("display", [](const Typelib::Type &s, std::string const& indent = ""){
            std::cout << Typelib::type_display(s, indent);
        }, "indent"_a = "")
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
        .def("get_type", &Typelib::Value::getType, py::return_value_policy::reference_internal)
        .def("__getitem__", [](const Typelib::Value &s, std::string const& name){
            return Typelib::value_get_field(s, name);
        })
        .def("cast", &cast_typelib_value, "recursive"_a=false)
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
        .def("destroy", [](const Typelib::Value &s){
            Typelib::destroy(s);
        })
    ;

    py::module_ m_pocolog = m.def_submodule("pocolog", "Pocolog namespace");

    py::class_<pocolog_cpp::Stream>(m_pocolog, "Stream")
        .def("get_name", &pocolog_cpp::Stream::getName)
        .def("get_type_name", &pocolog_cpp::Stream::getTypeName)
        .def("get_stream_type", &pocolog_cpp::Stream::getStreamType)
        .def("get_first_sample_time", &pocolog_cpp::Stream::getFistSampleTime)
        .def("get_last_sample_time", &pocolog_cpp::Stream::getLastSampleTime)
        .def("get_index", &pocolog_cpp::Stream::getIndex)
        .def("get_size", &pocolog_cpp::Stream::getSize)
        .def("get_sample_data", &pocolog_cpp::Stream::getSampleData)
        .def("get_sample", [](pocolog_cpp::InputDataStream &stream, size_t sampleNr) {
            std::vector<uint8_t> data;
            if(!stream.getSampleData(data, sampleNr))
                throw std::runtime_error("Error, sample for stream " + stream.getName() + " could not be loaded");

            const Typelib::Type* m_type = stream.getType();

            std::vector<uint8_t>* value_buffer = new std::vector<uint8_t>();
            value_buffer->resize(m_type->getSize());
            auto value = Typelib::Value(value_buffer->data(), *m_type);

            //init memory area
            Typelib::init(value);
            Typelib::load(value, data.data(), data.size());

            return value;
        })
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
        .def("get_all_streams", [](const pocolog_cpp::MultiFileIndex &s){
            std::vector<pocolog_cpp::Stream *> streams = s.getAllStreams();
            py::dict py_dict;

            for(std::vector<pocolog_cpp::Stream *>::iterator it = streams.begin(); it != streams.end(); ++it) {
                pocolog_cpp::Stream* stream = *it;
                py_dict[py::cast(stream->getName())] = stream;
            }
            return py_dict;
        })
        .def("get_size", &pocolog_cpp::MultiFileIndex::getSize)
        // .def("get_global_stream_idx", py::overload_cast<pocolog_cpp::Stream * &>(&pocolog_cpp::MultiFileIndex::getGlobalStreamIdx)); // requires min. C++14
        .def("get_pos_in_stream", &pocolog_cpp::MultiFileIndex::getPosInStream)
        .def("get_sample_stream", &pocolog_cpp::MultiFileIndex::getSampleStream)
        .def("create_index", py::overload_cast<const std::vector<std::string> &>(&pocolog_cpp::MultiFileIndex::createIndex)) // requires min. C++14
        .def("__len__", [](const pocolog_cpp::MultiFileIndex &s){
            return s.getSize();
        })
    ;

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
