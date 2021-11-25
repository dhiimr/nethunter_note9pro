/*
 * Copyright (c) 2015-2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _DSI_CTRL_REG_H_
#define _DSI_CTRL_REG_H_

#define DSI_HW_VERSION                             (0x0000)
#define DSI_CTRL                                   (0x0004)
#define DSI_STATUS                                 (0x0008)
#define DSI_FIFO_STATUS                            (0x000C)
#define DSI_VIDEO_MODE_CTRL                        (0x0010)
#define DSI_VIDEO_MODE_SYNC_DATATYPE               (0x0014)
#define DSI_VIDEO_MODE_PIXEL_DATATYPE              (0x0018)
#define DSI_VIDEO_MODE_BLANKING_DATATYPE           (0x001C)
#define DSI_VIDEO_MODE_DATA_CTRL                   (0x0020)
#define DSI_VIDEO_MODE_ACTIVE_H                    (0x0024)
#define DSI_VIDEO_MODE_ACTIVE_V                    (0x0028)
#define DSI_VIDEO_MODE_TOTAL                       (0x002C)
#define DSI_VIDEO_MODE_HSYNC                       (0x0030)
#define DSI_VIDEO_MODE_VSYNC                       (0x0034)
#define DSI_VIDEO_MODE_VSYNC_VPOS                  (0x0038)
#define DSI_COMMAND_MODE_DMA_CTRL                  (0x003C)
#define DSI_COMMAND_MODE_MDP_CTRL                  (0x0040)
#define DSI_COMMAND_MODE_MDP_DCS_CMD_CTRL          (0x0044)
#define DSI_DMA_CMD_OFFSET                         (0x0048)
#define DSI_DMA_CMD_LENGTH                         (0x004C)
#define DSI_DMA_FIFO_CTRL                          (0x0050)
#define DSI_DMA_NULL_PACKET_DATA                   (0x0054)
#define DSI_COMMAND_MODE_MDP_STREAM0_CTRL          (0x0058)
#define DSI_COMMAND_MODE_MDP_STREAM0_TOTAL         (0x005C)
#define DSI_COMMAND_MODE_MDP_STREAM1_CTRL          (0x0060)
#define DSI_COMMAND_MODE_MDP_STREAM1_TOTAL         (0x0064)
#define DSI_ACK_ERR_STATUS                         (0x0068)
#define DSI_RDBK_DATA0                             (0x006C)
#define DSI_RDBK_DATA1                             (0x0070)
#define DSI_RDBK_DATA2                             (0x0074)
#define DSI_RDBK_DATA3                             (0x0078)
#define DSI_RDBK_DATATYPE0                         (0x007C)
#define DSI_RDBK_DATATYPE1                         (0x0080)
#define DSI_TRIG_CTRL                              (0x0084)
#define DSI_EXT_MUX                                (0x0088)
#define DSI_EXT_MUX_TE_PULSE_DETECT_CTRL           (0x008C)
#define DSI_CMD_MODE_DMA_SW_TRIGGER                (0x0090)
#define DSI_CMD_MODE_MDP_SW_TRIGGER                (0x0094)
#define DSI_CMD_MODE_BTA_SW_TRIGGER                (0x0098)
#define DSI_RESET_SW_TRIGGER                       (0x009C)
#define DSI_MISR_CMD_CTRL                          (0x00A0)
#define DSI_MISR_VIDEO_CTRL                        (0x00A4)
#define DSI_LANE_STATUS                            (0x00A8)
#define DSI_LANE_CTRL                              (0x00AC)
#define DSI_LANE_SWAP_CTRL                         (0x00B0)
#define DSI_DLN0_PHY_ERR                           (0x00B4)
#define DSI_LP_TIMER_CTRL                          (0x00B8)
#define DSI_HS_TIMER_CTRL                          (0x00BC)
#define DSI_TIMEOUT_STATUS                         (0x00C0)
#define DSI_CLKOUT_TIMING_CTRL                     (0x00C4)
#define DSI_EOT_PACKET                             (0x00C8)
#define DSI_EOT_PACKET_CTRL                        (0x00CC)
#define DSI_GENERIC_ESC_TX_TRIGGER                 (0x00D0)
#define DSI_CAM_BIST_CTRL                          (0x00D4)
#define DSI_CAM_BIST_FRAME_SIZE                    (0x00D8)
#define DSI_CAM_BIST_BLOCK_SIZE                    (0x00DC)
#define DSI_CAM_BIST_FRAME_CONFIG                  (0x00E0)
#define DSI_CAM_BIST_LSFR_CTRL                     (0x00E4)
#define DSI_CAM_BIST_LSFR_INIT                     (0x00E8)
#define DSI_CAM_BIST_START                         (0x00EC)
#define DSI_CAM_BIST_STATUS                        (0x00F0)
#define DSI_ERR_INT_MASK0                          (0x010C)
#define DSI_INT_CTRL                               (0x0110)
#define DSI_IOBIST_CTRL                            (0x0114)
#define DSI_SOFT_RESET                             (0x0118)
#define DSI_CLK_CTRL                               (0x011C)
#define DSI_CLK_STATUS                             (0x0120)
#define DSI_DEBUG_BUS_CTL                          (0x0124)
#define DSI_DEBUG_BUS_STATUS                       (0x0128)
#define DSI_PHY_SW_RESET                           (0x012C)
#define DSI_AXI2AHB_CTRL                           (0x0130)
#define DSI_MISR_CMD_MDP0_32BIT                    (0x0134)
#define DSI_MISR_CMD_MDP1_32BIT                    (0x0138)
#define DSI_MISR_CMD_DMA_32BIT                     (0x013C)
#define DSI_MISR_VIDEO_32BIT                       (0x0140)
#define DSI_LANE_MISR_CTRL                         (0x0144)
#define DSI_LANE0_MISR                             (0x0148)
#define DSI_LANE1_MISR                             (0x014C)
#define DSI_LANE2_MISR                             (0x0150)
#define DSI_LANE3_MISR                             (0x0154)
#define DSI_TEST_PATTERN_GEN_CTRL                  (0x015C)
#define DSI_TEST_PATTERN_GEN_VIDEO_POLY            (0x0160)
#define DSI_TEST_PATTERN_GEN_VIDEO_INIT_VAL        (0x0164)
#define DSI_TEST_PATTERN_GEN_CMD_MDP_STREAM0_POLY  (0x0168)
#define DSI_TEST_PATTERN_GEN_CMD_MDP_INIT_VAL0     (0x016C)
#define DSI_TEST_PATTERN_GEN_CMD_MDP_STREAM1_POLY  (0x0170)
#define DSI_TEST_PATTERN_GEN_CMD_MDP_INIT_VAL1     (0x0174)
#define DSI_TEST_PATTERN_GEN_CMD_DMA_POLY          (0x0178)
#define DSI_TEST_PATTERN_GEN_CMD_DMA_INIT_VAL      (0x017C)
#define DSI_TEST_PATTERN_GEN_VIDEO_ENABLE          (0x0180)
#define DSI_TEST_PATTERN_GEN_CMD_STREAM0_TRIGGER   (0x0184)
#define DSI_TEST_PATTERN_GEN_CMD_STREAM1_TRIGGER   (0x0188)
#define DSI_TEST_PATTERN_GEN_CMD_MDP_INIT_VAL2     (0x018C)
#define DSI_TEST_PATTERN_GEN_CMD_MDP_STREAM2_POLY  (0x0190)
#define DSI_TEST_PATTERN_GEN_CMD_MDP_STREAM2_POLY  (0x0190)
#define DSI_COMMAND_MODE_MDP_IDLE_CTRL             (0x0194)
#define DSI_TEST_PATTERN_GEN_CMD_STREAM2_TRIGGER   (0x0198)
#define DSI_TPG_MAIN_CONTROL                       (0x019C)
#define DSI_TPG_MAIN_CONTROL2                      (0x01A0)
#define DSI_TPG_VIDEO_CONFIG                       (0x01A4)
#define DSI_TPG_COMPONENT_LIMITS                   (0x01A8)
#define DSI_TPG_RECTANGLE                          (0x01AC)
#define DSI_TPG_BLACK_WHITE_PATTERN_FRAMES         (0x01B0)
#define DSI_TPG_RGB_MAPPING                        (0x01B4)
#define DSI_COMMAND_MODE_MDP_CTRL2                 (0x01B8)
#define DSI_COMMAND_MODE_MDP_STREAM2_CTRL          (0x01BC)
#define DSI_COMMAND_MODE_MDP_STREAM2_TOTAL         (0x01C0)
#define DSI_MISR_CMD_MDP2_8BIT                     (0x01C4)
#define DSI_MISR_CMD_MDP2_32BIT                    (0x01C8)
#define DSI_VBIF_CTRL                              (0x01CC)
#define DSI_AES_CTRL                               (0x01D0)
#define DSI_RDBK_DATA_CTRL                         (0x01D4)
#define DSI_TEST_PATTERN_GEN_CMD_DMA_INIT_VAL2     (0x01D8)
#define DSI_TPG_DMA_FIFO_STATUS                    (0x01DC)
#define DSI_TPG_DMA_FIFO_WRITE_TRIGGER             (0x01E0)
#define DSI_DSI_TIMING_FLUSH                       (0x01E4)
#define DSI_DSI_TIMING_DB_MODE                     (0x01E8)
#define DSI_TPG_DMA_FIFO_RESET                     (0x01EC)
#define DSI_SCRATCH_REGISTER_0                     (0x01F0)
#define DSI_VERSION                                (0x01F4)
#define DSI_SCRATCH_REGISTER_1                     (0x01F8)
#define DSI_SCRATCH_REGISTER_2                     (0x01FC)
#define DSI_DYNAMIC_REFRESH_CTRL                   (0x0200)
#define DSI_DYNAMIC_REFRESH_STATUS                 (0x0210)
#define DSI_VIDEO_COMPRESSION_MODE_CTRL            (0x02A0)
#define DSI_VIDEO_COMPRESSION_MODE_CTRL2           (0x02A4)
#define DSI_COMMAND_COMPRESSION_MODE_CTRL          (0x02A8)
#define DSI_COMMAND_COMPRESSION_MODE_CTRL2         (0x02AC)
#define DSI_COMMAND_COMPRESSION_MODE_CTRL3         (0x02B0)
#define DSI_COMMAND_MODE_NULL_INSERTION_CTRL       (0x02B4)
#define DSI_READ_BACK_DISABLE_STATUS               (0x02B8)
#define DSI_DESKEW_CTRL                            (0x02BC)
#define DSI_DESKEW_DELAY_CTRL                      (0x02C0)
#define DSI_DESKEW_SW_TRIGGER                      (0x02C4)
#define DSI_DEBUG_CTRL                             (0x02C8)
#define DSI_SECURE_DISPLAY_STATUS                  (0x02CC)
#define DSI_SECURE_DISPLAY_BLOCK_COMMAND_COLOR     (0x02D0)
#define DSI_SECURE_DISPLAY_BLOCK_VIDEO_COLOR       (0x02D4)
#define DSI_CPHY_MODE_CTRL                         (0x02D8)
#define DSI_LOGICAL_LANE_SWAP_CTRL                 (0x0310)
#define DSI_SPLIT_LINK                             (0x0330)


#endif /* _DSI_CTRL_REG_H_ */
