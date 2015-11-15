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
			UsbConnection();
			~UsbConnection();
			vector<uint8_t> Fetch(std::string data);
			void test();
		private:
			int port;
			struct ftdi_context context;

			void PurgeBuffers();
			void Send(std::string data);
	};

	/**
	 * Initializes a USB connection with a specificed device 
	 * (e.g., "/dev/ttyUSB0").
	 */

	UsbConnection::UsbConnection() {		

		/*int initStatus = ftdi_init(&context);
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
		//Send("atsp0");  // automatic protocol detection
		//Send("ats0");  // no spaces in response
		//Send("atl0");  // no linefeeds after \r
		*/

	}

	UsbConnection::~UsbConnection() {
		//int closeStatus = ftdi_usb_close(&context);
		// if(closeStatus < 0) throw error

		//ftdi_deinit(&context);
	}

	std::vector<uint8_t> UsbConnection::Fetch(std::string data) {		
		/*const int MAX_LEN = 64;
		  unsigned char temp[MAX_LEN];

		  memset(temp, 0, MAX_LEN);
		//		PurgeRxBuffer();
		Send(data);

		while(temp[0] != '4') int len = ftdi_read_data(&context, temp, MAX_LEN);

		// if(len < 0) throw error */

		struct ftdi_context ftdic; /* FTDI context */
		/* line properties */
		enum ftdi_bits_type bits = BITS_8; // 8 data bits
		enum ftdi_stopbits_type sbit = STOP_BIT_1; // 1 stop bit
		enum ftdi_parity_type parity = NONE; // no parity
		enum ftdi_break_type lineend = BREAK_OFF;

		int ret;
		unsigned short modem_status = 0;
		unsigned char * tx;
		unsigned char * rx;
		int i,j; /* general purpose indices */

		if ((tx = malloc(128)) == NULL || (rx = malloc(128)) == NULL) {
			fprintf(stderr, "Error: could not allocate memory\n");
			//	return EXIT_FAILURE;
		}

		if (ftdi_init(&ftdic) < 0) {
			fprintf(stderr, "Error: could not initialize FTDI\n");
			//	return EXIT_FAILURE;
		}

		if ((ret = ftdi_usb_open(&ftdic, 0x0403, 0x6001)) < 0) {
			fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			//	return EXIT_FAILURE;
		}

		if ((ret = ftdi_usb_reset(&ftdic)) < 0) {
			fprintf(stderr, "unable to reset ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			//	return EXIT_FAILURE;
		}

		if ((ret = ftdi_usb_purge_buffers(&ftdic)) < 0) {
			fprintf(stderr, "unable to purge buffers on ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			//	return EXIT_FAILURE;
		}

		if ((ret = ftdi_set_baudrate(&ftdic, 115200)) < 0) {
			fprintf(stderr, "unable to set ftdi device baud rate: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			//	return EXIT_FAILURE;
		}

		if ((ret = ftdi_set_line_property2(&ftdic, bits, sbit, parity, lineend)) < 0) {
			fprintf(stderr, "unable to set ftdi device properties: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			//	return EXIT_FAILURE;                
		}

		ftdi_poll_modem_status(&ftdic, &modem_status);
		//		printf("R232 status: 0x%x\n", modem_status);

		memcpy(tx, "AT E0\r", 6); /* turn echoing off */
		if (ftdi_write_data(&ftdic, tx, sizeof(unsigned char) * 6) < 0) {
			fprintf(stderr, "unable to send ftdi device data: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			//	return EXIT_FAILURE;             
		}

		//while (1) {
		//		j = 0;

		//while ( ( ret = getchar() ) != 0x0a && j < 128) tx[j++] = ret;
		//	if (!j) continue;

		for(j = 0; j < data.length(); j++) tx[j] = (char)data[j];		
		tx[j++] = 0x0d; /* end */
		ret = 0;
		ftdi_usb_purge_tx_buffer(&ftdic);
		ftdi_usb_purge_rx_buffer(&ftdic);
		for (i = 0; i < j; i++) {      
			ret += ftdi_write_data(&ftdic, tx+i, sizeof(unsigned char) * 1);      
		}

		//	printf("Written %d bytes of data: ", ret);
		//	for (i = 0; i < ret; i++) printf("0x%x ", tx[i]);  

		//	printf("\n");

		bool cont = true;

		while(cont) {
	//		while (cont && (!(rx[0] != '4' && rx[0] != '\r' && ret > 8) || rx[0] == 0)) {
			sleep(1);	
			ret = ftdi_read_data(&ftdic, rx, 128);
				//	if (ret > 0) {
				//	printf("Read %d bytes of data\n", ret);
				ret -= 3; // remove > prompt
				//	printf("\tDATA: ");
				//	for (i = 0; i < ret; i++) printf("0x%x ",rx[i]);
				//	printf("\n\t(");
				//	for (i = 0; i < ret; i++) printf("%c",rx[i]);
				//	printf(")\n");
				//break;
				//	} else if (ret < 0) {
				//		fprintf(stderr, "unable to read from ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
				//		return EXIT_FAILURE;
				//	}
//			}

			int len = ret;

			//}

			if ((ret = ftdi_usb_close(&ftdic)) < 0) {
				fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
				//	return EXIT_FAILURE;
			}

			ftdi_deinit(&ftdic);

			std::vector<uint8_t> v(rx, rx + len);

			for(i = 0; i < len; i++) {
				if(!((v[i] >= '0' && v[i] <= '9') || (v[i] >= 'A' && v[i] <= 'F'))) {
					v.erase(v.begin() + i);
					i--;
					len--;
				}
			}

			cont = len < 2;

			if(!cont) return v;
	}

	free(tx);
	free(rx);

	//return v;
	}

	void UsbConnection::PurgeBuffers() {
		int writePurge = ftdi_usb_purge_tx_buffer(&context);
		int readPurge = ftdi_usb_purge_rx_buffer(&context);
		// if(purgeStatus < 0) throw error

		int purgeStatus = ftdi_usb_purge_buffers(&context);
	}

	void UsbConnection::Send(std::string data) {	
		//		ftdi_usb_purge_tx_buffer(&context);
		data += 0x0d;
		unsigned char* cstr = malloc(128);

		sleep(2);

		PurgeBuffers();

		for(int i = 0; i < data.length(); i++) {
			cstr[i] = (unsigned char)data[i];
		}

		cstr[data.length()] = 0x0d;

		for(int i = 0; i < (data.length() + 1); i++ ) {
			int writeStatus = ftdi_write_data(&context, cstr + i,
					//					sizeof(unsigned char) * data.length());
			    1);
			// if(writeStatus < 0) throw error
		}

	}


	void UsbConnection::test() {
		struct ftdi_context ftdic; /* FTDI context */
		/* line properties */
		enum ftdi_bits_type bits = BITS_8; // 8 data bits
		enum ftdi_stopbits_type sbit = STOP_BIT_1; // 1 stop bit
		enum ftdi_parity_type parity = NONE; // no parity
		enum ftdi_break_type lineend = BREAK_OFF;

		int ret;
		unsigned short modem_status = 0;
		unsigned char * tx;
		unsigned char * rx;
		int i,j; /* general purpose indices */

		if ((tx = malloc(128)) == NULL || (rx = malloc(128)) == NULL) {
			fprintf(stderr, "Error: could not allocate memory\n");
			return EXIT_FAILURE;
		}

		if (ftdi_init(&ftdic) < 0) {
			fprintf(stderr, "Error: could not initialize FTDI\n");
			return EXIT_FAILURE;
		}

		if ((ret = ftdi_usb_open(&ftdic, 0x0403, 0x6001)) < 0) {
			fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			return EXIT_FAILURE;
		}

		if ((ret = ftdi_usb_reset(&ftdic)) < 0) {
			fprintf(stderr, "unable to reset ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			return EXIT_FAILURE;
		}

		if ((ret = ftdi_usb_purge_buffers(&ftdic)) < 0) {
			fprintf(stderr, "unable to purge buffers on ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			return EXIT_FAILURE;
		}

		if ((ret = ftdi_set_baudrate(&ftdic, 115200)) < 0) {
			fprintf(stderr, "unable to set ftdi device baud rate: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			return EXIT_FAILURE;
		}

		if ((ret = ftdi_set_line_property2(&ftdic, bits, sbit, parity, lineend)) < 0) {
			fprintf(stderr, "unable to set ftdi device properties: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			return EXIT_FAILURE;                
		}

		ftdi_poll_modem_status(&ftdic, &modem_status);
		printf("R232 status: 0x%x\n", modem_status);

		memcpy(tx, "AT E0\r", 6); /* turn echoing off */
		if (ftdi_write_data(&ftdic, tx, sizeof(unsigned char) * 6) < 0) {
			fprintf(stderr, "unable to send ftdi device data: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			return EXIT_FAILURE;             
		}

		//while (1) {
		j = 0;

		//while ( ( ret = getchar() ) != 0x0a && j < 128) tx[j++] = ret;
		//	if (!j) continue;
		tx[0] = '0';
		tx[1] = '1';
		tx[2] = '0';
		tx[3] = 'c';
		tx[4] = '1';
		j = 5;
		tx[j++] = 0x0d; /* end */
		ret = 0;
		ftdi_usb_purge_tx_buffer(&ftdic);
		ftdi_usb_purge_rx_buffer(&ftdic);
		for (i = 0; i < j; i++) {      
			ret += ftdi_write_data(&ftdic, tx+i, sizeof(unsigned char) * 1);      
		}

		printf("Written %d bytes of data: ", ret);
		for (i = 0; i < ret; i++) printf("0x%x ", tx[i]);  

		printf("\n");

		while (rx[0] != '4') {
			ret = ftdi_read_data(&ftdic, rx, 128);
			if (ret > 0) {
				printf("Read %d bytes of data\n", ret);
				ret -= 3; /* remove > prompt */
				printf("\tDATA: ");
				for (i = 0; i < ret; i++) printf("0x%x ",rx[i]);
				printf("\n\t(");
				for (i = 0; i < ret; i++) printf("%c",rx[i]);
				printf(")\n");
				//break;
			} else if (ret < 0) {
				fprintf(stderr, "unable to read from ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
				return EXIT_FAILURE;
			}
		}
		//}

		if ((ret = ftdi_usb_close(&ftdic)) < 0) {
			fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
			return EXIT_FAILURE;
		}

		ftdi_deinit(&ftdic);

		free(tx);
		free(rx);
	}
}

#endif
