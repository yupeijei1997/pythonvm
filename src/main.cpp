#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"

int main(int argc, char** argv) {
    // BufferedInputStream stream("test/hello.pyc");
    BufferedInputStream stream(argv[1]);
    BinaryFileParser parser(&stream);
    CodeObject* main_code = parser.parse();
    Universe::genesis();

    Interpreter::get_instance()->run(main_code);

    printf("run finished\n");
    return 0;
}
