#ifndef BINARY_FILE_PARSER
#define BINARY_FILE_PARSER

#include "util/bufferedInputStream.hpp"
#include "util/arrayList.hpp"
#include "object/hiObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "code/codeObject.hpp"

class BinaryFileParser {
private:
    BufferedInputStream* file_stream;
    ArrayList<HiString*> _string_table;

public:
    BinaryFileParser(BufferedInputStream* stream);

    CodeObject* parse();
    CodeObject* get_code_object();
    HiString* get_byte_codes();
    HiString* get_string();
    ArrayList<HiObject*>* get_consts();
    ArrayList<HiObject*>* get_names();
    ArrayList<HiObject*>* get_var_names();
    ArrayList<HiObject*>* get_free_vars();
    ArrayList<HiObject*>* get_cell_vars();
    ArrayList<HiObject*>* get_tuple();

    HiString* get_file_name();
    HiString* get_name();
    HiString* get_no_table();
};

#endif