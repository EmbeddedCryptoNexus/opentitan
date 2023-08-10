// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/lib/testing/test_framework/check.h"
#include "sw/device/lib/testing/test_framework/ottf_main.h"
#include "sw/device/tests/sim_dv/pwrmgr_sleep_all_wake_ups_impl.h"
#include "sw/ip/aon_timer/test/utils/aon_timer_testutils.h"
#include "sw/ip/pwrmgr/dif/dif_pwrmgr.h"
#include "sw/ip/pwrmgr/test/utils/pwrmgr_testutils.h"
#include "sw/ip/rv_plic/dif/dif_rv_plic.h"
#include "sw/ip/rv_plic/test/utils/rv_plic_testutils.h"
#include "sw/lib/sw/device/runtime/log.h"

#include "hw/top_darjeeling/sw/autogen/top_darjeeling.h"
#include "pwrmgr_regs.h"
#include "sw/top_darjeeling/sw/test/utils/autogen/isr_testutils.h"

/*
  PWRMGR NORMAL SLEEP ALL WAKE UPS test

  This test runs power manager wake up from deep sleep mode by
  wake up inputs.

  There are 6 wake up inputs.
  0: sysrst_ctrl
  1: adc_ctrl
  2: pinmux
  3: usb
  4: aon_timer
  5: sensor_ctrl

 */

OTTF_DEFINE_TEST_CONFIG();

bool test_main(void) {
  // Enable global and external IRQ at Ibex.
  irq_global_ctrl(true);
  irq_external_ctrl(true);

  init_units();

  // Enable all the AON interrupts used in this test.
  rv_plic_testutils_irq_range_enable(&rv_plic, kTopDarjeelingPlicTargetIbex0,
                                     kTopDarjeelingPlicIrqIdPwrmgrAonWakeup,
                                     kTopDarjeelingPlicIrqIdPwrmgrAonWakeup);

  // Enable pwrmgr interrupt
  CHECK_DIF_OK(dif_pwrmgr_irq_set_enabled(&pwrmgr, 0, kDifToggleEnabled));

  if (UNWRAP(pwrmgr_testutils_is_wakeup_reason(&pwrmgr, 0)) == true) {
    LOG_INFO("POR reset");

    for (size_t i = 0; i < PWRMGR_PARAM_NUM_WKUPS; ++i) {
      LOG_INFO("Test %d begin", i);
      execute_test(i, /*deep_sleep=*/false);
      check_wakeup_reason(i);
      LOG_INFO("Woke up by source %d", i);
      cleanup(i);
      LOG_INFO("clean up done source %d", i);
    }

    return true;
  }

  return false;
}