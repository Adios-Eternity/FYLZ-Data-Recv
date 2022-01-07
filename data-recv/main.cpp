#include "function.cpp"

void *pthread_main(void *arg)
{
    pthread_detach(pthread_self());    
    struct Param *param;
    param = (struct Param *)arg;
    int conn=(*param).conn;
    MYSQL mysql=(*param).mysql;
    char buffer[1024];
    int len = recv(conn, buffer, sizeof(buffer), 0);
    while(len!=0)        //循环接收数据
    {
        string s0="";
        if(len==22)           //终端注册(心跳包)
        {
            char t[1024];
            Changestr_16_2(t,buffer,len);      //将buffer数组中接收到的二进制比特流转换为十六进制
            for(int i=0;i<strlen(t);i++)
                s0+=t[i];
            Decode(s0,conn,mysql);
        }
        else if(len>22)         //数据包
        {
            string s="";    //用于存储 二进制字节流转换成十六进制字符串
            for(int i=0;i<len;i++)
            {
                char flag[10];     
                sprintf(flag,"%X",0XFF&buffer[i]);    //buffer中每个字节转换成十六进制
                if(strlen(flag)==1)
                    s+='0';
                s+=flag;
            }
            s0=s.substr(0,30);
            for(int i=30;i<s.length()-2;i+=2)       //每两个十六进制数(一个字节)转换成ascii码
            {
                string s1=s.substr(i,2);
                char *data=htod(s1,0);
                int d=atoi(data);
                s0+=d;
            }
            s0+=s.substr(s.length()-2,2);
            Decode(s0,conn,mysql);
        }
        memset(buffer, 0 ,sizeof(buffer));
        len = recv(conn, buffer, sizeof(buffer), 0);
    }
    pthread_exit(0);
}


int main()
{

    MYSQL mysql;
    if(mysql_init(&mysql)==NULL){
        cout <<"mysql_init():"<<mysql_error(&mysql);
    }
    if(mysql_real_connect(&mysql,"localhost","root","!root1234321","socket",0,NULL,0)==NULL)
    {
        cout <<"mysql_real_connect():"<< mysql_error(&mysql)<<endl; 
    }
    else
    {
        cout <<"Connected MySQL successful!"<<endl;
        mysql_query(&mysql, "set names 'utf8'");
    }
    int i,maxi,maxfd,listenfd,connfd,sockfd;
    int nready,client[FD_SETSIZE];
    ssize_t n;
    fd_set rset,allset;
    pthread_t t_id;     
    char buf[1024];
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port=htons(8276);
    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)
    {
        printf("bind error!\n");
        exit(1);
    }
    if(listen(listenfd,10)==-1)
    {
        printf("listen error!\n");
        exit(1);
    }
    maxfd=listenfd;
    maxi=-1;
    for(i=0;i<FD_SETSIZE;i++)
        client[i]=-1;
    FD_ZERO(&allset);
    FD_SET(listenfd,&allset);
    for(; ;)
    {
        rset=allset;
        nready=select(maxfd+1,&rset,NULL,NULL,NULL);
        if(FD_ISSET(listenfd,&rset))
        {
            clilen=sizeof(cliaddr);
            connfd=accept(listenfd,(struct sockaddr *) &cliaddr,&clilen);
            for(i=0;i<FD_SETSIZE;i++)
                if(client[i]<0)
                {
                    client[i]=connfd;
                    break;
                }
            if(i==FD_SETSIZE)
                perror("too many clients");
            FD_SET(connfd,&allset);
            if(connfd>maxfd)
                maxfd=connfd;
            if(i>maxi)
                maxi=i;
            if(--nready<=0)
                continue;
        }
        for(i=0;i<=maxi;i++){
            if((sockfd=client[i])<0)
                continue;
            if(FD_ISSET(sockfd,&rset)){    //某个sockfd就绪
                struct Param param;
                param.conn=sockfd;
                param.mysql=mysql;
                pthread_create(&t_id,NULL,pthread_main,(void *)&param);  //创建新的线程处理
                if(--nready<=0)
                    break;
            }
        }
    }
    return 0;
    //2605
}