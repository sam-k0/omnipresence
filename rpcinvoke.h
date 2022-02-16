#ifndef RPCINVOKE_H_INCLUDED
#define RPCINVOKE_H_INCLUDED
#include "gms.h"
#include <windows.h>
#include <direct.h>
#include <iostream>
#include "discord-files/discord_register.h"
#include "discord-files/discord_rpc.h"

/// Discord handlers
static void handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("\nDiscord: connected to user %s#%s - %s\n",
           connectedUser->username,
           connectedUser->discriminator,
           connectedUser->userId);
}


static void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void handleDiscordJoin(const char* secret)
{
    printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
    printf("\nDiscord: spectate (%s)\n", secret);
}

static void handleDiscordJoinRequest(const DiscordUser* request)
{
    printf("\nDiscord: join request from %s#%s - %s\n",
           request->username,
           request->discriminator,
           request->userId);
}


/**
* @brief init discord sdk
*/
static void gmrpc_init(const char* APPLICATION_ID)
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    handlers.joinGame = handleDiscordJoin;
    handlers.spectateGame = handleDiscordSpectate;
    handlers.joinRequest = handleDiscordJoinRequest;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

static void gmrpc_setPresence(stringToDLL state, stringToDLL details, stringToDLL largeKey, stringToDLL smallKey )
{
    char statebuf[256];
    char detbuf[256];
    char smallBuf[256];
    char largeBuf[256];
    // Get RPC
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    /// Set state
    string tempState = gmu::constcharptr_to_string(state);
    sprintf(statebuf, "%s", gmu::string_to_charptr(tempState));
    discordPresence.state = statebuf;

    /// Set Details
    string tempDetails = gmu::constcharptr_to_string(details);
    sprintf(detbuf, "%s", gmu::string_to_charptr(tempDetails));
    discordPresence.details = detbuf;

    /// Set small image
    string tempSmall = gmu::constcharptr_to_string(smallKey);
    sprintf(smallBuf, "%s", gmu::string_to_charptr(tempSmall));
    discordPresence.smallImageKey = smallBuf;

    /// Set large image
    string tempLarge = gmu::constcharptr_to_string(largeKey);
    sprintf(largeBuf, "%s", gmu::string_to_charptr(tempLarge));
    discordPresence.largeImageKey = largeBuf;

    /// Finish and call update
    discordPresence.instance = 0;
    Discord_UpdatePresence(&discordPresence);
    Discord_RunCallbacks();

    cout << "Updated Presence to "<<tempState << " / "<< tempDetails<<endl;
}
static void gmrpc_exit()
{
    std::cout << "Exiting GMRPC"<<endl;
    Discord_Shutdown();
}
#endif // RPCINVOKE_H_INCLUDED
