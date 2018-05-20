#include "aeronsubscriber.hpp"

using namespace aeron;

void AeronSubscriber::init(const std::string& dirPrefix,
                          const std::string& channel,
                          const std::int32_t streamId){

    dirPrefix_ = dirPrefix;
    channel_ = channel;
    streamId_ = streamId;
}
void AeronSubscriber::start(){

    context_.aeronDir(dirPrefix_);


    context_.newSubscriptionHandler(
            [](const std::string& channel, std::int32_t streamId, std::int64_t correlationId)
            {
                std::cout << "Subscription: " << channel << " " << correlationId << ":" << streamId << std::endl;
            });

    context_.availableImageHandler([](Image &image)
                                  {
                                      std::cout << "Available image correlationId=" << image.correlationId() << " sessionId=" << image.sessionId();
                                      std::cout << " at position=" << image.position() << " from " << image.sourceIdentity() << std::endl;
                                  });

    context_.unavailableImageHandler([](Image &image)
                                    {
                                        std::cout << "Unavailable image on correlationId=" << image.correlationId() << " sessionId=" << image.sessionId();
                                        std::cout << " at position=" << image.position() << " from " << image.sourceIdentity() << std::endl;
                                    });

    aeron_ = Aeron::connect(context_);
    // add the subscription to start the process
    std::int64_t id = aeron_->addSubscription(channel_, streamId_);

    subscription_ = aeron_->findSubscription(id);
    // wait for the subscription to be valid
    while (!subscription_)
    {
        std::this_thread::yield();
        subscription_ = aeron_->findSubscription(id);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    running_ = true;
}

static const std::chrono::duration<long, std::milli> IDLE_SLEEP_MS(1);
static const int FRAGMENTS_LIMIT = 10;

void printEndOfStream(Image &image)
{
    std::cout << "End Of Stream image correlationId=" << image.correlationId()
              << " sessionId=" << image.sessionId()
              << " from " << image.sourceIdentity()
              << std::endl;
}


void AeronSubscriber::startReceiving(aeron::fragment_handler_t callback){
    bool reachedEos = false;
    SleepingIdleStrategy idleStrategy(IDLE_SLEEP_MS);
    while (running_)
    {
        const int fragmentsRead = subscription_->poll(callback, FRAGMENTS_LIMIT);

        if (0 == fragmentsRead)
        {
            if (!reachedEos && subscription_->pollEndOfStreams(printEndOfStream) > 0)
            {
                reachedEos = true;
            }
        }

        idleStrategy.idle(fragmentsRead);
    }
}