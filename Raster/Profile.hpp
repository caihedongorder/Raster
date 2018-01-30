#pragma once
#include <tchar.h>

namespace Raster
{
	class ProfileTimer
	{
	public:
		ProfileTimer(const TCHAR* InProfileName)
			:m_ProfileName(InProfileName)
		{
			QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
			QueryPerformanceCounter((LARGE_INTEGER*)&m_BeginTime);
		}
		void TraceConsumeTime()
		{

			TCHAR ProfileMsg[MAXBYTE];
			_stprintf_s(ProfileMsg,MAXBYTE, TEXT("[%s] :%02f\n"),m_ProfileName,GetEscapeTime() );

			OutputDebugString(ProfileMsg);
		}
	protected:
		inline double GetEscapeTime() const {
			__int64 currentTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

			return (double)(currentTime - m_BeginTime) / (double)(m_Frequency) * 1000.0;
		}
	private:
		__int64 m_Frequency;
		__int64 m_BeginTime;
		const TCHAR* m_ProfileName;
	};

	class AutoProfileTimer:public ProfileTimer
	{
	public:
		AutoProfileTimer(const TCHAR* szProfileName)
			:ProfileTimer(szProfileName)
		{

		}
		~AutoProfileTimer() {
			TraceConsumeTime();
		}
	};
	
#define AUTO_PROFILE(InProfileName)	AutoProfileTimer __Profile(InProfileName);
}
