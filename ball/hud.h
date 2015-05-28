#ifndef HUD_H
#define HUD_H

/*---------------------------------------------------------------------------*/

void hud_init(void);
void hud_free(void);

void hud_paint(void);
void hud_timer(float);
void hud_update(int);

void hud_cam_pulse(int);
void hud_cam_timer(float);
void hud_cam_paint();

void hud_speed_pulse(int);
void hud_speed_timer(float);
void hud_speed_paint();

int hud_hit_test(int, int);
void hud_pause();
void hud_resume();
void hud_demo();

/*---------------------------------------------------------------------------*/

#endif
