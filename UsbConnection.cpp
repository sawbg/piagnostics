#ifndef UsbConnection_H
#define UsbConnection_H

#include <ftdi.h>
#include <string>

namespace piagnostics {

	/**
	 * Manages ELM327 via USB.
	 * Based on code from https://codeseekah.com/2012/02/22/elml327-to-rs232-in-linux.
	 */
	
	class UsbConnection {

		public:
			UsbConnection();
			~UsbConnection();
			std::string Fetch(std::string data);

		private:
			int port;
			struct ftdi_context context;

			void Send(std::string data);
	};

	/**
	 * Initializes a USB connection with a specificed device 
	 * (e.g., "/dev/ttyUSB0").
	 */

	UsbConnection::UsbConnection() {		

		int initStatus = ftdi_init(&context);
		// if(initStatus < 0) throw error

		// I think numbers are vendor and product
		int openStatus = ftdi_usb_open(&context, 0x0403, 0x6001);
		// if(openStatus < 0) throw error
		
		int resetStatus = ftdi_usb_reset(&context);
		// if(resetStatus < 0) throw error
		
		int purgeStatus = ftdi_usb_purge_buffers(&context);
		// if(purgeStatus < 0) throw error
		
		int baudStatus = ftdi_set_baudrate(&context, 115200);
		// if(baudStatus < 0) throw error

		int lineStatus = ftdi_set_line_property2(&context, BITS_8, STOP_BIT_1, NONE, BREAK_OFF);
		// if(lineStatus < 0) throw error
		
		Send("ate0");  // disable echos
		Send("atsp0");  // automatic protocol detection
	}

	UsbConnection::~UsbConnection() {
		int closeStatus = ftdi_usb_close(&context);
		// if(closeStatus < 0) throw error

		ftdi_deinit(&context);
	}

	std::string UsbConnection::Fetch(std::string data) {
		const int MAX_LEN = 32;
		unsigned char temp[MAX_LEN];

		Send(data);
		ftdi_usb_purge_rx_buffer(&context);

		int len = ftdi_read_data(&context, temp, MAX_LEN);
		len -= 3;  // removes ">" prompt

		return std::string(reinterpret_cast<const char*>(temp));
	}
	
	void UsbConnection::Send(std::string data) {
		data += '\r';
		int writeStatus = ftdi_write_data(&context, (unsigned char*)data.c_str(),
				sizeof(unsigned char) * data.length());
		// if(writeStatus < 0) throw error
		
		ftdi_usb_purge_tx_buffer(&context);
	}

}

#endif
