#ifndef change_H
#define change_H

namespace piagnostics {
	void change(DiagnosticAdapter& adapter, Changes& changes) {
		if(changes.LangChanged()) adapter.ToggleLanguage();
		if(changes.UnitsChanged()) adapter.ToggleUnits();
	}
}

#endif
