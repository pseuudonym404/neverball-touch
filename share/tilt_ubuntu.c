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
#include <ubuntu/application/sensors/accelerometer.h>

static UASensorsAccelerometer *accel;
static float accelx = 0;
static float accely = 0;
static int status = 0;

void tilt_cb(UASAccelerometerEvent *event, void *context) {
    float value = 0;
    if (uas_accelerometer_event_get_acceleration_x(event, &value) == U_STATUS_SUCCESS) accelx = value;
    if (uas_accelerometer_event_get_acceleration_y(event, &value) == U_STATUS_SUCCESS) accely = value;
}

void tilt_init(void) {
    SDL_setenv("UBUNTU_PLATFORM_API_BACKEND", "touch_mirclient", 1);
    accel = ua_sensors_accelerometer_new();
    ua_sensors_accelerometer_set_reading_cb(accel, tilt_cb, 0);
    ua_sensors_accelerometer_enable(accel);
    status = 1;
}

void tilt_free(void) {
    status = 0;
    ua_sensors_accelerometer_disable(accel);
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
