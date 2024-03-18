#ifndef SQUEUE_H_
#define SQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {	
		uint16_t insert_i;
		uint16_t remove_i;
		uint16_t count;
		uint16_t node_max_count;
		uint32_t   node_size;
		uint16_t magic;
	} squeue_t;

	squeue_t* squeue_init(char_t* base, int32_t node_size, int32_t node_max_count);
	squeue_t* squeue_open(char_t* base);

	char_t* squeue_insert_p(squeue_t* head);
	char_t* squeue_remove_p(squeue_t* head);

	char_t* squeue_current_p(squeue_t* head);

	int32_t   squeue_count(squeue_t* head);

	int32_t   squeue_is_full(squeue_t* head);
	int32_t   squeue_is_empty(squeue_t* head);

	int32_t   squeue_flush(squeue_t* head);

#ifdef __cplusplus
}
#endif

#endif // SQUEUE_H_
