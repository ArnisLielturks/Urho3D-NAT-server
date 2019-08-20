//
// Copyright (c) 2008-2017 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/Core/Main.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>
#include <Urho3D/Engine/Console.h>

#include "Server.h"
#include <Urho3D/UI/Button.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/DebugNew.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/AngelScript/ScriptAPI.h>

URHO3D_DEFINE_APPLICATION_MAIN(Server);

using namespace SLNet;

static const char* RAKNET_MESSAGEID_STRINGS[] = {
    "ID_CONNECTED_PING",
    "ID_UNCONNECTED_PING",
    "ID_UNCONNECTED_PING_OPEN_CONNECTIONS",
    "ID_CONNECTED_PONG",
    "ID_DETECT_LOST_CONNECTIONS",
    "ID_OPEN_CONNECTION_REQUEST_1",
    "ID_OPEN_CONNECTION_REPLY_1",
    "ID_OPEN_CONNECTION_REQUEST_2",
    "ID_OPEN_CONNECTION_REPLY_2",
    "ID_CONNECTION_REQUEST",
    "ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY",
    "ID_OUR_SYSTEM_REQUIRES_SECURITY",
    "ID_PUBLIC_KEY_MISMATCH",
    "ID_OUT_OF_BAND_INTERNAL",
    "ID_SND_RECEIPT_ACKED",
    "ID_SND_RECEIPT_LOSS",
    "ID_CONNECTION_REQUEST_ACCEPTED",
    "ID_CONNECTION_ATTEMPT_FAILED",
    "ID_ALREADY_CONNECTED",
    "ID_NEW_INCOMING_CONNECTION",
    "ID_NO_FREE_INCOMING_CONNECTIONS",
    "ID_DISCONNECTION_NOTIFICATION",
    "ID_CONNECTION_LOST",
    "ID_CONNECTION_BANNED",
    "ID_INVALID_PASSWORD",
    "ID_INCOMPATIBLE_PROTOCOL_VERSION",
    "ID_IP_RECENTLY_CONNECTED",
    "ID_TIMESTAMP",
    "ID_UNCONNECTED_PONG",
    "ID_ADVERTISE_SYSTEM",
    "ID_DOWNLOAD_PROGRESS",
    "ID_REMOTE_DISCONNECTION_NOTIFICATION",
    "ID_REMOTE_CONNECTION_LOST",
    "ID_REMOTE_NEW_INCOMING_CONNECTION",
    "ID_FILE_LIST_TRANSFER_HEADER",
    "ID_FILE_LIST_TRANSFER_FILE",
    "ID_FILE_LIST_REFERENCE_PUSH_ACK",
    "ID_DDT_DOWNLOAD_REQUEST",
    "ID_TRANSPORT_STRING",
    "ID_REPLICA_MANAGER_CONSTRUCTION",
    "ID_REPLICA_MANAGER_SCOPE_CHANGE",
    "ID_REPLICA_MANAGER_SERIALIZE",
    "ID_REPLICA_MANAGER_DOWNLOAD_STARTED",
    "ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE",
    "ID_RAKVOICE_OPEN_CHANNEL_REQUEST",
    "ID_RAKVOICE_OPEN_CHANNEL_REPLY",
    "ID_RAKVOICE_CLOSE_CHANNEL",
    "ID_RAKVOICE_DATA",
    "ID_AUTOPATCHER_GET_CHANGELIST_SINCE_DATE",
    "ID_AUTOPATCHER_CREATION_LIST",
    "ID_AUTOPATCHER_DELETION_LIST",
    "ID_AUTOPATCHER_GET_PATCH",
    "ID_AUTOPATCHER_PATCH_LIST",
    "ID_AUTOPATCHER_REPOSITORY_FATAL_ERROR",
    "ID_AUTOPATCHER_CANNOT_DOWNLOAD_ORIGINAL_UNMODIFIED_FILES",
    "ID_AUTOPATCHER_FINISHED_INTERNAL",
    "ID_AUTOPATCHER_FINISHED",
    "ID_AUTOPATCHER_RESTART_APPLICATION",
    "ID_NAT_PUNCHTHROUGH_REQUEST",
    "ID_NAT_CONNECT_AT_TIME",
    "ID_NAT_GET_MOST_RECENT_PORT",
    "ID_NAT_CLIENT_READY",
    "ID_NAT_TARGET_NOT_CONNECTED",
    "ID_NAT_TARGET_UNRESPONSIVE",
    "ID_NAT_CONNECTION_TO_TARGET_LOST",
    "ID_NAT_ALREADY_IN_PROGRESS",
    "ID_NAT_PUNCHTHROUGH_FAILED",
    "ID_NAT_PUNCHTHROUGH_SUCCEEDED",
    "ID_READY_EVENT_SET",
    "ID_READY_EVENT_UNSET",
    "ID_READY_EVENT_ALL_SET",
    "ID_READY_EVENT_QUERY",
    "ID_LOBBY_GENERAL",
    "ID_RPC_REMOTE_ERROR",
    "ID_RPC_PLUGIN",
    "ID_FILE_LIST_REFERENCE_PUSH",
    "ID_READY_EVENT_FORCE_ALL_SET",
    "ID_ROOMS_EXECUTE_FUNC",
    "ID_ROOMS_LOGON_STATUS",
    "ID_ROOMS_HANDLE_CHANGE",
    "ID_LOBBY2_SEND_MESSAGE",
    "ID_LOBBY2_SERVER_ERROR",
    "ID_FCM2_NEW_HOST",
    "ID_FCM2_REQUEST_FCMGUID",
    "ID_FCM2_RESPOND_CONNECTION_COUNT",
    "ID_FCM2_INFORM_FCMGUID",
    "ID_FCM2_UPDATE_MIN_TOTAL_CONNECTION_COUNT",
    "ID_FCM2_VERIFIED_JOIN_START",
    "ID_FCM2_VERIFIED_JOIN_CAPABLE",
    "ID_FCM2_VERIFIED_JOIN_FAILED",
    "ID_FCM2_VERIFIED_JOIN_ACCEPTED",
    "ID_FCM2_VERIFIED_JOIN_REJECTED",
    "ID_UDP_PROXY_GENERAL",
    "ID_SQLite3_EXEC",
    "ID_SQLite3_UNKNOWN_DB",
    "ID_SQLLITE_LOGGER",
    "ID_NAT_TYPE_DETECTION_REQUEST",
    "ID_NAT_TYPE_DETECTION_RESULT",
    "ID_ROUTER_2_INTERNAL",
    "ID_ROUTER_2_FORWARDING_NO_PATH",
    "ID_ROUTER_2_FORWARDING_ESTABLISHED",
    "ID_ROUTER_2_REROUTED",
    "ID_TEAM_BALANCER_INTERNAL",
    "ID_TEAM_BALANCER_REQUESTED_TEAM_FULL",
    "ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED",
    "ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED",
    "ID_TEAM_BALANCER_TEAM_ASSIGNED",
    "ID_LIGHTSPEED_INTEGRATION",
    "ID_XBOX_LOBBY",
    "ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_SUCCESS",
    "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS",
    "ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_FAILURE",
    "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE",
    "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT",
    "ID_TWO_WAY_AUTHENTICATION_NEGOTIATION",
    "ID_CLOUD_POST_REQUEST",
    "ID_CLOUD_RELEASE_REQUEST",
    "ID_CLOUD_GET_REQUEST",
    "ID_CLOUD_GET_RESPONSE",
    "ID_CLOUD_UNSUBSCRIBE_REQUEST",
    "ID_CLOUD_SERVER_TO_SERVER_COMMAND",
    "ID_CLOUD_SUBSCRIPTION_NOTIFICATION",
    "ID_LIB_VOICE",
    "ID_RELAY_PLUGIN",
    "ID_NAT_REQUEST_BOUND_ADDRESSES",
    "ID_NAT_RESPOND_BOUND_ADDRESSES",
    "ID_FCM2_UPDATE_USER_CONTEXT",
    "ID_RESERVED_3",
    "ID_RESERVED_4",
    "ID_RESERVED_5",
    "ID_RESERVED_6",
    "ID_RESERVED_7",
    "ID_RESERVED_8",
    "ID_RESERVED_9",
    "ID_USER_PACKET_ENUM"
};

