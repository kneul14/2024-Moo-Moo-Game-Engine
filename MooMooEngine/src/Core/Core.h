#pragma once

#include<memory>

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

//This is created so we can verify something and if it's not verified then we must break/end the application
#define CORE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

namespace MooMooEngine {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	//template<typename T>
	//using Shared = std::shared_ptr<T>;
	//template<typename T, typename ... Args>
	//constexpr Shared<T> CreateRef(Args&& ... args, Args&& ... args2)
	//{
	//	return std::make_shared<T>(std::forward<Args>(args)..., std::forward<Args>(args2)...);
	//}

}