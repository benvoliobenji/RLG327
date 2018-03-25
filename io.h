#ifndef IO_H
# define IO_H

typedef struct dungeon dungeon_t;

# ifdef __cplusplus

extern "C"{
# endif

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon_t *d);
void io_handle_input(dungeon_t *d);
void io_queue_message(const char *format, ...);
void io_determine_display(dungeon_t *d);
void io_fog_display(dungeon_t *d);
void io_teleport(dungeon_t *d);

# ifdef __cplusplus
}
# endif

#endif
