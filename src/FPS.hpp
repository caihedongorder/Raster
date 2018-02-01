#pragma once

namespace Raster
{
	class FPS
	{
	public:
		FPS() {
			QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
		}
		void Update() {

			if (GetEscapeTime() > 1.0f) { QueryPerformanceCounter((LARGE_INTEGER*)&m_BeginTime); m_RenderCount = 0; }

			++m_RenderCount;
		}
		inline float GetFPS()const {
			return (float)m_RenderCount / GetEscapeTime();
		}
		inline float GetTimeConsumeOneFrame()const {
			return (float)GetEscapeTime()*1000.0f / m_RenderCount;
		}
	protected:
		inline double GetEscapeTime() const {
			__int64 currentTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

			return (double)(currentTime - m_BeginTime) / (double)(m_Frequency);
		}
	private:
		__int64 m_Frequency;
		__int64 m_BeginTime;
		int m_RenderCount;
	};
}
