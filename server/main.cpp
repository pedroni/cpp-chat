#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT "3490"
#define BACKLOG 10

void sigchld_handler(int s) {
  (void)s;

  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0) {
    // do nothing just wait for the pid (process id?)
  }

  errno = saved_errno;
}

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

int main() {
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
    return 1;
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
      return 1;
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
    return 1;
  }

  if (listen(sockfd, BACKLOG) == -1) {
    perror("failed to listen");
    return 1;
  }

  struct sigaction sa;
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  // assign to "sa"
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    return 1;
  }

  printf("Waiting for connections...\n");

  int newfd;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  char s[INET6_ADDRSTRLEN];

  // main connect loop, wait for connections?
  while (1) {
    sin_size = sizeof their_addr;
    newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (newfd == -1) {
      perror("failed to accept");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              s, sizeof s);
    printf("server: got connection from %s\n", s);

    // fork returns 0 when we succesfully create a child process.
    // it's also 0 when we're in the child process
    // for the parent it will have returned the process id (pid)
    // on error returns -1
    if (!fork()) {
      // in here we're the child process
      close(sockfd); // child doesn't need the listener, it will only send
                     // events from now on
      if (send(newfd, "Hello World", 11, 0) == -1) {
        perror("failed to send");
      }
      printf("sent message");
      close(newfd); // close child
      exit(0); // exit process with empty 0, successfull!, this only exits the
               // child process? thats odd. but i guess its alright. so what
               // happens if we return 0? i think we wont stop the main process
    } else {
      // in here we're on the parent process
      close(newfd); // the parent doesnt care about the child anymore
    }
  }

  return 0;
}
