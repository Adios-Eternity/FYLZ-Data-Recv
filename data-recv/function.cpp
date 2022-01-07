#include "Head.h"

//十六进制转二进制
char *htob(string s)
{
    char result[100];
    strcpy(result,"");
    char b[16][5]={"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    for(int i=0;i<s.length();i++)
    {
        switch (s[i])
        {
        case 'A':
            strcat(result,b[10]);
            break;
        case 'B':
            strcat(result,b[11]);
            break;
        case 'C':
            strcat(result,b[12]);
            break;
        case 'D':
            strcat(result,b[13]);
            break;
        case 'E':
            strcat(result,b[14]);
            break;
        case 'F':
            strcat(result,b[15]);
            break;
        default:
            strcat(result,b[s[i]-'0']);
            break;
        }
    }
    char *re=result;
    return re;
}

//十六进制转十进制   f为标志位，0：无符号十六进制转换  1：有符号十六进制转换
char *htod(string s,int f)
{
    int result=0;
    char b[100];
    strcpy(b,htob(s));
    //无符号十六进制
    if(f==0) 
    {
        for(int i=s.length()-1;i>=0;i--)
        {
            int l=0;
            switch (s[i])
            {
            case 'A':
                l=10;
                break;
            case 'B':
                l=11;
                break;
            case 'C':
                l=12;
                break;
            case 'D':
                l=13;
                break;
            case 'E':
                l=14;
                break;
            case 'F':
                l=15;
                break;
            default:
                l=s[i]-'0';
                break;
            }
        result += l*pow(16,s.length()-1-i);
        }
    }
    //有符号十六进制
    else{
        if(b[0]=='0')     //正数
        {
            for(int i=s.length()-1;i>=0;i--)
            {
                int l=0;
                switch (s[i])
                {
                case 'A':
                    l=10;
                    break;
                case 'B':
                    l=11;
                    break;
                case 'C':
                    l=12;
                    break;
                case 'D':
                    l=13;
                    break;
                case 'E':
                    l=14;
                    break;
                case 'F':
                    l=15;
                    break;
                default:
                    l=s[i]-'0';
                    break;
                }
                result += l*pow(16,s.length()-1-i);
            }
        }
        else{               //负数
            for(int i=1;i<strlen(b);i++)  //取反
            {
                if(b[i]=='1')
                    b[i]='0';
                    else
                    b[i]='1';
            }
            for(int i=strlen(b)-1;i>=0;i--)  //加一
            {
                if(i==0&&b[i]==1)
                {
                    for(int i=strlen(b);i>0;i--)
                        b[i]=b[i-1];
                    b[0]='1';
                }

                if(b[i]=='0')
                {
                    b[i]='1';
                    break;
                }
                else
                    b[i]='0';
            }
            for(int i=strlen(b)-1;i>0;i--)        //带符号的二进制转化为十进制
                result += (b[i]-'0')*pow(2,strlen(b)-1-i);
            result =0-result;
        }
    }
    char num[100];
    sprintf(num,"%d",result);
    char *Num=num;
    return Num;
}

//十六进制转ip
char *htoip(string s)
{
    char ip[20];
    strcpy(ip,"");
    strcat(ip,(char *)htod(s.substr(0,2),0));
    strcat(ip,(char *)".");
    strcat(ip,(char *)htod(s.substr(2,2),0));
    strcat(ip,(char *)".");
    strcat(ip,(char *)htod(s.substr(4,2),0));
    strcat(ip,(char *)".");
    strcat(ip,(char *)htod(s.substr(6,2),0));
    char *result=ip;
    return result;
}

//终端注册 (插入数据库使用，现在输入到文本文件，暂时不用)
bool Terminal_Register(const char *identity,const char *ip,const char *port,MYSQL mysql)//终端注册   
{
    
    char q[1024];
	strcpy(q,"insert into httprequest(identity,ip,port) values('");
    strcat(q,(char *)identity);
    strcat(q,(char *)"','");
    strcat(q,(char *)ip);
    strcat(q,(char *)"','");
    strcat(q,(char *)port);
    strcat(q,(char *)"')");
    int rc=mysql_real_query(&mysql,q,strlen(q));
    if(rc!=0)
    {
        cout <<"mysql_real_query():"<< mysql_error(&mysql);
        return false;
    }
    return true;
}

//终端注销  (插入数据库使用，现在输入到文本文件，暂时不用)
bool Terminal_down(const char *identity,MYSQL mysql)
{
    char p[1025];
    strcpy(p,"delete from httprequest where identity='");
    strcat(p,(char *)identity);
    strcat(p,(char *)"'");
    int rc=mysql_real_query(&mysql,p,strlen(p));
    if(rc!=0)
    {
        cout <<"mysql_real_query():"<< mysql_error(&mysql);
        return false;
    }
    return true;
}

//负氧离子数据插入  (插入数据库使用，现在输入到文本文件，暂时不用)
bool Data_query(char *num,char *name,const char *data,const char *identity,const char *time,MYSQL mysql)
{
    char q[1024];
	strcpy(q,"insert into data(name,truedata,time,identity,device_num) values('");
    strcat(q,(char *)name);
    strcat(q,(char *)"',");
    strcat(q,(char *)data);
    strcat(q,(char *)",'");
    strcat(q,(char *)time);
    strcat(q,(char *)"','");
    strcat(q,(char *)identity);
    strcat(q,(char *)"','");
    strcat(q,(char *)num);
    strcat(q,(char *)"')");
    printf("insert:%s\n",q);
    int rc=mysql_real_query(&mysql,q,strlen(q));
    if(rc!=0)
    {
        cout <<"mysql_real_query():"<< mysql_error(&mysql)<<endl;
        return false;
    }
    return true;
}

//阳光协议数据包解码
bool DataPacket_decode(string s, const char *identity, MYSQL mysql)
{
    char name[40][50]={"辐射1 日累计","辐射2日累计","地温1","地温2","地温3","地温4","地温5","环温","环湿","露点","气压","海拔","风速","2分钟平均风速","10分钟平均风速","风向","辐射1","辐射2","土湿","电量","雨量日累计","能见度","能见度10分钟平均","日照时日累计","CO2","电子罗盘","PM2.5","PM10","噪声","光照","二氧化硫SO2","臭氧O3","一氧化碳CO","硫化氢H2S","氨气NH3","一氧化氮NO","二氧化氮NO2","氧气O2","fylz"};
    int dt[40]={0,0,1,1,1,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    double jingdu[40]={0.001,0.001,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.01,0.1,1,0.1,0.1,0.1,1,1,1,0.1,0.1,0.1,1,1,1,0.1,1,0.001,0.001,0.1,10,0.1,1,0.1,0.1,0.1,0.1,1,0.1,1};
    string num=s.substr(1,4);    //记录仪编号
    string gncode=s.substr(5,4);    //功能码
    if(gncode=="030C")
    {
        string time=s.substr(9,12);
        char t[100];
        strcpy(t,"");
        strcat(t,time.substr(0,2).c_str());
        strcat(t,(char *)":");
        strcat(t,time.substr(2,2).c_str());
        strcat(t,(char *)":");
        strcat(t,time.substr(4,2).c_str());
        strcat(t,(char *)":");
        strcat(t,time.substr(6,2).c_str());
        strcat(t,(char *)":");
        strcat(t,time.substr(8,2).c_str());
        strcat(t,(char *)":");
        strcat(t,time.substr(10,2).c_str());
        int j=0;
        string d=s.substr(s.length()-10,8);
        char truedata[100];      //十六进制转换成十进制的数据
        strcpy(truedata,htod(d,0));
        int n=0;
        for(int m=strlen(truedata)-1;m>=0;m--)
        {
            if(m==0&&truedata[m]=='-')
            {
                n=0-n;
                break;
            }
            n+=(truedata[m]-'0')*pow(10,strlen(truedata)-1-m);
        }
        double Num=(double)n;
        char tdata[100];
        sprintf(tdata,"%f",Num);
        ofstream ofs("/Users/ryangee/Desktop/fylz/data.txt", ios::app);
        if(ofs.is_open())
        {
            string NAME=name[38],Tdata=tdata,iden=identity,TIME=t,device_num=htod(num,0);
            ofs<<NAME<<"\t"<<Tdata<<"\t"<<iden<<"\t"<<TIME<<"\t"<<device_num<<endl;
            ofs.close();
            return true;
        }
        else
        {
            cout <<"open data error!"<<endl;
            return false;
        }


        //数据库操作
        if(!Data_query(htod(num,0),name[38],tdata,identity,t,mysql))
            return false;
        else 
            return true;
        // for(int i=0;i<data.length();i+=4)
        // {
        //     string d;
        //     if(i>=152)
        //     {
        //         d=data.substr(i,8);
        //         i+=8;
        //     }
        //     else
        //         d=data.substr(i,4);
        //     cout <<"d:"<<d<<endl;
        //     char truedata[100];    //十进制数据
        //     strcpy(truedata,htod(d,dt[j]));
        //     int n=0;
        //     for(int m=strlen(truedata)-1;m>=0;m--)
        //     {
        //         if(m==0&&truedata[m]=='-')
        //         {
        //             n=0-n;
        //             break;
        //         }
        //         n+=(truedata[m]-'0')*pow(10,strlen(truedata)-1-m);
        //     }
        //     double Num=(double)n*jingdu[j];
        //     cout <<"jingdu:"<<jingdu[j]<<" Num:"<<Num<<endl;
        //     char tdata[100];
        //     sprintf(tdata,"%f",Num);
        //     //十进制时间
        //     printf("truedata:%s,time:%s\n",truedata,t);
        //     printf("name[j]:%s,tdata:%s,identity:%s\n",name[j],tdata,identity);
        //     int rc=Data_query(htod(num,0),name[j],tdata,identity,t,mysql);
        //     if(rc==0)
        //         return;
        //     j++;
        // }
    }
    else
        return false;
    return true;
}

//十六进制数组转二进制字节流
int Change16tobit(char *bit,char *s)
{
    int n=0;
    for(int i=0;s[i];i+=2)
    {
        if(s[i]>='A'&&s[i]<='F')
            bit[n]=s[i]-'A'+10;
        else
            bit[n]=s[i]-'0';
        if(s[i+1]>='A'&&s[i+1]<='F')
            bit[n]=(bit[n]<<4)|(s[i+1]-'A'+10);
        else
            bit[n]=(bit[n]<<4)|(s[i+1]-'0');
        ++n;
    }
    return n;
}

/*将recv函数接收的数组 自动转换的 中文字符数组 转换成 16进制字符串格式的字符数据*/
void Changestr_16_2(char *t,char *s,int len) {
    char i,chs;
    for(i = 0; i<len; ++i) {
        chs = s[i] & 0X0F;
        if(chs > 9) t[2 * i + 1] = chs - 10 + 'A';
        else t[2 * i + 1] = chs + '0';
        chs = (s[i] & 0XF0) >> 4;
        if(chs > 9) t[2 * i] = chs - 10 + 'A';
        else t[2 * i] = chs + '0';
    }
    t[2 * i] = '\0';
}

//无效的命令或数据
void badData(const char *identity,int conn)
{
    char senddata[1024];
    strcpy(senddata,"7B840010");
    strcat(senddata,identity);
    strcat(senddata,"7B");
    char sd[1024];
    int n=Change16tobit(sd,senddata);
    send(conn,sd,n,0);
}

//解码
void Decode(string s0, int conn, MYSQL mysql)
{
    if(s0.length()<32)
        return;
    string Type=s0.substr(2,2),len=s0.substr(4,4),identity=s0.substr(8,22);
    if(s0.substr(0,2)!="7B")     //如果不是以7B开头发送无效包
    {
        badData(identity.c_str(),conn);
        return;
    }
    char senddata[1024];
    if(Type=="01")    //终端请求注册(心跳包)
    {
        if(len!="0016"&&s0.length()!=44)    //长度不匹配
        {
            badData(identity.c_str(),conn);
            return;
        }
        string ip=s0.substr(30,8),port=s0.substr(38,4);   //ip、端口号
        char dip[20],dport[20];
        strcpy(dip,htoip(ip));
        strcpy(dport,htod(port,0));
        //先查看文件中是否已经存在该心跳包，如存在则忽略，不存在则存入
        ifstream ifs("/Users/ryangee/Desktop/fylz/httprequest.txt");
        if(ifs.is_open())
        {
            string inw="";
            inw+=(identity+"\t");
            inw+=dip;
            inw+="\t";
            inw+=dport;
            char buffer[1024];
            while(!ifs.eof())
            {
                ifs.getline(buffer,1024);
                if(inw==buffer)
                {
                    ifs.close();
                    return;
                }
            }
            ifs.close();
            ofstream ofs("/Users/ryangee/Desktop/fylz/httprequest.txt", ios::app);
            if(ofs.is_open())
            {
                ofs <<inw<<endl;
                ofs.close();
            }
            else{
                cout <<"open httprequest file error!"<<endl;
                return;
            }
        }
        else
        {
            cout <<"open htprequest file error"<<endl;
            return;
        }
        if(!Terminal_Register(identity.c_str(),dip,dport,mysql))
        {
            badData(identity.c_str(),conn);
            return;
        }
        strcpy(senddata,"7B810010");
        strcat(senddata,identity.c_str());
        strcat(senddata,"7B");
        char sdbit[1024];
        int n=Change16tobit(sdbit,senddata);
        send(conn,sdbit,n,0);
    }
    else if(Type=="02")  //终端请求注销
    {
        if(len!="0010"&&s0.length()!=32)
        {
            badData(identity.c_str(),conn);
            return ;
        }
        strcpy(senddata,"7B820010");
        strcat(senddata,identity.c_str());
        strcat(senddata,"7B");
        if(!Terminal_down(identity.c_str(),mysql))
        {
            badData(identity.c_str(),conn);
            return;
        }
        char sdbit[1024];
        int n=Change16tobit(sdbit,senddata);
        send(conn,sdbit,n,0);
    }
    else if(Type=="05")//接收到dsc用户数据的应答包
    {
    }
    else if(Type=="09")//负氧离子数据包
    {
        if(atoi(htod(len,0))*2<s0.length())   //长度不匹配
        {
            badData(identity.c_str(),conn);
            return ;
        }
        string pack_data=s0.substr(30,s0.length()-32);   //分理处 数据包部分
        if(!DataPacket_decode(pack_data,identity.c_str(),mysql))
        {
            badData(identity.c_str(),conn);
            return;
        }
        strcpy(senddata,"7B850010");
        strcat(senddata,identity.c_str());
        strcat(senddata,"7B");
        char sdbit[1024];
        int n=Change16tobit(sdbit,senddata);
        send(conn,sdbit,n,0);
    }
    else
        badData(identity.c_str(),conn);
}