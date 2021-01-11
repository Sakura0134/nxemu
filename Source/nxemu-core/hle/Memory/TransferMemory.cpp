#include <nxemu-core\hle\Memory\TransferMemory.h>

CTransferMemory::CTransferMemory(uint64_t Addr, uint64_t Size, uint32_t Permissions) :
    m_Addr(Addr),
    m_Size(Size),
    m_Permissions(Permissions)
{
}