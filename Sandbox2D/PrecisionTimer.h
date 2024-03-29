#pragma once

//=======================================================================================
// PrecisionTimer.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================
class PrecisionTimer
{
public:
	// -------------------------
	// Constructors & no Destructor -> use auto generated destructor and others
	// -------------------------
	PrecisionTimer();

	// -------------------------
	// General Methods
	// -------------------------

	// Returns the total time elapsed since reset() was called, NOT counting any
	// time when the clock is stopped.	
	double GetGameTime();  // in seconds

	// Returns the elapsed time since tick was called
	double GetDeltaTime() const; // in seconds

	// Resets all 
	void Reset(); // Call before message loop.
	// Stores the start time
	void Start(); // Call when unpaused.
	// Stores the current time
	void Stop();  // Call when paused.
	// Calculates the time difference between this frame and the previous.
	// Use GetDeltaTime to retrieve that time
	void Tick();  // Call every frame.
	// Returns the state
	bool IsStopped() const;

private:
	//---------------------------
	// Private methods
	//---------------------------
	double m_SecondsPerCount = 0.0;
	double m_DeltaTime = -1.0;

	//---------------------------
	// Datamembers
	//---------------------------
#ifdef _WIN32

	__int64 m_BaseTime = 0;
	__int64 m_PausedTime = 0;
	__int64 m_StopTime = 0;
	__int64 m_PrevTime = 0;
	__int64 m_CurrTime = 0;
#else
	std::clock_t m_BaseTime = 0;
	std::clock_t m_PausedTime = 0;
	std::clock_t m_StopTime = 0;
	std::clock_t m_PrevTime = 0;
	std::clock_t m_CurrTime = 0;
#endif

	bool m_bStopped = false;
};