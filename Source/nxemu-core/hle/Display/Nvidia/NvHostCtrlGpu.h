#pragma once
#include <nxemu-core\hle\Display\Nvidia\NvDevice.h>
#include <Common\stdtypes.h>
#include <vector>

class CNvHostCtrlGpu :
    public INvDevice
{
    enum
    {
        IOCTL_ZCULL_GET_CTX_SIZE = 0x01,
        IOCTL_ZCULL_GET_INFO = 0x02,
        IOCTL_GET_CHARACTERISTICS = 0x05,
        IOCTL_GET_TPC_MASKS = 0x06,
        IOCTL_ZBC_GET_ACTIVE_SLOT_MASK = 0x14,
    };

    struct NvGpuGpuGetCharacteristics
    {
        uint64_t BufferSize;
        uint64_t BufferAddress;
        uint32_t arch;                           // 0x120 (NVGPU_GPU_ARCH_GM200)
        uint32_t impl;                           // 0xB (NVGPU_GPU_IMPL_GM20B)
        uint32_t rev;                            // 0xA1 (Revision A1)
        uint32_t num_gpc;                        // 0x1
        uint64_t l2_cache_size;                  // 0x40000
        uint64_t on_board_video_memory_size;     // 0x0 (not used)
        uint32_t num_tpc_per_gpc;                // 0x2
        uint32_t bus_type;                       // 0x20 (NVGPU_GPU_BUS_TYPE_AXI)
        uint32_t big_page_size;                  // 0x20000
        uint32_t compression_page_size;          // 0x20000
        uint32_t pde_coverage_bit_count;         // 0x1B
        uint32_t available_big_page_sizes;       // 0x30000
        uint32_t gpc_mask;                       // 0x1
        uint32_t sm_arch_sm_version;             // 0x503 (Maxwell Generation 5.0.3?)
        uint32_t sm_arch_spa_version;            // 0x503 (Maxwell Generation 5.0.3?)
        uint32_t sm_arch_warp_count;             // 0x80
        uint32_t gpu_va_bit_count;               // 0x28
        uint32_t reserved;                       // nullptr
        uint64_t flags;                          // 0x55
        uint32_t twod_class;                     // 0x902D (FERMI_TWOD_A)
        uint32_t threed_class;                   // 0xB197 (MAXWELL_B)
        uint32_t compute_class;                  // 0xB1C0 (MAXWELL_COMPUTE_B)
        uint32_t gpfifo_class;                   // 0xB06F (MAXWELL_CHANNEL_GPFIFO_A)
        uint32_t inline_to_memory_class;         // 0xA140 (KEPLER_INLINE_TO_MEMORY_B)
        uint32_t dma_copy_class;                 // 0xB0B5 (MAXWELL_DMA_COPY_A)
        uint32_t max_fbps_count;                 // 0x1
        uint32_t fbp_en_mask;                    // 0x0 (disabled)
        uint32_t max_ltc_per_fbp;                // 0x2
        uint32_t max_lts_per_ltc;                // 0x1
        uint32_t max_tex_per_tpc;                // 0x0 (not supported)
        uint32_t max_gpc_count;                  // 0x1
        uint32_t rop_l2_en_mask_0;               // 0x21D70 (fuse_status_opt_rop_l2_fbp_r)
        uint32_t rop_l2_en_mask_1;               // 0x0
        uint64_t chipname;                       // 0x6230326D67 ("gm20b")
        uint64_t gr_compbit_store_base_hw;       // 0x0 (not supported)
    };

    struct NvGpuGpuGetTpcMasks
    {
        uint32_t MaskBufferSize;
        uint32_t Reserved;
        uint64_t MaskBufferAddress;
        uint32_t TpcMask;
        uint32_t Padding;
    };

public:
    CNvHostCtrlGpu(void);
    nvResult Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer& InData, CIPCRequest::RequestBuffer& OutData);

private:
    CNvHostCtrlGpu(const CNvHostCtrlGpu&);
    CNvHostCtrlGpu& operator=(const CNvHostCtrlGpu&);

    void ZcullGetCtxSize(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    void GetCharacteristics(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
    void GetTpcMasks(const std::vector<uint8_t> & InData, std::vector<uint8_t> & OutData);
};