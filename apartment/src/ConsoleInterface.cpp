//
// Created by valerio on 06/08/17.
//

#include "ConsoleInterface.hpp"

namespace cr = CppReadline;
using ret = cr::Console::ReturnCode;

unsigned ConsoleInterface::info(const std::vector<std::string> &){
    std::cout << "Welcome to the example console. This command does not really\n"
              << "do anything aside from printing this statement. Thus it does\n"
              << "not need to look into the arguments that are passed to it.\n";
    return ret::Ok;
}


// In this command we implement a basic calculator
unsigned ConsoleInterface::calc(const std::vector<std::string> & input) {
    if ( input.size() != 4 ) {
        // The first element of the input array is always the name of the
        // command as registered in the console.
        std::cout << "Usage: " << input[0] << " num1 operator num2\n";
        // We can return an arbitrary error code, which we can catch later
        // as Console will return it.
        return 1;
    }
    double num1 = std::stod(input[1]),
            num2 = std::stod(input[3]);

    char op = input[2][0];

    double result;
    switch ( op ) {
        case '*':
            result = num1 * num2;
            break;
        case '+':
            result = num1 + num2;
            break;
        case '/':
            result = num1 / num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        default:
            std::cout << "The inserted operator is not supported\n";
            // Again, we can return an arbitrary error code to catch it later.
            return 2;
    }
    std::cout << "Result: " << result << '\n';
    return 0;
}


ConsoleInterface::ConsoleInterface(Algorithm* algorithm_ptr):
        single_thread(thread_fun_t([this](const bool& t){this->worker(t);}), "ConsoleManager"),
        c(">"), algorithm_ptr(algorithm_ptr) {

    // Here we register a new command. The string "info" names the command that
    // the user will have to type in in order to trigger this command (it can
    // be different from the function name).

    c.registerCommand("info", [this](const std::vector<std::string> &s){return this->info(s);});
    c.registerCommand("calc", [this](const std::vector<std::string> &s){return this->calc(s);});


}

void ConsoleInterface::worker(const bool &terminating) {
    // Here we call one of the defaults command of the console, "help". It lists
    // all currently registered commands within the console, so that the user
    // can know which commands are available.
    c.executeCommand("help");

    // Otherwise we can modify the code to catch Console error codes
    int retCode;
    do {
        retCode = c.readLine();
        // We can also change the prompt based on last return value:
        if ( retCode == ret::Ok )
            c.setGreeting(">");
        else
            c.setGreeting("!>");

        if ( retCode == 1 ) {
            std::cout << "Received error code 1\n";
        }
        else if ( retCode == 2 ) {
            std::cout << "Received error code 2\n";
        }
    }
    while ( retCode != ret::Quit && !terminating);
}
