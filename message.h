#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define BUF_SZ 1536

enum msg_type_t {
    MSG_TYPE_INVALID = 0,
    MSG_TYPE_GET = 1,
    MSG_TYPE_GET_ERR = 2,
    MSG_TYPE_GET_RESP = 3,
    MSG_TYPE_GET_ACK = 4,
    MSG_TYPE_FINISH = 5,
    MSG_TYPE_MAX
};

static const char * const str_map[MSG_TYPE_MAX+1] = {
    "invalid",
    "get",
    "get_err",
    "get_resp",
    "get_ack",
    "finish",
    "max"
};

struct msg_t {
    enum msg_type_t msg_type;      /* message type */
    int cur_seq;                   /* current seq number */
    int max_seq;                   /* max seq number */
    int payload_len;               /* length of payload */
    unsigned char payload[BUF_SZ]; /* buffer for data */
};

#endif /* __MESSAGE_H__ */
