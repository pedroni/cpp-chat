.PHONY: client server clean

client:
	cmake --build build --target ChatClient && ./build/client/ChatClient

server:
	cmake --build build --target ChatServer && ./build/server/ChatServer

clean:
	rm -rf build && cmake --preset=default
