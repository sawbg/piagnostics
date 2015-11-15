#ifndef LCD_H
#define LCD_H

#include <string>

namespace piagnostics {

	/**
	 *
	 */
	class LCD {
		public:
			/**
			 *
			 */
			LCD(std::string initial = "");

			/**
			 *
			 */
			void Show(std::string first, std::string second = "",
					std::string third = "", std::string fourth = "");

		private:
			/**
			 *
			 */
			int handle;

			/**
			 *
			 */
			void Clear();

			/**
			 *
			 */
			void Home();

			/**
			 * 
			 */
			void Position(int row, int col = 0);

			/**
			 *
			 */
			void Print(std::string mesg);

	}

	LCD::LCD(std::string initial) {
		wiringPiSetup();
		handle = lcdInit(4, 16, 4, 6, 5, 7, 0, 2, 3, 0, 0, 0, 0);
		Clear();

		if(initial != "") {
			Home();
			Print(initial);
		}
	}

	void LCD::Clear() {
		lcdClear(handle);
	}

	void LCD::Home() {
		lcdHome(handle);
	}

	void LCD::Position(int row, int col) {
		if(row > 0) row -= 1;
		if(col > 0) col -= 1;
		lcdPosition(handle, col, row);  // yes, col before row
	}

	void Print(std::string mesg) {
		lcdPrintf(handle, mesg.c_str());
	}

	void Show(std::string first, std::string second, std::string third,
			std::string fourth) {
		Clear();
		
		Position(1);
		Print(first);
		
		Position(2);
		Print(second);
		
		Position(3);
		Print(third);

		Position(4);
		Print(fourth);
	}
}

#endif
