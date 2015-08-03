/* Some of the data types defined in the RISC OS VCHIQ module that
 * we need to access in Khronos.  */

#ifndef VCHIQ_MODULE_H
#define VCHIQ_MODULE_H

#include "vchiq_if.h"
#include "vchiq_cfg.h"
#include "vchiq_util.h"

#define VCHIQ_FOURCC_AS_4CHARS(fourcc)	\
	((fourcc) >> 24) & 0xff, \
	((fourcc) >> 16) & 0xff, \
	((fourcc) >>  8) & 0xff, \
	(fourcc) & 0xff

typedef struct {
	VCHIQ_SERVICE_HANDLE_T handle;

	VCHIU_QUEUE_T queue;

	VCHI_CALLBACK_T callback;
	void *callback_param;
	void *private_word;
} SHIM_SERVICE_T;

#define BITSET_SIZE(b)        ((b + 31) >> 5)
#define VCHIQ_NUM_SERVICE_BULKS        4
#define DEBUG_MAX 0

enum {
	VCHIQ_SRVSTATE_FREE,
	VCHIQ_SRVSTATE_HIDDEN,
	VCHIQ_SRVSTATE_LISTENING,
	VCHIQ_SRVSTATE_OPENING,
	VCHIQ_SRVSTATE_OPEN,
	VCHIQ_SRVSTATE_OPENSYNC,
	VCHIQ_SRVSTATE_CLOSESENT,
	VCHIQ_SRVSTATE_CLOSERECVD,
	VCHIQ_SRVSTATE_CLOSEWAIT,
	VCHIQ_SRVSTATE_CLOSED
};

typedef struct remote_event_struct {
	int armed;
	int fired;
	VCOS_SEMAPHORE_T *event;
} REMOTE_EVENT_T;

typedef struct vchiq_shared_state_struct {

	/* A non-zero value here indicates that the content is valid. */
	int initialised;

	/* The first and last (inclusive) slots allocated to the owner. */
	int slot_first;
	int slot_last;

	/* The slot allocated to synchronous messages from the owner. */
	int slot_sync;

	/* Signalling this event indicates that owner's slot handler thread
	** should run. */
	REMOTE_EVENT_T trigger;

	/* Indicates the byte position within the stream where the next message
	** will be written. The least significant bits are an index into the
	** slot. The next bits are the index of the slot in slot_queue. */
	int tx_pos;

	/* This event should be signalled when a slot is recycled. */
	REMOTE_EVENT_T recycle;

	/* The slot_queue index where the next recycled slot will be written. */
	int slot_queue_recycle;

	/* This event should be signalled when a synchronous message is sent. */
	REMOTE_EVENT_T sync_trigger;

	/* This event should be signalled when a synchronous message has been
	** released. */
	REMOTE_EVENT_T sync_release;

	/* A circular buffer of slot indexes. */
	int slot_queue[VCHIQ_MAX_SLOTS_PER_SIDE];

	/* Debugging state */
	int debug[DEBUG_MAX];
} VCHIQ_SHARED_STATE_T;

typedef struct vchiq_slot_struct {
	char data[VCHIQ_SLOT_SIZE];
} VCHIQ_SLOT_T;

typedef struct vchiq_slot_info_struct {
	/* Use two counters rather than one to avoid the need for a mutex. */
	short use_count;
	short release_count;
} VCHIQ_SLOT_INFO_T;

typedef struct vchiq_slot_zero_struct {
	int magic;
	short version;
	short version_min;
	int slot_zero_size;
	int slot_size;
	int max_slots;
	int max_slots_per_side;
	int platform_data[2];
	VCHIQ_SHARED_STATE_T master;
	VCHIQ_SHARED_STATE_T slave;
	VCHIQ_SLOT_INFO_T slots[VCHIQ_MAX_SLOTS];
} VCHIQ_SLOT_ZERO_T;

typedef struct vchiq_service_quota_struct {
	unsigned short slot_quota;
	unsigned short slot_use_count;
	unsigned short message_quota;
	unsigned short message_use_count;
	VCOS_SEMAPHORE_T quota_event;
	int previous_tx_index;
} VCHIQ_SERVICE_QUOTA_T;

typedef struct opaque_platform_state_t *VCHIQ_PLATFORM_STATE_T;
typedef uint32_t VCHIQ_CONNSTATE_T;

typedef struct vchiq_state_struct VCHIQ_STATE_T;

typedef struct vchiq_bulk_struct {
	short mode;
	short dir;
	void *userdata;
	VCHI_MEM_HANDLE_T handle;
	void *data;
	int size;
	void *remote_data;
	int remote_size;
	int actual;
} VCHIQ_BULK_T;

typedef struct vchiq_bulk_queue_struct {
	int local_insert;  /* Where to insert the next local bulk */
	int remote_insert; /* Where to insert the next remote bulk (master) */
	int process;       /* Bulk to transfer next */
	int remote_notify; /* Bulk to notify the remote client of next (mstr) */
	int remove;        /* Bulk to notify the local client of, and remove,
			   ** next */
	VCHIQ_BULK_T bulks[VCHIQ_NUM_SERVICE_BULKS];
} VCHIQ_BULK_QUEUE_T;

