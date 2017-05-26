// CPU Scheduler //
// Kunal Gupta 735/IT/15
// round robin is fixed :)
#include<iostream>
#include<math.h>
#include<bits/stdc++.h>
#include<queue>
using namespace std;

struct Process  // structure related to the process
{
    int pid;
    int pri = -1;
    int AT,BT,CT,TAT,WT;
};
int ret_min(int ar[],int n)   // to return the minimum of the values
{
    int mi = 10000000;
    int ind = 0;
    for(int i=0;i<n;i++)
    {
        if(ar[i]<mi && ar[i]>0)
        {
            mi = ar[i];
            ind = i;
        }
    }
    return ind;
}

int ret_max(int ar[],int n)   // to return the maximum of the values
{
    int ma = -1;
    int ind = 0;
    for(int i=0;i<n;i++)
    {
        if(ar[i]>ma)
        {
            ma = ar[i];
            ind = i;
        }
    }
    return ind;
}



void Display(struct Process *p,int n,int indexes[],char str[])   // to display the stats and show the gantt chart
{
    float avg_TAT = 0,avg_WT = 0;
    cout<<"\n-------------------------"<<str<<" Scheduler------------------------------------------";
    cout<<"\nPID\tPri\tAT\tBT\tCT\tTAT\tWT";
    for (int i=0;i<n;i++)
    {
        cout<<"\n"<<p[indexes[i]].pid+1<<"\t"<<p[indexes[i]].pri<<"\t"<<p[indexes[i]].AT<<"\t"<<p[indexes[i]].BT<<"\t"<<p[indexes[i]].CT<<"\t"<<p[indexes[i]].TAT<<"\t"<<p[indexes[i]].WT;
        avg_TAT+=float(p[indexes[i]].TAT);
        avg_WT+=float(p[indexes[i]].WT);
    }
    avg_TAT/=n;
    avg_WT/=n;
    cout<<"\n----------------------------------------------------------------------------------";
    cout<<"\nAverage TAT:"<<avg_TAT<<"\nAverage WT:"<<avg_WT;
    cout<<"\n--------------------------GANTT CHART------------------------------";
    cout<<"\n 0 ";
    for(int i=0;i<n;i++)
    {
       cout<<"-- P"<<p[indexes[i]].pid+1<<" --"<<p[indexes[i]].CT;
    }
}
void get_index(struct Process *p , int indexes[],int n)    // this function creates an array called indexes whose elements
{                                                           // are the process id according to sorted arrival times
    int ar_AT[n];
    for(int i=0;i<n;i++)
    {
       ar_AT[i] = p[i].AT;
    }
    sort(ar_AT,ar_AT+(sizeof(ar_AT)/sizeof(int)));
    int f[n] = {0};
    int z=0,j;

    for(int i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            if(p[j].AT == ar_AT[i] && f[j]==0){ f[j] = 1;break;}
        }

        indexes[z++] = p[j].pid;
    }
}
void fcfs(struct Process *p,int n)       /// FCFS Scheduler ///
{
    int indexes[n];
    get_index(p,indexes,n);
    p[indexes[0]].CT = p[indexes[0]].BT + p[indexes[0]].AT;
    p[indexes[0]].TAT = p[indexes[0]].CT - p[indexes[0]].AT;
    p[indexes[0]].WT = p[indexes[0]].TAT - p[indexes[0]].BT;
    for(int i=1;i<n;i++)
    {
        if(p[indexes[i]].AT <= p[indexes[i-1]].CT)
        {
            p[indexes[i]].CT = p[indexes[i]].BT + p[indexes[i-1]].CT;
        }
        else
        {
            p[indexes[i]].CT = p[indexes[i]].BT + p[indexes[i]].AT;
        }
        p[indexes[i]].TAT = p[indexes[i]].CT - p[indexes[i]].AT;
        p[indexes[i]].WT = p[indexes[i]].TAT - p[indexes[i]].BT;
    }
    Display(p,n,indexes,"FCFS");


}

int allcheck(int ar[],int n)   /// to check whether all elements of the array are zero or not ( for BT)
    {
        for(int i=0;i<n;i++)
        {
            if(ar[i]!=0)
            {
                return 0;
            }
        }
        return 1;
    }

