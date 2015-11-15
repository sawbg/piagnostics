#ifndef Changes_H
#define Changes_H

namespace piagnostics {

	/**
	 * This class serves as a container for changes in the adapter lanugage
	 * and system of measurements to be stored until they are enacted.
	 */
	class Changes {
		public:

			/**
			 * Whether the language has changed since the last
			 * "clearing". Shold ONLY be called by the change()
			 * function.
			 */
			bool lang = false;

			/**
			 * Whether the system of measurements has changed since
			 * the last "clearing". Should ONLY be called by the
			 * change() function.
			 */
			bool units = false;


			/**
			 * Returns whether the language has changed and clears
			 * this internal boolean value.
			 *
			 * @return true if the language has changed since it was
			 * last reset
			 */
			bool LangChanged();

			/**
			 * Returns whether the system of measurements has
			 * changed and clears this internal boolena value.
			 *
			 * @return true if the system of measurements has
			 * changed since it was last reset
			 */
			bool UnitsChanged();
	}

	bool Changes::LangChanged() {
		bool ret = lang;
		lang = false;
		return ret;
	}

	bool Changes::UnitsChanged() {
		bool ret = units;
		units = false;
		return ret;
	}
}

#endif
