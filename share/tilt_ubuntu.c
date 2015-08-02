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

#include <SDL.h>
#include <dlfcn.h>
#include <ubuntu/application/sensors/accelerometer.h>

#include "log.h"

static UASensorsAccelerometer *accel;
static float accelx = 0;
static float accely = 0;
static int status = 0;

static void *appapi = 0;

static UASensorsAccelerometer *(*accel_new)(void) = 0;
static void (*set_reading_cb)(UASensorsAccelerometer*, on_accelerometer_event_cb, void*) = 0;
static UStatus (*accel_enable)(UASensorsAccelerometer*) = 0;
static UStatus (*accel_disable)(UASensorsAccelerometer*) = 0;
static UStatus (*get_x)(UASAccelerometerEvent*, float*) = 0;
static UStatus (*get_y)(UASAccelerometerEvent*, float*) = 0;

void tilt_cb(UASAccelerometerEvent *event, void *context) {
    float value = 0;
    if (get_x(event, &value) == U_STATUS_SUCCESS) accelx = value;
    if (get_y(event, &value) == U_STATUS_SUCCESS) accely = value;
}

void tilt_init(void) {
    SDL_setenv("UBUNTU_PLATFORM_API_BACKEND", "touch_mirclient", 1);

    appapi = dlopen("libubuntu_application_api.so.3", RTLD_LAZY);
    if (!appapi) appapi = dlopen("libubuntu_application_api.so.2", RTLD_LAZY);
    if (!appapi) {
        log_printf("Failed to load ubuntu_application_api");
        status = 0;
        return;
    }

    accel_new = (UASensorsAccelerometer*(*)(void))dlsym(appapi, "ua_sensors_accelerometer_new");
    set_reading_cb = (void(*)(UASensorsAccelerometer*, on_accelerometer_event_cb, void*))dlsym(appapi, "ua_sensors_accelerometer_set_reading_cb");
    accel_enable = (UStatus(*)(UASensorsAccelerometer*))dlsym(appapi, "ua_sensors_accelerometer_enable");
    accel_disable = (UStatus(*)(UASensorsAccelerometer*))dlsym(appapi, "ua_sensors_accelerometer_disable");
    get_x = (UStatus(*)(UASensorsAccelerometer*, float*))dlsym(appapi, "uas_accelerometer_event_get_acceleration_x");
    get_y = (UStatus(*)(UASensorsAccelerometer*, float*))dlsym(appapi, "uas_accelerometer_event_get_acceleration_y");

    if (!accel_new || !set_reading_cb || !accel_enable || !accel_disable || !get_x || !get_y) {
        log_printf("Missing symbols in ubuntu_application_api");
        status = 0;
        return;
    }

    accel = accel_new();
    set_reading_cb(accel, tilt_cb, 0);
    accel_enable(accel);
    status = 1;
}

void tilt_free(void) {
    status = 0;
    accel_disable(accel);
}

int tilt_stat(void) {
    return status;
}

int tilt_get_button(int *b, int *s) {
    return 0;
}

float tilt_get_x(void) {
    return accelx;
}

float tilt_get_z(void) {
    return accely;
}
