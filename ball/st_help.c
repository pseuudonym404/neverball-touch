/*
 * Copyright (C) 2003 Robert Kooima
 *
 * NEVERBALL is  free software; you can redistribute  it and/or modify
 * it under the  terms of the GNU General  Public License as published
 * by the Free  Software Foundation; either version 2  of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 */

#include "gui.h"
#include "audio.h"
#include "config.h"
#include "demo.h"
#include "video.h"

#include "game_common.h"
#include "game_server.h"
#include "game_client.h"

#include "st_title.h"
#include "st_help.h"
#include "st_shared.h"

/*---------------------------------------------------------------------------*/

enum
{
    HELP_PAGE = GUI_LAST,
    HELP_DEMO
};

enum
{
    PAGE_RULES,
    PAGE_CONTROLS,
    PAGE_MODES,
    PAGE_TRICKS
};

static const char demos[][16] = {
    "gui/demo1.nbr",
    "gui/demo2.nbr"
};

static int page = PAGE_RULES;

/*---------------------------------------------------------------------------*/

static int help_action(int tok, int val)
{
    audio_play(AUD_MENU, 1.0f);

    switch (tok)
    {
    case GUI_BACK:
        page = PAGE_RULES;
        return goto_state(&st_title);

    case HELP_DEMO:
        if (demo_replay_init(demos[val], NULL, NULL, NULL, NULL, NULL))
            return goto_state(&st_help_demo);
        break;

    case HELP_PAGE:
        page = val;
        return goto_state(&st_help);

    }
    return 1;
}

/* -------------------------------------------------------------------------- */

static int help_button(int id, const char *text, int token, int value)
{
    int jd = gui_state(id, text, GUI_MED, token, value);

    /* Hilight current page. */

    if (token == HELP_PAGE && value == page)
    {
        gui_set_hilite(jd, 1);
        gui_focus(jd);
    }

    return jd;
}

static int help_menu(int id)
{
    int jd, kd;

    gui_space(id);

    if (video.device_w < video.device_h) {
        if ((jd = gui_vstack(id))) {
            if ((kd = gui_harray(jd))) {
                gui_label(kd, _("Help"), GUI_MED, gui_yel, gui_red);
                gui_space(kd);
                help_button(kd, _("Back"),     GUI_BACK, 0);
            }
            gui_space(jd);
            if ((kd = gui_harray(jd))) {
                help_button(kd, _("Tricks"),   HELP_PAGE, PAGE_TRICKS);
                help_button(kd, _("Modes"),    HELP_PAGE, PAGE_MODES);
                help_button(kd, _("Rules"),    HELP_PAGE, PAGE_RULES);
            }
        }
    } else if ((jd = gui_hstack(id))) {
        gui_label(jd, _("Help"), GUI_MED, gui_yel, gui_red);
        gui_space(jd);
        if ((kd = gui_harray(jd))) {
            help_button(kd, _("Tricks"),   HELP_PAGE, PAGE_TRICKS);
            help_button(kd, _("Modes"),    HELP_PAGE, PAGE_MODES);
            help_button(kd, _("Rules"),    HELP_PAGE, PAGE_RULES);
        }
        gui_space(jd);
        help_button(jd, _("Back"),     GUI_BACK, 0);
    }
    return jd;
}

/* -------------------------------------------------------------------------- */

