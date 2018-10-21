/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You
 * may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.  See the License for the specific language governing
 * permissions and limitations under the License.
 */
#ifdef ARDUINO_ARCH_SAMD

#include <Arduino.h>

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "hal/pwmout_api.h"
#include "platform/mbed_error.h"

#define TC_GCLK_NUM ((TCC_INST_NUM + TC_INST_NUM) / 2)

#define GetTCClockNumber(x) (GetTCNumber(x) >> 1)

static const uint16_t pwmout_gclk_clkctrl[] = {
    GCLK_CLKCTRL_GEN_GCLK4 | GCLK_CLKCTRL_ID(GCM_TCC0_TCC1),
    GCLK_CLKCTRL_GEN_GCLK5 | GCLK_CLKCTRL_ID(GCM_TCC2_TC3),
    GCLK_CLKCTRL_GEN_GCLK6 | GCLK_CLKCTRL_ID(GCM_TC4_TC5),
    GCLK_CLKCTRL_GEN_GCLK7 | GCLK_CLKCTRL_ID(GCM_TC6_TC7)
};

static const uint32_t pwmout_gclk_genctrl[] = {
    GCLK_GENCTRL_IDC | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_ID(4),
    GCLK_GENCTRL_IDC | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_ID(5),
    GCLK_GENCTRL_IDC | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_ID(6),
    GCLK_GENCTRL_IDC | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_ID(7)
};

static const uint32_t pwmout_gclk_gendiv[] = {
    GCLK_GENDIV_ID(4),
    GCLK_GENDIV_ID(5),
    GCLK_GENDIV_ID(6),
    GCLK_GENDIV_ID(7)
};

static void pwmout_sync_gclk(void)
{
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;
}

static void pwmout_sync_tcc(Tcc *tcc)
{
    while (tcc->SYNCBUSY.reg & TCC_SYNCBUSY_MASK)
        ;
}

static void pwmout_sync_tc(Tc *tc)
{
    while (tc->COUNT16.STATUS.bit.SYNCBUSY)
        ;
}

static uint16_t pwmout_tcc_read(unsigned channel)
{
    Tcc *tcc = (Tcc *)GetTC(channel);
    uint32_t per = tcc->PER.reg;
    uint32_t ccb = tcc->CCB[GetTCChannelNumber(channel)].reg;
    return (ccb * UINT16_MAX) / per;
}

static void pwmout_tcc_write(unsigned channel, uint16_t value)
{
    Tcc *tcc = (Tcc *)GetTC(channel);
    uint32_t duty = (value * tcc->PER.reg) / UINT16_MAX;
    tcc->CTRLBSET.bit.LUPD = 1;
    pwmout_sync_tcc(tcc);
    tcc->CCB[GetTCChannelNumber(channel)].reg = duty;
    pwmout_sync_tcc(tcc);
    tcc->CTRLBCLR.bit.LUPD = 1;
    pwmout_sync_tcc(tcc);
}

static void pwmout_tcc_period(unsigned channel, uint32_t value)
{
    //printf("TCC: value=%u\r\n", value);
    Tcc *tcc = (Tcc *)GetTC(channel);
    uint32_t div = 0;
    while (value > UINT16_MAX) {
        ++div;
        value >>= 1;
    }
    //printf("TCC: div=%u, per=%u\r\n", div, value);

    //GCLK->GENCTRL.reg = pwmout_gclk_genctrl[GetTCClockNumber(channel)];
    //pwmout_sync_gclk();

    if (div != 0) {
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(div - 1) | pwmout_gclk_gendiv[GetTCClockNumber(channel)];
        pwmout_sync_gclk();
        GCLK->GENCTRL.reg = GCLK_GENCTRL_DIVSEL | GCLK_GENCTRL_GENEN | pwmout_gclk_genctrl[GetTCClockNumber(channel)];
        pwmout_sync_gclk();
    } else {
        GCLK->GENDIV.reg = pwmout_gclk_gendiv[GetTCClockNumber(channel)];
        pwmout_sync_gclk();
        GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | pwmout_gclk_genctrl[GetTCClockNumber(channel)];
        pwmout_sync_gclk();
    }
    tcc->CTRLBSET.bit.LUPD = 1;
    pwmout_sync_tcc(tcc);
    tcc->PER.reg = value;
    pwmout_sync_tcc(tcc);
    tcc->CTRLBCLR.bit.LUPD = 1;
    pwmout_sync_tcc(tcc);
}

static void pwmout_tcc_pulsewidth(unsigned channel, uint32_t value)
{
    // not implemented (yet)
}

static uint16_t pwmout_tc_read(unsigned channel)
{
    Tc *tc = (Tc *)GetTC(channel);
    return tc->COUNT16.CC[GetTCChannelNumber(channel)].reg;
}

