/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "clk: %s: " fmt, __func__

#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>

#include "clk-debug.h"

static struct measure_clk_data debug_mux_priv = {
	.ctl_reg = 0x62038,
	.status_reg = 0x6203C,
	.xo_div4_cbcr = 0x28008,
};

static const char *const debug_mux_parent_names[] = {
	"snoc_clk",
	"bimc_clk",
	"pnoc_clk",
	"disp_cc_mdss_ahb_clk",
	"disp_cc_mdss_byte0_clk",
	"disp_cc_mdss_byte0_intf_clk",
	"disp_cc_mdss_dp_aux_clk",
	"disp_cc_mdss_dp_crypto_clk",
	"disp_cc_mdss_dp_link_clk",
	"disp_cc_mdss_dp_link_intf_clk",
	"disp_cc_mdss_dp_pixel_clk",
	"disp_cc_mdss_esc0_clk",
	"disp_cc_mdss_mdp_clk",
	"disp_cc_mdss_mdp_lut_clk",
	"disp_cc_mdss_non_gdsc_ahb_clk",
	"disp_cc_mdss_pclk0_clk",
	"disp_cc_mdss_rot_clk",
	"disp_cc_mdss_vsync_clk",
	"disp_cc_sleep_clk",
	"disp_cc_xo_clk",
	"gcc_ahb2phy_csi_clk",
	"gcc_ahb2phy_usb_clk",
	"gcc_apc_vs_clk",
	"gcc_boot_rom_ahb_clk",
	"gcc_camera_ahb_clk",
	"gcc_camera_xo_clk",
	"gcc_camss_cci_ahb_clk",
	"gcc_camss_cci_clk",
	"gcc_camss_cphy_csid0_clk",
	"gcc_camss_cphy_csid1_clk",
	"gcc_camss_cphy_csid2_clk",
	"gcc_camss_cphy_csid3_clk",
	"gcc_camss_cpp_ahb_clk",
	"gcc_camss_cpp_axi_clk",
	"gcc_camss_cpp_clk",
	"gcc_camss_cpp_tsctr_clk",
	"gcc_camss_cpp_vbif_ahb_clk",
	"gcc_camss_csi0_ahb_clk",
	"gcc_camss_csi0_clk",
	"gcc_camss_csi0phytimer_clk",
	"gcc_camss_csi0pix_clk",
	"gcc_camss_csi0rdi_clk",
	"gcc_camss_csi1_ahb_clk",
	"gcc_camss_csi1_clk",
	"gcc_camss_csi1phytimer_clk",
	"gcc_camss_csi1pix_clk",
	"gcc_camss_csi1rdi_clk",
	"gcc_camss_csi2_ahb_clk",
	"gcc_camss_csi2_clk",
	"gcc_camss_csi2phytimer_clk",
	"gcc_camss_csi2pix_clk",
	"gcc_camss_csi2rdi_clk",
	"gcc_camss_csi3_ahb_clk",
	"gcc_camss_csi3_clk",
	"gcc_camss_csi3pix_clk",
	"gcc_camss_csi3rdi_clk",
	"gcc_camss_csi_vfe0_clk",
	"gcc_camss_csi_vfe1_clk",
	"gcc_camss_csiphy0_clk",
	"gcc_camss_csiphy1_clk",
	"gcc_camss_csiphy2_clk",
	"gcc_camss_csiphy3_clk",
	"gcc_camss_gp0_clk",
	"gcc_camss_gp1_clk",
	"gcc_camss_ispif_ahb_clk",
	"gcc_camss_jpeg_ahb_clk",
	"gcc_camss_jpeg_axi_clk",
	"gcc_camss_jpeg_clk",
	"gcc_camss_jpeg_tsctr_clk",
	"gcc_camss_mclk0_clk",
	"gcc_camss_mclk1_clk",
	"gcc_camss_mclk2_clk",
	"gcc_camss_mclk3_clk",
	"gcc_camss_micro_ahb_clk",
	"gcc_camss_throttle_nrt_axi_clk",
	"gcc_camss_throttle_rt_axi_clk",
	"gcc_camss_top_ahb_clk",
	"gcc_camss_vfe0_ahb_clk",
	"gcc_camss_vfe0_clk",
	"gcc_camss_vfe0_stream_clk",
	"gcc_camss_vfe1_ahb_clk",
	"gcc_camss_vfe1_clk",
	"gcc_camss_vfe1_stream_clk",
	"gcc_camss_vfe_tsctr_clk",
	"gcc_camss_vfe_vbif_ahb_clk",
	"gcc_camss_vfe_vbif_axi_clk",
	"gcc_ce1_ahb_clk",
	"gcc_ce1_axi_clk",
	"gcc_ce1_clk",
	"gcc_cfg_noc_usb3_prim_axi_clk",
	"gcc_cpuss_ahb_clk",
	"gcc_cpuss_gnoc_clk",
	"gcc_cpuss_throttle_core_clk",
	"gcc_cpuss_throttle_xo_clk",
	"gcc_disp_ahb_clk",
	"gcc_disp_gpll0_div_clk_src",
	"gcc_disp_hf_axi_clk",
	"gcc_disp_throttle_core_clk",
	"gcc_disp_xo_clk",
	"gcc_gp1_clk",
	"gcc_gp2_clk",
	"gcc_gp3_clk",
	"gcc_gpu_cfg_ahb_clk",
	"gcc_gpu_gpll0_clk_src",
	"gcc_gpu_gpll0_div_clk_src",
	"gcc_gpu_memnoc_gfx_clk",
	"gcc_gpu_snoc_dvm_gfx_clk",
	"gcc_gpu_throttle_core_clk",
	"gcc_gpu_throttle_xo_clk",
	"gcc_mss_vs_clk",
	"gcc_pdm2_clk",
	"gcc_pdm_ahb_clk",
	"gcc_pdm_xo4_clk",
	"gcc_prng_ahb_clk",
	"gcc_qmip_camera_nrt_ahb_clk",
	"gcc_qmip_camera_rt_ahb_clk",
	"gcc_qmip_cpuss_cfg_ahb_clk",
	"gcc_qmip_disp_ahb_clk",
	"gcc_qmip_gpu_cfg_ahb_clk",
	"gcc_qmip_video_vcodec_ahb_clk",
	"gcc_qupv3_wrap0_core_2x_clk",
	"gcc_qupv3_wrap0_core_clk",
	"gcc_qupv3_wrap0_s0_clk",
	"gcc_qupv3_wrap0_s1_clk",
	"gcc_qupv3_wrap0_s2_clk",
	"gcc_qupv3_wrap0_s3_clk",
	"gcc_qupv3_wrap0_s4_clk",
	"gcc_qupv3_wrap0_s5_clk",
	"gcc_qupv3_wrap1_core_2x_clk",
	"gcc_qupv3_wrap1_core_clk",
	"gcc_qupv3_wrap1_s0_clk",
	"gcc_qupv3_wrap1_s1_clk",
	"gcc_qupv3_wrap1_s2_clk",
	"gcc_qupv3_wrap1_s3_clk",
	"gcc_qupv3_wrap1_s4_clk",
	"gcc_qupv3_wrap1_s5_clk",
	"gcc_qupv3_wrap_0_m_ahb_clk",
	"gcc_qupv3_wrap_0_s_ahb_clk",
	"gcc_qupv3_wrap_1_m_ahb_clk",
	"gcc_qupv3_wrap_1_s_ahb_clk",
	"gcc_sdcc1_ahb_clk",
	"gcc_sdcc1_apps_clk",
	"gcc_sdcc1_ice_core_clk",
	"gcc_sdcc2_ahb_clk",
	"gcc_sdcc2_apps_clk",
	"gcc_sys_noc_cpuss_ahb_clk",
	"gcc_sys_noc_ufs_phy_axi_clk",
	"gcc_sys_noc_usb3_prim_axi_clk",
	"gcc_ufs_phy_ahb_clk",
	"gcc_ufs_phy_axi_clk",
	"gcc_ufs_phy_ice_core_clk",
	"gcc_ufs_phy_phy_aux_clk",
	"gcc_ufs_phy_rx_symbol_0_clk",
	"gcc_ufs_phy_tx_symbol_0_clk",
	"gcc_ufs_phy_unipro_core_clk",
	"gcc_usb30_prim_master_clk",
	"gcc_usb30_prim_mock_utmi_clk",
	"gcc_usb30_prim_sleep_clk",
	"gcc_usb3_prim_phy_com_aux_clk",
	"gcc_usb3_prim_phy_pipe_clk",
	"gcc_vdda_vs_clk",
	"gcc_vddcx_vs_clk",
	"gcc_vddmx_vs_clk",
	"gcc_video_ahb_clk",
	"gcc_video_axi0_clk",
	"gcc_video_throttle_core_clk",
	"gcc_video_xo_clk",
	"gcc_vs_ctrl_ahb_clk",
	"gcc_vs_ctrl_clk",
	"gcc_wcss_vs_clk",
	"gpu_cc_ahb_clk",
	"gpu_cc_crc_ahb_clk",
	"gpu_cc_cx_apb_clk",
	"gpu_cc_cx_gfx3d_clk",
	"gpu_cc_cx_gfx3d_slv_clk",
	"gpu_cc_cx_gmu_clk",
	"gpu_cc_cx_snoc_dvm_clk",
	"gpu_cc_cxo_aon_clk",
	"gpu_cc_cxo_clk",
	"gpu_cc_gx_cxo_clk",
	"gpu_cc_gx_gfx3d_clk",
	"gpu_cc_sleep_clk",
	"measure_only_mccc_clk",
	"measure_only_ipa_2x_clk",
	"video_cc_apb_clk",
	"video_cc_at_clk",
	"video_cc_sleep_clk",
	"video_cc_vcodec0_axi_clk",
	"video_cc_vcodec0_core_clk",
	"video_cc_venus_ahb_clk",
	"video_cc_venus_ctl_axi_clk",
	"video_cc_venus_ctl_core_clk",
	"video_cc_xo_clk",
	"pwrcl_clk",
	"perfcl_clk",
};

