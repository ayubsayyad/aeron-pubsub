#pragma once
#include <Aeron.h>

class AeronPublisher {
public:
    AeronPublisher(){}

    void init(const std::string& dirPrefix,
              const std::string& channel,
              const std::int32_t streamId);
    void start();

    std::int64_t channelStatus(){
        return publication_->channelStatus();
    }

    std::int32_t channelStatusId(){
        return publication_->channelStatusId();
    }

    std::int64_t publish(char* message, const std::int32_t length);

private:
    std::string dirPrefix_;
    std::string channel_;
    std::int32_t streamId_;
    aeron::Context context_;
    std::shared_ptr<aeron::Aeron> aeron_;
    std::shared_ptr<aeron::Publication> publication_;

    typedef std::array<std::uint8_t, 4096> buffer_t;
    AERON_DECL_ALIGNED(buffer_t buffer, 16);
    aeron::concurrent::AtomicBuffer srcBuffer;
};