static void pwmout_tc_write(unsigned channel, uint16_t value)
{
    Tc *tc = (Tc *)GetTC(channel);
    tc->COUNT16.CC[GetTCChannelNumber(channel)].reg = value;
    pwmout_sync_tc(tc);
}

static void pwmout_tc_period(unsigned channel, uint32_t value)
{
    // not implemented (yet)
}

static void pwmout_tc_pulsewidth(unsigned channel, uint32_t value)
{
    // not implemented (yet)
}

static void pwmout_tcc_init(pwmout_t *obj, unsigned channel)
{
    static const pwmout_interface_t interface = {
        pwmout_tcc_read,
        pwmout_tcc_write,
        pwmout_tcc_period,
        pwmout_tcc_pulsewidth
    };
    Tcc *tcc = (Tcc *)GetTC(channel);
    tcc->CTRLA.bit.ENABLE = 0;
    pwmout_sync_tcc(tcc);
    tcc->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;
    pwmout_sync_tcc(tcc);
    tcc->CC[GetTCChannelNumber(channel)].reg = 0;
    pwmout_sync_tcc(tcc);
    tcc->PER.reg = UINT16_MAX;
    pwmout_sync_tcc(tcc);
    tcc->CTRLA.bit.ENABLE = 1;
    obj->interface = &interface;
    obj->channel = channel;
    pwmout_sync_tcc(tcc);
}

static void pwmout_tc_init(pwmout_t *obj, unsigned channel)
{
    static const pwmout_interface_t interface = {
        pwmout_tc_read,
        pwmout_tc_write,
        pwmout_tc_period,
        pwmout_tc_pulsewidth
    };
    Tc *tc = (Tc *)GetTC(channel);
    tc->COUNT16.CTRLA.bit.ENABLE = 0;
    pwmout_sync_tc(tc);
    tc->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_NPWM;
    pwmout_sync_tc(tc);
    tc->COUNT16.CC[GetTCChannelNumber(channel)].reg = 0;
    pwmout_sync_tc(tc);
    tc->COUNT16.CTRLA.bit.ENABLE = 1;
    obj->interface = &interface;
    obj->channel = channel;
    pwmout_sync_tc(tc);
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
    PinDescription pd = g_APinDescription[pin];
    if ((pd.ulPinAttribute & PIN_ATTR_PWM) == PIN_ATTR_PWM) {
        unsigned peripheral = (pd.ulPinAttribute & PIN_ATTR_TIMER) ? PIO_TIMER : PIO_TIMER_ALT;
        if (pd.ulPin & 1) {
            uint32_t tmp = (PORT->Group[pd.ulPort].PMUX[pd.ulPin >> 1].reg) & PORT_PMUX_PMUXE(0xF);
            PORT->Group[pd.ulPort].PMUX[pd.ulPin >> 1].reg = tmp | PORT_PMUX_PMUXO(peripheral);
        } else {
            uint32_t tmp = (PORT->Group[pd.ulPort].PMUX[pd.ulPin >> 1].reg) & PORT_PMUX_PMUXO(0xF);
            PORT->Group[pd.ulPort].PMUX[pd.ulPin >> 1].reg = tmp | PORT_PMUX_PMUXE(peripheral);
        }
        PORT->Group[pd.ulPort].PINCFG[pd.ulPin].reg |= PORT_PINCFG_PMUXEN;

        unsigned gclk = GetTCClockNumber(pd.ulPWMChannel);
        GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | pwmout_gclk_genctrl[gclk];
        pwmout_sync_gclk();
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | pwmout_gclk_clkctrl[gclk];
        pwmout_sync_gclk();

        if (GetTCNumber(pd.ulPWMChannel) < TCC_INST_NUM) {
            pwmout_tcc_init(obj, pd.ulPWMChannel);
        } else {
            pwmout_tc_init(obj, pd.ulPWMChannel);
        }
    } else {
        error("Not a PWM-enabled pin");
    }
}

void pwmout_write(pwmout_t *obj, float percent)
{
    uint16_t value = percent * 65535.0f;  // FIXME: restrict range?
    obj->interface->write(obj->channel, value);
}

float pwmout_read(pwmout_t *obj)
{
    uint16_t value = obj->interface->read(obj->channel);
    return value * (1.0f / 65535.0f);
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

void pwmout_period_us(pwmout_t *obj, int us)
{
    uint32_t cycles = microsecondsToClockCycles(us);
    obj->interface->period(obj->channel, cycles);
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    uint32_t cycles = microsecondsToClockCycles(us);
    obj->interface->pulsewidth(obj->channel, cycles);
}

#endif
