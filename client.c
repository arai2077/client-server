#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define SERVER "127.0.0.1" // Serverin osoite, tässä tapauksessa localhost
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
    struct sockaddr_in serv_addr;
    int sockfd, sock_len=sizeof(serv_addr);
    char buffer[BUFLEN];
    char message[BUFLEN];
 
    // Luodaan uusi UDP-protokollaa käyttävä socketti
    // Palautetaan virhe jos luonti epäonnistuu
    if ( (sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        error("Virhe socketia luodessa");
    }
 
    // Alustetaan bufferi nollaksi, socketin tiedot talteen
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Yhdistetään serveriin
    if (inet_aton(SERVER , &serv_addr.sin_addr) == 0) {
        error("Virhe yhdistettäessä serveriin");
    }
 
    while(1)
    {
        // Kysellään käyttäjältä viestiä
        printf("Syötä viesti: ");
        gets(message);
         
        // Lähetetään viesti
        if (sendto(sockfd, message, strlen(message) , 0 , (struct sockaddr *) &serv_addr, sock_len)==-1) {
            error("Virhe viestiä lähetettäessä");
        }
         
        // Tyhjennetään bufferi
        memset(buffer,'\0', BUFLEN);

        // Yritetään lukea socketista
        if (recvfrom(sockfd, buffer, BUFLEN, 0, (struct sockaddr *) &serv_addr, &sock_len) == -1) {
            error("Virhe lukiessa socketista");
        }
         
        puts(buffer);
    }
 
    close(sockfd);
    return 0;
}
