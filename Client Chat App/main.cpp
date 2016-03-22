/**
 * @armaango_assignment1
 * @author  Armaan Goyal <armaango@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 
 Several sources were consulted for different parts of the application
 1. For creating the client and server instances, most of the code used is from BeeJ programming guide, even the variables have been picked
 for ease of use.
 2.For creating, and managing Linkedlists for various puposes like for Clients List, Message Buffering and for Statistics data, the following sources have been consulted
 
 a. http://www.geeksforgeeks.org/delete-a-given-node-in-linked-list-under-given-constraints/
 
 b.http://www.sanfoundry.com/cpp-program-implement-single-linked-list/
 
 c.Data Structures book by Karumanchi
 
 
 3. Also for allocating memory to an array of strings(character arrays) the following source was referred
 // http://stackoverflow.com/questions/4008933/memory-allocation-for-array-of-pointers //
 
 4. Code used to find External IP sourced from Beej
 
 5.
 
 
 
 
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../include/global.h"
#include "../include/logger.h"

using namespace std;



void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}





struct stats
{
    char *clientIP;
    char *clientname;
    char *status;
    int clientPort;
    int msgsentcount;
    int msgrecvcount;
    struct stats *next;
}*statsListBegin;

void insertStats(char *clientIP, char *clientname, char *status, int clientPort, int msgsentcount, int msgrecvcount)
{
    
    struct stats *tempNode, *p;
    tempNode =  (struct stats *) malloc(sizeof(struct stats));
    if (tempNode == NULL)
    {
        printf("Memory allocated null\n");
        return ;
    }
    else
    {
        tempNode->clientIP = static_cast<char *> (malloc (MAXSIZE));
        tempNode->clientname = static_cast<char *>(malloc(MAXSIZE));
        tempNode->status = static_cast<char *>(malloc(MAXSIZE));
        
        strcpy(tempNode->clientIP, clientIP);
        strcpy(tempNode->clientname, clientname);
        strcpy(tempNode->status, status);
        tempNode->clientPort=clientPort;
        tempNode->msgsentcount=msgsentcount;
        tempNode->msgrecvcount=msgrecvcount;
        tempNode->next = NULL;
    }
    
    if (statsListBegin == NULL)
    {
        statsListBegin = tempNode;
        statsListBegin->next = NULL;
    }
    else
    {
        p = statsListBegin;
        statsListBegin = tempNode;
        statsListBegin->next = p;
    }
    
}

void updateStats(char * IPtoUpdate, int flag)
{
    if (statsListBegin == NULL)
    {
        cout<<"List is empty"<<endl;
        return;
    }
    struct stats *s;
    s = statsListBegin;
    while (s != NULL)
    {
        if (strcmp(s->clientIP,IPtoUpdate)==0)
        {
            
            if (flag==1) {
                s->msgsentcount++;
            }
            else if (flag==2)
            {
                s->msgrecvcount++;
            }
            else if (flag==3)
            {
                if (strcmp(s->status,"online")==0) {
                    strcpy(s->status,"offline");
                }
                else{
                    strcpy(s->status,"online");
                }
                
            }
        }
        s = s->next;
    }
    cout<<"Node Updated"<<endl;
    
}

void deleteStats(char *IPtodelete)
{
    char searchIP[30];
    strcpy(searchIP,IPtodelete);
    struct stats *tempNode, *prevNode;
    tempNode = statsListBegin;
    
    if (strcmp(statsListBegin->clientIP,searchIP)==0)
    {
        if (statsListBegin->next==NULL) {
            statsListBegin=NULL;
            return;
        }
        else{
            statsListBegin = tempNode->next;
            
            
            return;
        }
    }
    
    
    // When not first node, follow the normal deletion process
    
    // find the previous node
    prevNode = statsListBegin;
    while(prevNode->next != NULL && strcmp(prevNode->next->clientIP,searchIP)!=0)
        prevNode = prevNode->next;
    
    // Check if node really exists in Linked List
    if(prevNode->next == NULL)
    {
        printf("\n Given node is not present in Linked List");
        return;
    }
    
    // Remove node from Linked List
    prevNode->next = prevNode->next->next;
    
    // Free memory
    
    
    return;
    
}

struct stats * searchStats(char *IPtosearch)
{
    
    char searchIP[30];
    strcpy(searchIP,IPtosearch);
    int c=0;
    struct stats *s;
    if (statsListBegin == NULL)
    {
        cout<<"List is empty"<<endl;
        return NULL;
    }
    s = statsListBegin;
    while (s != NULL)
    {
        if (strcmp(s->clientIP,searchIP)==0)
        {
            c=1;
            return s;
            
        }
        s = s->next;
    }
    if (c==0)
    {
        
        return NULL;
    }
    else
    {
        return s;
    }
    
}

void sortStats()
{
    struct stats *ptr, *s;
    char clientIP[30];
    char clientname[50];
    char status[40];
    int clientPort;
    int msgsentcount;
    int msgrecvcount;
    
    if (statsListBegin == NULL)
    {
        cout<<"The List is empty"<<endl;
        return;
    }
    ptr = statsListBegin;
    while (ptr != NULL)
    {
        for (s = ptr->next;s !=NULL;s = s->next)
        {
            if (ptr->clientPort > s->clientPort)
            {
                clientPort = ptr->clientPort;
                msgsentcount=ptr->msgsentcount;
                msgrecvcount=ptr->msgrecvcount;
                strcpy(clientname, ptr->clientname);
                strcpy(clientIP, ptr->clientIP);
                strcpy(status, ptr->status);
                
                ptr->clientPort = s->clientPort;
                ptr->msgsentcount = s->msgsentcount;
                ptr->msgrecvcount=s->msgrecvcount;
                strcpy(ptr->clientname, s->clientname);
                strcpy(ptr->clientIP, s->clientIP);
                strcpy(ptr->status,s->status);
                
                s->clientPort = clientPort;
                s->msgsentcount = msgsentcount;
                s->msgrecvcount = msgrecvcount;
                strcpy(s->clientname,clientname);
                strcpy(s->clientIP,clientIP);
                strcpy(s->status,status);
            }
        }
        ptr = ptr->next;
    }
    printf("Sorted successfully\n");
    
    
}

void displaystats()
{
    sortStats();
    struct stats *tempNode;
    
    if (statsListBegin == NULL)
    {
        printf("Empty List\n");
        return;
    }
    tempNode = statsListBegin;
    int sNo=1;
    while (tempNode != NULL)
    {
        cse4589_print_and_log("%-5d%-35s%-8d%-8d%-8s\n", sNo, tempNode->clientname,tempNode->msgsentcount, tempNode->msgrecvcount, tempNode->status);
        sNo++;
        tempNode = tempNode->next;
    }
    
    
    
}



//stats---------------------------------------------------------------------------------------------------------------//



//Creating a linkedlist for Logged In clients and its methods//

//-----------------------------------------------------------//


struct client
{
    char *clientIP;
    char *clientname;
    int clientPort;
    int clientSocket;
    struct client *next;
}*clientListBegin, *blockedClientListBegin;


void insertClient(char *IPtoinsert, char *ClientHostName, int ClientportNum, int Clientsocket, int callingflag )       //will be fired on login
{
    
    struct client *tempNode, *p;
    tempNode =  (struct client *) malloc(sizeof(struct client));
    if (tempNode == NULL)
    {
        printf("Memory allocated null\n");
        return ;
    }
    else
    {
        tempNode->clientIP = static_cast<char *> (malloc (MAXSIZE));
        tempNode->clientname = static_cast<char *>(malloc(MAXSIZE));
        
        
        strcpy(tempNode->clientIP, IPtoinsert);
        strcpy(tempNode->clientname, ClientHostName);
        tempNode->clientPort=ClientportNum;
        tempNode->clientSocket=Clientsocket;
        tempNode->next = NULL;
    }
    
    if (callingflag==1) {
        if (clientListBegin == NULL)
        {
            clientListBegin = tempNode;
            clientListBegin->next = NULL;
        }
        else
        {
            p = clientListBegin;
            clientListBegin = tempNode;
            clientListBegin->next = p;
        }
    }
    else if(callingflag==2)
    {
        if (blockedClientListBegin == NULL)
        {
            blockedClientListBegin = tempNode;
            blockedClientListBegin->next = NULL;
        }
        else
        {
            p = blockedClientListBegin;
            blockedClientListBegin = tempNode;
            blockedClientListBegin->next = p;
        }
    }
    
}
void sortClientList(int callingFlag)
{
    struct client *ptr, *s;
    char clientIP[30];
    char clientname[50];
    int clientPort;
    int clientSocket;
    
    if (callingFlag==1) {
        if (clientListBegin == NULL)
        {
            cout<<"The List is empty"<<endl;
            return;
        }
        ptr = clientListBegin;
    }
    else if(callingFlag==2)
    {
        if (blockedClientListBegin == NULL)
        {
            cout<<"The List is empty"<<endl;
            return;
        }
        ptr = blockedClientListBegin;
    }
    while (ptr != NULL)
    {
        for (s = ptr->next;s !=NULL;s = s->next)
        {
            if (ptr->clientPort > s->clientPort)
            {
                clientPort = ptr->clientPort;
                clientSocket = ptr->clientSocket;
                strcpy(clientname, ptr->clientname);
                strcpy(clientIP, ptr->clientIP);
                
                ptr->clientPort = s->clientPort;
                ptr->clientSocket = s->clientSocket;
                strcpy(ptr->clientname, s->clientname);
                strcpy(ptr->clientIP, s->clientIP);
                
                s->clientPort = clientPort;
                s->clientSocket = clientSocket;
                strcpy(s->clientname,clientname);
                strcpy(s->clientIP,clientIP);
            }
        }
        ptr = ptr->next;
    }
}


void displayClientList(int callingflag)//call sortclientlist before printing to screen
{
    sortClientList(callingflag);
    struct client *tempNode;
    
    if (callingflag==1) {
        if (clientListBegin == NULL)
        {
            printf("Empty List\n");
            return;
        }
        tempNode = clientListBegin;
    }
    else if(callingflag==2)
    {
        if (blockedClientListBegin == NULL)
        {
            printf("Empty List\n");
            return;
        }
        tempNode = blockedClientListBegin;
    }
    int sNo=1;
    while (tempNode != NULL)
    {
        cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",sNo,tempNode->clientIP,tempNode->clientname,tempNode->clientPort);
        sNo++;
        tempNode = tempNode->next;
    }
}

void deleteClient(char *IP,int callingflag)     //will be fired on logout
{
    char searchIP[30];
    strcpy(searchIP,IP);
    
    
    struct client *tempNode, *prevNode;
    if(callingflag==1)
    {
        tempNode = clientListBegin;
        
        if (strcmp(clientListBegin->clientIP,searchIP)==0)
        {
            if (clientListBegin->next==NULL) {
                clientListBegin=NULL;
                return;
            }
            else{
                clientListBegin = tempNode->next;
                
                
                return;
            }
        }
        prevNode = clientListBegin;
    }
    else if(callingflag==2)
    {
        tempNode = blockedClientListBegin;
        
        if (strcmp(blockedClientListBegin->clientIP,searchIP)==0)
        {
            if (blockedClientListBegin->next==NULL) {
                blockedClientListBegin=NULL;
                return;
            }
            else{
                blockedClientListBegin = tempNode->next;
                
                
                return;
            }
        }
        prevNode = blockedClientListBegin;
    }
    while(prevNode->next != NULL && strcmp(prevNode->next->clientIP,searchIP)!=0)
        prevNode = prevNode->next;
    
    // Check if node really exists in Linked List
    if(prevNode->next == NULL)
    {
        printf("\n Given node is not present in Linked List");
        return;
    }
    
    // Remove node from Linked List
    prevNode->next = prevNode->next->next;
    
    // Free memory
    
    
    return;
    
}
struct client * clientSearch(char *IP, int callingflag)      //to find the sockfd of the client whose IP is passed
{
    char searchIP[30];
    strcpy(searchIP,IP);
    int c=0;
    struct client *s;
    if (callingflag==1)
    {
        if (clientListBegin == NULL)
        {
            cout<<"List is empty"<<endl;
            return NULL;
        }
        s = clientListBegin;
    }
    else if(callingflag==2)
    {
        if (blockedClientListBegin == NULL)
        {
            cout<<"List is empty"<<endl;
            return NULL;
        }
        s = blockedClientListBegin;
    }
    while (s != NULL)
    {
        if (strcmp(s->clientIP,searchIP)==0)
        {
            c=1;
            return s;
            
        }
        s = s->next;
    }
    if (c==0)
    {
        
        return NULL;
    }
    else
    {
        return s;
    }
}

char * clientSearchforIP(int sockfdtoSearch)      //to find the sockfd of the client whose IP is passed
{
    
    int c=0;
    if (clientListBegin == NULL)
    {
        cout<<"List is empty"<<endl;
        return NULL;
    }
    
    struct client *s;
    s = clientListBegin;
    while (s != NULL)
    {
        if (s->clientSocket==sockfdtoSearch)
        {
            c=1;
            cout<<"Client found"<<endl;
            return s->clientIP;
            
        }
        s = s->next;
    }
    if (c==0)
    {
        
        return NULL;
    }
    else
    {
        return s->clientIP;
    }
    
}


//-----------------------------------------------------------------------------//

//Ending linkedlist for Logged in clients






//-------------------------------------------------------------------//
//----Serialise and deserialise logics----//


char * serialise(int callingflag)                                              //--------------serialise---------------------//
{
    char  serialBuff[1000];
    bzero(serialBuff, 1000);
    char finalBuff[1000];
    bzero(finalBuff, 1000);
    struct client *tempNode;
    if (callingflag==1) {
        if (clientListBegin == NULL)
        {
            printf("Empty List\n");
            return NULL;
        }
        tempNode = clientListBegin;
    }
    else if(callingflag==2)
    {
        if (blockedClientListBegin == NULL)
        {
            printf("Empty List\n");
            return NULL;
        }
        tempNode = blockedClientListBegin;
    }
    while (tempNode != NULL)
    {
        sprintf(serialBuff, "%s*%s*%d*%d", tempNode->clientIP, tempNode->clientname, tempNode->clientPort, tempNode->clientSocket);
        strcat(serialBuff,"*");
        strcat(finalBuff, serialBuff);
        tempNode = tempNode->next;
    }
    
    size_t indexOfNullTerminator = strlen(finalBuff);
    finalBuff[indexOfNullTerminator - 1] = '\0';
    
    
    
    return finalBuff;
}

void deserialise(char * buffToderialise, int callingflag)                    ////-----------custom deserialise---------------------------//
{
    
    char derialise[1000];
    strcpy(derialise, buffToderialise);
    char *arrayofTokens[50];
    
    for (int i = 0 ; i< 50 ; i++)
    {
        arrayofTokens[i] = (char*)malloc( 1024 * sizeof(char));
    }
    
    
    char *token;
    int args = 0;
    token = strtok(derialise, "*");
    while(token!=NULL)
    {
        strcpy(arrayofTokens[args], token);
        args++;
        token=strtok(NULL, "*");
    }
    
    int i=0;
    int sNo=1;
    while(strlen(arrayofTokens[i])!=0)
    {
        char *clientIP=arrayofTokens[i];
        char *clientname=arrayofTokens[i+1];
        int clientPort=atoi(arrayofTokens[i+2]);
        int clientSocket=atoi(arrayofTokens[i+3]);
        i=i+4;
        
        insertClient(clientIP, clientname, clientPort, clientSocket,callingflag);
        
    }
    
    
    
}


void deserialiseMessageBuffer(char * buffToderialise,int callFlag,char*IPfromServer)                    ////-----------custom deserialise---------------------------//
{
    
    char derialise[1000];
    bzero(derialise,1000);
    strcpy(derialise, buffToderialise);
    char *arrayofTokens[2000];
    char IPneededforServerSide[30];
    
    strcpy(IPneededforServerSide,IPfromServer);
    // http://stackoverflow.com/questions/4008933/memory-allocation-for-array-of-pointers //
    for (int i = 0 ; i< 2000 ; i++)
    {
        arrayofTokens[i] = (char*)malloc( 1024 * sizeof(char));
    }
    char *token;
    int args = 0;
    token = strtok(derialise, "*");
    while(token!=NULL)
    {
        strcpy(arrayofTokens[args], token);
        args++;
        token=strtok(NULL, "*");
    }
    
    int i=0;
    int sNo=1;
    while(strlen(arrayofTokens[i])!=0)
    {
        char *clientIP=arrayofTokens[i];
        char *message=arrayofTokens[i+1];
        int seqNumber=atoi(arrayofTokens[i+2]);
        i=i+3;
        
        if(callFlag==1){
            cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
            cse4589_print_and_log("msg from:%s\n[msg]:%s\n", clientIP, message);
            cse4589_print_and_log("[RECEIVED:END]\n");
        }
        else if(callFlag==2 && seqNumber==0)
        {
            cse4589_print_and_log("[RELAYED:SUCCESS]\n");
            cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n",clientIP,IPneededforServerSide, message);
            cse4589_print_and_log("[RELAYED:END]\n");
            updateStats(IPneededforServerSide,2);
        }
        else if(callFlag==2 && seqNumber==1)
        {
            updateStats(IPneededforServerSide,2);
        }
        
        
    }
    bzero(derialise,1000);
    bzero(IPneededforServerSide,30);
    
    
}


//------------------------deserialisation ends here-----------------------------//


//---------------------------------------------------------blocking strcuts and functions-----------------------------//


struct blockedClient
{
    char *blockedClientIP;
    char *blockerClientIP;
    int blockedclientSocket;
    int blockerclientSocket;
    struct blockedClient *next;
}*blockingListBegin;
//
//
void insertBlockedClient(char *blockedClientIP,char *blockerClientIP, int blockedclientSocket, int blockerclientSocket )//will be fired on block command
{
    
    struct blockedClient *tempNode, *p;
    //temp = create_client(IP, port, socket, hostname);
    tempNode =  (struct blockedClient *) malloc(sizeof(struct blockedClient));
    if (tempNode == NULL)
    {
        printf("Memory allocated null\n");
        return ;
    }
    else
    {
        tempNode->blockedClientIP = static_cast<char *> (malloc (MAXSIZE));
        tempNode->blockerClientIP = static_cast<char *> (malloc (MAXSIZE));
        
        strcpy(tempNode->blockedClientIP, blockedClientIP);
        strcpy(tempNode->blockerClientIP, blockerClientIP);
        tempNode->blockedclientSocket=blockedclientSocket;
        tempNode->blockerclientSocket=blockerclientSocket;
        tempNode->next = NULL;
    }
    if (blockingListBegin == NULL)
    {
        blockingListBegin = tempNode;
        blockingListBegin->next = NULL;
    }
    else
    {
        p = blockingListBegin;
        blockingListBegin = tempNode;
        blockingListBegin->next = p;
    }
    
}

//
void deleteBlockedClient(char *blockedClientIP, char *blockerClientIP)     //will be fired on unblock
{
    char searchblockedIP[30];
    strcpy(searchblockedIP,blockedClientIP);
    
    char searchblockerIP[30];
    strcpy(searchblockerIP,blockerClientIP);
    
    
    struct blockedClient *tempNode, *prevNode;
    tempNode = blockingListBegin;
    
    if (strcmp(blockingListBegin->blockedClientIP,searchblockedIP)==0 && strcmp(blockingListBegin->blockerClientIP,searchblockerIP)==0 )
    {
        if (blockingListBegin->next==NULL) {
            blockingListBegin=NULL;
            return;
        }
        else{
            blockingListBegin = tempNode->next;
            
            
            return;
        }
    }
    
    
    // When not first node, follow the normal deletion process
    
    // find the previous node
    prevNode = blockingListBegin;
    while(prevNode->next != NULL && strcmp(prevNode->next->blockedClientIP,searchblockedIP)!=0 && strcmp(prevNode->next->blockerClientIP,searchblockerIP)!=0)
        prevNode = prevNode->next;
    
    // Check if node really exists in Linked List
    if(prevNode->next == NULL)
    {
        printf("\n Given node is not present in Linked List");
        return;
    }
    
    // Remove node from Linked List
    prevNode->next = prevNode->next->next;
    
    // Free memory
    
    return;
    
}

int searchBlockedClient(char *blockedClientIP, char *blockerClientIP)
{
    
    char searchblockedIP[30];
    char searchblockerIP[30];
    strcpy(searchblockedIP,blockedClientIP);
    strcpy(searchblockerIP,blockerClientIP);
    int c=0;
    struct blockedClient *s;
    if (blockingListBegin == NULL)
    {
        cout<<"List is empty"<<endl;
        return 0;
    }
    s = blockingListBegin;
    while (s != NULL)
    {
        if (strcmp(s->blockedClientIP,searchblockedIP)==0 && strcmp(s->blockerClientIP,searchblockerIP)==0)
        {
            c=1;
            return 1;
            
        }
        s = s->next;
    }
    if (c==0)
    {
        
        return 0;
    }
    else
    {
        return 1;
    }
    
    
}



//---------------//---------------------------------------------------------blocking strcuts and functions ends-----------------------------//


//stats---------------------------------------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------Message Buffering--------------------------------------------------//

struct messageBuffer
{
    char *senderIP;
    char *receiverIP;
    char *message;
    int seqNo;
    struct messageBuffer * next;
    
}*messageBufferListStart;

void insertMessageBuffer(char *senderIP,char *receiverIP,char *message, int seqNo)///inserting at end
{
    struct messageBuffer *tempNode, *s;
    tempNode =  (struct messageBuffer *) malloc(sizeof(struct messageBuffer));
    if (tempNode == NULL)
    {
        printf("Memory allocated null\n");
        return ;
    }
    else
    {
        tempNode->senderIP = static_cast<char *> (malloc (MAXSIZE));
        tempNode->receiverIP = static_cast<char *>(malloc(MAXSIZE));
        tempNode->message = static_cast<char *>(malloc(MAXSIZE));
        
        strcpy(tempNode->senderIP, senderIP);
        strcpy(tempNode->receiverIP, receiverIP);
        strcpy(tempNode->message, message);
        tempNode->seqNo=seqNo;
        tempNode->next = NULL;
    }
    
    if (messageBufferListStart == NULL)
    {
        messageBufferListStart = tempNode;
        messageBufferListStart->next = NULL;
        return;
    }
    
    
    else{
        s = messageBufferListStart;
        while (s->next != NULL)
        {
            s = s->next;
        }
        tempNode->next = NULL;
        s->next = tempNode;
        cout<<"Element Inserted at last"<<endl;
    }
    
}

char * relayMessageBuffer(char *receiverIP) //refine this yet
{
    
    
    char searchIP[30];
    strcpy(searchIP, receiverIP);
    char  serialBuff[2000];
    bzero(serialBuff, 2000);
    char finalBuff[2000];
    bzero(finalBuff, 2000);
    struct messageBuffer *tempNode;
    if (messageBufferListStart == NULL)
    {
        printf("Empty List\n");
        return NULL;
    }
    tempNode = messageBufferListStart;
    while (tempNode != NULL)
    {
        if (strcmp(tempNode->receiverIP,receiverIP)==0) {
            sprintf(serialBuff, "%s*%s*%d*", tempNode->senderIP, tempNode->message,tempNode->seqNo);
            strcat(finalBuff, serialBuff);
            strcpy(tempNode->receiverIP, "0.0.0.0");
        }
        tempNode = tempNode->next;
    }
    
    size_t indexOfNullTerminator = strlen(finalBuff);
    finalBuff[indexOfNullTerminator - 1] = '\0';
    
    
    
    return finalBuff;
    
    
}



//-------------------------------------------------------------------------------Message Buffering--------------------------------------------------//


/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
int main(int argc, char **argv)
{
    /*Init. Logger*/
    cse4589_init_log(argv[2]);
    
    /* Clear LOGFILE*/
    fclose(fopen(LOGFILE, "w"));
    
    /*Start Here*/
    
    //My code starts here//
    
    if(argc<3)
    {
        printf("Number of arguments should be 3");
        exit(1);
    }
    char mode[2];
    char portNum[6];
    strcpy(mode,argv[1]);
    strcpy(portNum,argv[2]);
    printf("mode is %s\n",mode);
    
    int sockfd;
    struct addrinfo *serverdata;
    fd_set master; // master file descriptor list
    fd_set read_fds; // temp file descriptor list for select()
    int fdmax; // maximum file descriptor number
    int listener; // listening socket descriptor
    int newfd; // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;
    char buf[256]; // buffer for client data
    int nbytes;
    char remoteIP[INET6_ADDRSTRLEN];
    int yes=1; // for setsockopt() SO_REUSEADDR, below
    int i, rv;
    struct addrinfo hints, *ai, *p;
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);
    
    // write logic for port validity check
    
    if (mode[0]=='s')
    {
        //start logic for creating a server
        
        printf("Started as server\n");
        
        int args = 0;
        string serverconsoleInput;
        char serverCommand[50];
        char commandfromClient[50];
        char host[1024];
        char service[20];
        
        // get us a socket and bind it
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        if ((rv = getaddrinfo(NULL, portNum, &hints, &ai)) != 0) {
            fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
            exit(1);
        }
        
        for(p = ai; p != NULL; p = p->ai_next) {
            listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (listener < 0) {
                continue;
            }
            
            // lose the pesky "address already in use" error message
            setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
            if (::bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
                close(listener);
                continue;
            }
            break;
        }
        // if we got here, it means we didn't get bound
        if (p == NULL) {
            fprintf(stderr, "selectserver: failed to bind\n");
            exit(2);
        }
        freeaddrinfo(ai); // all done with this
        // listen
        if (listen(listener, 10) == -1) {
            perror("listen");
            exit(3);
        }
        // add the listener to the master set
        FD_SET(listener, &master);
        FD_SET(STDIN, &master);//add the stdin socket to master
        // keep track of the biggest file descriptor
        fdmax = listener; // so far, it's this one
        // main loop
        for(;;) {
            read_fds = master;// copy it
            if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
                perror("select");
                exit(4);
            }
            char *arrayofTokens[25];
            
            for (i = 0 ; i< 25 ; i++)
            {
                arrayofTokens[i] = (char*)malloc( 1024 * sizeof(char));
            }
            for(i = 0; i <= fdmax; i++) {
                if (FD_ISSET(i, &read_fds)) { // we got one!!
                    if (i == listener)
                    {
                        // handle new connections
                        addrlen = sizeof remoteaddr;
                        newfd = accept(listener,
                                       (struct sockaddr *)&remoteaddr,
                                       &addrlen);
                        if (newfd == -1) {
                            perror("accept");
                        } else {
                            FD_SET(newfd, &master); // add to master set
                            if (newfd > fdmax) { // keep track of the max
                                fdmax = newfd;
                            }
                            printf("selectserver: new connection from %s on "
                                   "socket %d\n",
                                   inet_ntop(remoteaddr.ss_family,get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN),newfd);
                            
                            getnameinfo((struct sockaddr *)&remoteaddr, sizeof(remoteaddr), host, sizeof(host), service, sizeof(service), 0);
                            
                            
                        }
                    }
                    else if(i==STDIN)
                    {
                        //LOGIC for commands at server side
                        
                        getline(std::cin, serverconsoleInput);
                        char *token;
                        char *ptr;
                        int args = 0;
                        ptr= strdup(serverconsoleInput.c_str());
                        
                        token = strtok(ptr, " ");
                        while(token!=NULL)
                        {
                            strcpy(arrayofTokens[args], token);
                            args++;
                            token=strtok(NULL, " ");
                        }
                        strcpy(serverCommand,arrayofTokens[0]);
                        int uppercaseCheck=0;
                        for(int i=0;i< strlen(serverCommand);i++)
                        {
                            if(!(isupper(serverCommand[i])))
                                uppercaseCheck=1;
                        }
                        
                        if(uppercaseCheck==1)
                            cout<<"Only commands in uppercase will be accepted"<<"\n";
                        else
                        {
                            if(strcmp(serverCommand,"AUTHOR")==0)
                            {
                                // LOGIC for Author
                                cse4589_print_and_log("[%s:SUCCESS]\n", serverCommand);
                                cse4589_print_and_log("I, armaango, have read and understood the course academic integrity policy.\n");
                                cse4589_print_and_log("[%s:END]\n",serverCommand);
                            }
                            else if(strcmp(serverCommand,"LIST")==0)
                            {
                                //LOGIC for LIST
                                
                                int callingflag=1;
                                
                                cse4589_print_and_log("[%s:SUCCESS]\n", serverCommand);
                                displayClientList(callingflag);
                                cse4589_print_and_log("[%s:END]\n",serverCommand);
                                
                                //deserialise(max);
                                
                            }
                            else if (strcmp(serverCommand,"PORT")==0)
                            {
                                //Logic for PORT
                                cse4589_print_and_log("[%s:SUCCESS]\n", serverCommand);
                                cse4589_print_and_log("PORT:%d\n",atoi(portNum));
                                cse4589_print_and_log("[%s:END]\n",serverCommand);
                                
                            }
                            else if(strcmp(serverCommand,"IP")==0)
                            {
                                //Logic for IP
                                struct addrinfo addrforIP;
                                struct addrinfo * addrtoCheckIP;
                                char * temMsg="GET / HTTP/1.1\nhost: www.google.com\n\n";
                                memset(&addrforIP, 0, sizeof(addrforIP));
                                addrforIP.ai_family=AF_INET;
                                addrforIP.ai_socktype=SOCK_DGRAM;
                                getaddrinfo("8.8.8.8", "53", &addrforIP, &addrtoCheckIP);
                                int ipsocket=socket(addrtoCheckIP->ai_family, addrtoCheckIP->ai_socktype, addrtoCheckIP->ai_protocol);
                                if(ipsocket==-1)
                                {
                                    perror("checking IP at server");
                                }
                                int o=sendto(ipsocket, temMsg, strlen(temMsg), 0, addrtoCheckIP->ai_addr, addrtoCheckIP->ai_addrlen);
                                if(o==-1)
                                {
                                    perror("checking IP at server IP send");
                                }
                                
                                struct sockaddr sockaddrtocheckIP;
                                socklen_t socklentocheckIP;
                                socklentocheckIP=sizeof(sockaddrtocheckIP);
                                
                                getsockname(ipsocket, &sockaddrtocheckIP, &socklentocheckIP);
                                struct sockaddr_in *sockaddr_intoCheckIP;
                                sockaddr_intoCheckIP=(struct sockaddr_in *)&sockaddrtocheckIP;
                                char ExternalIP[30];
                                inet_ntop(AF_INET,&sockaddr_intoCheckIP->sin_addr,ExternalIP,sizeof(ExternalIP));
                                
                                cse4589_print_and_log("[%s:SUCCESS]\n", serverCommand);
                                cse4589_print_and_log("IP:%s\n",ExternalIP);
                                cse4589_print_and_log("[%s:END]\n",serverCommand);
                                
                                
                                
                            }
                            else if(strcmp(serverCommand,"BLOCKED")==0)
                            {
                                //Logic for BLOCKED
                                
                                char IPtocheck[30];
                                strcpy(IPtocheck,arrayofTokens[1]);
                                sockaddr_in tempSockaddr;
                                int validIPflag=0;
                                int r = inet_pton(AF_INET, IPtocheck, &(tempSockaddr.sin_addr));
                                if(r==0)
                                {
                                    printf("Invalid IP\n");
                                    validIPflag=1;
                                    cse4589_print_and_log("[%s:ERROR]\n", serverCommand);
                                    cse4589_print_and_log("[%s:END]\n",serverCommand);
                                }
                                if (validIPflag==0)
                                {
                                    client * clientData;
                                    clientData=clientSearch(arrayofTokens[1],1);
                                    if(clientData!=NULL){
                                        int sockfd=clientData->clientSocket;
                                        
                                        cse4589_print_and_log("[%s:SUCCESS]\n", serverCommand);
                                        int b=send(sockfd, "BLIST" , strlen("BLIST"), 0);
                                    }
                                    else
                                    {
                                        printf("No clients connected\n");
                                        cse4589_print_and_log("[%s:ERROR]\n", serverCommand);
                                        cse4589_print_and_log("[%s:END]\n",serverCommand);
                                        
                                    }
                                }
                                
                            }
                            else if(strcmp(serverCommand,"STATISTICS")==0)
                            {
                                cse4589_print_and_log("[%s:SUCCESS]\n", serverCommand);
                                displaystats();
                                cse4589_print_and_log("[%s:END]\n",serverCommand);
                            }
                            else
                            {
                                printf("Please enter a valid command\n");
                            }
                            
                        }
                    }//end of else for handling server side commands
                    
                    else {
                        // handle data from a client
                        if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                            
                            // got error or connection closed by client
                            if (nbytes == 0) {
                                // connection closed
                                printf("selectserver: socket %d hung up\n", i);
                            } else {
                                perror("recv");
                            }
                            close(i); // bye!
                            FD_CLR(i, &master); // remove from master set
                        }
                        else
                        {
                            
                            //tokenise the received data
                            
                            char *token;
                            char *ptr;
                            int args = 0;
                            token = strtok(buf, "_");
                            while(token!=NULL)
                            {
                                strcpy(arrayofTokens[args], token);
                                args++;
                                token=strtok(NULL, "_");
                            }
                            bzero(buf, 256);
                            strcpy(commandfromClient,arrayofTokens[0]);
                            if (strcmp(commandfromClient,"LOGIN")==0)
                            {
                                char * portToInsert=arrayofTokens[1];
                                char buffertoSend[400];
                                insertClient(remoteIP, host, atoi(portToInsert), i,1); //inserting into client list function
                                
                                
                                stats * checkforLogin = searchStats(remoteIP);
                                if(checkforLogin==NULL)
                                {
                                    insertStats(remoteIP, host, "online",  atoi(portToInsert), 0,0);
                                }
                                else if(strcmp(checkforLogin->status,"offline")==0){
                                    updateStats(remoteIP, 3);
                                }
                                
                                //displayClientList(1);
                                char * max=serialise(1);
                                bzero(buffertoSend,400);
                                strcpy(buffertoSend, "LIST_");
                                strcat(buffertoSend, max);
                                
                                
                                int b=send(i, buffertoSend , strlen(buffertoSend), 0);
                                
                                bzero(buffertoSend,400);
                                
                                
                            }
                            else if(strcmp(commandfromClient,"SEND")==0)
                            {
                                //LOGIC to relay message
                                char *IPtoSendMessage=arrayofTokens[1];
                                char messagetobesent[256];
                                strcpy(messagetobesent,arrayofTokens[2]);
                                char *IPsender=clientSearchforIP(i);
                                
                                updateStats(IPsender, 1);
                                
                                
                                
                                
                                //go create a linked list of logged in Clients
                                //search the list for the sockfd of the IPtoSendMessage
                                //get the entire struct and then extract sockfd
                                client * clientData;
                                int sockettoSend;
                                clientData=clientSearch(IPtoSendMessage,1);
                                if(clientData!=NULL)
                                {
                                    sockettoSend=clientData->clientSocket;
                                }
                                
                                
                                int check=searchBlockedClient(IPsender, IPtoSendMessage);
                                
                                
                                if (check==1) {
                                    printf("%s is blocked by %s\n",IPsender,IPtoSendMessage);
                                }
                                else if(check==0)
                                {
                                    
                                    
                                    stats * statsDataforstatus=searchStats(IPtoSendMessage);
                                    
                                    if (strcmp(statsDataforstatus->status,"offline")==0)
                                    {
                                        
                                        insertMessageBuffer(IPsender, IPtoSendMessage, messagetobesent, 0);
                                        
                                    }
                                    else if(strcmp(statsDataforstatus->status,"online")==0)
                                    {
                                        
                                        if(sockettoSend>0)
                                        {
                                            char finalmessageBuff[300];
                                            bzero(finalmessageBuff, 300);
                                            sprintf(finalmessageBuff, "SEND_%s_%s",IPsender,messagetobesent);
                                            int b=send(sockettoSend, finalmessageBuff , strlen(finalmessageBuff), 0);
                                            
                                            bzero(finalmessageBuff, 300);
                                            if (b>0) {
                                                cse4589_print_and_log("[RELAYED:SUCCESS]\n");
                                                cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n", IPsender, IPtoSendMessage, messagetobesent);
                                                cse4589_print_and_log("[RELAYED:END]\n");
                                                updateStats(IPtoSendMessage,2);
                                                
                                            }
                                        }
                                        
                                        
                                    }
                                }
                                
                                
                            }
                            else if(strcmp(commandfromClient,"REFRESH")==0)
                            {
                                char * max=serialise(1);
                                char buffertoSend[400];
                                bzero(buffertoSend,400);
                                strcpy(buffertoSend, "LIST_");
                                strcat(buffertoSend, max);
                                int b=send(i, buffertoSend , strlen(buffertoSend), 0);
                                bzero(buffertoSend,400);
                                
                            }
                            else if(strcmp(commandfromClient,"BLOCK")==0)
                            {
                                //LOGIC to BLOCK IP
                                
                                char *IPsender=clientSearchforIP(i);
                                char blockedClientIP[30];
                                char blockerClientIP[30];
                                strcpy(blockedClientIP,arrayofTokens[1]);
                                strcpy(blockerClientIP,IPsender);
                                insertBlockedClient(blockedClientIP, blockerClientIP, 0, i);
                                bzero(blockedClientIP, 30);
                                bzero(blockerClientIP, 30);
                                
                            }
                            
                            else if(strcmp(commandfromClient,"SENDBUFFER")==0)
                            {
                                
                                char messageBufferToSend[2000];
                                bzero(messageBufferToSend,2000);
                                char *IPsender=clientSearchforIP(i);
                                char * msgBuf= relayMessageBuffer(IPsender);
                                
                                //strcpy(messageBufferToSend, msgBuf);
                                sprintf(messageBufferToSend, "MGBUF_%s\n",msgBuf);
                                if(msgBuf!=NULL){
                                    char serverSideCopyofMessageBuffer[5000];
                                    bzero(serverSideCopyofMessageBuffer,5000);
                                    strcpy(serverSideCopyofMessageBuffer,msgBuf);
                                    
                                    
                                    deserialiseMessageBuffer(serverSideCopyofMessageBuffer,2,IPsender);
                                    
                                    int y=send(i, messageBufferToSend , strlen(messageBufferToSend), 0);
                                    
                                }
                                else
                                {
                                    printf("No buffered messages to send\n");
                                }
                                bzero(messageBufferToSend,400);
                                
                            }
                            
                            else if(strcmp(commandfromClient,"UNBLOCK")==0)
                            {
                                //LOGIC to BLOCK IP
                                
                                char *IPsender=clientSearchforIP(i);
                                char blockedClientIP[30];
                                char blockerClientIP[30];
                                strcpy(blockedClientIP,arrayofTokens[1]);
                                strcpy(blockerClientIP,IPsender);
                                deleteBlockedClient(blockedClientIP, blockerClientIP);
                                bzero(blockedClientIP, 30);
                                bzero(blockerClientIP, 30);
                            }
                            else if(strcmp(commandfromClient,"BLIST")==0)
                            {
                                blockedClientListBegin=NULL;
                                
                                deserialise(arrayofTokens[1],2);
                                displayClientList(2);
                                cse4589_print_and_log("[BLOCKED:END]\n");
                                
                                
                                
                            }
                            else if(strcmp(commandfromClient,"BROADCAST")==0)
                            {
                                bzero(buf,256);
                                strcpy(buf, arrayofTokens[1]);
                                char bcastBuffer[1000];
                                bzero(bcastBuffer, 1000);
                                char IPofBroadcaster[30];
                                strcpy(IPofBroadcaster,clientSearchforIP(i));
                                
                                sprintf(bcastBuffer, "BCAST_%s_%s",IPofBroadcaster,buf);
                                updateStats(IPofBroadcaster, 1);
                                int relayflag=0;
                                
                                
                                for(int j = 1; j <= fdmax; j++)
                                {
                                    // send to everyone!
                                    if (FD_ISSET(j, &master))
                                    {
                                        // except the listener and ourselves
                                        if (j != listener && j!=i)
                                        {
                                            sockaddr_storage cAddress;
                                            socklen_t cLength;
                                            char IPofremoteforBCast[30];
                                            
                                            
                                            getpeername(j, (struct sockaddr *) &cAddress, &cLength);
                                            if(cAddress.ss_family == AF_INET)
                                            {
                                                struct sockaddr_in *pN = (struct sockaddr_in *) &cAddress;
                                                inet_ntop(AF_INET, &pN->sin_addr, IPofremoteforBCast, sizeof (IPofremoteforBCast));
                                            }
                                            else
                                            {
                                                struct sockaddr_in6 *pN = (struct sockaddr_in6 *) &cAddress;
                                                //clientPort = ntohs(peername->sin6_port);
                                                inet_ntop(AF_INET, &pN->sin6_addr, IPofremoteforBCast, sizeof (IPofremoteforBCast));
                                            }
                                            
                                            char *IPofrecv=clientSearchforIP(j);
                                            
                                            
                                            
                                            if((IPofremoteforBCast != NULL) && (IPofremoteforBCast[0] != '\0'))
                                            {
                                                int check=searchBlockedClient(IPofBroadcaster, IPofremoteforBCast);
                                                if (check==1)
                                                {
                                                    printf("%s is blocked by %s\n",IPofBroadcaster,IPofremoteforBCast);
                                                }
                                                else if(check==0)
                                                {
                                                    
                                                    stats * statsDataforstatus=searchStats(IPofremoteforBCast);
                                                    
                                                    if (strcmp(statsDataforstatus->status,"offline")==0)
                                                    {
                                                        
                                                        insertMessageBuffer(IPofBroadcaster, IPofremoteforBCast, buf, 1);
                                                        
                                                    }
                                                    else if(strcmp(statsDataforstatus->status,"online")==0)
                                                    {
                                                        if (send(j, bcastBuffer, strlen(bcastBuffer), 0) == -1)
                                                        {
                                                            perror("send");
                                                        }
                                                        else{
                                                            relayflag=1;
                                                            updateStats(IPofremoteforBCast,2);
                                                            
                                                        }
                                                    }
                                                    
                                                }
                                            }
                                            
                                        }
                                    }
                                }
                                if (relayflag==1) {
                                    cse4589_print_and_log("[RELAYED:SUCCESS]\n");
                                    cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n", IPofBroadcaster, "255.255.255.255", buf);
                                    cse4589_print_and_log("[RELAYED:END]\n");
                                }
                                else
                                {
                                    cse4589_print_and_log("[RELAYED:ERROR]\n");
                                    
                                    cse4589_print_and_log("[RELAYED:END]\n");
                                    
                                }
                                bzero(buf,256);
                                bzero(bcastBuffer, 1000);
                            }
                            else if(strcmp(commandfromClient,"LOGOUT")==0)
                            {
                                //LOGIC to logout
                                char *IPsender=clientSearchforIP(i);
                                
                                deleteClient(IPsender,1);
                                
                                updateStats(IPsender, 3);
                                
                            }
                            else if(strcmp(commandfromClient,"EXIT")==0)
                            {
                                //LOGIC to logout
                                char *IPsender=clientSearchforIP(i);
                                
                                deleteClient(IPsender,1);
                                
                                updateStats(IPsender, 3);
                                deleteStats(IPsender);
                                
                                close(i); // bye!
                                FD_CLR(i, &master);
                            }// remove from master set
                            
                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    }
    
    //-------------------------------------------------------------Client code starts here----------------------------------//
    
    else if(mode[0]=='c')
    {
        //start logic to create a client
        printf("Started as client\n");
        string consoleInput;
        char command[50];
        int refreshflag;
        
        
        int loginFlag=0;
        fdmax=0;
        FD_ZERO(&read_fds);
        FD_ZERO(&master);
        FD_SET(STDIN,&master);
        while (1)
        {
            
            read_fds=master;
            select(fdmax+1, &read_fds, NULL, NULL, NULL);
            if(FD_ISSET(STDIN, &read_fds))
            {
                
                getline(std::cin, consoleInput);
                // http://stackoverflow.com/questions/4008933/memory-allocation-for-array-of-pointers //
                
                char *arrayofTokens[25];
                
                for (i = 0 ; i< 25 ; i++)
                {
                    arrayofTokens[i] = (char*)malloc( 1024 * sizeof(char));
                }
                char *token;
                char *ptr;
                int args = 0;
                ptr= strdup(consoleInput.c_str());
                token = strtok(ptr, " ");
                while(token!=NULL)
                {
                    strcpy(arrayofTokens[args], token);
                    args++;
                    token=strtok(NULL, " ");
                }
                strcpy(command,arrayofTokens[0]);
                int uppercaseCheck=0;
                for(int i=0;i< strlen(command);i++)
                {
                    if(!(isupper(command[i])))
                        uppercaseCheck=1;
                }
                
                if(uppercaseCheck==1)
                    printf("PLEASE enter the commands in uppercase only\n");
                else
                {
                    
                    if(strcmp(command,"LOGIN")==0)
                    {
                        
                        // LOGIC for LOGIN
                        
                        
                        
                        //checking for valid IP and port
                        char serverIPtoLogin[30];
                        int serverPorttoLogin;
                        strcpy(serverIPtoLogin,arrayofTokens[1]);
                        serverPorttoLogin=atoi(arrayofTokens[2]);
                        sockaddr_in tempSockaddr;
                        int validIPPortFlag=0;
                        int r = inet_pton(AF_INET, serverIPtoLogin, &(tempSockaddr.sin_addr));
                        if(r==0)
                        {
                            printf("Invalid IP\n");
                            validIPPortFlag=1;
                        }
                        if(!(serverPorttoLogin>1024 && serverPorttoLogin<65535))
                        {
                            printf("Invalid Port\n");
                            validIPPortFlag=1;
                        }
                        
                        if (validIPPortFlag==0)
                        {
                            
                            struct addrinfo hints, *servinfo, *p;
                            char s[INET6_ADDRSTRLEN];
                            memset(&hints, 0, sizeof hints);
                            hints.ai_family = AF_UNSPEC;
                            hints.ai_socktype = SOCK_STREAM;
                            if ((rv = getaddrinfo(arrayofTokens[1], arrayofTokens[2], &hints, &servinfo)) != 0) {
                                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                                return 1;
                            }
                            // loop through all the results and connect to the first we can
                            for(p = servinfo; p != NULL; p = p->ai_next) {
                                if ((sockfd = socket(p->ai_family, p->ai_socktype,
                                                     p->ai_protocol)) == -1) {
                                    perror("client: socket");
                                    continue;
                                }
                                if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                                    close(sockfd);
                                    perror("client: connect");
                                    continue;
                                }
                                break;
                            }
                            if (p == NULL) {
                                fprintf(stderr, "client: failed to connect\n");
                            }
                            inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                                      s, sizeof s);
                            printf("client: connecting to %s\n", s);
                            freeaddrinfo(servinfo);
                            
                            //connection created//
                            
                            
                            char finalBuffer[40];
                            bzero(finalBuffer,40);
                            
                            sprintf(finalBuffer,"LOGIN_%s",portNum);
                            int nbytes = send(sockfd,finalBuffer,strlen(finalBuffer),0);
                            
                            
                            if (nbytes < 0)
                            {
                                perror("ERROR writing to socket");
                            }
                            bzero(finalBuffer,40);
                            
                            cse4589_print_and_log("[%s:SUCCESS]\n", command);
                            
                            loginFlag=1;
                            refreshflag=0;
                            FD_SET(sockfd,&master);
                            if (sockfd > fdmax) { // keep track of the max
                                fdmax = sockfd;
                            }
                            
                        }
                        else {
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                        }
                        cse4589_print_and_log("[%s:END]\n", command);
                    }
                    
                    else if(strcmp(command,"SEND")==0)
                    {
                        
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                            
                        }
                        else
                        {
                            
                            //checking for valid IP
                            char serverIPtoLogin[30];
                            strcpy(serverIPtoLogin,arrayofTokens[1]);
                            sockaddr_in tempSockaddr;
                            int validIPPortFlag=0;
                            int r = inet_pton(AF_INET, serverIPtoLogin, &(tempSockaddr.sin_addr));
                            if(r==0)
                            {
                                printf("IP NOT IN CORRECT FORMAT\n");
                                
                                validIPPortFlag=1;
                            }
                            if (validIPPortFlag==0) {
                                
                                client * clientData1;
                                clientData1=clientSearch(arrayofTokens[1],1);
                                if (clientData1==NULL) {
                                    printf("Valid but incorrect/non-existent IP address\n");
                                }
                                else
                                {
                                    //Logic for Send
                                    char buffertoSend[300];
                                    char *IPtosend = arrayofTokens[1];
                                    strcpy(buffertoSend, arrayofTokens[2]);
                                    
                                    for(int i=3;i<=args;i++)
                                    {
                                        strcat(buffertoSend, " ");
                                        strcat(buffertoSend,arrayofTokens[i]);
                                        
                                    }
                                    char finalBuffer[400];
                                    bzero(finalBuffer,400);
                                    sprintf(finalBuffer,"SEND_%s_%s",IPtosend,buffertoSend);
                                    int nbytes = send(sockfd,finalBuffer,strlen(finalBuffer),0);
                                    
                                    
                                    if (nbytes < 0)
                                    {
                                        perror("ERROR writing to socket");
                                        cse4589_print_and_log("[%s:ERROR]\n",command);
                                    }
                                    if (nbytes>0) {
                                        cse4589_print_and_log("[%s:SUCCESS]\n",command);
                                    }
                                    bzero(finalBuffer,400);
                                }
                                
                            }
                            else if(validIPPortFlag==1)
                            {
                                cse4589_print_and_log("[%s:ERROR]\n",command);
                            }
                        }
                        cse4589_print_and_log("[%s:END]\n",command);
                        
                    }
                    else if(strcmp(command,"BLOCK")==0)
                    {
                        
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                        }
                        //Logic for Block
                        
                        else
                        {
                            char serverIPtoLogin[30];
                            strcpy(serverIPtoLogin,arrayofTokens[1]);
                            sockaddr_in tempSockaddr;
                            int validIPPortFlag=0;
                            int r = inet_pton(AF_INET, serverIPtoLogin, &(tempSockaddr.sin_addr));
                            if(r==0)
                            {
                                printf("Invalid IP\n");
                                cse4589_print_and_log("[%s:ERROR]\n",command);
                                validIPPortFlag=1;
                            }
                            if (validIPPortFlag==0) {
                                client * clientData1;
                                clientData1=clientSearch(arrayofTokens[1],1);
                                if (clientData1==NULL) {
                                    printf("Valid but incorrect/non-existent IP address\n");
                                    cse4589_print_and_log("[%s:ERROR]\n",command);
                                    
                                }
                                else
                                {
                                    client * clientData2;
                                    clientData2=clientSearch(arrayofTokens[1],2);
                                    if (clientData2==NULL)
                                    {
                                        insertClient(clientData1->clientIP, clientData1->clientname, clientData1->clientPort, clientData1->clientSocket,2);
                                        cse4589_print_and_log("[%s:SUCCESS]\n",command);
                                        //displayClientList(2);
                                        char finalBuffer[400];
                                        bzero(finalBuffer,400);
                                        //                                    strcpy(finalBuffer,"BLOCK_");
                                        //                                    strcat(finalBuffer, arrayofTokens[1]);
                                        sprintf(finalBuffer,"BLOCK_%s",arrayofTokens[1]);
                                        int nbytes = send(sockfd,finalBuffer,strlen(finalBuffer),0);
                                        if (nbytes < 0)
                                        {
                                            perror("ERROR writing to socket");
                                        }
                                        bzero(finalBuffer,400);
                                    }
                                    else
                                    {
                                        printf("Client with IP address is already blocked\n");
                                    }
                                }
                            }
                            else if(validIPPortFlag==1)
                            {
                                cse4589_print_and_log("[%s:ERROR]\n",command);
                            }
                        }
                        
                        cse4589_print_and_log("[%s:END]\n",command);
                    }
                    else if(strcmp(command,"UNBLOCK")==0)
                    {
                        
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                        }
                        //Logic for UNBlock
                        
                        else
                        {
                            char serverIPtoLogin[30];
                            strcpy(serverIPtoLogin,arrayofTokens[1]);
                            sockaddr_in tempSockaddr;
                            int validIPPortFlag=0;
                            int r = inet_pton(AF_INET, serverIPtoLogin, &(tempSockaddr.sin_addr));
                            if(r==0)
                            {
                                printf("Invalid IP\n");
                                cse4589_print_and_log("[%s:ERROR]\n",command);
                                validIPPortFlag=1;
                            }
                            if (validIPPortFlag==0)
                            {
                                client * clientData1;
                                clientData1=clientSearch(arrayofTokens[1],1);
                                if (clientData1==NULL)
                                {
                                    printf("Valid but incorrect/non-existent IP address\n");
                                    cse4589_print_and_log("[%s:ERROR]\n",command);
                                    
                                }
                                else
                                {
                                    client * clientData2;
                                    clientData2=clientSearch(arrayofTokens[1],2);
                                    if (clientData2==NULL)
                                    {
                                        printf("Client with IP address: %s  is not blocked\n",arrayofTokens[1]);
                                        cse4589_print_and_log("[%s:ERROR]\n",command);
                                    }
                                    else
                                    {
                                        deleteClient(arrayofTokens[1], 2);
                                        cse4589_print_and_log("[%s:SUCCESS]\n",command);
                                        char finalBuffer[400];
                                        bzero(finalBuffer,400);
                                        //                                    strcpy(finalBuffer,"UNBLOCK_");
                                        //                                    strcat(finalBuffer, arrayofTokens[1]);
                                        sprintf(finalBuffer,"UNBLOCK_%s",arrayofTokens[1]);
                                        int nbytes = send(sockfd,finalBuffer,strlen(finalBuffer),0);
                                        if (nbytes < 0)
                                        {
                                            perror("ERROR writing to socket");
                                        }
                                        bzero(finalBuffer,400);
                                        
                                    }
                                }
                            }
                        }
                        cse4589_print_and_log("[%s:END]\n",command);
                    }
                    else if(strcmp(command,"PORT")==0)
                    {
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                            
                        }
                        else
                        {
                            cse4589_print_and_log("[%s:SUCCESS]\n",command);
                            cse4589_print_and_log("PORT:%s\n",portNum);
                        }
                        cse4589_print_and_log("[%s:END]\n",command);
                    }
                    else if(strcmp(command,"IP")==0)
                    {
                        
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                            
                        }
                        else
                        {
                            
                            sockaddr_storage cAddress;
                            socklen_t cLength;
                            char myExternalIP[30];
                            
                            
                            getsockname(sockfd, (struct sockaddr *) &cAddress, &cLength);
                            if(cAddress.ss_family == AF_INET)
                            {
                                struct sockaddr_in *pN = (struct sockaddr_in *) &cAddress;
                                inet_ntop(AF_INET, &pN->sin_addr, myExternalIP, sizeof (myExternalIP));
                            }
                            else
                            {
                                struct sockaddr_in6 *pN = (struct sockaddr_in6 *) &cAddress;
                                inet_ntop(AF_INET, &pN->sin6_addr, myExternalIP, sizeof (myExternalIP));
                            }
                            
                            cse4589_print_and_log("[%s:SUCCESS]\n",command);
                            cse4589_print_and_log("IP:%s\n", myExternalIP);
                            
                        }
                        cse4589_print_and_log("[%s:END]\n",command);
                        
                    }
                    else if(strcmp(command,"AUTHOR")==0)
                    {
                        cse4589_print_and_log("[%s:SUCCESS]\n",command);
                        cse4589_print_and_log("I, armaango, have read and understood the course academic integrity policy.\n");
                        cse4589_print_and_log("[%s:END]\n",command);
                        
                    }
                    else if(strcmp(command,"LIST")==0)
                    {
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                        }
                        else
                        {
                            
                            
                            cse4589_print_and_log("[%s:SUCCESS]\n",command);
                            displayClientList(1);
                            cse4589_print_and_log("[%s:END]\n",command);
                            
                        }
                        //LOGIC for LIST
                    }
                    else if(strcmp(command,"REFRESH")==0)
                    {
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                        }
                        else
                        {
                            cse4589_print_and_log("[%s:SUCCESS]\n",command);
                            
                            char finalBuffer[400];
                            bzero(finalBuffer,400);
                            strcpy(finalBuffer,"REFRESH");
                            int nbytes = send(sockfd,finalBuffer,strlen(finalBuffer),0);
                            if (nbytes < 0)
                            {
                                perror("ERROR writing to socket");
                            }
                            bzero(finalBuffer,400);
                            
                        }
                        cse4589_print_and_log("[%s:END]\n",command);
                        //LOGIC for REfresh
                    }
                    else if(strcmp(command,"BROADCAST")==0)
                    {
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                        }
                        else
                        {
                            
                            char buffertoSend[300];
                            bzero(buffertoSend, 300);
                            strcpy(buffertoSend, arrayofTokens[1]);
                            
                            for(int i=2;i<=args;i++)
                            {
                                strcat(buffertoSend, " ");
                                strcat(buffertoSend,arrayofTokens[i]);
                                
                            }
                            char finalBuffer[400];
                            
                            sprintf(finalBuffer,"BROADCAST_%s",buffertoSend);
                            int nbytes = send(sockfd,finalBuffer,strlen(finalBuffer),0);
                            if (nbytes < 0)
                            {
                                perror("ERROR writing to socket");
                                cse4589_print_and_log("[%s:ERROR]\n",command);
                            }
                            if (nbytes >0) {
                                cse4589_print_and_log("[%s:SUCCESS]\n",command);
                            }
                            bzero(finalBuffer,400);
                        }
                        
                        cse4589_print_and_log("[%s:END]\n",command);
                        
                    }
                    else if(strcmp(command,"LOGOUT")==0)
                    {
                        
                        if(loginFlag!=1)//Checking whether logged in or not
                        {
                            printf("You need to LOGIN first\n");
                            cse4589_print_and_log("[%s:ERROR]\n",command);
                        }
                        //LOGIC for LOGOUT
                        
                        else{
                            cse4589_print_and_log("[%s:SUCCESS]\n",command);
                            
                            int nbytes = send(sockfd,"LOGOUT",strlen("LOGOUT"),0);
                            clientListBegin=NULL;
                            //blockedClientListBegin=NULL;
                            loginFlag=0;
                        }
                        cse4589_print_and_log("[%s:END]\n",command);
                        
                    }
                    else if(strcmp(command,"EXIT")==0)
                    {                                                       //modify this logic
                        //LOGIC for EXIT
                        
                        int nbytes = send(sockfd,"EXIT",strlen("EXIT"),0);
                        clientListBegin=NULL;
                        blockedClientListBegin=NULL;
                        cse4589_print_and_log("[%s:SUCCESS]\n",command);
                        cse4589_print_and_log("[%s:END]\n",command);
                        exit(0);
                    }
                    else
                    {
                        printf("Please enter a valid command\n");
                    }
                    
                }//ending the block for execution of commands
            }//ENDING block for reading from stdin
            else if(FD_ISSET(sockfd, &read_fds))
            {
                char recieveBuffer[2000];
                char copyRecieveBuffer[2000];
                bzero(recieveBuffer,2000);
                bzero(copyRecieveBuffer,2000);
                //writing the receive logic for client from server
                int recBytes;
                if ((recBytes = recv(sockfd, recieveBuffer, sizeof(recieveBuffer), 0)) <= 0) {
                    
                    // got error or connection closed by client
                    if (recBytes == 0) {
                        // connection closed
                        printf("selectclient: server %d hung up\n", sockfd);
                    } else {
                        perror("recv");
                    }
                    close(sockfd); // bye!
                    FD_CLR(sockfd, &master); // remove from master set
                }
                
                else
                {
                    
                    strcpy(copyRecieveBuffer,recieveBuffer);
                    
                    size_t indexOfNullTerminator = 5;
                    copyRecieveBuffer[indexOfNullTerminator] = '\0';
                    
                    
                    
                    
                    if (strcmp(copyRecieveBuffer, "LIST_")==0)
                    {
                        //find alternate logic for memory allocation
                        
                        char *arrayofTokens[2];
                        
                        for (i = 0 ; i< 2 ; i++)
                        {
                            arrayofTokens[i] = (char*)malloc( 1024 * sizeof(char));
                        }
                        
                        char *token;
                        int args = 0;
                        token = strtok(recieveBuffer, "_");
                        while(token!=NULL)
                        {
                            strcpy(arrayofTokens[args], token);
                            args++;
                            token=strtok(NULL, "_");
                            
                        }
                        
                        if (refreshflag==1)
                        {
                            clientListBegin=NULL;
                            deserialise(arrayofTokens[1],1);
                            refreshflag=1;
                        }
                        else if(refreshflag==0){
                            deserialise(arrayofTokens[1],1);
                            refreshflag=1;
                            int r=send(sockfd, "SENDBUFFER_NOW", strlen("SENDBUFFER_NOW"), 0);
                        }
                        
                    }
                    else if (strcmp(copyRecieveBuffer, "BLIST")==0)
                    {
                        char msg[1000];
                        bzero(msg,1000);
                        strcpy(msg,serialise(2));
                        
                        char buffertoSend[1000];
                        bzero(buffertoSend,1000);
                        sprintf(buffertoSend, "BLIST_%s",msg );
                        bzero(msg,1000);
                        
                        
                        int d=send(sockfd, buffertoSend, strlen(buffertoSend), 0);
                        bzero(buffertoSend,1000);
                    }
                    else if(strcmp(copyRecieveBuffer, "MGBUF")==0)
                    {
                        char *arrayofTokens[2];
                        
                        for (i = 0 ; i< 2 ; i++)
                        {
                            arrayofTokens[i] = (char*)malloc( 1024 * sizeof(char));
                        }
                        
                        char *token;
                        int args = 0;
                        token = strtok(recieveBuffer, "_");
                        while(token!=NULL)
                        {
                            strcpy(arrayofTokens[args], token);
                            args++;
                            token=strtok(NULL, "_");
                        }
                        char msgBuff[2000];
                        strcpy(msgBuff, arrayofTokens[1]);
                        
                        
                        deserialiseMessageBuffer(msgBuff,1,"NotRequired");
                        
                    }
                    
                    else if(strcmp(copyRecieveBuffer, "SEND_")==0)
                    {
                        
                        char *arrayofTokens[3];
                        
                        for (i = 0 ; i< 3 ; i++)
                        {
                            arrayofTokens[i] = (char*)malloc( 1024 * sizeof(char));
                        }
                        
                        char *token;
                        int args = 0;
                        token = strtok(recieveBuffer, "_");
                        while(token!=NULL)
                        {
                            strcpy(arrayofTokens[args], token);
                            args++;
                            token=strtok(NULL, "_");
                        }
                        cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
                        cse4589_print_and_log("msg from:%s\n[msg]:%s\n", arrayofTokens[1], arrayofTokens[2]);
                        cse4589_print_and_log("[RECEIVED:END]\n");
                        
                    }
                    else if(strcmp(copyRecieveBuffer, "BCAST")==0)// this means a broadcast message
                    {
                        
                        char *arrayofTokens[3];
                        
                        for (i = 0 ; i< 3 ; i++)
                        {
                            arrayofTokens[i] = (char*)malloc( 1024 * sizeof(char));
                        }
                        
                        char *token;
                        int args = 0;
                        token = strtok(recieveBuffer, "_");
                        while(token!=NULL)
                        {
                            strcpy(arrayofTokens[args], token);
                            args++;
                            token=strtok(NULL, "_");
                        }
                        cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
                        cse4589_print_and_log("msg from:%s\n[msg]:%s\n", arrayofTokens[1], arrayofTokens[2]);
                        cse4589_print_and_log("[RECEIVED:END]\n");
                        
                    }
                    
                }
                bzero(copyRecieveBuffer,2000);
                bzero(recieveBuffer,2000);
            }//ending the else if for receiving from the server
        }//ENDING the infinite while loop for select on client side
    }//ending else if for choice as Client
    
    else
    {
        printf("Please choose either s for Server mode or s for Client mode");
    }//Ending else
    
    return 0;
}
