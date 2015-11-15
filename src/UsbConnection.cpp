#ifndef UsbConnection_H
#define UsbConnection_H

#include <cstdint>
#include <cstring>
#include <ftdi.h>
#include <string>
#include <unistd.h>
#include <vector>

namespace piagnostics {

	/**
	 * Manages ELM327 via USB.
	 * Based on code from https://codeseekah.com/2012/02/22/elml327-to-rs232-in-linux.
	 */

	class UsbConnection {

		public:
			vector<uint8_t> Fetch(std::string data);
		private:
			int port;
			struct ftdi_context context;
	};

	std::vector<uint8_t> UsbConnection::Fetch(std::string data) {		
		struct ftdi_context ftdic; /* FTDI context */
		/* line properties */
		enum ftdi_bits_type bits = BITS_8; // 8 data bits
		enum ftdi_stopbits_type sbit = STOP_BIT_1; // 1 stop bit
		enum ftdi_parity_type parity = NONE; // no parity
		enum ftdi_break_type lineend = BREAK_OFF;

		int ret = 0;
		unsigned short modem_status = 0;
		unsigned char * tx = (unsigned char*)malloc(32);
		unsigned char * rx = (unsigned char*)malloc(32);
		int i,j; /* general purpose indices */

		ftdi_init(&ftdic);
		ftdi_usb_open(&ftdic, 0x0403, 0x6001);
		ftdi_usb_reset(&ftdic);
		ftdi_usb_purge_buffers(&ftdic);
		ftdi_set_baudrate(&ftdic, 115200);
		ftdi_set_line_property2(&ftdic, bits, sbit, parity, lineend);
		ftdi_poll_modem_status(&ftdic, &modem_status);
		memcpy(tx, "AT E0\r", 6); /* turn echoing off */
		ftdi_write_data(&ftdic, tx, sizeof(unsigned char) * 6);

		for(j = 0; j < data.length(); j++) tx[j] = (char)data[j];		
		tx[j++] = 0x0d; /* end */
		ret = 0;
		ftdi_usb_purge_tx_buffer(&ftdic);
		ftdi_usb_purge_rx_buffer(&ftdic);

		for (i = 0; i < j; i++) {      
			ret += ftdi_write_data(&ftdic, tx+i, sizeof(unsigned char) * 1);      
		}

		bool cont = true;

		while(cont) {
			usleep(100000);  // THIS LINE IS CRITICAL TO THIS PROGRAM!	
			ret = ftdi_read_data(&ftdic, rx, 128);
			ret -= 3; // remove > prompt
			int len = ret;
			ftdi_usb_close(&ftdic);
			ftdi_deinit(&ftdic);
			bool started = false;
			std::vector<uint8_t> v(rx, rx + len);

			for(i = 0; i < len; i++) {
				if(!((v[i] >= '0' && v[i] <= '9') || (v[i] >= 'A' && v[i] <= 'F'))
						|| (!started && v[i] != '4')) {
					v.erase(v.begin() + i);
					i--;
					len--;
				} else if(v[i] == '4') started = true;
			}

			cont = len < 2;

			if(!cont) return v;
		}

		free(tx);
		free(rx);
	}

}

#endif
