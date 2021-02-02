#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pocolog_cpp/MultiFileIndex.hpp>
#include <pocolog_cpp/Stream.hpp>
#include <pocolog_cpp/InputDataStream.hpp>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;

class PocologIterator {
protected:
    int idx = 0;

    std::vector<std::string> logfiles;
    pocolog_cpp::MultiFileIndex multiIndex;
public:
    PocologIterator (const std::vector<std::string> logfiles_) {
        logfiles = logfiles_;
        pocolog_cpp::MultiFileIndex multiIndex = pocolog_cpp::MultiFileIndex();
        multiIndex.createIndex(logfiles);
        std::vector<pocolog_cpp::Stream*> streams = multiIndex.getAllStreams();
        // std::vector<pocolog_cpp::InputDataStream*> dataStreams;
        // addValidInputDataStreams(streams, dataStreams, only);
        std::cout << "[pocolog_pybind] " << streams.size() << " streams" << std::endl;
    }
    int getIndex() {
        return idx;
    }
    void __next__() {
        /* we iterate to next */
    }
};


PYBIND11_MODULE(pocolog_pybind, m) {
    m.doc() = R"pbdoc(
        Pybind11 plugin for rock tools-pocolog component
        -----------------------
        .. currentmodule:: pocolog_pybind
        .. autosummary::
           __init__
    )pbdoc";

    
    py::class_<PocologIterator>(m, "PocologIterator")
        .def(py::init<const std::vector<std::string>>())
        .def("get_index", &PocologIterator::getIndex)
        /*.def("__len__", [](const std::vector<int> &v) { return v.size(); })
        .def("__iter__", [](std::vector<int> &v) {
           return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>()) // Keep vector alive while iterator is used*/
    ;

    py::class_<std::vector<int>>(m, "IntVector")
        .def(py::init<>())
        .def("clear", &std::vector<int>::clear)
        .def("pop_back", &std::vector<int>::pop_back)
        .def("__len__", [](const std::vector<int> &v) { return v.size(); })
        .def("__iter__", [](std::vector<int> &v) {
           return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>()) /* Keep vector alive while iterator is used */
    ;

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}