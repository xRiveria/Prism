#pragma once

namespace Prism
{
	class ApplicationVersion
	{
	public:

		ApplicationVersion(const int majorVersion, const int minorVersion) : m_majorVersion(majorVersion), m_minorVersion(minorVersion)
		{

		}

		const std::string RetrieveApplicationVersion() const;
		void SetApplicationVersion(const int majorVersion, const int minorVersion);
		
	private:
		int m_majorVersion;
		int m_minorVersion;
	};
}