void sjf(struct Process*p, int n, int tq)   /// shortest job first scheduler
{


    int indexes[n];
    get_index(p,indexes,n);

    int left_bt[n];  /// this array has the remaining BT of the processes
    for(int i =0;i<n;i++)
    {
        left_bt[i] = p[indexes[i]].BT;
    }


    int init = p[indexes[0]].AT;  /// initial time is the arrival time of first process

    while(!allcheck(left_bt,n))
    {
        int i = 0;
        for(i=0;i<n;i++)
        {
            if(p[indexes[i]].AT > init) { break;}
        }

        int ind = ret_min(left_bt,i);

        if(left_bt[ind]<=tq){init+=left_bt[ind]; left_bt[ind] = 0;}
        else
        {
            left_bt[ind]-=tq;
            init+=tq;
        }
        if(left_bt[ind]==0) { p[indexes[ind]].CT = init;}  /// this had to be corrected!  :)

    }
    for(int i=0;i<n;i++)
    {
        p[indexes[i]].TAT = p[indexes[i]].CT - p[indexes[i]].AT;
        p[indexes[i]].WT = p[indexes[i]].TAT - p[indexes[i]].BT;
    }

    Display(p,n,indexes,"SJF");

}

void ljf(struct Process *p, int n)
{
    int indexes[n];
    get_index(p,indexes,n);

    int left_bt[n];  /// this array has the remaining BT of the processes
    for(int i =0;i<n;i++)
    {
        left_bt[i] = p[indexes[i]].BT;
    }

    int init = p[indexes[0]].AT;  /// initial time is the arrival time of first process

    while(!allcheck(left_bt,n))
    {
        int i = 0;
        for(i=0;i<n;i++)
        {
            if(p[indexes[i]].AT > init) { break;}
        }

        int ind = ret_max(left_bt,i);
        init+=left_bt[ind];
        p[indexes[ind]].CT = init;
        left_bt[ind]= 0;
    }
    for(int i=0;i<n;i++)
    {
        p[indexes[i]].TAT = p[indexes[i]].CT - p[indexes[i]].AT;
        p[indexes[i]].WT = p[indexes[i]].TAT - p[indexes[i]].BT;
    }

    Display(p,n,indexes,"LJF");
}

void longest_rem_time_first(struct Process *p, int n, int tq)
{
    int indexes[n];
    get_index(p,indexes,n);

    int left_bt[n];  /// this array has the remaining BT of the processes
    for(int i =0;i<n;i++)
    {
        left_bt[i] = p[indexes[i]].BT;
    }


    int init = p[indexes[0]].AT;  /// initial time is the arrival time of first process

    while(!allcheck(left_bt,n))
    {
        int i = 0;
        for(i=0;i<n;i++)
        {
            if(p[indexes[i]].AT > init) { break;}
        }

        int ind = ret_max(left_bt,i);

        if(left_bt[ind]<=tq){init+=left_bt[ind]; left_bt[ind] = 0;}
        else
        {
            left_bt[ind]-=tq;
            init+=tq;
        }
        if(left_bt[ind]==0) { p[indexes[ind]].CT = init;}  /// this had to be corrected!  :)

    }
    for(int i=0;i<n;i++)
    {
        p[indexes[i]].TAT = p[indexes[i]].CT - p[indexes[i]].AT;
        p[indexes[i]].WT = p[indexes[i]].TAT - p[indexes[i]].BT;
    }

    Display(p,n,indexes,"Longest remaining time first");

}
void round_robin(struct Process *p, int n, int tq)   /// working :)
{
    int indexes[n];
    get_index(p,indexes,n);

    queue<int> Q;

    int left_bt[n];  /// this array has the remaining BT of the processes
    for(int i =0;i<n;i++)
    {
        left_bt[i] = p[indexes[i]].BT;
    }

    Q.push(0);
    int init = p[indexes[0]].AT;  /// initial time is the arrival time of first process

    while(!allcheck(left_bt,n))
    {
        int k = Q.front();
        Q.pop();
        cout<<"\nk = "<<k;
        if(left_bt[k]>tq)
        {
            init+=tq;
            left_bt[k]-=tq;
        }
        else
        {
            init+=left_bt[k];
            left_bt[k] = 0;
            p[indexes[k]].CT = init;
        }

        cout<<"\nTime:"<<init;

        if(init <= p[indexes[n-1]].AT)
        {
            for(int i=k+1;i<n;i++)
            {
                if(p[indexes[i]].AT<=init)
                {
                    Q.push(i);
                }
            }
        }
        if(left_bt[k]>0)
        {
            Q.push(k);
        }


    }


    for(int i=0;i<n;i++)
    {
        p[indexes[i]].TAT = p[indexes[i]].CT - p[indexes[i]].AT;
        p[indexes[i]].WT = p[indexes[i]].TAT - p[indexes[i]].BT;
    }

    Display(p,n,indexes,"Round Robin");
}


