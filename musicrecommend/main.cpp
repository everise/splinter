#include <iostream>
#include <fstream>
#include <cmath>
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
#define threshold 500 //在一定范围内，threshold越小精确度越高，但是运算量越大
#define su 4192 //评分次数超过threshold的种子用户数
#define si 2145 //被评分次数超过threshold的种子项目数
#define pick 30 //选中用来参考的种子用户数，与su正相关，试出来的
double scoreaverage[296111]={0};//存每一项的平均分（track,album,artist,genre等）
int countitem[296111]={0};//存每一项被评分次数
int seeduser[su]={0};//种子用户
int seeditem[si]={0};//种子项目
int seedscore[su][si]={{0}};//种子用户的种子项目的分数

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
            if(score[i]==0)//评分为0则改为1，与未评分项目区分
                score[i]=1;
            countitem[item[i]]++;
            scoreaverage[item[i]]=(scoreaverage[item[i]]*(countitem[item[i]]-1)+score[i])/countitem[item[i]];//该项目平均分
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

struct sim//存放用于冒泡排序的对象
{
    int no=0;
    double index=0;
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
        c=new String [7];//一个用户共有七行文本
        c[0]=String(a);
        for(int i=1;i<7;i++)
        {
            fin1>>a;
            c[i]=String(a);
        }
        test[k]=Testidx(c);//一个用户数据存入test[]中
        delete [] c;
    }
    fin1.close();

    ifstream fin2("trainIdx2.txt");
    Trainidx* train=new Trainidx [40265];//trainidx2中共有40265个用户，数据存入train[]中
    cout<<"loading trainIdx2.txt..."<<endl;
    for(int k=0;k<40265;k++)
    {
        fin2>>a;
        b=String(a);//读该用户第一行（用户编号|评分项目项数）
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
    }
    fin3.close();

    int ii=0;
    for(int i=0;i<40265;i++)//获取评分数大于threshold的种子用户
        if(train[i].Getnum()>threshold)
            seeduser[ii++]=i;
    cout<<"seeduser: "<<ii<<endl;
    ii=0;
    for(int i=0;i<296111;i++)//获取被评分次数大于threshold的种子项目
        if(countitem[i]>threshold)
            seeditem[ii++]=i;
    cout<<"seeditem: "<<ii<<endl;
    cout<<"building similarity database......"<<endl;
    int num0=0;
    for(int i=0;i<su;i++)//获取种子用户的种子项目的分数
    {
        num0=train[seeduser[i]].Getnum();
        for(int j=0;j<si;j++)
        {
            for(int k=0;k<num0;k++)
            {
                if(seeditem[j]==train[seeduser[i]].Getitem()[k])
                {
                    seedscore[i][j]=train[seeduser[i]].Getscore()[k];
                    break;
                }
                else
                    seedscore[i][j]=1;
            }
        }
    }

    cout<<"recommending......"<<endl;
    int count=0;//某一曲目的项目数（不包括track）
    int num=0;//用户在trainidx里的评分项目项数
    //double similar[su]={0};//存放目标用户与种子用户相似度
    sim similar[su];//存放目标用户与种子用户相似度
    double testscore[si]={0};//存放目标用户的种子项目的分数
    double numerator=0;//用户协同过滤分子
    double denominator1=0;//用户协同过滤分母1
    double denominator2=0;//用户协同过滤分母2
    double trackscore=0;//推荐曲目track部分分数（从相似种子用户中获得）
    double albumscore=0;//推荐曲目album部分分数（从相似种子用户中获得）
    double trackscore2=0;//推荐曲目track部分分数（从目标用户相似曲目中获得）
    sim* similarcount;//存放相似项目数
    double* tempscore;//存用户待推荐首曲目的各项评分（album，artist，genre...）
    double genrescore=0;//待评分曲目的genre得分
    sim score[6];//存用户待推荐6首曲目的最终得分
    char rec[6]={'0','0','0','0','0','0'};//存6个曲目的‘1’或‘0’
    ofstream fout("result.txt");//结果写入result.txt
    for(int k=0;k<15715;k++)
    {
        num=train[test[k].Getuser()].Getnum();
        for(int j=0;j<si;j++)//获得目标用户的种子项目的分数
        {
            for(int n=0;n<num;n++)
            {
                if(seeditem[j]==train[test[k].Getuser()].Getitem()[n])
                {
                    testscore[j]=train[test[k].Getuser()].Getscore()[n];
                    break;
                }
                else
                    testscore[j]=0;
            }
        }
        for(int i=0;i<su;i++)//通过用户协同过滤计算目标用户与种子用户相似度
        {
            for(int j=0;j<si;j++)
            {
                numerator+=testscore[j]*seedscore[i][j];
                denominator1+=testscore[j]*testscore[j];
                denominator2+=seedscore[i][j]*seedscore[i][j];
            }
            similar[i].no=i;
            similar[i].index=numerator/sqrt(denominator1*denominator2);
            if(denominator1==0||denominator2==0)
                similar[i].index=0;
            numerator=denominator1=denominator2=0;
        }
        for(int p=0;p<pick;p++)//对该用户与种子用户相似度前pick个从高到低排序
        {
            for(int q=su-1;q>p;q--)
                if(similar[q].index>similar[q-1].index)
                    swap(similar[q],similar[q-1]);
        }

        for(int l=0;l<6;l++)
        {
            for(int j=0,p=0,q=0;j<pick;j++)//找出相似度前30的种子用户（不为0），如果他们对该曲目、专辑打过分，该项目分数为他们打分的平均分
            {
                if(similar[j].index!=0)
                {
                    num0=train[seeduser[similar[j].no]].Getnum();
                    for(int n=0;n<num0;n++)
                    {
                        if(test[k].Gettrack()[l]==train[seeduser[similar[j].no]].Getitem()[n])
                        {
                            p++;
                            trackscore=(trackscore*(p-1)+train[seeduser[similar[j].no]].Getscore()[n])/p;
                        }
                        if(track[test[k].Gettrack()[l]].Getitem()[1]==train[seeduser[similar[j].no]].Getitem()[n])
                        {
                            q++;
                            albumscore=(albumscore*(q-1)+train[seeduser[similar[j].no]].Getscore()[n])/q;
                        }
                    }
                }
            }

            count=track[test[k].Gettrack()[l]].Getcount();
            similarcount=new sim [num];
            for(int n=0;n<num;n++)
            {
                similarcount[n].index=0;
                similarcount[n].no=0;
            }
            for(int n=0;n<num;n++)//计算目标曲目与该用户已评分曲目的相似度
            {
                similarcount[n].no=n;
                if(track[train[test[k].Getuser()].Getitem()[n]].Getcount()>0)//判断该项目是否是曲目
                {
                    if(track[test[k].Gettrack()[l]].Getitem()[1]==track[train[test[k].Getuser()].Getitem()[n]].Getitem()[1]&&track[test[k].Gettrack()[l]].Getitem()[1]!=0)
                        similarcount[n].index++;//album相同
                    if(track[test[k].Gettrack()[l]].Getitem()[2]==track[train[test[k].Getuser()].Getitem()[n]].Getitem()[2]&&track[test[k].Gettrack()[l]].Getitem()[2]!=0)
                        similarcount[n].index++;//artist相同
                }
                for(int m=3;m<=count;m++)
                {
                    for(int p=3;p<=track[train[test[k].Getuser()].Getitem()[n]].Getcount();p++)
                    {
                        if(track[test[k].Gettrack()[l]].Getitem()[m]==track[train[test[k].Getuser()].Getitem()[n]].Getitem()[p])
                            similarcount[n].index++;//genre相同
                    }
                }
            }
            for(int m=0;m<15;m++)
            {
                for(int n=num-1;n>m;n--)//找出相似度最高9首的曲目
                {
                    if(similarcount[n].index>similarcount[n-1].index)
                        swap(similarcount[n],similarcount[n-1]);
                }
            }
            for(int m=0;m<9;m++)//曲目分2=9首相似曲目得分*相似度 之和，9试出来的
            {
                if(similarcount[m].index>0)
                    trackscore2=trackscore2+train[test[k].Getuser()].Getscore()[similarcount[m].no]*similarcount[m].index/count;
                else
                    break;
            }

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
                        tempscore[m-1]=0;
                }
            }
            score[l].no=l;
            if(count<=2)//若果该曲目只有album和artist两项，得分取该用户打分album*100+artist*10+该曲目相似分1*3.2+该曲目相似分2*2.5
                score[l].index=tempscore[0]*100+tempscore[1]*10+trackscore*3.2*+trackscore2*2.5;
            else//若果该曲目有album和artist以及genre项目，得分取该用户打分album*100+artist*10+genre该用户所打平均分*0.5+该曲目相似分1*3.2+该曲目相似分2*2.5，3.2和2.5试出来的
            {
                for(int o=2;o<count;o++)
                    genrescore+=tempscore[o];
                genrescore=genrescore/(count-2);//该曲目中该用户对各个genre打分的平均分
                score[l].index=tempscore[0]*100+tempscore[1]*10+genrescore*0.5+trackscore*3.2+trackscore2*2.5;
            }
            if(score[l].index==0)//如果依然没有评分，参考相似用户对此曲目的专辑的分数
                score[l].index=albumscore*1;
            delete [] tempscore;
            delete [] similarcount;
            genrescore=0;
            trackscore=0;
            albumscore=0;
            trackscore2=0;
        }

        for(int p=0;p<3;p++)//6首歌选出最高的3首从高到低排序
        {
            for(int q=5;q>p;q--)
                if(score[q].index>score[q-1].index)
                    swap(score[q],score[q-1]);
            rec[score[p].no]='1';
        }
        for(int p=0;p<6;p++)//该用户的推荐结果写入result.txt
        {
            fout<<rec[p]<<endl;
            rec[p]='0';
        }
        cout<<"progress: "<<(k/15714.0)*100<<"%"<<endl;//显示当前进度
    }
    cout<<"complete!"<<endl;
}