Server::Server(Context* context) :
    Sample(context),
    commandLineRead_(false)
{
}

void Server::Setup()
{
    // Web platform depends on the resource system to read any data files. Skip parsing the command line file now
    // and try later when the resource system is live
    // Read command line from a file if no arguments given. This is primarily intended for mobile platforms.
    // Note that the command file name uses a hardcoded path that does not utilize the resource system
    // properly (including resource path prefix), as the resource system is not yet initialized at this point
    FileSystem* filesystem = GetSubsystem<FileSystem>();
    const String commandFileName = filesystem->GetProgramDir() + "Data/CommandLine.txt";
    if (filesystem->FileExists(commandFileName)) {
        SharedPtr<File> commandFile(new File(context_, commandFileName));
        if (commandFile->IsOpen()) {
            commandLineRead_ = true;
            //while (!commandFile->IsEof()) {
                String commandLine = commandFile->ReadLine();
                URHO3D_LOGINFO("Line: " + commandLine);
                ParseArguments(commandLine, false);
                // Reparse engine startup parameters now
                engineParameters_ = Engine::ParseParameters(GetArguments());
            //}
            commandFile->Close();
        }
    }

    // Show usage if not found
    // Use the script file name as the base name for the log file
	engineParameters_[EP_LOG_NAME] = "NATServer.log";// filesystem->GetAppPreferencesDir("urho3d", "logs") + GetFileNameAndExtension(scriptFileName_) + ".log";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_WIDTH] = 800;
    engineParameters_[EP_WINDOW_HEIGHT] = 600;
	engineParameters_[EP_LOG_LEVEL] = LOG_INFO;
    engineParameters_[EP_WINDOW_TITLE] = "NAT punchtrough server";

    // Construct a search path to find the resource prefix with two entries:
    // The first entry is an empty path which will be substituted with program/bin directory -- this entry is for binary when it is still in build tree
    // The second and third entries are possible relative paths from the installed program/bin directory to the asset directory -- these entries are for binary when it is in the Urho3D SDK installation location
    if (!engineParameters_.Contains(EP_RESOURCE_PREFIX_PATHS)) {
        engineParameters_[EP_RESOURCE_PREFIX_PATHS] = ";../share/Resources;../share/Urho3D/Resources";
    }
}

