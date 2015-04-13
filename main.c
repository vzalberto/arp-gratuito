#include<sys/types.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<net/if.h>
#include<net/ethernet.h>
#include<net/if_arp.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#define INTERFAZ "eth4"

#define OTRA "\n%02X:%02X:%02X:%02X:%02X:%02X quiere saber que onda con %d.%d.%d.%d\n"
#define INFO "\nMAC ORIGEN: %02X:%02X:%02X:%02X:%02X:%02X\nMAC DESTINO: %02X:%02X:%02X:%02X:%02X:%02X\nIP ORIGEN: %d.%d.%d.%d\nIP DESTINO: %d.%d.%d.%d"

struct msgARP{
	unsigned char destinoEthernet[6];
	unsigned char origenEthernet[6];
	unsigned short tipoEthernet;
	unsigned short tipoHardware;
	unsigned short protocolo;
	unsigned char longitudMAC;
	unsigned char longitudRed;
	unsigned short tipoARP;
	unsigned char origenMAC[6];
	unsigned char origenIP[4];
	unsigned char destinoMAC[6];
	unsigned char destinoIP[4];
};

int estructuraGratuito(unsigned char * mac, unsigned char * ip)
{
	int sd, optval;
	struct sockaddr sa;
	struct msgARP msg;
	struct ifreq ifr;

	memset(&msg.destinoEthernet, 0xff, 6);
	msg.tipoEthernet = htons(ETH_P_ARP);
	msg.tipoHardware = htons(ARPHRD_ETHER);
	msg.protocolo = htons(ETH_P_IP);
	msg.longitudMAC = 6;
	msg.longitudMAC = 6;
	msg.longitudRed = 4;
	msg.tipoARP = htons(ARPOP_REPLY);
	memcpy(&msg.origenMAC, mac, 6);
	memcpy(&msg.origenIP, ip, 4);
	memcpy(&msg.destinoMAC, mac, 6);
	memcpy(&msg.origenIP, ip, 4);

	if((sd = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP))) < 0)
		{
			perror("ERROR al abrir socket");
			return -1;
		}

	if(setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0) 
	{
		perror("setsockopt");
		return -1;
	}

	strcpy(ifr.ifr_name, INTERFAZ);
	if(ioctl(sd, SIOCGIFHWADDR, &ifr) < 0)
		{
			perror("ERROR al obtener MAC origen");
			return -1;
		}
	memcpy(&msg.origenEthernet, &ifr.ifr_hwaddr.sa_data, 6);

	bzero(&sa, sizeof(sa));
	strcpy(sa.sa_data, INTERFAZ);

	if(sendto(sd, &msg, sizeof(msg), 0, (struct sockaddr *) &sa, sizeof(sa)) < 0)
	{
	perror("ERROR al enviar ARP gratuito");
	return -1;
	}
	printf("\nARP gratuito enviado\n");
	return 0;
}

int main(int argc, char *argv[]){
	struct msgARP msg;
	struct ifreq ifr;
	struct sockaddr sa;
	int s, optval, n;
	unsigned char ip[4];

	if((s = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP))) < 0)
		{
			perror("ERROR al abrir socket");
			return -1;
		}

	if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0) 
		{
			perror("setsockopt");
			return -1;
		}

//Recepcion
printf("\nESCUCHANDO\n");
	do{
		bzero(&sa, sizeof(sa));
		bzero(&msg, sizeof(msg));
		n = sizeof(sa);
		if(recvfrom(s, &msg, sizeof(msg), 0, (struct sockaddr *) &sa, &n) < 0)
			{
				perror("ERROR al recibir");
				return -1;
			}

//		if((ntohs(msg.tipoARP) == ARPOP_REQUEST) && (msg.origenIP[3] != 0x01))
//			{

			 fprintf(stdout, INFO,
					msg.origenMAC[0], msg.origenMAC[1], msg.origenMAC[2], msg.origenMAC[3],
					msg.origenMAC[4], msg.origenMAC[5], msg.destinoMAC[0],
					msg.destinoMAC[1],msg.destinoMAC[2],msg.destinoMAC[3],msg.destinoMAC[4],
					msg.destinoMAC[5], 
					msg.destinoIP[0], msg.destinoIP[1], msg.destinoIP[2], msg.destinoIP[3],
					msg.origenIP[0], msg.origenIP[1], msg.origenIP[2], msg.origenIP[3]);


				
				// AQUI SE ENCUENTRA UN ARP GRATUITO
				if( memcmp(&msg.origenIP, &msg.destinoIP, 4) == 0){
										
				/*

				estructuraMensaje(MAC EN BYTES, BROADCAST, );
					
				Respuesta ARP gratuito con los datos de la MAC definida

				*/

				printf("\nTe toca ARP gratuito, amigo\n");
				unsigned char macOrigen[6];
				printf("\nAHI TE VA\n");

				macOrigen[0] = 0x00;
				macOrigen[1] = 0xA0;
				macOrigen[2] = 0x24;
				macOrigen[3] = 0x24;
				macOrigen[4] = 0x49;
				macOrigen[5] = 0x4A;				

				
				if(estructuraGratuito(macOrigen, msg.destinoIP) != 0)
					printf("\nNEL NO SE PUDO ENVIAR EL GRATUITO, TE LO DEBO\n");

				else
					
				exit(0);				
									
				}
				printf("\n\n");
//			}


		}while(1);
	close(s);
}


