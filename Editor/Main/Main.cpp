#include <Core/Console.h>

int main()
{
    Console console;

    console.Initialize();
    console.AddOutput<ConsoleTerminalOutput>();

    INFO("This is a test, Some random number: {}", 42);
    WARN("This is a test, Some random number: {}", 42);
    ERROR("This is a test, Some random number: {}", 42);

    console.Destroy();
}