static int page_rules(int id)
{
    const char *s0 = _(
        "Tilt your device to cause\\"
        "the ball to roll.\\");
    const char *s1 = _(
        "Roll over coins to collect\\"
        "them.  Collect coins to\\"
        "unlock the goal and finish\\"
        "the level.\\");

    int w = video.device_w;
    int h = video.device_h;

    int jd, kd, ld, md;

    if (w < h) {
        int iw = (w - 120) / 2;
        int ih = iw * 3 / 4;

        if ((jd = gui_hstack(id))) {
            gui_filler(jd);

            if ((kd = gui_vstack(jd))) {
                if ((md = gui_hstack(kd))) {
                    gui_filler(md);
                    if ((ld = gui_vstack(md))) {
                        gui_space(ld);
                        gui_image(ld, "gui/help1.jpg", iw, ih);
                        gui_filler(ld);
                    }
                    gui_filler(md);
                }

                if ((ld = gui_vstack(kd))) {
                    gui_space(ld);
                    gui_multi(ld, s0, GUI_SML, gui_wht, gui_wht);
                    gui_filler(ld);
                }

                if ((md = gui_hstack(kd))) {
                    gui_filler(md);
                    if ((ld = gui_vstack(md))) {
                        gui_space(ld);
                        gui_image(ld, "gui/help2.jpg", iw, ih);
                        gui_filler(ld);
                    }
                    gui_filler(md);
                }

                if ((ld = gui_vstack(kd))) {
                    gui_space(ld);
                    gui_multi(ld, s1, GUI_SML, gui_wht, gui_wht);
                    gui_filler(ld);
                }
            }

            gui_filler(jd);
        }
    } else {
        int ih = (h - 200) / 2;
        int iw = ih * 4 / 3;

        if ((jd = gui_hstack(id)))
        {
            gui_filler(jd);

            if ((kd = gui_varray(jd)))
            {
                if ((ld = gui_vstack(kd)))
                {
                    gui_space(ld);
                    gui_multi(ld, s0, GUI_SML, gui_wht, gui_wht);
                    gui_filler(ld);
                }

                if ((ld = gui_vstack(kd)))
                {
                    gui_space(ld);
                    gui_multi(ld, s1, GUI_SML, gui_wht, gui_wht);
                    gui_filler(ld);
                }
            }

            gui_space(jd);

            if ((kd = gui_varray(jd)))
            {
                if ((ld = gui_vstack(kd)))
                {
                    gui_space(ld);
                    gui_image(ld, "gui/help1.jpg", iw, ih);
                    gui_filler(ld);
                }

                if ((ld = gui_vstack(kd)))
                {
                    gui_space(ld);
                    gui_image(ld, "gui/help2.jpg", iw, ih);
                    gui_filler(ld);
                }
            }

            gui_filler(jd);
        }
    }

    return id;
}

static int page_controls(int id)
{
    const char *s_rotate  = _("Left and right mouse buttons rotate the view.\\"
                              "Hold Shift for faster view rotation.");
    const char *s_exit    = _("Exit / Pause");
    const char *s_camera1 = _("Chase Camera");
    const char *s_camera2 = _("Lazy Camera");
    const char *s_camera3 = _("Manual Camera");
    const char *s_shot    = _("Screenshot");

    /*
     * SDL_GetKeyName overwrites some of its results, that's why we
     * don't cache the pointers anymore.
     */

    const SDL_Keycode k_exit = KEY_EXIT;
    const SDL_Keycode k_cam1 = config_get_d(CONFIG_KEY_CAMERA_1);
    const SDL_Keycode k_cam2 = config_get_d(CONFIG_KEY_CAMERA_2);
    const SDL_Keycode k_cam3 = config_get_d(CONFIG_KEY_CAMERA_3);
    const SDL_Keycode k_shot = KEY_SCREENSHOT;

    int jd, kd;

    gui_space(id);

    if ((jd = gui_vstack(id)))
    {
        if ((kd = gui_harray(jd)))
        {
            gui_label(kd, s_exit,   GUI_SML, gui_wht, gui_wht);
            gui_label(kd, SDL_GetKeyName(k_exit), GUI_SML, gui_yel, gui_yel);
        }
        if ((kd = gui_harray(jd)))
        {
            gui_label(kd, s_camera1, GUI_SML, gui_wht, gui_wht);
            gui_label(kd, SDL_GetKeyName(k_cam1), GUI_SML, gui_yel, gui_yel);
        }
        if ((kd = gui_harray(jd)))
        {
            gui_label(kd, s_camera2, GUI_SML, gui_wht, gui_wht);
            gui_label(kd, SDL_GetKeyName(k_cam2), GUI_SML, gui_yel, gui_yel);
        }
        if ((kd = gui_harray(jd)))
        {
            gui_label(kd, s_camera3, GUI_SML, gui_wht, gui_wht);
            gui_label(kd, SDL_GetKeyName(k_cam3), GUI_SML, gui_yel, gui_yel);
        }
        if ((kd = gui_harray(jd)))
        {
            gui_label(kd, s_shot, GUI_SML, gui_wht, gui_wht);
            gui_label(kd, SDL_GetKeyName(k_shot), GUI_SML, gui_yel, gui_yel);
        }

        gui_set_rect(jd, GUI_ALL);
    }

    gui_space(id);

    gui_multi(id, s_rotate, GUI_SML, gui_wht, gui_wht);

    return id;
}