static struct clk_debug_mux gcc_debug_mux = {
	.priv = &debug_mux_priv,
	.debug_offset = 0x62000,
	.post_div_offset = 0x30000,
	.cbcr_offset = 0x30004,
	.period_offset = 0x20,
	.src_sel_mask = 0x3FF,
	.src_sel_shift = 0,
	.post_div_mask = 0xF,
	.post_div_shift = 0,
	MUX_SRC_LIST(
		{ "snoc_clk", 0x7, 1, GCC,
			0x7, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "bimc_clk", 0x97, 1, GCC,
			0x97, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "pnoc_clk", 0x11, 1, GCC,
			0x11, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "disp_cc_mdss_ahb_clk", 0x41, 1, DISP_CC,
			0x1A, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_byte0_clk", 0x41, 1, DISP_CC,
			0x12, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_byte0_intf_clk", 0x41, 1, DISP_CC,
			0x13, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_aux_clk", 0x41, 1, DISP_CC,
			0x19, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_crypto_clk", 0x41, 1, DISP_CC,
			0x17, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_link_clk", 0x41, 1, DISP_CC,
			0x15, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_link_intf_clk", 0x41, 1, DISP_CC,
			0x16, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_pixel_clk", 0x41, 1, DISP_CC,
			0x18, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_esc0_clk", 0x41, 1, DISP_CC,
			0x14, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_mdp_clk", 0x41, 1, DISP_CC,
			0xE, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_mdp_lut_clk", 0x41, 1, DISP_CC,
			0x10, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_non_gdsc_ahb_clk", 0x41, 1, DISP_CC,
			0x1B, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_pclk0_clk", 0x41, 1, DISP_CC,
			0xD, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_rot_clk", 0x41, 1, DISP_CC,
			0xF, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_vsync_clk", 0x41, 1, DISP_CC,
			0x11, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_sleep_clk", 0x41, 1, DISP_CC,
			0x24, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_xo_clk", 0x41, 1, DISP_CC,
			0x23, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "gcc_ahb2phy_csi_clk", 0x5F, 1, GCC,
			0x5F, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ahb2phy_usb_clk", 0x60, 1, GCC,
			0x60, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_apc_vs_clk", 0xBF, 1, GCC,
			0xBF, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_boot_rom_ahb_clk", 0x72, 1, GCC,
			0x72, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camera_ahb_clk", 0x36, 1, GCC,
			0x36, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camera_xo_clk", 0x3E, 1, GCC,
			0x3E, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cci_ahb_clk", 0x11F, 1, GCC,
			0x11F, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cci_clk", 0x11E, 1, GCC,
			0x11E, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cphy_csid0_clk", 0x13A, 1, GCC,
			0x13A, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cphy_csid1_clk", 0x140, 1, GCC,
			0x140, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cphy_csid2_clk", 0x145, 1, GCC,
			0x145, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cphy_csid3_clk", 0x14B, 1, GCC,
			0x14B, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cpp_ahb_clk", 0x154, 1, GCC,
			0x154, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cpp_axi_clk", 0x156, 1, GCC,
			0x156, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cpp_clk", 0x153, 1, GCC,
			0x153, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cpp_tsctr_clk", 0x158, 1, GCC,
			0x158, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_cpp_vbif_ahb_clk", 0x155, 1, GCC,
			0x155, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi0_ahb_clk", 0x13B, 1, GCC,
			0x13B, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi0_clk", 0x139, 1, GCC,
			0x139, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi0phytimer_clk", 0x120, 1, GCC,
			0x120, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi0pix_clk", 0x13D, 1, GCC,
			0x13D, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi0rdi_clk", 0x13C, 1, GCC,
			0x13C, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi1_ahb_clk", 0x141, 1, GCC,
			0x141, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi1_clk", 0x13F, 1, GCC,
			0x13F, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi1phytimer_clk", 0x121, 1, GCC,
			0x121, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi1pix_clk", 0x143, 1, GCC,
			0x143, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi1rdi_clk", 0x142, 1, GCC,
			0x142, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi2_ahb_clk", 0x146, 1, GCC,
			0x146, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi2_clk", 0x144, 1, GCC,
			0x144, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi2phytimer_clk", 0x122, 1, GCC,
			0x122, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi2pix_clk", 0x148, 1, GCC,
			0x148, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi2rdi_clk", 0x147, 1, GCC,
			0x147, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi3_ahb_clk", 0x14C, 1, GCC,
			0x14C, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi3_clk", 0x14A, 1, GCC,
			0x14A, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi3pix_clk", 0x14E, 1, GCC,
			0x14E, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi3rdi_clk", 0x14D, 1, GCC,
			0x14D, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi_vfe0_clk", 0x12E, 1, GCC,
			0x12E, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csi_vfe1_clk", 0x12F, 1, GCC,
			0x12F, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csiphy0_clk", 0x135, 1, GCC,
			0x135, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csiphy1_clk", 0x136, 1, GCC,
			0x136, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csiphy2_clk", 0x137, 1, GCC,
			0x137, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_csiphy3_clk", 0x138, 1, GCC,
			0x138, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_gp0_clk", 0x118, 1, GCC,
			0x118, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_gp1_clk", 0x119, 1, GCC,
			0x119, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_ispif_ahb_clk", 0x134, 1, GCC,
			0x134, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_jpeg_ahb_clk", 0x124, 1, GCC,
			0x124, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_jpeg_axi_clk", 0x125, 1, GCC,
			0x125, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_jpeg_clk", 0x123, 1, GCC,
			0x123, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_jpeg_tsctr_clk", 0x127, 1, GCC,
			0x127, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_mclk0_clk", 0x11A, 1, GCC,
			0x11A, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_mclk1_clk", 0x11B, 1, GCC,
			0x11B, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_mclk2_clk", 0x11C, 1, GCC,
			0x11C, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_mclk3_clk", 0x11D, 1, GCC,
			0x11D, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_micro_ahb_clk", 0x152, 1, GCC,
			0x152, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_throttle_nrt_axi_clk", 0x150, 1, GCC,
			0x150, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_throttle_rt_axi_clk", 0x151, 1, GCC,
			0x151, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_top_ahb_clk", 0x14F, 1, GCC,
			0x14F, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe0_ahb_clk", 0x12A, 1, GCC,
			0x12A, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe0_clk", 0x128, 1, GCC,
			0x128, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe0_stream_clk", 0x129, 1, GCC,
			0x129, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe1_ahb_clk", 0x12D, 1, GCC,
			0x12D, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe1_clk", 0x12B, 1, GCC,
			0x12B, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe1_stream_clk", 0x12C, 1, GCC,
			0x12C, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe_tsctr_clk", 0x133, 1, GCC,
			0x133, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe_vbif_ahb_clk", 0x130, 1, GCC,
			0x130, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_camss_vfe_vbif_axi_clk", 0x131, 1, GCC,
			0x131, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ce1_ahb_clk", 0x82, 1, GCC,
			0x82, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ce1_axi_clk", 0x81, 1, GCC,
			0x81, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ce1_clk", 0x80, 1, GCC,
			0x80, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_cfg_noc_usb3_prim_axi_clk", 0x1D, 1, GCC,
			0x1D, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_cpuss_ahb_clk", 0xA5, 1, GCC,
			0xA5, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_cpuss_gnoc_clk", 0xA6, 1, GCC,
			0xA6, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_cpuss_throttle_core_clk", 0xAE, 1, GCC,
			0xAE, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_cpuss_throttle_xo_clk", 0xAD, 1, GCC,
			0xAD, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_disp_ahb_clk", 0x37, 1, GCC,
			0x37, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_disp_gpll0_div_clk_src", 0x46, 1, GCC,
			0x46, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_disp_hf_axi_clk", 0x3C, 1, GCC,
			0x3C, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_disp_throttle_core_clk", 0x48, 1, GCC,
			0x48, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_disp_xo_clk", 0x3F, 1, GCC,
			0x3F, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gp1_clk", 0xB6, 1, GCC,
			0xB6, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gp2_clk", 0xB7, 1, GCC,
			0xB7, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gp3_clk", 0xB8, 1, GCC,
			0xB8, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gpu_cfg_ahb_clk", 0xDB, 1, GCC,
			0xDB, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gpu_gpll0_clk_src", 0xE1, 1, GCC,
			0xE1, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gpu_gpll0_div_clk_src", 0xE2, 1, GCC,
			0xE2, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gpu_memnoc_gfx_clk", 0xDE, 1, GCC,
			0xDE, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gpu_snoc_dvm_gfx_clk", 0xE0, 1, GCC,
			0xE0, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gpu_throttle_core_clk", 0xE5, 1, GCC,
			0xE5, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_gpu_throttle_xo_clk", 0xE4, 1, GCC,
			0xE4, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_mss_vs_clk", 0xBE, 1, GCC,
			0xBE, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_pdm2_clk", 0x6F, 1, GCC,
			0x6F, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_pdm_ahb_clk", 0x6D, 1, GCC,
			0x6D, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_pdm_xo4_clk", 0x6E, 1, GCC,
			0x6E, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_prng_ahb_clk", 0x70, 1, GCC,
			0x70, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qmip_camera_nrt_ahb_clk", 0x39, 1, GCC,
			0x39, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qmip_camera_rt_ahb_clk", 0x47, 1, GCC,
			0x47, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qmip_cpuss_cfg_ahb_clk", 0xAC, 1, GCC,
			0xAC, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qmip_disp_ahb_clk", 0x3A, 1, GCC,
			0x3A, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qmip_gpu_cfg_ahb_clk", 0xE3, 1, GCC,
			0xE3, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qmip_video_vcodec_ahb_clk", 0x38, 1, GCC,
			0x38, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap0_core_2x_clk", 0x66, 1, GCC,
			0x66, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap0_core_clk", 0x65, 1, GCC,
			0x65, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap0_s0_clk", 0x67, 1, GCC,
			0x67, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap0_s1_clk", 0x68, 1, GCC,
			0x68, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap0_s2_clk", 0x69, 1, GCC,
			0x69, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap0_s3_clk", 0x6A, 1, GCC,
			0x6A, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap0_s4_clk", 0x6B, 1, GCC,
			0x6B, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap0_s5_clk", 0x6C, 1, GCC,
			0x6C, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap1_core_2x_clk", 0xED, 1, GCC,
			0xED, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap1_core_clk", 0xEC, 1, GCC,
			0xEC, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap1_s0_clk", 0xEE, 1, GCC,
			0xEE, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap1_s1_clk", 0xEF, 1, GCC,
			0xEF, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap1_s2_clk", 0xF0, 1, GCC,
			0xF0, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap1_s3_clk", 0xF1, 1, GCC,
			0xF1, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap1_s4_clk", 0xF2, 1, GCC,
			0xF2, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap1_s5_clk", 0xF3, 1, GCC,
			0xF3, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap_0_m_ahb_clk", 0x63, 1, GCC,
			0x63, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap_0_s_ahb_clk", 0x64, 1, GCC,
			0x64, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap_1_m_ahb_clk", 0xEA, 1, GCC,
			0xEA, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_qupv3_wrap_1_s_ahb_clk", 0xEB, 1, GCC,
			0xEB, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_sdcc1_ahb_clk", 0xE8, 1, GCC,
			0xE8, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_sdcc1_apps_clk", 0xE7, 1, GCC,
			0xE7, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_sdcc1_ice_core_clk", 0xE9, 1, GCC,
			0xE9, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_sdcc2_ahb_clk", 0x62, 1, GCC,
			0x62, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_sdcc2_apps_clk", 0x61, 1, GCC,
			0x61, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_sys_noc_cpuss_ahb_clk", 0x9, 1, GCC,
			0x9, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_sys_noc_ufs_phy_axi_clk", 0x19, 1, GCC,
			0x19, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_sys_noc_usb3_prim_axi_clk", 0x18, 1, GCC,
			0x18, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ufs_phy_ahb_clk", 0x10F, 1, GCC,
			0x10F, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ufs_phy_axi_clk", 0x10E, 1, GCC,
			0x10E, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ufs_phy_ice_core_clk", 0x115, 1, GCC,
			0x115, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ufs_phy_phy_aux_clk", 0x116, 1, GCC,
			0x116, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ufs_phy_rx_symbol_0_clk", 0x111, 1, GCC,
			0x111, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ufs_phy_tx_symbol_0_clk", 0x110, 1, GCC,
			0x110, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_ufs_phy_unipro_core_clk", 0x114, 1, GCC,
			0x114, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_usb30_prim_master_clk", 0x58, 1, GCC,
			0x58, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_usb30_prim_mock_utmi_clk", 0x5A, 1, GCC,
			0x5A, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_usb30_prim_sleep_clk", 0x59, 1, GCC,
			0x59, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_usb3_prim_phy_com_aux_clk", 0x5B, 1, GCC,
			0x5B, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_usb3_prim_phy_pipe_clk", 0x5C, 1, GCC,
			0x5C, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_vdda_vs_clk", 0xBB, 1, GCC,
			0xBB, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_vddcx_vs_clk", 0xB9, 1, GCC,
			0xB9, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_vddmx_vs_clk", 0xBA, 1, GCC,
			0xBA, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_video_ahb_clk", 0x35, 1, GCC,
			0x35, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_video_axi0_clk", 0x3B, 1, GCC,
			0x3B, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_video_throttle_core_clk", 0x49, 1, GCC,
			0x49, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_video_xo_clk", 0x3D, 1, GCC,
			0x3D, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_vs_ctrl_ahb_clk", 0xBD, 1, GCC,
			0xBD, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_vs_ctrl_clk", 0xBC, 1, GCC,
			0xBC, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gcc_wcss_vs_clk", 0xC0, 1, GCC,
			0xC0, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "gpu_cc_ahb_clk", 0xDD, 1, GPU_CC,
			0x10, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_crc_ahb_clk", 0xDD, 1, GPU_CC,
			0x11, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_apb_clk", 0xDD, 1, GPU_CC,
			0x14, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_gfx3d_clk", 0xDD, 1, GPU_CC,
			0x1A, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_gfx3d_slv_clk", 0xDD, 1, GPU_CC,
			0x1B, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_gmu_clk", 0xDD, 1, GPU_CC,
			0x18, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_snoc_dvm_clk", 0xDD, 1, GPU_CC,
			0x15, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cxo_aon_clk", 0xDD, 1, GPU_CC,
			0xA, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cxo_clk", 0xDD, 1, GPU_CC,
			0x19, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_gx_cxo_clk", 0xDD, 1, GPU_CC,
			0xE, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_gx_gfx3d_clk", 0xDD, 1, GPU_CC,
			0xB, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_sleep_clk", 0xDD, 1, GPU_CC,
			0x16, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "measure_only_mccc_clk", 0x97, 1, MC_CC,
			0x97, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "measure_only_ipa_2x_clk", 0xC2, 1, GCC,
			0xC2, 0x3FF, 0, 0xF, 0, 1, 0x62000, 0x30000, 0x30004 },
		{ "video_cc_apb_clk", 0x42, 1, VIDEO_CC,
			0x8, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "video_cc_at_clk", 0x42, 1, VIDEO_CC,
			0xB, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "video_cc_sleep_clk", 0x42, 1, VIDEO_CC,
			0xD, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "video_cc_vcodec0_axi_clk", 0x42, 1, VIDEO_CC,
			0x6, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "video_cc_vcodec0_core_clk", 0x42, 1, VIDEO_CC,
			0x3, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "video_cc_venus_ahb_clk", 0x42, 1, VIDEO_CC,
			0x9, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "video_cc_venus_ctl_axi_clk", 0x42, 1, VIDEO_CC,
			0x5, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "video_cc_venus_ctl_core_clk", 0x42, 1, VIDEO_CC,
			0x1, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "video_cc_xo_clk", 0x42, 1, VIDEO_CC,
			0xC, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0xA30, 0xA38 },
		{ "pwrcl_clk", 0xAB, 4, CPU_CC,
			0x0, 0x3FF, 8, 0xF, 28, 1, 0x0, 0x0, U32_MAX, 8 },
		{ "perfcl_clk", 0xAB, 4, CPU_CC,
			0x1, 0x3FF, 8, 0xF, 28, 1, 0x0, 0x0, U32_MAX, 8 },
	),
	.hw.init = &(struct clk_init_data){
		.name = "gcc_debug_mux",
		.ops = &clk_debug_mux_ops,
		.parent_names = debug_mux_parent_names,
		.num_parents = ARRAY_SIZE(debug_mux_parent_names),
		.flags = CLK_IS_MEASURE,
	},
};

