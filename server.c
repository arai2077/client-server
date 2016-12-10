#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512 // Bufferin maksimipituus
#define PORT 8081 // Käytettävä portti
 
// Virhetilanteita varten virheviesti
void error(char *s)
{
    perror(s);
    exit(1);
}
 
int main(void)
{

    // Socketin tiedot ja bufferi
    struct sockaddr_in serv_addr, cli_addr;
    int sockfd, cli_len = sizeof(cli_addr), recv_len;
    char buffer[BUFLEN];
     
    // Luodaan uusi UDP-protokollaa käyttävä socketti
    // Palautetaan virhe jos luonti epäonnistuu
    if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        error("Virhe avattaessa socketia");
    }
     
    // Alustetaan bufferi nollaksi
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    
    // Alustetaan socketin tiedot
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     
    // Bindataan socketti porttiin, erroria jos failaa
    if( bind(sockfd , (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1) {
        error("Virhe bindatessa");
    }

    // Kuunnellaan yhteyksiä
    while(1)
    {
        printf("Odotellaan dataa...\n");
        fflush(stdout);
         
        // Yritetään lukea socketista, virhe jos feilaa
        if ((recv_len = recvfrom(sockfd, buffer, BUFLEN, 0, (struct sockaddr *) &cli_addr, &cli_len)) == -1) {
            error("Virhe lukiessa socketista");
        }
         
        // Tulostetaan viestin sisältö
        printf("Vastaanotettu data: %s\n" , buffer);
         
        // Vastataan clientille, virhe jos feilaa
	int lotnum = "12345";
        if (sendto(sockfd, lotnum, sizeof(lotnum), 0, (struct sockaddr*) &cli_addr, cli_len) == -1) {
            error("Virhe kirjoitettaessa sockettiin");
        }
    }
 
    close(sockfd);
    return 0;
}
