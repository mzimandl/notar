#include "SDL/SDL.h"

#include <math.h>

class cTimer
{
	private:
		int StartTicks;
		int StopTicks;
		bool Running;
	public:		
		void Start();
		void Stop();
		void Continue();
		void Reset();
		
		int GetTicks();
		bool Counting();
};

void cTimer::Start()
{
	StartTicks = SDL_GetTicks();
	StopTicks = 0;
	Running = true;
}

void cTimer::Stop()
{
	StopTicks = SDL_GetTicks();
	Running = false;
}

void cTimer::Continue()
{
	if (!Running) {
		StartTicks = SDL_GetTicks() - (StopTicks - StartTicks);
		Running = true;
	}
}

void cTimer::Reset()
{
	StartTicks = 0;
	StopTicks = 0;
	Running = false;
}

int cTimer::GetTicks()
{
	if (!Running) return StopTicks - StartTicks;
	else return SDL_GetTicks() - StartTicks;
}

bool cTimer::Counting()
{
	return Running;
}
