## Hidden STUN
Hiding data inside the STUN transaction field.

## Overview
Hidden STUN sends Binding Requests to multiple STUN servers where most are real STUN servers and one is a fake STUN server controlled by this project. The fake STUN server returns a normal looking Binding Response, but hides data inside the transaction field. The client receives Binding Responses from all servers, checks the transaction field, and prints the hidden data when it is found.

## Workflow
1. Client sends Binding Requests to real STUN servers and the fake STUN server.
2. Real STUN servers return normal Binding Responses.
3. The fake STUN server returns a Binding Response with hidden data in the transaction field.
4. Client reads the transaction field from each response.
5. Client prints the hidden data.

## How to Test
Build and run the fake STUN server:
```bash
make server
./server
```

Build and run the client:
```bash
make client
./client
```
