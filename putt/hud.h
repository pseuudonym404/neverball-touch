#ifndef HUD_H
#define HUD_H

/*---------------------------------------------------------------------------*/

void hud_init(void);
void hud_free(void);

void hud_paint(void);

int hud_hit_test(int, int);
void hud_hide_controls();
void hud_show_controls();

/*---------------------------------------------------------------------------*/

#endif
