#ifndef NP_SYNCHRONIZER_H
#define NP_SYNCHRONIZER_H

#include <threads.h>
#include <stdatomic.h>

/*
 Atomic
*/
typedef struct {
	atomic_int value;
} NPAtomic;

#define NP_MEMORY_ORDER_RELAXED memory_order_relaxed
#define NP_MEMORY_ORDER_RELEASE memory_order_release
#define NP_MEMORY_ORDER_ACQUIRE memory_order_acquire

static inline void NPAtomic_Store(NPAtomic *ptr, int v, memory_order _memory_order) {
	atomic_store_explicit(&ptr->value, v, _memory_order);
}

static inline int NPAtomic_Load(NPAtomic *ptr, memory_order _memory_order) {
 return	atomic_load_explicit(&ptr->value, _memory_order);
}

static inline void NPAtomic_FetchAdd(NPAtomic *ptr, int v, memory_order _memory_order) {
	atomic_fetch_add_explicit(&ptr->value, v, _memory_order);
}

static inline void NPAtomic_FetchSub(NPAtomic *ptr, int v, memory_order _memory_order) {
	atomic_fetch_sub_explicit(&ptr->value, v, _memory_order);
}



/*
 Mutex
*/
typedef struct {
	mtx_t value;
} NPMutex;

static inline void NPMutex_Init(NPMutex *ptr) {
	mtx_init(&ptr->value, mtx_plain);
}

static inline void NPMutex_Destroy(NPMutex *ptr) {
	mtx_destroy(&ptr->value);
}

static inline void NPMutex_Lock(NPMutex *ptr) {
	mtx_lock(&ptr->value);
}

static inline void NPMutex_Unlock(NPMutex *ptr) {
	mtx_unlock(&ptr->value);
}

/*
 Cond var
*/
typedef struct {
	cnd_t value;
} NPCondVar;


static inline void NPCondVar_Init(NPCondVar *ptr) {
	cnd_init(&ptr->value);
}

static inline void NPCondVar_Destroy(NPCondVar *ptr) {
	cnd_destroy(&ptr->value);
}

/*
 wake one
*/
static inline void NPCondVar_Signal(NPCondVar *ptr) {
	cnd_signal(&ptr->value);
}

/*
 wake all
*/
static inline void NPCondVar_Broadcast(NPCondVar *ptr) {
	cnd_broadcast(&ptr->value);
}

static inline void NPCondVar_Wait(NPCondVar *ptr, NPMutex *mtx) {
	cnd_wait(&ptr->value, &mtx->value);
}



#endif