static const struct of_device_id clk_debug_match_table[] = {
	{ .compatible = "qcom,debugcc-trinket" },
	{ }
};

static int map_debug_bases(struct platform_device *pdev, char *base, int cc)
{
	if (!of_get_property(pdev->dev.of_node, base, NULL)) {
		pr_err("error in of_get prop\n");
		return -ENODEV;
	}

	gcc_debug_mux.regmap[cc] =
		syscon_regmap_lookup_by_phandle(pdev->dev.of_node,
				base);
	if (IS_ERR(gcc_debug_mux.regmap[cc])) {
		pr_err("Failed to map %s (ret=%ld)\n", base,
			PTR_ERR(gcc_debug_mux.regmap[cc]));
		return PTR_ERR(gcc_debug_mux.regmap[cc]);
	}

	return 0;
}

static int clk_debug_trinket_probe(struct platform_device *pdev)
{
	struct clk *clk;
	int ret;

	clk = devm_clk_get(&pdev->dev, "cxo");
	if (IS_ERR(clk)) {
		if (PTR_ERR(clk) != -EPROBE_DEFER)
			dev_err(&pdev->dev, "Unable to get xo clock\n");
		return PTR_ERR(clk);
	}

	debug_mux_priv.cxo = clk;

	gcc_debug_mux.regmap = devm_kcalloc(&pdev->dev, MAX_NUM_CC,
				sizeof(*gcc_debug_mux.regmap), GFP_KERNEL);
	if (!gcc_debug_mux.regmap)
		return -ENOMEM;

	ret = map_debug_bases(pdev, "qcom,gcc", GCC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,videocc", VIDEO_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,gpucc", GPU_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,dispcc", DISP_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,mccc", MC_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,cpucc", CPU_CC);
	if (ret)
		return ret;

	clk = devm_clk_register(&pdev->dev, &gcc_debug_mux.hw);
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "Unable to register GCC debug mux\n");
		return PTR_ERR(clk);
	}

	ret = clk_debug_measure_register(&gcc_debug_mux.hw);
	if (ret)
		dev_err(&pdev->dev, "Could not register Measure clock\n");

	return ret;
}

static struct platform_driver clk_debug_driver = {
	.probe = clk_debug_trinket_probe,
	.driver = {
		.name = "debugcc-trinket",
		.of_match_table = clk_debug_match_table,
	},
};

int __init clk_debug_trinket_init(void)
{
	return platform_driver_register(&clk_debug_driver);
}
fs_initcall(clk_debug_trinket_init);

MODULE_DESCRIPTION("QTI DEBUG CC TRINKET Driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:debugcc-trinket");
