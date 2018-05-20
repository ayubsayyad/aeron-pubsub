#pragma once
#include <Aeron.h>

class AeronSubscriber {
public:
    AeronSubscriber(){}

    void init(const std::string& dirPrefix,
              const std::string& channel,
              const std::int32_t streamId);
    void start();

    std::int64_t channelStatus(){
        return subscription_->channelStatus();
    }

    std::int32_t channelStatusId(){
        return subscription_->channelStatusId();
    }

    void startReceiving(aeron::fragment_handler_t callback);

private:
    std::string dirPrefix_;
    std::string channel_;
    std::int32_t streamId_;
    aeron::Context context_;
    std::shared_ptr<aeron::Aeron> aeron_;
    std::shared_ptr<aeron::Subscription> subscription_;
    std::atomic<bool> running_;

};

