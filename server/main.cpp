#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  printf("Heya! See whos here: The server!\n");
  char domain[] = "bottlecaps.de";

  int status;
  struct addrinfo hints;
  struct addrinfo *results;

  // make sure hints is empty, when initializing structs it fills with garbage
  // data
  memset(&hints, 0, sizeof hints);

  // ai here means address information
  hints.ai_family = AF_UNSPEC;     // dont care about ipv4 or ipv6
  hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM means tdp
  // hints.ai_flags = AI_PASSIVE;     // fill in my ip address, necessary when
  // you're a server

  status = getaddrinfo(domain, NULL, &hints, &results);

  if (status != 0) {
    fprintf(stderr, "Something went wrong: %s", gai_strerror(status));
    return 1;
  }

  // stores the ip string in here. we cosntantly override it though. we just use
  // it as a mean to print it
  char ipstring[INET6_ADDRSTRLEN];

  struct addrinfo *p = results;
  while (p != NULL) {
    void *pointerOfNetworkRepresentation;
    const char *version;

    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;

    // get the pointer to the address it self. we then later have to cast to the
    // approriate sockaddr_in/sockaddr_in6
    // AF_INET = IPV4
    // AF_INET6 = IPV6
    if (p->ai_family == AF_INET) {
      ipv4 = (sockaddr_in *)p->ai_addr;
      pointerOfNetworkRepresentation = &(ipv4->sin_addr);
      version = "IPv4";
    } else {
      ipv6 = (sockaddr_in6 *)p->ai_addr;
      pointerOfNetworkRepresentation = &(ipv6->sin6_addr);
      version = "IPv6";
    }

    // interet network to presentation, we could also do like presentation to
    // network afterwards?
    inet_ntop(p->ai_family, pointerOfNetworkRepresentation, ipstring,
              sizeof ipstring);
    // cool now that we have these information about the domain, we can start
    // making connections

    printf("(%s): %s\n", version, ipstring);

    p = p->ai_next;
  }

  freeaddrinfo(results); // free the linked-list, this is memory cleaning
                         // to avoid leaks

  return 0;
}
