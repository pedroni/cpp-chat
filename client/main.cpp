#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT "3490"
#define MAX_BYTES 100

// void * means pointer of something/unknown thing
void *get_in_addr(struct sockaddr *sa) {
  // AF_INET stands for IPv4 family
  if (sa->sa_family == AF_INET) {
    struct sockaddr_in *sa = ((sockaddr_in *)sa);
    return &sa->sin_addr;
  }

  struct sockaddr_in6 *sa6 = ((sockaddr_in6 *)sa);
  return &sa6->sin6_addr;
}

int main(int argc, char *argv[]) {
  int sockfd, numbytes;
  char buff[MAX_BYTES];

  struct addrinfo hints, *servinfo, *p;
  int rv;
  char humanIpAddress[INET6_ADDRSTRLEN];

  if (argc != 2) {
    printf("usage: ChatClient hostname");
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  rv = getaddrinfo(argv[1], PORT, &hints, &servinfo);
  if (rv != 0) {
    printf("failed to getaddrinfo: %s\n", gai_strerror(rv));
    exit(1);
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {

    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("failed to socket handshake");
      continue;
    }

    if (inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                  humanIpAddress, sizeof humanIpAddress) == NULL) {
      perror("failed to inet_ntop");
      continue;
    }

    printf("Connecting to %s\n", humanIpAddress);

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      perror("failed to connect");
      close(sockfd);
      continue;
    }

    break;
  }

  // clears memory of linked list
  freeaddrinfo(servinfo);

  if (p == NULL) {
    fprintf(stderr, "could not find addressinfo for hostname %s\n", argv[1]);
    exit(2);
  }

  printf("Connected to %s\n", humanIpAddress);

  const char *message = "A message from the client";
  if (send(sockfd, message, strlen(message), 0) == -1) {
    printf("Failed to send\n");
  }

  printf("Now we want to receive it back\n");
  numbytes = recv(sockfd, buff, MAX_BYTES - 1, 0);
  if (numbytes == -1) {
    perror("Failed to recv");
    exit(1);
  }

  buff[numbytes] = '\0';

  printf("I received it back: %s\n", buff);

  close(sockfd);
  exit(0);
}
