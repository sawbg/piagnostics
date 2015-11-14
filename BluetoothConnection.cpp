#ifndef BluetoothConnection_H
#define BluetoothConnection_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <cstdint>
#include <cstring>
#include <exception>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

using namespace std;

namespace piagnostics {

	class BluetoothConnection {

		public:
			BluetoothConnection();
			~BluetoothConnection();			
			uint8_t* Send(string msg);
		private:
			int WriteSocket;

			void Close();
			bdaddr_t FindAddress();
			void Open(bdaddr_t addr);
	};

	BluetoothConnection::BluetoothConnection() {
		Open(FindAddress());
	}


	BluetoothConnection::~BluetoothConnection() {
		Close();
	}

	void BluetoothConnection::Close() {
		close(WriteSocket);
	}

	bdaddr_t BluetoothConnection::FindAddress() {
		const int MAX_RESPONSES = 16;
		const int SEARCH_TIME = 8;  // 1.28*var is the total seconds

		int numResponses;
		bdaddr_t address;
		inquiry_info* info = NULL;

		int devId = hci_get_route(NULL);
		// if(devId < 0) throw no_bluetooth_device_error
		cout << to_string(devId);

		int testSocket = hci_open_dev(devId);
		// if(sock < 0) throw bt_adapter_communication_error

		info = (inquiry_info*)malloc(MAX_RESPONSES * sizeof(inquiry_info));
		numResponses = hci_inquiry(devId, SEARCH_TIME, MAX_RESPONSES, NULL, &info, IREQ_CACHE_FLUSH);
		// if(numResponses < 0) throw bt_search_error

		for(int i = 0; i < numResponses; i++) {
			char name[248];
			memset(name, 0, sizeof(name));
			hci_read_remote_name(testSocket, &(info + i)->bdaddr, sizeof(name), name, 0);

			//			if(strcmp(name, "OBDII")) {
			address = (info + i)->bdaddr;
			//				break;
			//			}
		}

		free(info);
		close(testSocket);

		// if(address == NULL) throw no_obdii_found_error

		return address;
	}

	void BluetoothConnection::Open(bdaddr_t addr) {
		WriteSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);  // socket setup	

		// Connection parameters
		struct sockaddr_rc saddr = { 0 };
		saddr.rc_family = AF_BLUETOOTH;
		saddr.rc_channel = (uint8_t)1;
		saddr.rc_bdaddr = addr;

		// Make connection
		int status = connect(WriteSocket, (struct sockaddr*)&saddr, sizeof(saddr));

		// if(status < 0) throw bluetooth_socket_error
		int bob = 0;
	}

	uint8_t* BluetoothConnection::Send(string msg) {
		uint8_t* v;
		return v;
	}
}

#endif
