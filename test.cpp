#include <iostream>
#include <string>

#include "Language.cpp"
#include "Pid.cpp"
#include "DiagnosticAdapter.cpp"
#include "Units.cpp"
#include "UsbConnection.cpp"

using namespace piagnostics;

void test() {

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

int main() {
	
	DiagnosticAdapter diag(English);
	cout << diag.Speed(Units::Imperial) << endl;
	cout << diag.Rpm() << endl;
	cout << diag.OutsideTemperature(Units::Imperial) << endl;
	cout << diag.CheckEngineLight() << endl;
	diag.ToggleLanguage();
	

	//UsbConnection conn;
	//vector<uint8_t> v = conn.Fetch("010c1");
	//cout << (char)v[0] << (char)v[1] << endl;
//	test();	
	return 0;
}
