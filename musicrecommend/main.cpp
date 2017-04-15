#include <iostream>
#include <fstream>
using namespace std;
/*
 * Testidx2.txt
 * 用户编号|6
 * 推荐曲目1
 * 推荐曲目2
 * ......
 * 推荐曲目6
 * -------------------
 * Trainidx2.txt
 * 用户编号|评分项目项数
 * 项目1
 * 分数1
 * 项目2
 * 分数2
 * ......
 * -------------------
 * TrackData2.txt
 * track|album|artist|genre1|genre2|...
 */
double scoreaverage[296111]={0};//存每一项的平均分（track,album,artist,genre等）
int countitem[296111]={0};//存每一项被评分次数

class String //处理文件Testidx2.txt和Trainidx2.txt里每一行的文本
{
private:
    char* s; //文本
    int len; //文本长度
public:
    String(const char* str)
    {
        int i;
        for(i=0;str[i]!='\0';i++)
            ;
        len=i;
        s=new char[len];
        for(i=0;i<len;i++)
            s[i]=str[i];
    }
    String()
    {
        s=nullptr;
        len=0;
    }
    String(const String& org)
    {
        len=org.len;
        s=new char[len];
        for(int i=0;i<len;i++)
            s[i]=org.s[i];
    }
    String(String&& org)
    {
        len=org.len;
        s=org.s;
        org.s=nullptr;
    }
    ~String()
    {
        delete [] s;
    }
    String& operator=(const String& org)
    {
        if(this!=&org)
        {
            len=org.len;
            delete [] s;
            s=new char[len];
            for(int i=0;i<len;i++)
                s[i]=org.s[i];
        }
        return *this;
    }
    int Getuser()//user编号，字符型转整型（文本xxx|xxxx的前半部分）
    {
        int i;
        for(i=0;i<len;i++)
        {
            if(s[i]=='|')
                break;
        }
        int user=0;
        for(int j=0;j<i;j++)
            user=user*10+(s[j]-'0');
        return user;
    }
    int Getitemnum()//该user评分项目总数，字符型转整型 （文本xxx|xxxx的后半部分）
    {
        int i;
        for(i=0;i<len;i++)
        {
            if(s[i]=='|')
                break;
        }
        i++;
        int num=0;
        for(;i<len;i++)
            num=num*10+(s[i]-'0');
        return num;
    }
    int Getuseritem()//项目编号，字符型转整型
    {
        int item=0;
        for(int i=0;i<len;i++)
            item=item*10+(s[i]-'0');
        return item;
    }
};

class Testidx//处理testidx2里一个用户的数据
{
private:
    int user;//用户编号
    int* track;//推荐曲目
public:
    Testidx(String* a)//存用户编号，6首待推荐歌曲号
    {
        user=a[0].Getuser();
        track=new int [6];
        for(int i=0;i<6;i++)
            track[i]=a[i+1].Getuseritem();
    }
    Testidx()
    {
        user=0;
        track=nullptr;
    }
    Testidx(const Testidx& org)
    {
        user=org.user;
        track=new int [6];
        for(int i=0;i<6;i++)
            track[i]=org.track[i];
    }
    Testidx(Testidx&& org)
    {
        user=org.user;
        track=org.track;
        org.track=nullptr;
    }
    ~Testidx()
    {
        delete [] track;
    }
    Testidx& operator=(const Testidx& org)
    {
        if(this!=&org)
        {
            user=org.user;
            delete [] track;
            track=new int [6];
            for(int i=0;i<6;i++)
                track[i]=org.track[i];
        }
        return *this;
    }
    int& Getuser()
    {
        return user;
    }
    int*& Gettrack()
    {
        return track;
    }
};

class Trainidx//处理trainidx2里一个用户的数据
{
private:
    int user;//用户编号
    int num;//该用户评分项目数
    int* item;//该用户评分项目编号
    int* score;//该用户评分
public:
    Trainidx(String* a)//存用户编号，评分项目数，评分项目，评分
    {
        user=a[0].Getuser();
        num=a[0].Getitemnum();
        item=new int [num];
        score=new int [num];
        for(int i=0;i<num;i++)
        {
            item[i]=a[2*i+1].Getuseritem();
            score[i]=a[2*i+2].Getuseritem();
            countitem[item[i]]++;
            scoreaverage[item[i]]=(scoreaverage[item[i]]*(countitem[item[i]]-1)+score[i])/countitem[item[i]];
        }
    }
    Trainidx()
    {
        user=0;
        num=0;
        item=nullptr;
        score=nullptr;
    }
    Trainidx(const Trainidx& org)
    {
        user=org.user;
        num=org.num;
        item=new int [num];
        score=new int [num];
        for(int i=0;i<num;i++)
        {
            item[i]=org.item[i];
            score[i]=org.score[i];
        }
    }
    Trainidx(Trainidx&& org)
    {
        user=org.user;
        num=org.num;
        item=org.item;
        score=org.score;
        org.item=nullptr;
        org.score=nullptr;
    }
    ~Trainidx()
    {
        delete [] item;
        delete [] score;
    }
    Trainidx& operator=(const Trainidx& org)
    {
        if(this!=&org)
        {
            user=org.user;
            num=org.num;
            delete [] item;
            delete [] score;
            item=new int [num];
            score=new int [num];
            for(int i=0;i<num;i++)
            {
                item[i]=org.item[i];
                score[i]=org.score[i];
            }
        }
        return *this;
    }
    int& Getnum()
    {
        return num;
    }
    int*& Getitem()
    {
        return item;
    }
    int*& Getscore()
    {
        return score;
    }
};

