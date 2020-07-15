#include <iostream>
#include <bits/stdc++.h>
#define NPMAX 20 //maximum number of processes in the input file
using namespace std;
//global variables
int  t=0, quantum , qcounter=0 ; //t:cycles counter
int running , id[NPMAX],Btime[NPMAX],Btime2[NPMAX],io[NPMAX],arrive[NPMAX],state[NPMAX],np=0,algorithm,finished=0,cpubusy,tround[NPMAX];
float ut=0,cpuu,ft=0;
FILE *fp, *frr,*fcfs;
//======================================================
// CPP program to implement Queue using two stacks with costly enQueue()
//A First Input First Output data structure used to declare queues as readyQ

struct Queue
{
    stack<int> s1, s2;

    void enQueue(int x)
    {
        // Move all elements from s1 to s2
        while (!s1.empty())
        {
            s2.push(s1.top());
            s1.pop();
        }

        // Push item into s1
        s1.push(x);

        // Push everything back to s1
        while (!s2.empty())
        {
            s1.push(s2.top());
            s2.pop();
        }
    }

    // Dequeue an item from the queue
    int deQueue()
    {
        // if first stack is empty
        if (s1.empty())
        {
            cout << "Q is Empty";
            exit(0);
        }

        // Return top of s1
        int x = s1.top();
        s1.pop();
        return x;
    }
};


void readfile()
{
    np=0;
    fp = fopen("processes.txt","r");
    frr =fopen("RRout.txt","w" );
    fcfs =fopen("FCFS.txt","w" );
    //PROCESS ID,BRUST TIME,I/O BRUST,ARRIVE TIME
    printf("id, Btime, io, arrive \n");
    while(!feof(fp))
    {
        fscanf(fp,"%d\t%d\t%d\t%d\n",&id[np],&Btime[np],&io[np],&arrive[np]);
        Btime2[np]=Btime[np];
        state[np]=0;
        printf("%d ",id[np]);
        printf("%d ",Btime[np]);
        printf("%d ",io[np]);
        printf("%d\n",arrive[np]);
        np++;
    }
    printf("num of processes is %d \n ----------------\n",np);
}

void swap1(int* h, int* k)
{
    int temp = *h;
    *h = *k;
    *k = temp;
}

void swap2proc(int a, int b)
{
    swap1(&id[a],&id[b]);
    swap1(&Btime[a],&Btime[b]);
    swap1(&Btime2[a],&Btime2[b]);
    swap1(&io[a],&io[b]);
    swap1(&arrive[a],&arrive[b]);
}

void sortwith (int* sorter)
{
    for(int i=0; i<np-1 ; i++)
    {
        if (sorter[i]>sorter[i+1])
        {
            swap2proc(i,i+1);
            i=-1;
        }
    }
    for (int i=0; i<np; i++)
    {
        printf("%d ",id[i]);
        printf("%d ",Btime[i]);
        printf("%d ",io[i]);
        printf("%d\n",arrive[i]);
    }
    printf("\n ----------------\n");
}
struct Process {
        int ID , BrustTime , IOTime , ArriveTime ;
        int turnaround ;
        int  running_times  ;
        bool finish = 0 ; // 0:doesn't terminate 1:terminate(toggling occurs after seconed run)
};


