#ifndef Timer_h
#define Timer_h

namespace piagnostics {

	/**
	 *
	 */
	class Timer {
		public:
			/*
			 *
			 */
			Timer(unsigned int seconds);

			/**
			 *
			 */
			void Begin();

			/**
			 *
			 */
			bool Expired();

			/**
			 *
			 */
			bool Running();

			/**
			 *
			 */
			explicit operator bool() const { return Running(); }

		private:
			unsigned int duration;
			unsigned long long end;
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
