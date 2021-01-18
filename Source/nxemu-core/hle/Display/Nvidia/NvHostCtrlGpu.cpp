#include <nxemu-core\hle\Display\Nvidia\NvHostCtrlGpu.h>
#include <nxemu-core\SystemGlobals.h>

CNvHostCtrlGpu::CNvHostCtrlGpu(void)
{
}

nvResult CNvHostCtrlGpu::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData)
{
    if (Ioctl.Group == nvIoctl::NvGpuMagic)
    {
        switch (Ioctl.Cmd)
        {
        case IOCTL_GET_CHARACTERISTICS:
            GetCharacteristics(InData, OutData);
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return nvResult_Success;
}

void CNvHostCtrlGpu::GetCharacteristics(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData)
{
	if (InData.size() < sizeof(NvGpuGpuGetCharacteristics))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    OutData.resize(sizeof(NvGpuGpuGetCharacteristics));
    memcpy(OutData.data(), InData.data(), InData.size());
    NvGpuGpuGetCharacteristics & Characteristics = *((NvGpuGpuGetCharacteristics *)OutData.data());
    Characteristics.BufferSize = 0xa0;
    Characteristics.BufferAddress = 0xdeadbeef;
    Characteristics.arch = 0x120;
    Characteristics.impl = 0xb;
    Characteristics.rev = 0xa1;
    Characteristics.num_gpc = 0x1;
    Characteristics.l2_cache_size = 0x40000;
    Characteristics.on_board_video_memory_size = 0x0;
    Characteristics.num_tpc_per_gpc = 0x2;
    Characteristics.bus_type = 0x20;
    Characteristics.big_page_size = 0x20000;
    Characteristics.compression_page_size = 0x20000;
    Characteristics.pde_coverage_bit_count = 0x1b;
    Characteristics.available_big_page_sizes = 0x30000;
    Characteristics.gpc_mask = 0x1;
    Characteristics.sm_arch_sm_version = 0x503;
    Characteristics.sm_arch_spa_version = 0x503;
    Characteristics.sm_arch_warp_count = 0x80;
    Characteristics.gpu_va_bit_count = 0x28;
    Characteristics.reserved = 0x0;
    Characteristics.flags = 0x55;
    Characteristics.twod_class = 0x902d;
    Characteristics.threed_class = 0xb197;
    Characteristics.compute_class = 0xb1c0;
    Characteristics.gpfifo_class = 0xb06f;
    Characteristics.inline_to_memory_class = 0xa140;
    Characteristics.dma_copy_class = 0xb0b5;
    Characteristics.max_fbps_count = 0x1;
    Characteristics.fbp_en_mask = 0x0;
    Characteristics.max_ltc_per_fbp = 0x2;
    Characteristics.max_lts_per_ltc = 0x1;
    Characteristics.max_tex_per_tpc = 0x0;
    Characteristics.max_gpc_count = 0x1;
    Characteristics.rop_l2_en_mask_0 = 0x21d70;
    Characteristics.rop_l2_en_mask_1 = 0x0;
    Characteristics.chipname = 0x6230326d67;
    Characteristics.gr_compbit_store_base_hw = 0x0;
}