int main()
{
    Queue readyQ;
    readfile();
    //sort processes with arrival time
    sortwith(arrive);
    Process p[np] ;
    //initializing process objects members
     for(int x =0 ; x<np ; x++){
        p[x] = {id[x],Btime[x],io[x],arrive[x]};
        p[x].turnaround = p[x].BrustTime+p[x].IOTime+1;
        p[x].running_times = p[x].BrustTime+p[x].IOTime;
    }
    //choose algorithm
    cout <<"Please choose scheduling algorithm 0:FCFS  1:Round Robin\n";
    scanf("%d",algorithm);

if (algorithm == 0){//FCFS Algorithm
    while (finished < np) // will be incremented in the end of each process (finish flag =1 )
{
    cout <<"cycle:" << t <<"\t";
    fprintf (fcfs, "cycle:%d  ",t);
    for(int k=0 ; k<np ; k++){
        if(t >= p[k].ArriveTime){
            if(!(p[k].finish)){
                if(p[k].running_times > p[k].IOTime ){//still in first running cycles
                    cout<< p[k].ID <<":Running " ;
                    fprintf (fcfs, "%d:Running ",p[k].ID);
                    p[k].running_times -- ;
                    ut++;
                }else if(p[k].running_times <= p[k].IOTime){//seconed running or blocking cycles
                    if(p[k].running_times == 0){//seconed running
                        cout<<p[k].ID<<":Running2 " ;
                        fprintf (fcfs, "%d:Running2 ",p[k].ID);
                        ut++;
                        p[k].finish = 1 ;
                        finished ++ ;
                        if(t == p[k+1].ArriveTime){//ready process
                        cout<<p[k+1].ID<<":Ready " ;
                        fprintf (fcfs, "%d:Ready ",p[k+1].ID);
                        p[k+1].turnaround++ ;
                        break ;
                    }
                    }else{//blocking cycles
                        cout <<p[k].ID<<":Blocked " ;
                        fprintf (fcfs, "%d:Blocked ",p[k].ID);
                        p[k].running_times -- ;
                        continue;
                    }
                }
            }else{
                continue;
            }
        }
    }
cout << "\n" ;
fprintf (fcfs,"\n");
t++;
}
//calculations
cout<<"\n\n";
fprintf (fcfs,"\n\n");
cout << "Finishing time:"<<t-1<<"\n";
fprintf (fcfs,"Finishing time:%d\n",t-1);
cout <<"CPU Utilization:"<<ut/t<<"\n" ;
fprintf (fcfs,"CPU Utilization:%.2f\n",ut/t);
fprintf (fcfs,"\n");
for(int i =0 ;i<np;i++){
cout <<"Turnaround time of Process " <<p[i].ID << ":" << p[i].turnaround<<"\n";
fprintf(fcfs,"Turnaround time of Process %d: %d\n",p[i].ID,p[i].turnaround);
}
}
else if (algorithm == 1){//RR Algorithm
        //------------------------------
        //cycle iteration
        //state is 0:new 1:ready 2:blocked 3: running
        printf("Algorithm: Round Robin \n");
        //printf("Please Enter quantum value    :");
        //scanf("%d", &quantum );
        while(finished<np)
        {
            printf("cycle:%d  ",t);
            fprintf (frr, "cycle:%d  ",t);
            for (int i=0; i<=np-1; i++)
            {
                if (t>=arrive[i])
                {
                    //-------------------------------------------------------------
                    if  (state[i]==0)
                    {
                        //new
                        //printf("\n Btime %d\n",Btime[i]);
                        if (Btime>0)
                        {
                            readyQ.enQueue(i);
                            state[i]=1; //ready
                        }
                    }
                    //-------------------------------------------------------------
                    if  (state[i]==1)
                    {
                        //ready
                        //top() � Returns a reference to the top most element of the stack
                        running=readyQ.s1.top();
                        state[running]=3; //running

                        //printf("\n************************\n");
                        //printf("%d:Ready   (%d ,%d) ---",id[i],Btime[i],io[i]);
                        //printf("%d", &running);
                        //printf("\n************************\n");
                    }
                    //-------------------------------------------------------------
                    if  (state[i]==3)
                    {
                        //running

                        if(qcounter<quantum)
                        {
                            if (io[i]>0)  // running before I\O
                            {
                                if (Btime[i]>0)
                                {
                                    Btime[i]-=1;
                                    ut++;  //used time
                                    printf("%d:Running (%d ,%d) ---",id[i],Btime[i],io[i]);
                                    fprintf(frr,"%d:Running (%d ,%d) ---",id[i],Btime[i],io[i]);
                                    if(Btime[i]==0)
                                    {
                                        if(readyQ.s1.top()==i)
                                        {
                                            readyQ.deQueue();
                                            break;
                                        }
                                    }

                                }
                                else if (Btime[i]==0)
                                {

                                    state[i]=2; //blocked
                                }
                            }
                            else if (io[i]==0)    // running after I\O
                            {
                                if (Btime2[i]>0)
                                {
                                    Btime2[i]-=1;
                                    ut++;
                                    printf("%d:Running2 (%d ,%d) ---",id[i],Btime2[i],io[i]);
                                    fprintf(frr,"%d:Running2 (%d ,%d) ---",id[i],Btime2[i],io[i]);
                                    if(Btime2[i]==0)
                                    {
                                        if(readyQ.s1.top()==i)
                                        {
                                            readyQ.deQueue();
                                            break;
                                        }
                                    }
                                }
                                else if (Btime2[i]==0)
                                {
                                    state[i]=4; //terminated
                                    tround[i]=t-arrive[i];
                                    finished++;
                                    ft=t-1;
                                }
                            }
                            qcounter++;
                        }
                        else
                        {
                            qcounter=0;
                            readyQ.deQueue();
                            readyQ.enQueue(i);
                            state[i]=1; //ready
                        }

                    }
                    //-------------------------------------------------------------
                    if  (state[i]==2)
                    {
                        //blocked
                        if (io[i]>0)
                        {
                            io[i]-=1;
                            printf("%d:Blocked (%d ,%d) ---",id[i],Btime[i],io[i]);
                            fprintf(frr,"%d:Blocked (%d ,%d) ---",id[i],Btime[i],io[i]);
                        }
                        else
                        {
                            state[i]=1; //ready
                            readyQ.enQueue(i);
                            i=-1;
                        }
                    }
                    //-------------------------------------------------------------
                    if  (state[i]==1)
                    {
                        //ready
                        /*running=readyQ.s1.top();
                        state[running]=3; //running */

                        printf("%d:Ready   (%d ,%d) ---",id[i],Btime[i],io[i]);
                        fprintf(frr,"%d:Ready   (%d ,%d) ---",id[i],Btime[i],io[i]);
                    }
                    //-------------------------------------------------------------
                    if(state[i]==4)  //terminated
                    {
                        //printf("%d:terminated (%d ,%d) ---",id[i],Btime[i],io[i]);
                        fprintf(frr,"%d:terminated (%d ,%d) ---",id[i],Btime[i],io[i]);

                    }
                }
            }
            printf("\n");
            fprintf (frr, "\n");
            t++;
        } //End of cycle
         for (int i=0; i<np; i++)
    {
        printf("Turnaround time of Process %d = %d \n",id[i],tround[i]);
    }
    printf("finishing time = %f \n",ft);
    fprintf(frr,"finishing time = %f \n",ft);
    printf("used time:%f \n",ut);
    fprintf(frr,"used time:%f \n",ut);
    cpuu = ut/ft;
    printf("CPU utillization: %f",cpuu);
    fprintf(frr,"CPU utillization: %f",cpuu);
    }

    return 0;
}
