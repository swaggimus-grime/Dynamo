#pragma once

#define _USE_MATH_DEFINES
#define NOMINMAX

#include <string>
using namespace std::literals;

#include <memory>
#include <wrl.h>
#include <d3d11.h>
#include <array>
#include <DirectXMath.h>
#include <vector>
#include <queue>
#include <optional>
#include <math.h>
#include <map>
#include <comdef.h>
#include <type_traits>
#include <unordered_map>
#include <sstream>
#include "Debug/DynamoException.h"

static std::wstring NarrowToWide(const std::string& str)
{
	wchar_t buff[1000];
	mbstowcs_s(nullptr, buff, str.c_str(), 1000);
	return buff;
}

static std::vector<std::string> Parse(const std::string& s, char del)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    while (ss.good()) {
        std::string sub;
        std::getline(ss, sub, del);
        tokens.push_back(std::move(sub));
    }

    return tokens;
}

using Microsoft::WRL::ComPtr;
using namespace DirectX;

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Unique<T> MakeUnique(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Shared = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Shared<T> MakeShared(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}
