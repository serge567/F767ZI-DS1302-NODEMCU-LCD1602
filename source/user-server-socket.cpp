#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "globalvars.h"

#define IP      "192.168.1.181"
#define GATEWAY "192.168.1.181"
#define NETMASK "255.255.255.0"
#define PORT    80

EthernetInterface*  net;

TCPSocket           server;
TCPSocket*          clientSocket;
SocketAddress       clientAddress;
char                rxBuf[512] = { 0 };
char                txBuf[512] = { 0 };

int userserversock(void)
{
    printf("Starting\r\n");

    net = new EthernetInterface;

    if (!net) {
        printf("Error! No network inteface found.\n");
        return 0;
    }

    net->set_network (IP, NETMASK, GATEWAY);  // include to use static IP address
    nsapi_size_or_error_t   r = net->connect();
    if (r != 0) {
        printf("Error! net->connect() returned: %d\n", r);
        return r;
    }

    // Show the network address
    SocketAddress   ip;
    SocketAddress   netmask;
    SocketAddress   gateway;

    net->get_ip_address(&ip);
    net->get_netmask(&netmask);
    net->get_gateway(&gateway);

    ip.set_port(PORT);

    const char*     ipAddr = ip.get_ip_address();
    const char*     netmaskAddr = netmask.get_ip_address();
    const char*     gatewayAddr = gateway.get_ip_address();

    printf("IP address: %s\r\n", ipAddr ? ipAddr : "None");
    printf("Netmask: %s\r\n", netmaskAddr ? netmaskAddr : "None");
    printf("Gateway: %s\r\n\r\n", gatewayAddr ? gatewayAddr : "None");

    /* Open the server on ethernet stack */
    server.open(net);

    /* Bind the HTTP port (TCP 80) to the server */
    server.bind(ip);

    /* Can handle 1 simultaneous connections */
    server.listen(1);

    //listening for http GET request
    while (true) {
        printf("Listening GET request from client browser...\r\n");

        nsapi_error_t   error = 0;

        clientSocket = server.accept(&error);
        
        if (error != 0) {
            printf("Connection failed!\r\n");
        }
        else {
            //clientSocket->set_timeout(200);
            clientSocket->getpeername(&clientAddress);
            printf("Client with IP address %s connected.\r\n\r\n", clientAddress.get_ip_address());
            error = clientSocket->recv(rxBuf, sizeof(rxBuf));
            
            switch (error) {
                case 0:
                    printf("Recieved buffer is empty.\r\n");
                    break;

                case -1:
                    printf("Failed to read data from client.\r\n");
                    break;

                default:
                    printf("Recieved Data: %d\n\r\n\r%.*s\r\n\n\r", strlen(rxBuf), strlen(rxBuf), rxBuf);
                    if (rxBuf[0] == 'G' && rxBuf[1] == 'E' && rxBuf[2] == 'T') {
                        //setup http response header & data
                     //   sprintf
                     //   (
                    //        txBuf,
                     //       "HTTP/1.1 200 OK\nContent-Length: %d\r\nContent-Type: text\r\nConnection: Close\r\n\r\n",
                      //      strlen(rxBuf)
                     //   );
                      //  strcat(txBuf, rxBuf);
                       
                       sprintf(txBuf, "Date and time: %s, %s, %d, %d  %d:%d:%d .\r\n",
                            messdateandtime.weekday.c_str(), 
                            messdateandtime.smonth.c_str(), 
                            messdateandtime.day, 
                            messdateandtime.year + 2000, 
                            messdateandtime.hr, 
                            messdateandtime.minu, 
                            messdateandtime.sec); 
                        clientSocket->send(txBuf, strlen(txBuf)); 

                        // printf("------------------------------------\r\n");
                        // printf("Sent:\r\n%s\r\n", txBuf);
                        printf("echo done.\r\n");
                    }
                    break;
            }
        }

        clientSocket->close();
        printf("Client socket closed\r\n");
    }
}
