//
// Created by x on 3/1/21.
//

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>

void msgCallback(const geometry_msgs::PoseStamped::ConstPtr &P) {
    float f[3];
    f[0] = P->pose.position.x;
    f[1] = P->pose.position.y;
    f[2] = P->pose.position.z;
    ROS_INFO("I recevied the topic: %f %f %f",f[0],f[1],f[2]);
}

void *udp_get( void *arg )
{
    int port = 7999;
    int sockfd;

    // 创建socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1==sockfd){
        puts("Failed to create socket");
    }

    // 设置地址与端口
    struct sockaddr_in addr;
    socklen_t  addr_len=sizeof(addr);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;       // Use IPV4
    addr.sin_port   = htons(port);    //
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Time out
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 200000;  // 200 ms
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));

    int counter = 0;

    while(ros::ok())
    {
        sendto(sockfd, "hello world", 11, 0, (sockaddr*)&addr, addr_len);
        printf("Sended %d\n", ++counter);
        sleep(1);
    }
    close(sockfd);
}
void create_all_thread(void)
{
    pthread_t thread_udp_get;//声明线程ID
    if( (pthread_create( &thread_udp_get , NULL , udp_get, NULL )) != 0 )//创建线程
    {
        perror("Create the thread_joy fail");
        exit( 1 );
    }
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"send_pos");
    ros::NodeHandle nh;
    ros::Subscriber msg_sub = nh.subscribe("mavros/local_position/pose", 1000, msgCallback);
    create_all_thread();
    ros::spin();

    return 0;
}