void Server::Start()
{
	// Switch level
    // Reattempt reading the command line from the resource system now if not read before
    // Note that the engine can not be reconfigured at this point; only the script name can be specified

    GetSubsystem<Engine>()->SetMaxFps(1);
    SharedPtr<File> commandFile = GetSubsystem<ResourceCache>()->GetFile("Data/CommandLine.txt", false);
    if (commandFile) {
        String commandLine = commandFile->ReadLine();
        commandFile->Close();
        auto arguments = ParseArguments(commandLine, false);
        for (unsigned i = 0; i < arguments.Size(); ++i) {
            if (arguments[i].Length() > 1 && arguments[i][0] == '-') {
                String argument = arguments[i].Substring(1).ToLower();
                String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;
                if (argument == "fps") {
                    GetSubsystem<Engine>()->SetMaxFps(ToInt(value));
                    URHO3D_LOGINFO("Setting FPS to " + value);
                }
            }
        }
    }

    CreateUI();
    StartupServer();

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Server, HandleUpdate));
}

void Server::Stop()
{
    ShutdownServer();
}

void Server::CreateUI()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Construct new Text object
    connectionCount_ = new Text(context_);

    // Set String to display
    connectionCount_->SetText("Connections: 0");

    // Set font and text color
    connectionCount_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 16);
    connectionCount_->SetColor(Color(0.0f, 1.0f, 0.0f));

    // Align Text center-screen
    connectionCount_->SetHorizontalAlignment(HA_LEFT);
    connectionCount_->SetVerticalAlignment(VA_TOP);
    connectionCount_->SetPosition(IntVector2(10, 10));

    // Add Text instance to the UI root element
    GetSubsystem<UI>()->GetRoot()->AddChild(connectionCount_);

    // Construct new Text object
    networkStatistics_ = new Text(context_);

    // Set String to display
    networkStatistics_->SetText("");

    // Set font and text color
    networkStatistics_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 14);
    networkStatistics_->SetColor(Color(1.0f, 1.0f, 0.0f));

    // Align Text center-screen
    networkStatistics_->SetHorizontalAlignment(HA_LEFT);
    networkStatistics_->SetVerticalAlignment(VA_TOP);
    networkStatistics_->SetPosition(IntVector2(10, 30));

    // Add Text instance to the UI root element
    GetSubsystem<UI>()->GetRoot()->AddChild(networkStatistics_);
}

