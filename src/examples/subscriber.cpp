#include <iostream>
#include <Aeron.h>
#include <util/CommandOptionParser.h>
#include <fstream>
#include "aeronsubscriber.hpp"

using namespace aeron;

struct Settings
{
    int lingerTimeoutMs = 1000;
};


Settings parseCmdLine(CommandOptionParser& cp, int argc, char** argv)
{
    cp.parse(argc, argv);
    if (cp.getOption('h').isPresent())
    {
        cp.displayOptionsHelp(std::cout);
        exit(0);
    }

    Settings s;

    s.lingerTimeoutMs = cp.getOption('l').getParamAsInt(0, 0, 60 * 60 * 1000, s.lingerTimeoutMs);

    return s;
}

fragment_handler_t callback()
{
    return [&](const AtomicBuffer& buffer, util::index_t offset, util::index_t length, const Header& header)
    {
        std::cout << "Message to stream " << header.streamId() << " from session " << header.sessionId();
        std::cout << "(" << length << "@" << offset << ") <<";
        std::cout << std::string(reinterpret_cast<const char *>(buffer.buffer()) + offset, static_cast<std::size_t>(length)) << ">>" << std::endl;
    };
}


int main(int argc, char** argv)
{
    CommandOptionParser cp;
    cp.addOption(CommandOption ('h',     0, 0, "                Displays help information."));
    cp.addOption(CommandOption ('l',     1, 1, "milliseconds    Linger timeout in milliseconds."));
    Settings settings = parseCmdLine(cp, argc, argv);

    AeronSubscriber subscriber_channel;
    subscriber_channel.init("/dev/shm/aeron-anam/", "aeron:udp?endpoint=localhost:40123", 10);
    subscriber_channel.start();


    subscriber_channel.startReceiving(callback());

    std::cout << "Completed receiving:" << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(settings.lingerTimeoutMs));

    return 0;
}