static int page_modes(int id)
{
    int jd;

    gui_space(id);

    if ((jd = gui_vstack(id)))
    {
        gui_label(jd, _("Normal Mode"), GUI_MED, 0, 0);
        gui_multi(jd,
                  _("Finish a level before the time runs out.\\"
                    "You need to collect coins to open the goal."),
                  GUI_SML, gui_wht, gui_wht);

        gui_set_rect(jd, GUI_ALL);
    }

    gui_space(id);

    if ((jd = gui_vstack(id)))
    {
        gui_label(jd, _("Challenge Mode"), GUI_MED, 0, 0);
        gui_multi(jd,
                  _("Start playing from the first level of the set.\\"
                    "You start with three balls, do not lose them.\\"
                    "Earn an extra ball for each 100 coins."),
                  GUI_SML, gui_wht, gui_wht);

        gui_set_rect(jd, GUI_ALL);
    }

    return id;
}

static int page_tricks(int id)
{
    const char *s0 = _(
        "Corners can be used to jump.\\"
        "Get rolling and take aim\\"
        "at the angle. You may be able\\"
        "to reach new places.\\");
    const char *s1 = _(
        "Tilting in 2 directions increases\\"
        "the slope. Use the manual camera\\"
        "and turn the camera by 45\\"
        "degrees for best results.\\");

    int w = video.device_w;
    int h = video.device_h;

    int jd, kd, ld, md;

    if (w < h) {
        int iw = (w - 120) / 2;
        int ih = iw * 3 / 4;

        if ((jd = gui_hstack(id))) {
            gui_filler(jd);

            if ((kd = gui_vstack(jd))) {
                if ((ld = gui_vstack(kd))) {
                    gui_space(ld);
                    gui_multi(ld, s0, GUI_SML, gui_wht, gui_wht);
                    gui_filler(ld);
                }

                if ((md = gui_hstack(kd))) {
                    gui_filler(md);
                    if ((ld = gui_vstack(md))) {
                        gui_space(ld);
                        gui_image(ld, "gui/help3.jpg", iw, ih);
                        gui_state(ld, _("Watch demo"), GUI_SML, 0, 0);
                        gui_filler(ld);
                        gui_set_state(ld, HELP_DEMO, 0);
                    }
                    gui_filler(md);
                }

                if ((ld = gui_vstack(kd))) {
                    gui_space(ld);
                    gui_multi(ld, s1, GUI_SML, gui_wht, gui_wht);
                    gui_filler(ld);
                }

                if ((md = gui_hstack(kd))) {
                    gui_filler(md);
                    if ((ld = gui_vstack(md))) {
                        gui_space(ld);
                        gui_image(ld, "gui/help4.jpg", iw, ih);
                        gui_state(ld, _("Watch demo"), GUI_SML, 0, 0);
                        gui_filler(ld);
                        gui_set_state(ld, HELP_DEMO, 0);
                    }
                    gui_filler(md);
                }
            }

            gui_filler(jd);
        }
    } else {
        int ih = (h - 200) / 2;
        int iw = ih * 4 / 3;

        if ((jd = gui_hstack(id)))
        {
            gui_filler(jd);

            if ((kd = gui_varray(jd)))
            {
                if ((ld = gui_vstack(kd)))
                {
                    gui_space(ld);
                    gui_image(ld, "gui/help3.jpg", iw, ih);
                    gui_state(ld, _("Watch demo"), GUI_SML, 0, 0);
                    gui_filler(ld);
                    gui_set_state(ld, HELP_DEMO, 0);
                }

                if ((ld = gui_vstack(kd)))
                {
                    gui_space(ld);
                    gui_image(ld, "gui/help4.jpg", iw, ih);
                    gui_state(ld, _("Watch demo"), GUI_SML, 0, 0);
                    gui_filler(ld);
                    gui_set_state(ld, HELP_DEMO, 1);
                }
            }

            gui_space(jd);

            if ((kd = gui_varray(jd)))
            {
                if ((ld = gui_vstack(kd)))
                {
                    gui_space(ld);
                    gui_multi(ld, s0, GUI_SML, gui_wht, gui_wht);
                    gui_filler(ld);
                }

                if ((ld = gui_vstack(kd)))
                {
                    gui_space(ld);
                    gui_multi(ld, s1, GUI_SML, gui_wht, gui_wht);
                    gui_filler(ld);
                }
            }

            gui_filler(jd);
        }
    }

    return id;
}

