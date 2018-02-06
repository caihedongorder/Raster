#pragma once

namespace Raster
{
	class FPS
	{
	public:
		FPS() {
			QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
            QueryPerformanceCounter((LARGE_INTEGER*)&m_BeginTime);
            mFPS = 0 ;
            m_RenderCount = 0;
		}
		void Update() {

            double TimeEscape = GetEscapeTime();

			if ( TimeEscape > 1.0) { 
                QueryPerformanceCounter((LARGE_INTEGER*)&m_BeginTime);
                mFPS = m_RenderCount / TimeEscape ;
                m_RenderCount = 0;
                
            }
            else
            {
                ++m_RenderCount;
            }

		}
		inline float GetFPS()const {
			return mFPS;
		}
		inline float GetTimeConsumeOneFrame()const {
			return mFPS * 1000.0f / m_RenderCount;
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
        float mFPS;
		int m_RenderCount;
	};
}
