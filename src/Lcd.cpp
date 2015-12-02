#ifndef Lcd_H
#define Lcd_H

#include <string>

namespace piagnostics {

	/**
	 * This class is a wrapper around the necessary functions in wiringPi.h
	 * and lcd.h.
	 *
	 * The design of the LCD screen and its wiring to the
	 * Pi that is used in Piagnostics is hard-coded into this class. Thus,
	 * this class caters to 4x20 screen layouts.
	 */
	class Lcd {
		public:
			/**
			 * Initializes the LCD screen based on hard-coded
			 * Wiring Pi pin numbers.
			 */
			Lcd();

			/**
			 * Displays one to four lines of text on the LCD.
			 *
			 * Note that this function prints text on the screen
			 * from the top downwards. Even though three of the four
			 * parameters are optional, a blank row on the LCD
			 * separating two rows requires an empty string to be
			 * passed to this function.
			 *
			 * @param first the <=20 character string to display in
			 * the top row of the LCD screen
			 *
			 * @param second the (optional) <=20 character string
			 * to display in the second row from the top of the LCD
			 * screen
			 *
			 * @param thrid the (optional) <=20 character string to
			 * display in the third row from the top of the LCD
			 * screen
			 *
			 * @param fourth the (optional)
			 */
			void Show(std::string first, std::string second = "",
					std::string third = "", std::string fourth = "");

		private:
			/**
			 * The handle for the LCD screen, as given by the
			 * lcdInit() function in lcd.h.
			 */
			int handle;

			/**
			 * Clears the LCD screen but does NOT reset the cursor
			 * position.
			 */
			void Clear();

			/**
			 * Sets the cursor to (0,0) but does NOT clear the
			 * screen.
			 */
			void Home();

			/**
			 * Sets the cursor position to the specified
			 * coordinates.
			 *
			 * The row and column numbering starts at 1, no 0 as
			 * with lcdPosition(). An additional difference between
			 * this and the aforementioned function is the switched
			 * order of parameters.
			 *
			 * Calling Home(row) will simply place the cursor at the
			 * beginning of that row.
			 *
			 * @param row the row on which to place the cursor as
			 * counted from the top downwards beginning with 1.
			 *
			 * @param col the (optional) column on which to place
			 * the cursor as counted from the left towards the right
			 * beginning with 1.
			 */
			void Position(int row, int col = 0);

			/**
			 * Prints a message to the LCD screen beginning at the
			 * place of the cursor.
			 *
			 * @param mesg the <=20 character string to display
			 */
			void Print(std::string mesg);

	};

	Lcd::Lcd() {
		wiringPiSetup();
		handle = lcdInit(4, 16, 4, 6, 5, 7, 0, 2, 3, 0, 0, 0, 0);
		Clear();
	}

	void Lcd::Clear() {
		lcdClear(handle);
	}

	void Lcd::Home() {
		lcdHome(handle);
	}

	void Lcd::Position(int row, int col = 0) {
		if(row > 0) row -= 1;
		if(col > 0) col -= 1;
		lcdPosition(handle, col, row);  // yes, col before row
	}

	void Lcd::Print(std::string mesg) {
		lcdPrintf(handle, mesg.c_str());
	}

	void Lcd::Show(std::string first, std::string second, std::string third,
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
