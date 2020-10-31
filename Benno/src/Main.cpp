#include "Game.h"
#include "SgException.h"
#include "Log.h"

int main()
{
    sg::Log::Init();

    sg::Log::SG_LOG_DEBUG("[main()] Starting main.");
    sg::Log::SG_LOG_DEBUG("[main()] Logger was initialized.");

    try
    {
        auto game{ std::make_unique<sg::Game>() };
        game->Run();

        return EXIT_SUCCESS;
    }
    catch (const sg::SgException& e)
    {
        sg::Log::SG_LOG_ERROR("SgException {}", e.what());
    }
    catch (const std::exception& e)
    {
        sg::Log::SG_LOG_ERROR("Exception: {}", e.what());
    }
    catch (...)
    {
        sg::Log::SG_LOG_ERROR("Unknown Exception. No details available.");
    }

    return EXIT_FAILURE;
}
