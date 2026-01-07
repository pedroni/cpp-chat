#include "globals.h"
#include "rand.h"
#include <arpa/inet.h>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define PORT "3490"
#define BACKLOG 10
#define MAX_BYTES 100

int get_listener() {

  int sockfd;

  struct addrinfo hints, *servinfo, *p;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // get addr info of the host, when null is the first arg (hostname)
  // we're telling that the running machine is the host (server)
  // the second arg is servport, serv in here means service.
  // the third arg is where the result will be returned to.
  // it's a linked list that we have to iterate through until we find an
  // appropriate servinfo that we can use.
  int rv;
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  int yes = 1;
  p = servinfo;

  // this loop will look through all of the items in the servinfo linked list
  // until it can find an addrinfo that it can BIND to, once it founds it breaks
  // out of the loop
  for (p = servinfo; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("failed to socket");
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
      perror("failed to setsockopt");
      return -1;
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("failed to bind");
    }
    break;
  }

  // free the memory of the linked list servinfo that was returned from the
  // getaddrinfo function
  freeaddrinfo(servinfo);

  // if there's noting on our pointer from the linked list we have to break out
  // of the loop
  if (p == NULL) {
    printf("Unable to bind to any of the addrinfo in servinfo.");
    return -1;
  }

  if (listen(sockfd, BACKLOG) == -1) {
    perror("failed to listen");
    return -1;
  }

  return sockfd;
}

const char *inet_ntop2(void *addr, char *buf, size_t size) {
  struct sockaddr_storage *sas = (struct sockaddr_storage *)addr;
  struct sockaddr_in *sa4;
  struct sockaddr_in6 *sa6;
  void *src;

  switch (sas->ss_family) {
  case AF_INET:
    sa4 = (struct sockaddr_in *)addr;
    src = &(sa4->sin_addr);
    break;
  case AF_INET6:
    sa6 = (struct sockaddr_in6 *)addr;
    src = &(sa6->sin6_addr);
    break;
  default:
    return NULL;
  }

  return inet_ntop(sas->ss_family, src, buf, size);
}

void remove_poll(std::vector<pollfd> &pollfds, unsigned long &i) {
  printf("removing connection %d\n", pollfds[i].fd);

  close(pollfds[i].fd);

  pollfds[i] = pollfds.back();
  pollfds.pop_back();
  i -= 1;
}

int new_poll(std::vector<pollfd> &pollfds, int sockfd) {
  struct sockaddr_storage remoteaddr; // Client address
  char remoteIP[INET6_ADDRSTRLEN];

  socklen_t addrlen = sizeof remoteaddr;

  // Newly accept()ed socket descriptor
  // if we don't care about the client IP address we can pass NULL as the second
  // parameter and NULL as the third parameter. we could then look up the
  // client's IP address with getpeername, which receives the same arguments,
  // except the first argument is now the newfd and not the sockfd
  int newfd = accept(sockfd, (struct sockaddr *)&remoteaddr, &addrlen);

  if (newfd == -1) {
    perror("accept");
    return -1;
  }

  printf("pollserver: new connection from %s on socket %d\n",
         inet_ntop2(&remoteaddr, remoteIP, sizeof remoteIP), newfd);

  pollfds.push_back({newfd, POLLIN | POLLOUT, 0});
  return newfd;
}

/**
 * sends the received message to everyone
 */
void broadcast_message(int sockfd, std::vector<pollfd> &pollfds,
                       unsigned long &i) {
  char buf[256];
  int numbytes = recv(pollfds[i].fd, buf, sizeof(buf), 0);

  if (numbytes == -1) {
    perror("failed to receive: ");
    remove_poll(pollfds, i);
    return;
  } else {
    // delimit the message
    buf[numbytes] = '\0';
    printf("Received: %s\n", buf);
  }

  // todo: use ranged based to properly remove the client that failed to send to
  for (u64 k = 0; pollfds.size(); k++) {
    pollfd poll = pollfds[k];
    // avoid sending to the same client and to ourselves
    if (poll.fd == sockfd || poll.fd == pollfds[i].fd) {
      continue;
    }

    if (send(poll.fd, buf, numbytes, 0) == -1) {
      perror("failed to send: ");
      remove_poll(pollfds, k);
      continue;
    } else {
      printf("Sent: %s\n", buf);
    }
  }
}

int main() {

  int sockfd = get_listener();
  if (sockfd == -1) {
    printf("Something went wrong. Not able to get a listener.\n");
    return -1;
  }

  std::vector<pollfd> pollfds;
  pollfds.reserve(10);

  pollfds.push_back({sockfd, POLLIN, 0});

  printf("Waiting for connections...\n");
  printf("waiting a random %d\n", Rand::generate());

  // main connect loop, wait for connections?
  while (1) {
    int poll_count =
        poll(pollfds.data(), pollfds.size(), 100); // timeout in 100ms

    for (size_t i = 0; i < pollfds.size(); i++) {
      if (!(pollfds[i].revents & (POLLIN | POLLHUP))) {
        continue;
      }

      // when its the socket we wait for new connections
      if (pollfds[i].fd == sockfd) {
        new_poll(pollfds, sockfd);
      } else {
        // if not the socket, then its a client here we're able to send or recv
        broadcast_message(sockfd, pollfds, i);
      }
    }
  }

  printf("Server is closing.");

  return 0;
}
