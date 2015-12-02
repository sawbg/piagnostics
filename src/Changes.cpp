#ifndef Changes_H
#define Changes_H

namespace piagnostics {

	/**
	 *
	 */
	class Changes {
		public:

			/**
			 *
			 */
			bool lang = false;

			/**
			 *
			 */
			bool units = false;


			/**
			 *
			 */
			bool LangChanged();

			/**
			 *
			 */
			bool UnitsChanged();
	};

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
