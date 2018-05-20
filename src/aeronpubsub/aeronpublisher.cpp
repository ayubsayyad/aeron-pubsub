#include "aeronpublisher.hpp"

using namespace aeron;

void AeronPublisher::init(const std::string& dirPrefix,
                          const std::string& channel,
                          const std::int32_t streamId){

    dirPrefix_ = dirPrefix;
    channel_ = channel;
    streamId_ = streamId;

    srcBuffer.wrap(&buffer[0], buffer.size());

}
void AeronPublisher::start(){

    context_.aeronDir(dirPrefix_);

    context_.newPublicationHandler(
            [](const std::string& channel, std::int32_t streamId, std::int32_t sessionId, std::int64_t correlationId)
            {
                std::cout << "Publication: " << channel << " " << correlationId << ":" << streamId << ":" << sessionId << std::endl;
            });

    aeron_ = Aeron::connect(context_);

    std::int64_t id = aeron_->addPublication(channel_, streamId_);
    publication_ = aeron_->findPublication(id);
    while (!publication_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "finding publication" << std::endl;
        std::this_thread::yield();
        publication_ = aeron_->findPublication(id);
    }
}

std::int64_t AeronPublisher::publish(char* message, const std::int32_t messageLen){
    srcBuffer.putBytes(0, reinterpret_cast<std::uint8_t *>(message), messageLen);
    return publication_->offer(srcBuffer, 0, messageLen);
}
