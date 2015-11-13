#ifndef USB_CONNECTION_H
#define USB_CONNECTION_H

#include <fcntl.h>
#include <string>
#include <termios.h>
#include <libexplain/read.h>
#include <libexplain/write.h>

namespace piagnostics {

	class UsbConnection {

		public:
			UsbConnection(std::string device);
			~UsbConnection();
			std::string Send(std::string data);

		private:
			int port;
			struct termios config;

		
	};

	/**
	 * Initializes a USB connection with a specificed device 
	 * (e.g., "/dev/ttyUSB0").
	 *
	 * This code was based off example code taken from
	 * https://en.wikibooks.org/wiki/Serial_Programming/terminos
	 */

	UsbConnection::UsbConnection(std::string device) {		
		//		if(!isatty(port)) throw ...

		/* Opens port.
		 *
		 * In order:
		 * 	Opens port for reading and writing
		 * 	Port never becomes the controlling terminal
		 * 	Use non-blocking I/O
		 */

		port = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
//		port = open(device.c_str(), O_RDWR );	

		// if(port < 0) throw error

		/*
		 * Turns off input processing.
		 *
		 * In order:
		 * 	Convert break to nullbyte (first two args, I think)
		 * 	No CR to NL translation
		 * 	No NL to CR translation
		 * 	Don't mark parity errors or breaks
		 *	No Input parity check
		 *	Don't strip high bit off
		 *	No XON/XOFF software flow control
		 */

		//config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR
		//		| PARMRK | INPCK | ISTRIP | IXON);

		/* Output processing. Not sure if it matters that none of these matter */
		//config.c_oflag = 0;

		/*
		 * Line processing. Not sure why exactly these matter, but they work in the example.
		 * 
		 * In order:
		 * 	echo off
		 * 	echo newline off
		 * 	canonical mode off
		 * 	extended input processing off
		 * 	signal chars off
		 */

		//config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

		/*
		 * Turn off character processing.
		 *
		 * In order:
		 * 	Clear current char size mask
		 * 	No parity checking
		 * 	Force 8-bit input
		 */

		config.c_cflag &= ~(CSIZE | PARENB);
		config.c_cflag |= CS8;

		config.c_cc[VMIN] = 1;  // One input byte is enough to return from read()
		config.c_cc[VTIME] = 0;  // Inter-character timer off

		// set speeds
		int ispeed = cfsetospeed(&config, B115200);
		int ospeed = cfsetispeed(&config, B115200);

		//		if(ispeed < 0) throw error
		//		if(ospeed < 0) throw error

		int applyconfig = tcsetattr(port, TCSAFLUSH, &config);

		/*if (applyconfig < 0) {
			cout <<	explain_tcsetattr(port,TCSAFLUSH,&config);
		}*/
		
		// if(applyconfig < 0) throw error
		



		char byte[10];
		int status1 = write(port, "ati", 3);
		int status2 = read(port, byte, 5);
	
		cout << status1 << endl;
		cout << status2 << endl;

		cout << explain_write(port, "ati", 3) << endl << endl;
		cout << explain_read(port, byte, 5) << endl << endl;
		cout << byte << endl;
	}

	UsbConnection::~UsbConnection() {
		close(port);
	}

	std::string UsbConnection::Send(std::string data) {

	}

}

#endif