class Trackdata//处理trackdata2里一行的文本（一个track的数据）
{
private:
    int len;//文本长度
    int count;//该曲目包含项目的项数，不包括track
    int* item;//该曲目包含的项目（track，album，artist，genre。。。）
public:
    Trackdata(const char* str)
    {
        int i;
        for(i=0;str[i]!='\0';i++)
            ;
        len=i;
        count=0;
        for(i=0;i<len;i++)
            if(str[i]=='|')
                count++;
        int* c=new int [count+2];
        item=new int [count+1];
        for(i=0;i<count+1;i++)
            item[i]=0;
        c[0]=-1;//c存一行文本中‘|’所在的位数，假设新增第0个‘|’在-1位置，最后一个‘|’在len位置，则每两个‘|’之间的文本为项目编号，转整型
        c[count+1]=len;
        int j=1;
        for(i=0;i<len;i++)
            if(str[i]=='|')
            {
                c[j]=i;
                j++;
            }
        for(j=0;j<=count;j++)
        {
            for(i=c[j]+1;i<c[j+1];i++)
            {
                if(str[i]=='N')
                    break;//如果该项目为"NONE"，转成0
                else
                {
                    item[j]=item[j]*10+str[i]-'0';
                }
            }
        }
        delete [] c;
    }
    Trackdata()
    {
        len=0;
        count=0;
        item=nullptr;
    }
    Trackdata(const Trackdata& org)
    {
        len=org.len;
        count=org.count;
        item=new int [count+1];
        for(int i=0;i<count+1;i++)
            item[i]=org.item[i];
    }
    Trackdata(Trackdata&& org)
    {
        len=org.len;
        count=org.count;
        item=org.item;
        org.item=nullptr;
    }
    ~Trackdata()
    {
        delete [] item;
    }
    Trackdata& operator=(const Trackdata& org)
    {
        if(this!=&org)
        {
            len=org.len;
            count=org.count;
            delete [] item;
            item=new int [count+1];
            for(int i=0;i<count+1;i++)
                item[i]=org.item[i];
        }
        return *this;
    }
    int& Getcount()
    {
        return count;
    }
    int*& Getitem()
    {
        return item;
    }
};

