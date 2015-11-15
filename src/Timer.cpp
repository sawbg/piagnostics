#ifndef Timer_h
#define Timer_h

#include <ctime>

namespace piagnostics {

	/**
	 * This class is designed to act as a timer for a series of commands in
	 * a while loop.
	 *
	 * Note that this timer can be up to one second too short or too long.
	 *
	 * In Piagnostics, this class times the various groups' diagnostic
	 * information display time.
	 *
	 * And example use of Timer:
	 * 	
	 * 	Timer timer(5);  // initialize Timer instance, 5 sec time
	 *
	 * 	while(timer) {
	 *		DoStuff();
	 *		DoMoreStuff();
	 *		Chillax();
	 * 	}
	 */
	class Timer {
		public:
			/*
			 * Initializes a timer for the specified number of
			 * seconds but does NOT start the timer.
			 *
			 * @param seconds the number of seconds for which to set
			 * the timer
			 */
			Timer(unsigned int seconds);

			/**
			 * Starts or restarts the timer.
			 */
			void Begin();

			/**
			 * Whether the timer has expired.
			 *
			 * @return is time up?
			 */
			bool Expired();

			/**
			 * Whether the timer is still running. Opposite of
			 * Expired().
			 *
			 * @return is there still time?
			 */
			bool Running();

			/**
			 * Allows Timer instance itself to be evaluated as a
			 * boolean.
			 *
			 * Example:
			 * 	Timer timer(5);
			 * 	if(timer) doStuff();  // do if still time
			 */
			explicit operator bool() const { return Running(); }

		private:
			unsigned int duration;  // timer duration in seconds
			unsigned long long end;  // UNIX time to stop timer
	}

	void Timer::Timer(unsigned int seconds) {
		duration = seconds;
	}

	void Timer::Begin() {
		end = std::time(nullptr) + duration;
	}

	bool Timer::Expired() {
		return std::time(nullptr) > end;
	}

	bool Timer::Running() { 
		return std::time(nullptr) <= end;
	}
}

#endif
