#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include <iostream>
#include<fstream>
using namespace std;

struct Param{          //线程传入多个参数
    int conn;
    MYSQL mysql;
};

//十六进制转2进制
char *htob(string s);

//十六进制转十进制;
char *htod(string s,int f); 

//十六进制转ip
char *htoip(string s);

//终端注册数据库操作
bool Terminal_Register(const char *identity,const char *ip,const char *port,MYSQL mysql);

//终端注销数据库操作
bool Terminal_down(const char *identity,MYSQL mysql);

//负氧离子数据入数据库
bool Data_query(char *num,char *name,const char *data,const char *identity,const char *time,MYSQL mysql);

//解析阳光协议数据
bool DataPacket_decode(string s,const char *identity,MYSQL mysql);

//十六进制数组转字节流
int Change16tobit(char *bit,char *s);

/*将recv函数自动转换的 中文字符数组 转换成 16进制字符串格式的字符数据*/
void Changestr_16_2(char *t,char *s,int len);

//无效的命令或数据
void badData(const char *identity,int conn);

//解码
void Decode(string s0,int conn);


