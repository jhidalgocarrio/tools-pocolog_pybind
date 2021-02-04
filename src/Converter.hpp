#pragma once
#include <msgpack.h>
#include <msgpack/fbuffer.h>
#include <typelib/typemodel.hh>
#include <typelib/typevisitor.hh>
#include <typelib/value.hh>


/**
 * Actual converter logic for one sample.
 *
 * The converter loads a single type from its serialized typelib representation
 * and stores it in Python-compatible formats. The typelib type information must be
 * provided.
 */
class Converter : public Typelib::ValueVisitor
{
    /** Typelib's type description. */
    Typelib::Type const& type;
    /** Current field name of a compound type. */
    std::list<std::string> fieldName;
    /** All data will be put in this packer. */
    msgpack_packer& pk;
    /** Verbosity level. */
    int verbose;
    /** debug output. */
    bool debug;
    /** Current depth in the type hierarchy. */
    int depth;
    /** A constant that will be used to format debug output on stdout. */
    const int indentation;

    bool modeNumericToString = false;

    std::string numericToStringBuffer = "";

public:
    Converter(std::string const& basename, Typelib::Type const& type, msgpack_packer& pk, int verbose);
    virtual ~Converter();
    void convertSample(std::vector<uint8_t>& data);
protected:
    bool visit_ (int8_t  & v);
    bool visit_ (uint8_t & v);
    bool visit_ (int16_t & v);
    bool visit_ (uint16_t& v);
    bool visit_ (int32_t & v);
    bool visit_ (uint32_t& v);
    bool visit_ (int64_t & v);
    bool visit_ (uint64_t& v);
    bool visit_ (float   & v);
    bool visit_ (double  & v);

    bool visit_(Typelib::Value const& v, Typelib::OpaqueType const& type);
    bool visit_(Typelib::Value const& v, Typelib::Pointer const& type);
    bool visit_(Typelib::Value const& v, Typelib::Array const& type);
    bool visit_(Typelib::Value const& v, Typelib::Container const& type);
    bool visit_(Typelib::Value const& v, Typelib::Compound const& type);
    bool visit_(Typelib::Value const& v, Typelib::Compound const& type, Typelib::Field const& field);
    bool visit_(Typelib::Enum::integral_type&, Typelib::Enum const& e);

    using ValueVisitor::visit_;
private:
    void reset();
    void printBegin();
};