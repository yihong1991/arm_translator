#ifndef PLATFORMDESC_H
#define PLATFORMDESC_H

struct PlatformDesc {
    size_t m_contextSize;
    size_t m_pcFieldOffset;
    size_t m_prologueSize;
    size_t m_assistSize;
    size_t m_tcgSize;
};

#endif /* PLATFORMDESC_H */
