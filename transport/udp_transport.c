# include"microros_xr806.h"


#if defined MICROROS_UDP

#define REMOTE_IP	 "192.168.6.7"
#define PORT	     8080

#define micro_rollover_useconds 4294967295


static int sock;
static struct sockaddr_in server_addr;


int clock_gettime(clockid_t unused, struct timespec *tp)
{
    (void)unused;
    static uint32_t rollover = 0;
    static uint32_t last_measure = 0;

    uint32_t m = OS_GetTicks() * 1000;
    

    rollover += (m < last_measure) ? 1 : 0;

    uint64_t real_us = (uint64_t) (m + rollover * micro_rollover_useconds);

    tp->tv_sec = real_us / 1000000;
    tp->tv_nsec = (real_us % 1000000) * 1000;
    last_measure = m;
    return 0;
}

/*UDP初始化*/
int16_t udpclient_init(){
	/*创建一个socket*/
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		printf("Socket error\n");
		return -1;
	}
	 /* 初始化预连接的服务端地址 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(REMOTE_IP);
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
	/*连接*/
	if(connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("Connect <%d> fail!\n", sock);
        return -2;
	}
    printf("Connect %d sucessful!\n", sock);
  
	return 1;
}

bool udp_transport_open(uxrCustomTransport* transport){

    if(udpclient_init())
    {
        printf("open udp successful!\n");
        return 1;
    }
    else{
        printf("open udp fail!\n");
        printf("remote IP: %s/t port: %d!", REMOTE_IP, PORT);
        return 0;
    }      
}

bool udp_transport_close(uxrCustomTransport* transport){

    return closesocket(sock);
}

size_t udp_transport_write(uxrCustomTransport* transport,const uint8_t* buffer,
                            size_t length,uint8_t* errcode){

    size_t rv = 0;
    ssize_t bytes_sent = send(sock, (void*)buffer, length, 0);
    if (-1 != bytes_sent)
    {
        rv = (size_t)bytes_sent;
        *errcode = 0;
    }
    else
    {
        *errcode = 1;
    }
    return rv;
}

size_t udp_transport_read(uxrCustomTransport* transport,uint8_t* buffer,
                            size_t length,int timeout,uint8_t* errcode){
    
    size_t rv = 0;
    
    timeout = (timeout <= 0) ? 1 : timeout;

    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    select(sock+1, &readfds, NULL, NULL, &tv);
    
    if(FD_ISSET(sock, &readfds))
    {
        size_t bytes_received = recv(sock, (void*)buffer, length, 0);
        if (-1 != bytes_received)
        {
            rv = (size_t)bytes_received;
            *errcode = 0;
        }
        else
        {
            *errcode = 1;
        }
    }
    else{
        *errcode = 1;
        return 0;
    }

    return rv;
}
#endif
