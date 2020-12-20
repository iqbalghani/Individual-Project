#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define NTP_TIMESTAMP_DELTA 2208988800ull

#define LI(packet)   (uint8_t) ((packet.li_vn_mode & 0xC0) >> 6) 
#define VN(packet)   (uint8_t) ((packet.li_vn_mode & 0x38) >> 3) 
#define MODE(packet) (uint8_t) ((packet.li_vn_mode & 0x07) >> 0) 

void error( char* msg )
{
    perror( msg ); // Print the error message to stderr.

    exit( 0 ); // Quit the process.
}

int main( int argc, char* argv[ ] )
{
  int sockfd, n; // Socket file descriptor 

  int portno = 123; // NTP UDP port number.

  char* host_name = "192.168.56.103"; // NTP server host-name.

 

  typedef struct
  {

    uint8_t li_vn_mode;      

    uint8_t stratum;         
    uint8_t poll;            
    uint8_t precision;       

    uint32_t rootDelay;      
    uint32_t rootDispersion; 
    uint32_t refId;          

    uint32_t refTm_s;        
    uint32_t refTm_f;        

    uint32_t origTm_s;       
    uint32_t origTm_f;       

    uint32_t rxTm_s;         
    uint32_t rxTm_f;         
    uint32_t txTm_s;         
    uint32_t txTm_f;         

  } ntp_packet;            


  ntp_packet packet = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  memset( &packet, 0, sizeof( ntp_packet ) );


  *( ( char * ) &packet + 0 ) = 0x1b;


  struct sockaddr_in serv_addr; // Server address data structure.
  struct hostent *server;      // Server data structure.

  sockfd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.

  if ( sockfd < 0 )
    error( "ERROR opening socket" );

  server = gethostbyname( host_name ); 

  if ( server == NULL )
    error( "ERROR, no such host" );


  bzero( ( char* ) &serv_addr, sizeof( serv_addr ) );

  serv_addr.sin_family = AF_INET;

  // Copy the server's IP address to the server address structure.

  bcopy( ( char* )server->h_addr, ( char* ) &serv_addr.sin_addr.s_addr, server->h_length );



  serv_addr.sin_port = htons( portno );

  // Call up the server using its IP address and port number.

  if ( connect( sockfd, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr) ) < 0 )
    error( "ERROR connecting" );

  // Send it the NTP packet it wants. If n == -1, it failed.

  n = write( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );

  if ( n < 0 )
    error( "ERROR writing to socket" );

  // Wait and receive the packet back from the server. If n == -1, it failed.

  n = read( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );

  if ( n < 0 )
    error( "ERROR reading from socket" );


  packet.txTm_s = ntohl( packet.txTm_s ); // Time-stamp seconds.
  packet.txTm_f = ntohl( packet.txTm_f ); // Time-stamp fraction of a second.

 

  time_t txTm = ( time_t ) ( packet.txTm_s - NTP_TIMESTAMP_DELTA );

  // Print the time we got from the server, accounting for local timezone and conversion from UTC time.

  printf( "Time: %s", ctime( ( const time_t* ) &txTm ) );

  return 0;
}