typedef struct vchiq_service_struct {
	VCHIQ_SERVICE_BASE_T base;
	VCHIQ_SERVICE_HANDLE_T handle;
	unsigned int ref_count;
	int srvstate;
	unsigned int localport;
	unsigned int remoteport;
	int public_fourcc;
	int client_id;
	char auto_close;
	char sync;
	char closing;
	int32_t poll_flags;
	short version;
	short version_min;
	short peer_version;

	VCHIQ_STATE_T *state;
	VCHIQ_INSTANCE_T instance;

	int service_use_count;

	VCHIQ_BULK_QUEUE_T bulk_tx;
	VCHIQ_BULK_QUEUE_T bulk_rx;

	VCOS_SEMAPHORE_T remove_event;
	VCOS_SEMAPHORE_T bulk_remove_event;
	VCOS_MUTEX_T bulk_mutex;

	struct service_stats_struct {
		int quota_stalls;
		int slot_stalls;
		int bulk_stalls;
		int error_count;
		int ctrl_tx_count;
		int ctrl_rx_count;
		int bulk_tx_count;
		int bulk_rx_count;
		int bulk_aborted_count;
		uint64_t ctrl_tx_bytes;
		uint64_t ctrl_rx_bytes;
		uint64_t bulk_tx_bytes;
		uint64_t bulk_rx_bytes;
	} stats;
} VCHIQ_SERVICE_T;

typedef struct vchiq_service_struct VCHI_SERVICE_T;

struct vchiq_state_struct {
	int id;
	int initialised;
	VCHIQ_CONNSTATE_T conn_state;
	int is_master;

	VCHIQ_SHARED_STATE_T *local;
	VCHIQ_SHARED_STATE_T *remote;
	VCHIQ_SLOT_T *slot_data;

	unsigned short default_slot_quota;
	unsigned short default_message_quota;

	/* Event indicating connect message received */
	VCOS_SEMAPHORE_T connect;

	/* Mutex protecting services */
	VCOS_MUTEX_T mutex;
	VCHIQ_INSTANCE_T *instance;

	/* Processes incoming messages */
	VCOS_TASK_T slot_handler_thread;

	/* Processes recycled slots */
	VCOS_TASK_T recycle_thread;

	/* Processes synchronous messages */
	VCOS_TASK_T sync_thread;

	/* Local implementation of the trigger remote event */
	VCOS_SEMAPHORE_T trigger_event;

	/* Local implementation of the recycle remote event */
	VCOS_SEMAPHORE_T recycle_event;

	/* Local implementation of the sync trigger remote event */
	VCOS_SEMAPHORE_T sync_trigger_event;

	/* Local implementation of the sync release remote event */
	VCOS_SEMAPHORE_T sync_release_event;

	char *tx_data;
	char *rx_data;
	VCHIQ_SLOT_INFO_T *rx_info;

	VCOS_MUTEX_T slot_mutex;

	VCOS_MUTEX_T recycle_mutex;

	VCOS_MUTEX_T sync_mutex;

	VCOS_MUTEX_T bulk_transfer_mutex;

	/* Indicates the byte position within the stream from where the next
	** message will be read. The least significant bits are an index into
	** the slot.The next bits are the index of the slot in
	** remote->slot_queue. */
	int rx_pos;

	/* A cached copy of local->tx_pos. Only write to local->tx_pos, and read
		from remote->tx_pos. */
	int local_tx_pos;

	/* The slot_queue index of the slot to become available next. */
	int slot_queue_available;

	/* A flag to indicate if any poll has been requested */
	int poll_needed;

	/* Ths index of the previous slot used for data messages. */
	int previous_data_index;

	/* The number of slots occupied by data messages. */
	unsigned short data_use_count;

	/* The maximum number of slots to be occupied by data messages. */
	unsigned short data_quota;

	/* An array of bit sets indicating which services must be polled. */
	int32_t poll_services[BITSET_SIZE(VCHIQ_MAX_SERVICES)];

	/* The number of the first unused service */
	int unused_service;

	/* Signalled when a free slot becomes available. */
	VCOS_SEMAPHORE_T slot_available_event;

	VCOS_SEMAPHORE_T slot_remove_event;

	/* Signalled when a free data slot becomes available. */
	VCOS_SEMAPHORE_T data_quota_event;

	/* Incremented when there are bulk transfers which cannot be processed
	 * whilst paused and must be processed on resume */
	int deferred_bulks;

	struct state_stats_struct {
		int slot_stalls;
		int data_stalls;
		int ctrl_tx_count;
		int ctrl_rx_count;
		int error_count;
	} stats;

	VCHIQ_SERVICE_T * services[VCHIQ_MAX_SERVICES];
	VCHIQ_SERVICE_QUOTA_T service_quotas[VCHIQ_MAX_SERVICES];
	VCHIQ_SLOT_INFO_T slot_info[VCHIQ_MAX_SLOTS];

	VCHIQ_PLATFORM_STATE_T platform_state;
};
typedef struct vchiq_state_struct VCHIQ_STATE_T;

struct vchiq_instance_struct {
	VCHIQ_STATE_T *state;

	int connected;

	struct __vcos_list_head bulk_waiter_list;
	VCOS_MUTEX_T bulk_waiter_list_mutex;

	void *private_word;
} *vchiq_instance;

#endif
