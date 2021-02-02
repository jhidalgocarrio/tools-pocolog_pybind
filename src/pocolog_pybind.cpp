#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pocolog_cpp/InputDataStream.hpp>
#include <pocolog_cpp/MultiFileIndex.hpp>
#include <pocolog_cpp/Stream.hpp>
#include <pocolog_cpp/Format.hpp>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;


void addValidInputDataStreams(
    const std::vector<pocolog_cpp::Stream*>& streams,
    std::vector<pocolog_cpp::InputDataStream*>& dataStreams)
{
    dataStreams.reserve(streams.size());
    for(size_t i = 0; i < streams.size(); i++)
    {
        pocolog_cpp::InputDataStream* dataStream =
            dynamic_cast<pocolog_cpp::InputDataStream*>(streams[i]);
        if(!dataStream)
        {
            std::cerr << "[pocolog_pybind] Stream #" << i
                      << " is not an InputDataStream, will be ignored!" << std::endl;
            continue;
        }
        dataStreams.push_back(dataStream);
    }
}

class PocologIterator {
protected:
    int idx = 0;

    std::vector<std::string> logfiles;
    pocolog_cpp::MultiFileIndex multiIndex;
    std::vector<pocolog_cpp::InputDataStream*> dataStreams;
public:
    PocologIterator (const std::vector<std::string> logfiles_) {
        logfiles = logfiles_;

        pocolog_cpp::MultiFileIndex multiIndex = pocolog_cpp::MultiFileIndex();
        multiIndex.createIndex(logfiles);
        std::vector<pocolog_cpp::Stream*> streams = multiIndex.getAllStreams();
        addValidInputDataStreams(streams, dataStreams);
        std::cout << "[pocolog_pybind] " << dataStreams.size() << " streams" << std::endl;
    }
    pocolog_cpp::MultiFileIndex getMultiFileIndex(){
        return multiIndex;
    }
    int getIndex() {
        return idx;
    }
    void __next__() {
        /* we iterate to next */
    }
    std::vector<pocolog_cpp::InputDataStream*> getInputDataStreams() {
        return dataStreams;
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

    py::class_<pocolog_cpp::Stream>(m, "Stream")
        .def("get_name", &pocolog_cpp::Stream::getName)
        .def("get_type_name", &pocolog_cpp::Stream::getTypeName)
        .def("get_stream_type", &pocolog_cpp::Stream::getStreamType)
        .def("get_first_sample_time", &pocolog_cpp::Stream::getFistSampleTime)
        .def("get_last_sample_time", &pocolog_cpp::Stream::getLastSampleTime)
        .def("get_index", &pocolog_cpp::Stream::getIndex)
        .def("get_size", &pocolog_cpp::Stream::getSize)
        .def("get_sample_data", &pocolog_cpp::Stream::getSampleData)
    ;

    py::class_<pocolog_cpp::InputDataStream>(m, "InputDataStream")
        .def("get_cxx_type", &pocolog_cpp::InputDataStream::getCXXType)
        .def("get_type_memory_size", &pocolog_cpp::InputDataStream::getTypeMemorySize)
        .def("get_sample_data", &pocolog_cpp::InputDataStream::getSampleData)
    ;

    py::class_<pocolog_cpp::MultiFileIndex>(m, "MultiFileIndex")
        .def("get_size", &pocolog_cpp::MultiFileIndex::getSize)
        // .def("get_global_stream_idx", py::overload_cast<pocolog_cpp::Stream * &>(&pocolog_cpp::MultiFileIndex::getGlobalStreamIdx)); // requires min. C++14
        .def("get_pos_in_stream", &pocolog_cpp::MultiFileIndex::getPosInStream)
        .def("get_sample_stream", &pocolog_cpp::MultiFileIndex::getSampleStream)
        .def("create_index", py::overload_cast<const std::vector<std::string> &>(&pocolog_cpp::MultiFileIndex::createIndex)) // requires min. C++14
    ;

    
    py::class_<PocologIterator>(m, "PocologIterator")
        .def(py::init<const std::vector<std::string>>())
        .def("get_index", &PocologIterator::getIndex)
        .def("get_multi_file_index", &PocologIterator::getMultiFileIndex)
        .def("get_input_data_streams", &PocologIterator::getInputDataStreams)
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