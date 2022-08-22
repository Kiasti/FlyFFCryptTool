#pragma once
#include <random>
#include <limits>
#include <mutex>
#include <ctime>

namespace rng
{
	class random
	{
			static inline std::mt19937_64 gen = std::mt19937_64(time(nullptr) + 1);
			static std::mutex random_mutex;
	
		public:
			template<class T = unsigned long> static T rand(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) 
				requires std::is_floating_point_v<T> || std::is_integral_v<T>
			{
				std::scoped_lock lck(random_mutex);
				if constexpr (std::is_integral_v<T>)
				{
					const std::uniform_int_distribution dis(min, max);
					return dis(gen);
				}
				else
				{
					const std::uniform_real_distribution dis(min, max);
					return dis(gen);
				}
			}
	};
	std::mutex random::random_mutex;
}

