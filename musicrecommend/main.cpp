#include <iostream>
#include <fstream>
using namespace std;

double scoreaverage[296111]={0};
int countitem[296111]={0};

class String
{
private:
    char* s;
    int len;
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
    int Getuser()
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
    int Getitemnum()
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
    int Getuseritem()
    {
        int item=0;
        for(int i=0;i<len;i++)
            item=item*10+(s[i]-'0');
        return item;
    }
};

class Testidx
{
private:
    int user;
    int* track;
public:
    Testidx(String* a)
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

class Trainidx
{
private:
    int user;
    int num;
    int* item;
    int* score;
public:
    Trainidx(String* a)
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

class Trackdata
{
private:
    int len;
    int count;
    int* item;
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
        c[0]=-1;
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
                    break;
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
    Testidx* test=new Testidx [15715];
    cout<<"loading testIdx2.txt..."<<endl;
    for(int k=0;k<15715;k++)
    {
        fin1>>a;
        b=String(a);
//        cout<<b.Getuser()<<endl;
        c=new String [7];
        c[0]=String(a);
        for(int i=1;i<7;i++)
        {
            fin1>>a;
            c[i]=String(a);
        }
        test[k]=Testidx(c);
        delete [] c;
//        for(int i=0;i<6;i++)
//        {
//            cout<<i+1<<" "<<test[k].Gettrack()[i]<<endl;
//        }
    }
    fin1.close();

    ifstream fin2("trainIdx2.txt");
    Trainidx* train=new Trainidx [40265];
    cout<<"loading trainIdx2.txt..."<<endl;
    for(int k=0;k<40265;k++)
    {
        fin2>>a;
        b=String(a);
//        cout<<b.Getuser()<<'|';
//        cout<<b.Getitemnum()<<endl;
        int num=b.Getitemnum()*2+1;
        c=new String [num];
        c[0]=String(a);
        for(int i=1;i<num;i++)
        {
            fin2>>a;
            c[i]=String(a);
        }
        train[k]=Trainidx(c);
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
    Trackdata* track=new Trackdata [296111];
    int j=0;
    cout<<"loading trackData2.txt..."<<endl;
    while(j<296110)
    {
        fin3>>a1;
        d=Trackdata(a1);
        j=d.Getitem()[0];
        track[j]=d;
//        cout<<track[j].Getitem()[0]<<endl;
//        for(int i=0;i<=track[j].Getcount();i++)
//            cout<<track[j].Getitem()[i]<<'|';
//        cout<<endl;
    }
    fin3.close();

//    for(int i=0;i<296111;i++)
//        cout<<scoreaverage[i]<<" "<<countitem[i]<<endl;
    cout<<"recommending..."<<endl;
    int count=0;
    int num=0;
    double* tempscore;
    double genrescore=0;
    double score[6]={0};
    double box[6]={0};
    int recm=0;
    char rec[6];
    ofstream fout("result.txt");
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
                for(int n=0;n<num;n++)
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
            if(count<=2)
                score[l]=tempscore[0]*1000+tempscore[1]*100+scoreaverage[test[k].Gettrack()[l]];
            else
            {
                for(int o=2;o<count;o++)
                    genrescore+=tempscore[o];
                genrescore=genrescore/(count-2);
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
        for(int p=0;p<5;p++)
        {
            for(int q=0;q<5-p;q++)
                if(box[q]<box[q+1])
                    swap(box[q],box[q+1]);
        }
//        for(int p=0;p<6;p++)
//            cout<<p<<" "<<score[p]<<endl;
        for(int p=0;p<6;p++)
        {
            if(score[p]==box[0])
            {
                recm++;
                if(recm<=3)
                    rec[p]='1';
            }
        }
        if(box[1]!=box[0])
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
        if(box[2]!=box[1])
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
        for(int p=0;p<6;p++)
            fout<<rec[p]<<endl;
        recm=0;
    }
    cout<<"complete!"<<endl;
}