/* -------------------------------------------------------------------------- */

static int help_gui(void)
{
    int id;

    if ((id = gui_vstack(0)))
    {
        help_menu(id);

        switch (page)
        {
        case PAGE_RULES:    page_rules(id);    break;
        case PAGE_CONTROLS: page_controls(id); break;
        case PAGE_MODES:    page_modes(id);    break;
        case PAGE_TRICKS:   page_tricks(id);   break;
        }

        gui_layout(id, 0, +1);
    }

    return id;
}

static int help_enter(struct state *st, struct state *prev)
{
    return help_gui();
}

static int help_keybd(int c, int d)
{
    if (d)
    {
        if (c == KEY_EXIT)
            return help_action(GUI_BACK, 0);
    }
    return 1;
}

static int help_buttn(int b, int d)
{
    if (d)
    {
        int active = gui_active();

        if (config_tst_d(CONFIG_JOYSTICK_BUTTON_A, b))
            return help_action(gui_token(active), gui_value(active));
        if (config_tst_d(CONFIG_JOYSTICK_BUTTON_B, b))
            return help_action(GUI_BACK, 0);
    }
    return 1;
}

/*---------------------------------------------------------------------------*/

static int help_demo_enter(struct state *st, struct state *prev)
{
    game_client_fly(0.0f);
    return 0;
}

static void help_demo_leave(struct state *st, struct state *next, int id)
{
    demo_replay_stop(0);
}

static void help_demo_paint(int id, float t)
{
    game_client_draw(0, t);
}

static void help_demo_timer(int id, float dt)
{
    game_step_fade(dt);

    if (!demo_replay_step(dt))
        goto_state(&st_help);

    game_client_blend(demo_replay_blend());
}

static int help_demo_keybd(int c, int d)
{
    if (d)
    {
        if (c == KEY_EXIT)
            return goto_state(&st_help);
    }
    return 1;
}

static int help_demo_buttn(int b, int d)
{
    if (d)
    {
        if (config_tst_d(CONFIG_JOYSTICK_BUTTON_B, b))
            return goto_state(&st_help);
    }
    return 1;
}
/*---------------------------------------------------------------------------*/

struct state st_help = {
    help_enter,
    shared_leave,
    shared_paint,
    shared_timer,
    shared_point,
    shared_stick,
    shared_angle,
    shared_click,
    help_keybd,
    help_buttn
};

struct state st_help_demo = {
    help_demo_enter,
    help_demo_leave,
    help_demo_paint,
    help_demo_timer,
    NULL,
    NULL,
    NULL,
    NULL,
    help_demo_keybd,
    help_demo_buttn
};
