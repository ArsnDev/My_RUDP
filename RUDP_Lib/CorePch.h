#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> 

#include "Log.h"
#include "Logger.h"
#include "Packet.h"
#include "RUDP.h"