int pool_submit(void (*f)(void *p), void *p);
void *worker(void *param);
void pool_init(void);
void pool_shutdown(void);
