#ifndef change_H
#define change_H

#include "Changes.cpp"
#include "DiagnosticAdapter.cpp"

namespace piagnostics {

	/**
	 * Changes the adapter language and system of measurements if necessary.
	 *
	 * @param adapter the DiagnosticAdapter for which to change the language
	 * and units
	 *
	 * @param changes the Changes container class storing the value of
	 * changes between calls to change()
	 */
	void change(DiagnosticAdapter& adapter, Changes& changes) {
		if(changes.LangChanged()) adapter.ToggleLanguage();
		if(changes.UnitsChanged()) adapter.ToggleUnits();
	}
}

#endif
