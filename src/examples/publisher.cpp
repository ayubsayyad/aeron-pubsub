#include <iostream>
#include <Aeron.h>
#include <util/CommandOptionParser.h>
#include <fstream>
#include "aeronpublisher.hpp"

using namespace aeron;

struct Settings
{
    std::string file;
    int repeatCount = 1;
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

    s.file = cp.getOption('f').getParam(0, s.file);
    s.repeatCount = cp.getOption('n').getParamAsInt(0, 0, INT32_MAX, s.repeatCount);
    s.lingerTimeoutMs = cp.getOption('l').getParamAsInt(0, 0, 60 * 60 * 1000, s.lingerTimeoutMs);

    return s;
}


int main(int argc, char** argv)
{
    CommandOptionParser cp;
    cp.addOption(CommandOption ('h',     0, 0, "                Displays help information."));
    cp.addOption(CommandOption ('f',     1, 1, "file            file to read."));
    cp.addOption(CommandOption ('n',     1, 1, "number          repeat count."));
    cp.addOption(CommandOption ('l',     1, 1, "milliseconds    Linger timeout in milliseconds."));
    Settings settings = parseCmdLine(cp, argc, argv);

    std::cout << "file:" << settings.file << std::endl;

    AeronPublisher publisher_channel;
    publisher_channel.init("/dev/shm/aeron-anam/", "aeron:udp?endpoint=localhost:40123", 10);
    publisher_channel.start();

    const std::int64_t channelStatus = publisher_channel.channelStatus();

    std::cout << "Publication channel status (id=" << publisher_channel.channelStatusId() << ") "
              << ((channelStatus == ChannelEndpointStatus::CHANNEL_ENDPOINT_ACTIVE) ?
                  "ACTIVE" : std::to_string(channelStatus))
              << std::endl;

    std::ifstream input_file(settings.file);
    if(!input_file.is_open()){
        std::cout << "unable to open file: " << settings.file << std::endl;
        exit(1);
    }
    std::vector<std::string> lines_to_send;
    std::string line;
    while (std::getline(input_file, line)) {
        lines_to_send.push_back(line);
    }

    for(int i = 0; i < settings.repeatCount; ++i) {
        for (auto &line : lines_to_send) {
            auto ret = publisher_channel.publish((char*)line.c_str(), line.size());
            if(ret < 0){
                std::cout << "Error publishing message, error code: " << ret << std::endl;
            }else{
                std::cout << "Message Published" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::cout << "Completed sending:" << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(settings.lingerTimeoutMs));

    return 0;
}