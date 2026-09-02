#include "OutputModule.hpp"

namespace bennu {
namespace io {

OutputModule::OutputModule() :
    IOModule()
{
}

void OutputModule::start(const distributed::Endpoint& endpoint)
{
    mClient.reset(new distributed::Client(endpoint));
}

void OutputModule::scanOutputs()
{
    auto bTags = mDataManager->getUpdatedBinaryTags();
    std::vector<std::string> bTagsToDelete;
    for (auto& t : bTags)
    {
        std::string point;
        if (mDataManager->getPointByTag(t.first, point))
        {
            // write to provider
            mClient->writePoint(point, t.second);
            // write to rtu datastore
            mDataManager->setDataByTag<bool>(t.first, t.second);
            // mark tag for deletion
            bTagsToDelete.push_back(t.first);
        }
    }
    // delete binary tags
    for (std::string tag : bTagsToDelete)
    {
        mDataManager->deleteBinaryTag(tag);
    }

    auto aTags = mDataManager->getUpdatedAnalogTags();
    std::vector<std::string> aTagsToDelete;
    for (auto& t : aTags)
    {
        std::string point;
        if (mDataManager->getPointByTag(t.first, point))
        {
            // write to provider
            mClient->writePoint(point, t.second);
            // write to rtu datastore
            mDataManager->setDataByTag<double>(t.first, t.second);
            // mark tag for deletion
            aTagsToDelete.push_back(t.first);
        }
    }
    // delete analog tags
    for (std::string tag : aTagsToDelete)
    {
        mDataManager->deleteAnalogTag(tag);
    }
}

} // namespace io
} // namespace bennu
