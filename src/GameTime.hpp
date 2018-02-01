#pragma once

namespace Raster
{
	class GameTime
	{
	public:
		GameTime() {
			QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
			
			Update();
		}
		float GetDeltaTime() const {
			return float((double)(CurrentTime - TimeLastTick) / (double)(m_Frequency));
		}
		void Update() {
			TimeLastTick = CurrentTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);

			
		}
	private:
		__int64 TimeLastTick;
		__int64 CurrentTime;
		__int64 m_Frequency;
	};
}