int main()
{
    char a[10];
    ifstream fin1("testIdx2.txt");
    String b;
    String* c;
    Testidx* test=new Testidx [15715];//testidx2中共有15715个用户，数据存入test[]中
    cout<<"loading testIdx2.txt..."<<endl;
    for(int k=0;k<15715;k++)
    {
        fin1>>a;
        b=String(a);//读该用户第一行（用户编号|6）
//        cout<<b.Getuser()<<endl;
        c=new String [7];//一个用户共有七行文本
        c[0]=String(a);
        for(int i=1;i<7;i++)
        {
            fin1>>a;
            c[i]=String(a);
        }
        test[k]=Testidx(c);//一个用户数据存入test[]中
        delete [] c;
//        for(int i=0;i<6;i++)
//        {
//            cout<<i+1<<" "<<test[k].Gettrack()[i]<<endl;
//        }
    }
    fin1.close();

    ifstream fin2("trainIdx2.txt");
    Trainidx* train=new Trainidx [40265];//trainidx2中共有40265个用户，数据存入train[]中
    cout<<"loading trainIdx2.txt..."<<endl;
    for(int k=0;k<40265;k++)
    {
        fin2>>a;
        b=String(a);//读该用户第一行（用户编号|评分项目项数）
//        cout<<b.Getuser()<<'|';
//        cout<<b.Getitemnum()<<endl;
        int num=b.Getitemnum()*2+1;//一个用户的文本行数
        c=new String [num];//存一个用户全部文本
        c[0]=String(a);
        for(int i=1;i<num;i++)
        {
            fin2>>a;
            c[i]=String(a);
        }
        train[k]=Trainidx(c);//一个用户数据存入train[]中
        delete [] c;
//        for(int i=0;i<b.Getitemnum();i++)
//        {
//            cout<<train[k].Getitem()[i]<<" "<<train[k].Getscore()[i]<<endl;
//        }
    }
    fin2.close();

    char a1[200];
    ifstream fin3("trackData2.txt");
    Trackdata d;
    Trackdata* track=new Trackdata [296111];//trackdata中少于296111条曲目，存入track[]中
    int j=0;
    cout<<"loading trackData2.txt..."<<endl;
    while(j<296110)
    {
        fin3>>a1;
        d=Trackdata(a1);//一条曲目的信息
        j=d.Getitem()[0];//该曲目编号
        track[j]=d;//存入对应编号的track[]中
//        cout<<track[j].Getitem()[0]<<endl;
//        for(int i=0;i<=track[j].Getcount();i++)
//            cout<<track[j].Getitem()[i]<<'|';
//        cout<<endl;
    }
    fin3.close();

//    for(int i=0;i<296111;i++)
//        cout<<scoreaverage[i]<<" "<<countitem[i]<<endl;
    cout<<"recommending..."<<endl;
    int count=0;//某一曲目的项目数（不包括track）
    int num=0;//用户在trainidx里的评分项目项数
    double* tempscore;//存用户待推荐首曲目的各项评分（album，artist，genre...）
    double genrescore=0;//待评分曲目的genre得分
    double score[6]={0};//存用户待推荐6首曲目的最终得分
    double box[6]={0};//从高到低存6个曲目分数
    int recm=0;//已推荐曲目数（0～3）
    char rec[6];//存6个曲目的‘1’或‘0’
    ofstream fout("result.txt");//结果写入result.txt
    for(int k=0;k<15715;k++)
    {
        num=train[test[k].Getuser()].Getnum();
        for(int l=0;l<6;l++)
        {
            count=track[test[k].Gettrack()[l]].Getcount();
            tempscore=new double [count];
            for(int m=0;m<count;m++)
                tempscore[m]=0;
            for(int m=1;m<=count;m++)
            {
                for(int n=0;n<num;n++)//如果该用户在trainidx2中给该曲目的该项目打过分，取该分数，否则0分
                {
                    if(track[test[k].Gettrack()[l]].Getitem()[m]==train[test[k].Getuser()].Getitem()[n])
                    {
                        tempscore[m-1]=train[test[k].Getuser()].Getscore()[n];
                        break;
                    }
                    else
                        tempscore[m-1]=0;//(int)(scoreaverage[track[test[k].Gettrack()[l]].Getitem()[m]]+0.5);//
                }
            }
            if(count<=2)//若果该曲目只有album和artist两项，得分取该用户打分album*1000+artist*100+该曲目平均分
                score[l]=tempscore[0]*1000+tempscore[1]*100+scoreaverage[test[k].Gettrack()[l]];
            else//若果该曲目有album和artist以及genre项目，得分取该用户打分album*1000+artist*100+genre该用户所打平均分*10+该曲目平均分
            {
                for(int o=2;o<count;o++)
                    genrescore+=tempscore[o];
                genrescore=genrescore/(count-2);//该曲目中该用户对各个genre打分的平均分
                score[l]=tempscore[0]*1000+tempscore[1]*100+genrescore*10+scoreaverage[test[k].Gettrack()[l]];
            }
            delete [] tempscore;
            genrescore=0;
        }
        for(int p=0;p<6;p++)
        {
            box[p]=score[p];
            rec[p]='0';
        }
        for(int p=0;p<5;p++)//box中对6首歌的分数从高到低排序
        {
            for(int q=0;q<5-p;q++)
                if(box[q]<box[q+1])
                    swap(box[q],box[q+1]);
        }
//        for(int p=0;p<6;p++)
//            cout<<p<<" "<<score[p]<<endl;
        for(int p=0;p<6;p++)//选出分最高的歌,最多3首歌
        {
            if(score[p]==box[0])
            {
                recm++;
                if(recm<=3)
                    rec[p]='1';
            }
        }
        if(box[1]!=box[0])//分次高的歌，最多3首歌
        {
            for(int p=0;p<6;p++)
            {
                if(score[p]==box[1])
                {
                    recm++;
                    if(recm<=3)
                        rec[p]='1';
                }
            }
        }
        if(box[2]!=box[1])//分第三高的歌，最多3首歌
        {
            for(int p=0;p<6;p++)
            {
                if(score[p]==box[2])
                {
                    recm++;
                    if(recm<=3)
                        rec[p]='1';
                }
            }
        }
        for(int p=0;p<6;p++)//该用户的推荐结果写入result.txt
            fout<<rec[p]<<endl;
        recm=0;
    }
    cout<<"complete!"<<endl;
}
