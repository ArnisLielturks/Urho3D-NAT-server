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

#pragma once

#include <Urho3D/Engine/Application.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

#include "SLikeNet/peerinterface.h"
#include "SLikeNet/sleep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits> // used for std::numeric_limits
#include "SLikeNet/Kbhit.h"
#include "SLikeNet/MessageIdentifiers.h"
#include "SLikeNet/BitStream.h"
#include "SLikeNet/sleep.h"
#include "SLikeNet/UDPProxyServer.h"
#include "SLikeNet/UDPProxyCoordinator.h"
#include "SLikeNet/NatPunchthroughServer.h"
#include "SLikeNet/SocketLayer.h"
#include "SLikeNet/Getche.h"
#include "SLikeNet/Gets.h"
#include "SLikeNet/statistics.h"
#include "SLikeNet/RelayPlugin.h"
#include "SLikeNet/linux_adapter.h"
#include "SLikeNet/osx_adapter.h"

using namespace Urho3D;

const int SERVER_PORT = 61111;

/// Urho3DPlayer application runs a script specified on the command line.
class Server : public Application
{
    URHO3D_OBJECT(Server, Application);

public:
    /// Construct.
    Server(Context* context);

    /// Setup before engine initialization. Verify that a script file has been specified.
    virtual void Setup() override;
    /// Setup after engine initialization. Load the script and execute its start function.
    virtual void Start() override;
    /// Cleanup after the main loop. Run the script's stop function if it exists.
    virtual void Stop() override;

private:
	void HandleUpdate(StringHash eventType, VariantMap& eventData);

    void HandleIncomingPacket(SLNet::Packet *packet);

    void CreateUI();
    void StartupServer();
    void ShutdownServer();

    /// Flag whether CommandLine.txt was already successfully read.
    bool commandLineRead_;

    SLNet::RakPeerInterface *rakPeer_;
    SLNet::NatPunchthroughServer *nps_;

    SharedPtr<Text> connectionCount_;
    SharedPtr<Text> networkStatistics_;
};
