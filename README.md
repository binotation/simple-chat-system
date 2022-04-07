# Simple server-client chat system
This is a simple, networked, multi-threaded server-client chat system. Only works locally.

Build:
```bash
make
```

## Usage
Server usage: `server authfile [port]`
- The server must be set up with an authentication password contained in the first line of <i>authfile</i>.
- Send SIGHUP to the server for chat session statistics.

Client usage: `client name authfile port`
- The server authenticates the client using the first line of <i>authfile</i>.

The client has access to the following commands
- "*LIST:" - list session participants
- "*KICK:name" - kick another participant
- "*LEAVE:" - leave the chat

### Example
Run server
```bash
./server test-files/auth_file.txt 22427
```

Run client
```bash
./client clientName test-files/auth_file.txt 22427
```