void Server::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    if (!rakPeer_ || !rakPeer_->IsActive()) {
        return;
    }

    if (connectionCount_) {
        DataStructures::List<SystemAddress> addresses;
		DataStructures::List<RakNetGUID> guids;
		rakPeer_->GetSystemList(addresses, guids);
        connectionCount_->SetText("Connections: " + String(addresses.Size()));
    }

    if (networkStatistics_) {
        RakNetStatistics rns;
        bool hasStatistics = rakPeer_->GetStatistics(0, &rns);
		if (hasStatistics) {
            char message[2048];
			SLNet::StatisticsToString(&rns, message, 2048, 2);

			memset(&rns, 0, sizeof(SLNet::RakNetStatistics));
			rakPeer_->GetStatistics(SLNet::UNASSIGNED_SYSTEM_ADDRESS, &rns);
			SLNet::StatisticsToString(&rns, message, 2048, 2);
			networkStatistics_->SetText(String(message));
		} else {
            networkStatistics_->SetText("");
        }
    }
    SLNet::Packet *packet;
    for (packet = rakPeer_->Receive(); packet; rakPeer_->DeallocatePacket(packet), packet = rakPeer_->Receive()) {
        HandleIncomingPacket(packet);
    }
}

void Server::StartupServer()
{
    URHO3D_LOGINFO("Server startup");
    rakPeer_ = SLNet::RakPeerInterface::GetInstance();
    nps_ = new NatPunchthroughServer;
	rakPeer_->AttachPlugin(nps_);

    SLNet::SocketDescriptor sd[2];
    sd[0].port = SERVER_PORT;
    sd[1].port = SERVER_PORT + 1;
    strcpy(sd[0].hostAddress, String(String("*:") + String(SERVER_PORT)).CString());
    strcpy(sd[1].hostAddress, String(String("*:") + String(SERVER_PORT)).CString());
    rakPeer_->SetTimeoutTime(10000, UNASSIGNED_SYSTEM_ADDRESS);
    if (rakPeer_->Startup(8096, sd, 2) != SLNet::RAKNET_STARTED) {
        ShutdownServer();
    }

    rakPeer_->SetMaximumIncomingConnections(8096);
}

void Server::ShutdownServer()
{
    rakPeer_->DetachPlugin(nps_);
    delete nps_;
    rakPeer_->Shutdown(100);
	SLNet::RakPeerInterface::DestroyInstance(rakPeer_);
}

void Server::HandleIncomingPacket(SLNet::Packet *packet)
{
    unsigned char packetID = packet->data[0];
    URHO3D_LOGINFO("Incoming packet " + String(RAKNET_MESSAGEID_STRINGS[packetID]));
}