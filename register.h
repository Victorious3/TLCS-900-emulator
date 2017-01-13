#include "900L.h"

// Register access

/// General purpose & bank registers, byte
inline BYTE cpu_getr_b(BYTE reg);

/// General purpose & bank registers, word
inline WORD cpu_getr_w(BYTE reg);

/// General purpose & bank registers, dword
inline DWORD cpu_getr_dw(BYTE reg);

/// General purpose & bank registers, byte
inline void cpu_setr_b(BYTE reg, BYTE value);

/// General purpose & bank registers, word
inline void cpu_setr_w(BYTE reg, WORD value);

/// General purpose & bank registers, dword
inline void cpu_setr_dw(BYTE reg, DWORD value);

inline BYTE cpu_getR_b(BYTE reg);

inline WORD cpu_getR_w(BYTE reg);

inline DWORD cpu_getR_dw(BYTE reg);

inline void cpu_setR_b(BYTE reg, BYTE value);

inline void cpu_setR_w(BYTE reg, WORD value);

inline void cpu_setR_dw(BYTE reg, DWORD value);
