/*==============================================================================

Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt
Copyright 2007-2008 Oleg Fryazinov, Denis Kravtsov
Copyright 2018-2019 Dmitry Popov

This Work or file is part of the greater total Work, software or group of
files named HyperFun Polygonizer.

HyperFun Polygonizer can be redistributed and/or modified under the terms 
of the CGPL, The Common Good Public License as published by and at CGPL.org
(http://CGPL.org).  It is released under version 1.0 Beta of the License
until the 1.0 version is released after which either version 1.0 of the
License, or (at your option) any later version can be applied.

THIS WORK, OR SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED (See the
CGPL, The Common Good Public License for more information.)

You should have received a copy of the CGPL along with HyperFun Polygonizer;  
if not, see -  http://CGPL.org to get a copy of the License.

==============================================================================*/

/* 
 * "client" version of 'hfp'
 *
 * Connect to socket established by external program.
 *
 * Send "0" on socket to indicate completion.
 */

/* Beej's "Simple Stream Client"
 * https://beej.us/guide/bgnet/html/index-wide.html#client-server-background
 */

#include "main.h"
#include "hfp-sock.h"
#include "hfp-sockdata.h"

int vn_t;

void freeRessources (void) 
{
    if(edi!=NULL){
        delete (edi);
        edi=NULL;
    }
}

void *get_in_addr(struct sockaddr *sa)
{
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

int main(int argc, char *argv[])
{
    struct addrinfo *servinfo, *p, hints;
    int rv;
    char s[INET_ADDRSTRLEN];

    edi = new CMainApp (argc, argv);
    
#ifdef _WIN32
    WSADATA wsaData;
    int iResult;

    HF_sockfd = -1;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
#endif 

    // Allow external program to create socket
    sleep(1);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ((rv = getaddrinfo("127.0.0.1", edi->getPort(), 
                          &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((HF_sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
#ifdef _WIN32
            WSACleanup();
#endif
            continue;
        }

        if (connect(HF_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(HF_sockfd);
            perror("client: connect");
#ifdef _WIN32
            WSACleanup();
#endif
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    vn_t = edi->init();
    if (vn_t == 0) {
        if (edi != NULL) {
            delete(edi);
            edi=NULL;
        }
        cout << "Isosurface not generated.\n";
        exit(0);
    }
    
    freeRessources();

    // send "0"
    if (send(HF_sockfd, "0", 1, 0) == -1)
        perror("send");

    close(HF_sockfd);
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