void priority_scheduler(struct Process *p, int n,int mode,int tq)
{
    int indexes[n];
    get_index(p,indexes,n);
    int left_bt[n];  /// this array has the remaining BT of the processes
    int priori[n];
    for(int i =0;i<n;i++)
    {
        left_bt[i] = p[indexes[i]].BT;
        priori[i] = p[indexes[i]].pri;
    }




    if(mode ==1)  /// means non preemptive
    {



    int init = p[indexes[0]].AT;  /// initial time is the arrival time of first process

    while(!allcheck(left_bt,n))
    {
        int i = 0;
        for(i=0;i<n;i++)
        {
            if(p[indexes[i]].AT > init) { break;}
        }

        int ind = ret_max(priori,i);   // high priority means process will be executed first
        init+=left_bt[ind];
        left_bt[ind] = 0;
        priori[ind] = -1;
        p[indexes[ind]].CT = init;


    }
    for(int i=0;i<n;i++)
    {
        p[indexes[i]].TAT = p[indexes[i]].CT - p[indexes[i]].AT;
        p[indexes[i]].WT = p[indexes[i]].TAT - p[indexes[i]].BT;
    }

    Display(p,n,indexes,"Priority Based ");

    }

    else
    {


        /// mode is pre emptive

        int init = p[indexes[0]].AT;  /// initial time is the arrival time of first process

    while(!allcheck(left_bt,n))
    {
        int i = 0;
        for(i=0;i<n;i++)
        {
            if(p[indexes[i]].AT > init) { break;}
        }

        int ind = ret_max(priori,i);  // high priority means process will be executed first

       if(left_bt[ind]<=tq){init+=left_bt[ind]; left_bt[ind] = 0; priori[ind] = -1;}
        else
        {
            left_bt[ind]-=tq;
            init+=tq;
        }
        if(left_bt[ind]==0) { p[indexes[ind]].CT = init;}  /// this had to be corrected!  :)

    }
    for(int i=0;i<n;i++)
    {
        p[indexes[i]].TAT = p[indexes[i]].CT - p[indexes[i]].AT;
        p[indexes[i]].WT = p[indexes[i]].TAT - p[indexes[i]].BT;
    }

    Display(p,n,indexes,"Priority Based (Pre emptive) ");


    }
}

int main()
{
    int n; // no of processes
    cout<<"\n------------------ CPU Process Scheduler -----------------------";
    cout<<"\nSelect Your Preferred Scheduling Algorithm:";
    cout<<"\n1.) First Come First Served (FCFS) \n2.) Shortest Job First(Pre Emptive) \n3.) Longest Job First";
    cout<<"\n4.) Longest Remaining Time First (pre emptive) \n5.) Round Robin (pre emptive) \n6.) Priority Based ";
    int choice;
    cout<<"\nEnter Your Choice:";
    cin>>choice;
    cout<<"\nEnter the number of processes:";
    cin>>n;
    int tq = 0,opt;
    if( choice == 2 || choice == 4 || choice == 5 || choice == 6)
    {
        if(choice == 6)
        {
            cout<<"\nSelect Priority Based Mode: 1.) Pre Emptive 2.) Non Pre Emptive";
            cin>>opt;
            if(opt == 1)
            {
                cout<<"\nEnter Time Quantum for Process execution:";
                cin>>tq;
            }
        }
        else
        {
            cout<<"\nEnter Time Quantum for Process execution:";
            cin>>tq;
        }
    }
    cout<<"\nEnter Process Metadata ( put priority as -1 for non priority based schedulers )";
    struct Process p[n];
    for(int i=0;i<n;i++)
    {
        cout<<"\nEnter AT and BT and Priority for Process ID#"<<i+1<<":";
        cin>>p[i].AT>>p[i].BT>>p[i].pri;
        p[i].pid = i;
    }
    switch(choice)
    {
        case 1:fcfs(p,n);
                break;
        case 2:sjf(p,n,tq);
                break;
        case 3:ljf(p,n);
                break;
        case 4:longest_rem_time_first(p,n,tq);
                break;
        case 5:round_robin(p,n,tq);
                break;
        case 6:priority_scheduler(p,n,opt-1,tq);
                break;
    }
    return 0;
}
