#pragma once
#include <memory>
#include <string>

namespace Prism
{
	class ILogInterface
	{
	public:
		virtual ~ILogInterface() = default;
		virtual void Log(const std::string& log, const int& type) = 0;
	};
}