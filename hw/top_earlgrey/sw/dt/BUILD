# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

load("//rules/opentitan:defs.bzl", "OPENTITAN_CPU")

package(default_visibility = ["//visibility:public"])

TARGETS = [
    "fpga_cw310",
    "fpga_cw340",
    "sim_dv",
    "sim_qemu",
    "sim_verilator",
    "silicon",
]

[
    cc_library(
        name = target,
        srcs = [
            "{}.c".format(target),
        ],
        deps = [
            "//hw/top:dt_api",
            "//sw/device/lib/base:macros",
        ],
    )
    for target in TARGETS
]
