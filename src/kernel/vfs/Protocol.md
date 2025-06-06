# VFS FS Server Protocol

> For reference see `.../.../{TBD}.h`

This is the protocol used for communication with FS Servers.
It allows for performing all necesary operations to manage access and modify a file system.

## FS Server

This term describes a userspace application that:

1. Creates a server handle via call to `open` in `{TBD, maybe /serv?/}/`. The result of this call will be a readable `fd`.
2. Accepts and responds to messages on `fd` using FS server protocol

## Protocol

The *FS Server Protocol* consists of requests and responses to requests; all requests must receive exactly one response.

All requests/responses start with a **metadata** section containing:

- `u32 size` – total size of the request (including this field)
- `FSMessageTag_t tag` – **unique** identifier for the request
- `union{ FSRequestType_t, FSResponseType_t }` – type of request/response; Element of union present is implied by application type (fs servers receive only requests, vfs receives only responses)

### Request-Response Types

#### VERSION

Used to initiate connection, exactly one request-response messages should be exchanged before any other request is made.
Client and server negotiate the maximal response size and protocol version (not used now, but maybe...).

> If error response is received the server will not be registered.

##### Request

- `u32 max_size` – max total response size
- `FSProtocolVersion_t versions` – client protocol version implementation

##### Response

- `u32 max_size` – max total request size
- `u32 FSProtocolVersion_t version` – server protocol version implementation

#### CONNECT

Used by

### Binary Protocol Scheme


