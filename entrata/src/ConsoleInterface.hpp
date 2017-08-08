//
// Created by valerio on 06/08/17.
//

#ifndef __ENTRY_CONSOLEINTERFACE_HPP__
#define __ENTRY_CONSOLEINTERFACE_HPP__

#include "../../common/cpp_readline/Console.hpp"
#include "../../common/single_thread.hpp"
#include "Algorithm.hpp"

class ConsoleInterface: public single_thread {


private:
public:
    /**
     * Initializer of the console interface
     * @param algorithm_ptr is a pointer to the current working alghorithm, that must keeped alive
     * @return
     */
    ConsoleInterface(Algorithm* algorithm_ptr);


private:
    unsigned info(const std::vector<std::string> &);
    unsigned calc(const std::vector<std::string> & input);

    void worker(const bool& terminating);

    // We create a console. The '>' character is used as the prompt.
    // Note that multiple Consoles can exist at once, as they automatically
    // manage the underlying global readline state.
    CppReadline::Console c;
    Algorithm* algorithm_ptr;
};


#endif /* __ENTRY_CONSOLEINTERFACE_HPP__ */
