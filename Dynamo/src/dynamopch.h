#pragma once

#define _USE_MATH_DEFINES

#include <string>
using namespace std::literals;

#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <queue>
#include <optional>
#include <math.h>
#include <map>
#include <comdef.h>

static LPWSTR NarrowToWide(const std::string& str)
{
	wchar_t buff[1000];
	mbstowcs_s(nullptr, buff, str.c_str(), 1000);
	return buff;
}

#include "Graphics/VectorOps.h"
#include "Debug/DynamoException.h"

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