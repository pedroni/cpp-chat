if [[ "$ZED_RELATIVE_DIR" == client* ]]; then
    cmake --build build --target ChatClient && ./build/client/ChatClient
elif [[ "$ZED_RELATIVE_DIR" == server* ]]; then
    cmake --build build --target ChatServer && ./build/server/ChatServer
else
    echo 'Not in client or server folder'
fi
