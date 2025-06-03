// TODO: Invent a name for this protocol

#ifndef _FS_SERVER_PROTOCOL_
#define _FS_SERVER_PROTOCOL_

#include <stdbigos/types.h>

// TODO: create proper authentication types.
//       This will be sufficient if we create an authentication server.
typedef u32 uid_t;

/// FSProtocol Version
typedef u32 FSProtocolVersion_t;

/// Unique tag for a request (response will have the same tag)
typedef u16 FSMessageTag_t;

/// Abstract inode number representation
typedef u64 ino_t;

/// Mode for opening a file
typedef u8 FSOpenMode_t;

/// 'Cursor' type representing abstract pointer to a file in fs tree
/// It might be 'nice' to by default have some information about pointed element here (file type at least maybe?)
typedef struct {
	ino_t ino;
	// TODO: Decide whether we want such things:
	u8 file_type;
	FSOpenMode_t file_modifiers;
} FSTreeCursor_t;

/// Represents a handle to a file opened for reading/writing
typedef u64 FSFileHandle_t;

// TODO: Create stat structure
typedef u8 FSStatData_t;

// TODO: Create a real structure and not just a placeholder
/// Information about a file to be created
typedef struct {
	u16 permissions;
	u16 fiele_type;
} FSCrateInfo_t;

/// Possible request types
typedef enum : u8 {
	/// Negotiate version and max request size, sanity check for fs server
	FS_REQUEST_VERSION,
	/// Requset access to the root of fs tree handled by server
	FS_REQUEST_CONNECT,
	/// Walk the tree (move the cursor around)
	FS_REQUEST_WALK,

	/// Request information about file pointed by the cursor
	FS_REQUEST_STAT,
	// TODO: Decide whether we want create/remove (remove is kind of necesary)
	FS_REQUEST_CREATE,
	FS_REQUEST_DELETE,

	/// Open a file for reading
	FS_REQUEST_OPEN,
	FS_REQUEST_READ,
	FS_REQUEST_WRITE,
	FS_REQUEST_WSTAT,
	FS_REQUEST_CLOSE,
	/// Flush all requested operations for a given file before *this* message.
	FS_REQUEST_FLUSH,
} FSRequestType_t;

/// Possible response types
typedef enum : u8 {
	/// Response with version and max response size, sanity check
	FS_RESPONSE_VERSION,
	/// Response with a cursor pointing to the root of the tree
	FS_RESPONSE_CONNECT,
	/// Response with a cursor at a new position (and information on how 'walks' were successful)
	FS_RESPONSE_WALK,

	/// Response with file information
	FS_RESPONSE_STAT,
	/// Response with cursor pointing to a newly created file
	FS_RESPONSE_CREATE,
	/// Empty response (no error occured)
	FS_RESPONSE_DELETE,

	/// Response with a reading handle
	FS_RESPONSE_OPEN,
	FS_RESPONSE_READ,
	FS_RESPONSE_WRITE,
	FS_RESPONSE_WSTAT,
	/// Empty response (no error occured)
	FS_RESPONSE_CLOSE,
	/// Empty response when flush is complete
	FS_RESPONSE_FLUSH,

	/// Error occured when handling a request
	FS_RESPONSE_ERROR,
} FSResponseType_t;

/// Metadata for bigos-protocol messages.
/// Using analogy to 'computer networks' this data lives on a *layer* above the requests.
/// Maybe because of that it should be moved to a different file...
typedef struct {
	u32 size;
	FSMessageTag_t tag;
	union {
		FSRequestType_t request_type;
		FSResponseType_t response_type;
	};
} FSMessageMetadata_t;

typedef struct {
	u32 max_size;
	FSProtocolVersion_t version;
} FSRequestVersion_t;

typedef struct {
	u32 max_size;
	FSProtocolVersion_t version;
} FSResponseVersion_t;

typedef struct {
	uid_t uid;
} FSRequestConnect_t;

typedef struct {
	FSTreeCursor_t cursor;
} FSResponseConnect_t;

typedef struct {
	uid_t uid;
	FSTreeCursor_t cursor;
	// TODO: Decide whether we want multi-walks; if we do, then:
	u16 walk_count;
	/// Sequence of walks to take.
	// pstring_t walk_path[0]; // Of length 'path_count'
} FSRequestWalk_t;

typedef struct {
	FSTreeCursor_t cursor;
	// TODO: Decide whether we want to allow for 'parially-successful-multi-walks'.
	//       If we want to then then this must return some indicator.
} FSResponseWalk_t;

typedef struct {
	uid_t uid;
	FSTreeCursor_t cursor;
} FSRequestStat_t;

typedef struct {
	FSStatData_t stat;
} FSResponseStat_t;

typedef struct {
	uid_t uid;
	FSTreeCursor_t cursor;
	// pstring_t name;
	FSCrateInfo_t create_info;
} FSRequestCreate_t;

typedef struct {
	FSTreeCursor_t cursor;
} FSResponseCreate_t;

typedef struct {
	uid_t uid;
	FSTreeCursor_t cursor;
} FSRequestDelete_t;

typedef struct {
} FSResponseDelete_t;

typedef struct {
	uid_t uid;
	FSTreeCursor_t cursor;
	FSOpenMode_t mode;
} FSRequestOpen_t;

typedef struct {
	FSFileHandle_t file_handle;
	/// Max data that can be read/written in one operation (one protocol message)
	u32 max_atomic_op; // TODO: Decide whether this is necessary
} FSResponseOpen_t;

typedef struct {
	FSFileHandle_t file_handle;
	u64 offset;
	u32 count;
} FSRequestRead_t;

typedef struct {
	u32 count;
	u8 data[0];
} FSResponseRead_t;

typedef struct {
	FSFileHandle_t file_handle;
	u64 offset;
	u32 count;
	u8 data[0];
} FSRequestWrite_t;

typedef struct {
	u32 count;
} FSResponseWrite_t;

typedef struct {
	FSFileHandle_t file_handle;
	FSStatData_t stat_data;
} FSRequestWstat_t;

typedef struct {
} FSResponseWstat_t;

typedef struct {
	FSFileHandle_t file_handle;
} FSRequestClose_t;

typedef struct {
} FSResponseClose_t;

typedef struct {
	FSFileHandle_t file_handle;
} FSRequestFlush_t;

typedef struct {
} FSResponseFlush_t;

typedef struct {
	// pstring_t error_name;
} FSResponseError_t;

#endif
