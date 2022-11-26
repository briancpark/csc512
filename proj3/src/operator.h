/*
 *  Copyright (c) 2020-2021 Xuhpclab. All rights reserved.
 *  Licensed under the MIT License.
 *  See LICENSE file for more information.
 */

#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#include "register.h"

namespace runtime_profiling {
typedef enum  {
    /*   0 */ kOPINVALID,
    /* NULL, */ /**< INVALID opcode */
    /*   1 */ kOPUNDECODED,
    /* NULL, */ /**< UNDECODED opcode */
    /*   2 */ kOPCONTD,
    /* NULL, */ /**< CONTD opcode */
    /*   3 */ kOPLABEL,
    /* NULL, */ /**< LABEL opcode */

    /*   4 */ kOPadd,      /**< IA-32/AMD64 add opcode. */
    /*   5 */ kOPor,       /**< IA-32/AMD64 or opcode. */
    /*   6 */ kOPadc,      /**< IA-32/AMD64 adc opcode. */
    /*   7 */ kOPsbb,      /**< IA-32/AMD64 sbb opcode. */
    /*   8 */ kOPand,      /**< IA-32/AMD64 and opcode. */
    /*   9 */ kOPdaa,      /**< IA-32/AMD64 daa opcode. */
    /*  10 */ kOPsub,      /**< IA-32/AMD64 sub opcode. */
    /*  11 */ kOPdas,      /**< IA-32/AMD64 das opcode. */
    /*  12 */ kOPxor,      /**< IA-32/AMD64 xor opcode. */
    /*  13 */ kOPaaa,      /**< IA-32/AMD64 aaa opcode. */
    /*  14 */ kOPcmp,      /**< IA-32/AMD64 cmp opcode. */
    /*  15 */ kOPaas,      /**< IA-32/AMD64 aas opcode. */
    /*  16 */ kOPinc,      /**< IA-32/AMD64 inc opcode. */
    /*  17 */ kOPdec,      /**< IA-32/AMD64 dec opcode. */
    /*  18 */ kOPpush,     /**< IA-32/AMD64 push opcode. */
    /*  19 */ kOPpush_imm, /**< IA-32/AMD64 push_imm opcode. */
    /*  20 */ kOPpop,      /**< IA-32/AMD64 pop opcode. */
    /*  21 */ kOPpusha,    /**< IA-32/AMD64 pusha opcode. */
    /*  22 */ kOPpopa,     /**< IA-32/AMD64 popa opcode. */
    /*  23 */ kOPbound,    /**< IA-32/AMD64 bound opcode. */
    /*  24 */ kOParpl,     /**< IA-32/AMD64 arpl opcode. */
    /*  25 */ kOPimul,     /**< IA-32/AMD64 imul opcode. */

    /*  26 */ kOPjo_short,   /**< IA-32/AMD64 jo_short opcode. */
    /*  27 */ kOPjno_short,  /**< IA-32/AMD64 jno_short opcode. */
    /*  28 */ kOPjb_short,   /**< IA-32/AMD64 jb_short opcode. */
    /*  29 */ kOPjnb_short,  /**< IA-32/AMD64 jnb_short opcode. */
    /*  30 */ kOPjz_short,   /**< IA-32/AMD64 jz_short opcode. */
    /*  31 */ kOPjnz_short,  /**< IA-32/AMD64 jnz_short opcode. */
    /*  32 */ kOPjbe_short,  /**< IA-32/AMD64 jbe_short opcode. */
    /*  33 */ kOPjnbe_short, /**< IA-32/AMD64 jnbe_short opcode. */
    /*  34 */ kOPjs_short,   /**< IA-32/AMD64 js_short opcode. */
    /*  35 */ kOPjns_short,  /**< IA-32/AMD64 jns_short opcode. */
    /*  36 */ kOPjp_short,   /**< IA-32/AMD64 jp_short opcode. */
    /*  37 */ kOPjnp_short,  /**< IA-32/AMD64 jnp_short opcode. */
    /*  38 */ kOPjl_short,   /**< IA-32/AMD64 jl_short opcode. */
    /*  39 */ kOPjnl_short,  /**< IA-32/AMD64 jnl_short opcode. */
    /*  40 */ kOPjle_short,  /**< IA-32/AMD64 jle_short opcode. */
    /*  41 */ kOPjnle_short, /**< IA-32/AMD64 jnle_short opcode. */

    /*  42 */ kOPcall,         /**< IA-32/AMD64 call opcode. */
    /*  43 */ kOPcall_ind,     /**< IA-32/AMD64 call_ind opcode. */
    /*  44 */ kOPcall_far,     /**< IA-32/AMD64 call_far opcode. */
    /*  45 */ kOPcall_far_ind, /**< IA-32/AMD64 call_far_ind opcode. */
    /*  46 */ kOPjmp,          /**< IA-32/AMD64 jmp opcode. */
    /*  47 */ kOPjmp_short,    /**< IA-32/AMD64 jmp_short opcode. */
    /*  48 */ kOPjmp_ind,      /**< IA-32/AMD64 jmp_ind opcode. */
    /*  49 */ kOPjmp_far,      /**< IA-32/AMD64 jmp_far opcode. */
    /*  50 */ kOPjmp_far_ind,  /**< IA-32/AMD64 jmp_far_ind opcode. */

    /*  51 */ kOPloopne, /**< IA-32/AMD64 loopne opcode. */
    /*  52 */ kOPloope,  /**< IA-32/AMD64 loope opcode. */
    /*  53 */ kOPloop,   /**< IA-32/AMD64 loop opcode. */
    /*  54 */ kOPjecxz,  /**< IA-32/AMD64 jecxz opcode. */

    /* point ld & st at eAX & al instrs, they save 1 byte (no modrm),
     * hopefully time taken considering them doesn't offset that */
    /*  55 */ kOPmov_ld, /**< IA-32/AMD64 mov_ld opcode. */
    /*  56 */ kOPmov_st, /**< IA-32/AMD64 mov_st opcode. */
    /* PR 250397: store of immed is mov_st not mov_imm, even though can be immed->reg,
     * which we address by sharing part of the mov_st template chain */
    /*  57 */ kOPmov_imm,  /**< IA-32/AMD64 mov_imm opcode. */
    /*  58 */ kOPmov_seg,  /**< IA-32/AMD64 mov_seg opcode. */
    /*  59 */ kOPmov_priv, /**< IA-32/AMD64 mov_priv opcode. */

    /*  60 */ kOPtest,  /**< IA-32/AMD64 test opcode. */
    /*  61 */ kOPlea,   /**< IA-32/AMD64 lea opcode. */
    /*  62 */ kOPxchg,  /**< IA-32/AMD64 xchg opcode. */
    /*  63 */ kOPcwde,  /**< IA-32/AMD64 cwde opcode. */
    /*  64 */ kOPcdq,   /**< IA-32/AMD64 cdq opcode. */
    /*  65 */ kOPfwait, /**< IA-32/AMD64 fwait opcode. */
    /*  66 */ kOPpushf, /**< IA-32/AMD64 pushf opcode. */
    /*  67 */ kOPpopf,  /**< IA-32/AMD64 popf opcode. */
    /*  68 */ kOPsahf,  /**< IA-32/AMD64 sahf opcode. */
    /*  69 */ kOPlahf,  /**< IA-32/AMD64 lahf opcode. */

    /*  70 */ kOPret,     /**< IA-32/AMD64 ret opcode. */
    /*  71 */ kOPret_far, /**< IA-32/AMD64 ret_far opcode. */

    /*  72 */ kOPles,   /**< IA-32/AMD64 les opcode. */
    /*  73 */ kOPlds,   /**< IA-32/AMD64 lds opcode. */
    /*  74 */ kOPenter, /**< IA-32/AMD64 enter opcode. */
    /*  75 */ kOPleave, /**< IA-32/AMD64 leave opcode. */
    /*  76 */ kOPint3,  /**< IA-32/AMD64 int3 opcode. */
    /*  77 */ kOPint,   /**< IA-32/AMD64 int opcode. */
    /*  78 */ kOPinto,  /**< IA-32/AMD64 into opcode. */
    /*  79 */ kOPiret,  /**< IA-32/AMD64 iret opcode. */
    /*  80 */ kOPaam,   /**< IA-32/AMD64 aam opcode. */
    /*  81 */ kOPaad,   /**< IA-32/AMD64 aad opcode. */
    /*  82 */ kOPxlat,  /**< IA-32/AMD64 xlat opcode. */
    /*  83 */ kOPin,    /**< IA-32/AMD64 in opcode. */
    /*  84 */ kOPout,   /**< IA-32/AMD64 out opcode. */
    /*  85 */ kOPhlt,   /**< IA-32/AMD64 hlt opcode. */
    /*  86 */ kOPcmc,   /**< IA-32/AMD64 cmc opcode. */
    /*  87 */ kOPclc,   /**< IA-32/AMD64 clc opcode. */
    /*  88 */ kOPstc,   /**< IA-32/AMD64 stc opcode. */
    /*  89 */ kOPcli,   /**< IA-32/AMD64 cli opcode. */
    /*  90 */ kOPsti,   /**< IA-32/AMD64 sti opcode. */
    /*  91 */ kOPcld,   /**< IA-32/AMD64 cld opcode. */
    /*  92 */ kOPstd,   /**< IA-32/AMD64 std opcode. */

    /*  93 */ kOPlar,       /**< IA-32/AMD64 lar opcode. */
    /*  94 */ kOPlsl,       /**< IA-32/AMD64 lsl opcode. */
    /*  95 */ kOPsyscall,   /**< IA-32/AMD64 syscall opcode. */
    /*  96 */ kOPclts,      /**< IA-32/AMD64 clts opcode. */
    /*  97 */ kOPsysret,    /**< IA-32/AMD64 sysret opcode. */
    /*  98 */ kOPinvd,      /**< IA-32/AMD64 invd opcode. */
    /*  99 */ kOPwbinvd,    /**< IA-32/AMD64 wbinvd opcode. */
    /* 100 */ kOPud2a,      /**< IA-32/AMD64 ud2a opcode. */
    /* 101 */ kOPnkOPmodrm, /**< IA-32/AMD64 nkOPmodrm opcode. */
    /* 102 */ kOPmovntps,   /**< IA-32/AMD64 movntps opcode. */
    /* 103 */ kOPmovntpd,   /**< IA-32/AMD64 movntpd opcode. */
    /* 104 */ kOPwrmsr,     /**< IA-32/AMD64 wrmsr opcode. */
    /* 105 */ kOPrdtsc,     /**< IA-32/AMD64 rdtsc opcode. */
    /* 106 */ kOPrdmsr,     /**< IA-32/AMD64 rdmsr opcode. */
    /* 107 */ kOPrdpmc,     /**< IA-32/AMD64 rdpmc opcode. */
    /* 108 */ kOPsysenter,  /**< IA-32/AMD64 sysenter opcode. */
    /* 109 */ kOPsysexit,   /**< IA-32/AMD64 sysexit opcode. */

    /* 110 */ kOPcmovo,   /**< IA-32/AMD64 cmovo opcode. */
    /* 111 */ kOPcmovno,  /**< IA-32/AMD64 cmovno opcode. */
    /* 112 */ kOPcmovb,   /**< IA-32/AMD64 cmovb opcode. */
    /* 113 */ kOPcmovnb,  /**< IA-32/AMD64 cmovnb opcode. */
    /* 114 */ kOPcmovz,   /**< IA-32/AMD64 cmovz opcode. */
    /* 115 */ kOPcmovnz,  /**< IA-32/AMD64 cmovnz opcode. */
    /* 116 */ kOPcmovbe,  /**< IA-32/AMD64 cmovbe opcode. */
    /* 117 */ kOPcmovnbe, /**< IA-32/AMD64 cmovnbe opcode. */
    /* 118 */ kOPcmovs,   /**< IA-32/AMD64 cmovs opcode. */
    /* 119 */ kOPcmovns,  /**< IA-32/AMD64 cmovns opcode. */
    /* 120 */ kOPcmovp,   /**< IA-32/AMD64 cmovp opcode. */
    /* 121 */ kOPcmovnp,  /**< IA-32/AMD64 cmovnp opcode. */
    /* 122 */ kOPcmovl,   /**< IA-32/AMD64 cmovl opcode. */
    /* 123 */ kOPcmovnl,  /**< IA-32/AMD64 cmovnl opcode. */
    /* 124 */ kOPcmovle,  /**< IA-32/AMD64 cmovle opcode. */
    /* 125 */ kOPcmovnle, /**< IA-32/AMD64 cmovnle opcode. */

    /* 126 */ kOPpunpcklbw,  /**< IA-32/AMD64 punpcklbw opcode. */
    /* 127 */ kOPpunpcklwd,  /**< IA-32/AMD64 punpcklwd opcode. */
    /* 128 */ kOPpunpckldq,  /**< IA-32/AMD64 punpckldq opcode. */
    /* 129 */ kOPpacksswb,   /**< IA-32/AMD64 packsswb opcode. */
    /* 130 */ kOPpcmpgtb,    /**< IA-32/AMD64 pcmpgtb opcode. */
    /* 131 */ kOPpcmpgtw,    /**< IA-32/AMD64 pcmpgtw opcode. */
    /* 132 */ kOPpcmpgtd,    /**< IA-32/AMD64 pcmpgtd opcode. */
    /* 133 */ kOPpackuswb,   /**< IA-32/AMD64 packuswb opcode. */
    /* 134 */ kOPpunpckhbw,  /**< IA-32/AMD64 punpckhbw opcode. */
    /* 135 */ kOPpunpckhwd,  /**< IA-32/AMD64 punpckhwd opcode. */
    /* 136 */ kOPpunpckhdq,  /**< IA-32/AMD64 punpckhdq opcode. */
    /* 137 */ kOPpackssdw,   /**< IA-32/AMD64 packssdw opcode. */
    /* 138 */ kOPpunpcklqdq, /**< IA-32/AMD64 punpcklqdq opcode. */
    /* 139 */ kOPpunpckhqdq, /**< IA-32/AMD64 punpckhqdq opcode. */
    /* 140 */ kOPmovd,       /**< IA-32/AMD64 movd opcode. */
    /* 141 */ kOPmovq,       /**< IA-32/AMD64 movq opcode. */
    /* 142 */ kOPmovdqu,     /**< IA-32/AMD64 movdqu opcode. */
    /* 143 */ kOPmovdqa,     /**< IA-32/AMD64 movdqa opcode. */
    /* 144 */ kOPpshufw,     /**< IA-32/AMD64 pshufw opcode. */
    /* 145 */ kOPpshufd,     /**< IA-32/AMD64 pshufd opcode. */
    /* 146 */ kOPpshufhw,    /**< IA-32/AMD64 pshufhw opcode. */
    /* 147 */ kOPpshuflw,    /**< IA-32/AMD64 pshuflw opcode. */
    /* 148 */ kOPpcmpeqb,    /**< IA-32/AMD64 pcmpeqb opcode. */
    /* 149 */ kOPpcmpeqw,    /**< IA-32/AMD64 pcmpeqw opcode. */
    /* 150 */ kOPpcmpeqd,    /**< IA-32/AMD64 pcmpeqd opcode. */
    /* 151 */ kOPemms,       /**< IA-32/AMD64 emms opcode. */

    /* 152 */ kOPjo,   /**< IA-32/AMD64 jo opcode. */
    /* 153 */ kOPjno,  /**< IA-32/AMD64 jno opcode. */
    /* 154 */ kOPjb,   /**< IA-32/AMD64 jb opcode. */
    /* 155 */ kOPjnb,  /**< IA-32/AMD64 jnb opcode. */
    /* 156 */ kOPjz,   /**< IA-32/AMD64 jz opcode. */
    /* 157 */ kOPjnz,  /**< IA-32/AMD64 jnz opcode. */
    /* 158 */ kOPjbe,  /**< IA-32/AMD64 jbe opcode. */
    /* 159 */ kOPjnbe, /**< IA-32/AMD64 jnbe opcode. */
    /* 160 */ kOPjs,   /**< IA-32/AMD64 js opcode. */
    /* 161 */ kOPjns,  /**< IA-32/AMD64 jns opcode. */
    /* 162 */ kOPjp,   /**< IA-32/AMD64 jp opcode. */
    /* 163 */ kOPjnp,  /**< IA-32/AMD64 jnp opcode. */
    /* 164 */ kOPjl,   /**< IA-32/AMD64 jl opcode. */
    /* 165 */ kOPjnl,  /**< IA-32/AMD64 jnl opcode. */
    /* 166 */ kOPjle,  /**< IA-32/AMD64 jle opcode. */
    /* 167 */ kOPjnle, /**< IA-32/AMD64 jnle opcode. */

    /* 168 */ kOPseto,   /**< IA-32/AMD64 seto opcode. */
    /* 169 */ kOPsetno,  /**< IA-32/AMD64 setno opcode. */
    /* 170 */ kOPsetb,   /**< IA-32/AMD64 setb opcode. */
    /* 171 */ kOPsetnb,  /**< IA-32/AMD64 setnb opcode. */
    /* 172 */ kOPsetz,   /**< IA-32/AMD64 setz opcode. */
    /* 173 */ kOPsetnz,  /**< IA-32/AMD64 setnz opcode. */
    /* 174 */ kOPsetbe,  /**< IA-32/AMD64 setbe opcode. */
    /* 175 */ kOPsetnbe, /**< IA-32/AMD64 setnbe opcode. */
    /* 176 */ kOPsets,   /**< IA-32/AMD64 sets opcode. */
    /* 177 */ kOPsetns,  /**< IA-32/AMD64 setns opcode. */
    /* 178 */ kOPsetp,   /**< IA-32/AMD64 setp opcode. */
    /* 179 */ kOPsetnp,  /**< IA-32/AMD64 setnp opcode. */
    /* 180 */ kOPsetl,   /**< IA-32/AMD64 setl opcode. */
    /* 181 */ kOPsetnl,  /**< IA-32/AMD64 setnl opcode. */
    /* 182 */ kOPsetle,  /**< IA-32/AMD64 setle opcode. */
    /* 183 */ kOPsetnle, /**< IA-32/AMD64 setnle opcode. */

    /* 184 */ kOPcpuid,      /**< IA-32/AMD64 cpuid opcode. */
    /* 185 */ kOPbt,         /**< IA-32/AMD64 bt opcode. */
    /* 186 */ kOPshld,       /**< IA-32/AMD64 shld opcode. */
    /* 187 */ kOPrsm,        /**< IA-32/AMD64 rsm opcode. */
    /* 188 */ kOPbts,        /**< IA-32/AMD64 bts opcode. */
    /* 189 */ kOPshrd,       /**< IA-32/AMD64 shrd opcode. */
    /* 190 */ kOPcmpxchg,    /**< IA-32/AMD64 cmpxchg opcode. */
    /* 191 */ kOPlss,        /**< IA-32/AMD64 lss opcode. */
    /* 192 */ kOPbtr,        /**< IA-32/AMD64 btr opcode. */
    /* 193 */ kOPlfs,        /**< IA-32/AMD64 lfs opcode. */
    /* 194 */ kOPlgs,        /**< IA-32/AMD64 lgs opcode. */
    /* 195 */ kOPmovzx,      /**< IA-32/AMD64 movzx opcode. */
    /* 196 */ kOPud2b,       /**< IA-32/AMD64 ud2b opcode. */
    /* 197 */ kOPbtc,        /**< IA-32/AMD64 btc opcode. */
    /* 198 */ kOPbsf,        /**< IA-32/AMD64 bsf opcode. */
    /* 199 */ kOPbsr,        /**< IA-32/AMD64 bsr opcode. */
    /* 200 */ kOPmovsx,      /**< IA-32/AMD64 movsx opcode. */
    /* 201 */ kOPxadd,       /**< IA-32/AMD64 xadd opcode. */
    /* 202 */ kOPmovnti,     /**< IA-32/AMD64 movnti opcode. */
    /* 203 */ kOPpinsrw,     /**< IA-32/AMD64 pinsrw opcode. */
    /* 204 */ kOPpextrw,     /**< IA-32/AMD64 pextrw opcode. */
    /* 205 */ kOPbswap,      /**< IA-32/AMD64 bswap opcode. */
    /* 206 */ kOPpsrlw,      /**< IA-32/AMD64 psrlw opcode. */
    /* 207 */ kOPpsrld,      /**< IA-32/AMD64 psrld opcode. */
    /* 208 */ kOPpsrlq,      /**< IA-32/AMD64 psrlq opcode. */
    /* 209 */ kOPpaddq,      /**< IA-32/AMD64 paddq opcode. */
    /* 210 */ kOPpmullw,     /**< IA-32/AMD64 pmullw opcode. */
    /* 211 */ kOPpmovmskb,   /**< IA-32/AMD64 pmovmskb opcode. */
    /* 212 */ kOPpsubusb,    /**< IA-32/AMD64 psubusb opcode. */
    /* 213 */ kOPpsubusw,    /**< IA-32/AMD64 psubusw opcode. */
    /* 214 */ kOPpminub,     /**< IA-32/AMD64 pminub opcode. */
    /* 215 */ kOPpand,       /**< IA-32/AMD64 pand opcode. */
    /* 216 */ kOPpaddusb,    /**< IA-32/AMD64 paddusb opcode. */
    /* 217 */ kOPpaddusw,    /**< IA-32/AMD64 paddusw opcode. */
    /* 218 */ kOPpmaxub,     /**< IA-32/AMD64 pmaxub opcode. */
    /* 219 */ kOPpandn,      /**< IA-32/AMD64 pandn opcode. */
    /* 220 */ kOPpavgb,      /**< IA-32/AMD64 pavgb opcode. */
    /* 221 */ kOPpsraw,      /**< IA-32/AMD64 psraw opcode. */
    /* 222 */ kOPpsrad,      /**< IA-32/AMD64 psrad opcode. */
    /* 223 */ kOPpavgw,      /**< IA-32/AMD64 pavgw opcode. */
    /* 224 */ kOPpmulhuw,    /**< IA-32/AMD64 pmulhuw opcode. */
    /* 225 */ kOPpmulhw,     /**< IA-32/AMD64 pmulhw opcode. */
    /* 226 */ kOPmovntq,     /**< IA-32/AMD64 movntq opcode. */
    /* 227 */ kOPmovntdq,    /**< IA-32/AMD64 movntdq opcode. */
    /* 228 */ kOPpsubsb,     /**< IA-32/AMD64 psubsb opcode. */
    /* 229 */ kOPpsubsw,     /**< IA-32/AMD64 psubsw opcode. */
    /* 230 */ kOPpminsw,     /**< IA-32/AMD64 pminsw opcode. */
    /* 231 */ kOPpor,        /**< IA-32/AMD64 por opcode. */
    /* 232 */ kOPpaddsb,     /**< IA-32/AMD64 paddsb opcode. */
    /* 233 */ kOPpaddsw,     /**< IA-32/AMD64 paddsw opcode. */
    /* 234 */ kOPpmaxsw,     /**< IA-32/AMD64 pmaxsw opcode. */
    /* 235 */ kOPpxor,       /**< IA-32/AMD64 pxor opcode. */
    /* 236 */ kOPpsllw,      /**< IA-32/AMD64 psllw opcode. */
    /* 237 */ kOPpslld,      /**< IA-32/AMD64 pslld opcode. */
    /* 238 */ kOPpsllq,      /**< IA-32/AMD64 psllq opcode. */
    /* 239 */ kOPpmuludq,    /**< IA-32/AMD64 pmuludq opcode. */
    /* 240 */ kOPpmaddwd,    /**< IA-32/AMD64 pmaddwd opcode. */
    /* 241 */ kOPpsadbw,     /**< IA-32/AMD64 psadbw opcode. */
    /* 242 */ kOPmaskmovq,   /**< IA-32/AMD64 maskmovq opcode. */
    /* 243 */ kOPmaskmovdqu, /**< IA-32/AMD64 maskmovdqu opcode. */
    /* 244 */ kOPpsubb,      /**< IA-32/AMD64 psubb opcode. */
    /* 245 */ kOPpsubw,      /**< IA-32/AMD64 psubw opcode. */
    /* 246 */ kOPpsubd,      /**< IA-32/AMD64 psubd opcode. */
    /* 247 */ kOPpsubq,      /**< IA-32/AMD64 psubq opcode. */
    /* 248 */ kOPpaddb,      /**< IA-32/AMD64 paddb opcode. */
    /* 249 */ kOPpaddw,      /**< IA-32/AMD64 paddw opcode. */
    /* 250 */ kOPpaddd,      /**< IA-32/AMD64 paddd opcode. */
    /* 251 */ kOPpsrldq,     /**< IA-32/AMD64 psrldq opcode. */
    /* 252 */ kOPpslldq,     /**< IA-32/AMD64 pslldq opcode. */

    /* 253 */ kOProl,         /**< IA-32/AMD64 rol opcode. */
    /* 254 */ kOPror,         /**< IA-32/AMD64 ror opcode. */
    /* 255 */ kOPrcl,         /**< IA-32/AMD64 rcl opcode. */
    /* 256 */ kOPrcr,         /**< IA-32/AMD64 rcr opcode. */
    /* 257 */ kOPshl,         /**< IA-32/AMD64 shl opcode. */
    /* 258 */ kOPshr,         /**< IA-32/AMD64 shr opcode. */
    /* 259 */ kOPsar,         /**< IA-32/AMD64 sar opcode. */
    /* 260 */ kOPnot,         /**< IA-32/AMD64 not opcode. */
    /* 261 */ kOPneg,         /**< IA-32/AMD64 neg opcode. */
    /* 262 */ kOPmul,         /**< IA-32/AMD64 mul opcode. */
    /* 263 */ kOPdiv,         /**< IA-32/AMD64 div opcode. */
    /* 264 */ kOPidiv,        /**< IA-32/AMD64 idiv opcode. */
    /* 265 */ kOPsldt,        /**< IA-32/AMD64 sldt opcode. */
    /* 266 */ kOPstr,         /**< IA-32/AMD64 str opcode. */
    /* 267 */ kOPlldt,        /**< IA-32/AMD64 lldt opcode. */
    /* 268 */ kOPltr,         /**< IA-32/AMD64 ltr opcode. */
    /* 269 */ kOPverr,        /**< IA-32/AMD64 verr opcode. */
    /* 270 */ kOPverw,        /**< IA-32/AMD64 verw opcode. */
    /* 271 */ kOPsgdt,        /**< IA-32/AMD64 sgdt opcode. */
    /* 272 */ kOPsidt,        /**< IA-32/AMD64 sidt opcode. */
    /* 273 */ kOPlgdt,        /**< IA-32/AMD64 lgdt opcode. */
    /* 274 */ kOPlidt,        /**< IA-32/AMD64 lidt opcode. */
    /* 275 */ kOPsmsw,        /**< IA-32/AMD64 smsw opcode. */
    /* 276 */ kOPlmsw,        /**< IA-32/AMD64 lmsw opcode. */
    /* 277 */ kOPinvlpg,      /**< IA-32/AMD64 invlpg opcode. */
    /* 278 */ kOPcmpxchg8b,   /**< IA-32/AMD64 cmpxchg8b opcode. */
    /* 279 */ kOPfxsave32,    /**< IA-32/AMD64 fxsave opcode. */
    /* 280 */ kOPfxrstor32,   /**< IA-32/AMD64 fxrstor opcode. */
    /* 281 */ kOPldmxcsr,     /**< IA-32/AMD64 ldmxcsr opcode. */
    /* 282 */ kOPstmxcsr,     /**< IA-32/AMD64 stmxcsr opcode. */
    /* 283 */ kOPlfence,      /**< IA-32/AMD64 lfence opcode. */
    /* 284 */ kOPmfence,      /**< IA-32/AMD64 mfence opcode. */
    /* 285 */ kOPclflush,     /**< IA-32/AMD64 clflush opcode. */
    /* 286 */ kOPsfence,      /**< IA-32/AMD64 sfence opcode. */
    /* 287 */ kOPprefetchnta, /**< IA-32/AMD64 prefetchnta opcode. */
    /* 288 */ kOPprefetcht0,  /**< IA-32/AMD64 prefetcht0 opcode. */
    /* 289 */ kOPprefetcht1,  /**< IA-32/AMD64 prefetcht1 opcode. */
    /* 290 */ kOPprefetcht2,  /**< IA-32/AMD64 prefetcht2 opcode. */
    /* 291 */ kOPprefetch,    /**< IA-32/AMD64 prefetch opcode. */
    /* 292 */ kOPprefetchw,   /**< IA-32/AMD64 prefetchw opcode. */

    /* 293 */ kOPmovups,    /**< IA-32/AMD64 movups opcode. */
    /* 294 */ kOPmovss,     /**< IA-32/AMD64 movss opcode. */
    /* 295 */ kOPmovupd,    /**< IA-32/AMD64 movupd opcode. */
    /* 296 */ kOPmovsd,     /**< IA-32/AMD64 movsd opcode. */
    /* 297 */ kOPmovlps,    /**< IA-32/AMD64 movlps opcode. */
    /* 298 */ kOPmovlpd,    /**< IA-32/AMD64 movlpd opcode. */
    /* 299 */ kOPunpcklps,  /**< IA-32/AMD64 unpcklps opcode. */
    /* 300 */ kOPunpcklpd,  /**< IA-32/AMD64 unpcklpd opcode. */
    /* 301 */ kOPunpckhps,  /**< IA-32/AMD64 unpckhps opcode. */
    /* 302 */ kOPunpckhpd,  /**< IA-32/AMD64 unpckhpd opcode. */
    /* 303 */ kOPmovhps,    /**< IA-32/AMD64 movhps opcode. */
    /* 304 */ kOPmovhpd,    /**< IA-32/AMD64 movhpd opcode. */
    /* 305 */ kOPmovaps,    /**< IA-32/AMD64 movaps opcode. */
    /* 306 */ kOPmovapd,    /**< IA-32/AMD64 movapd opcode. */
    /* 307 */ kOPcvtpi2ps,  /**< IA-32/AMD64 cvtpi2ps opcode. */
    /* 308 */ kOPcvtsi2ss,  /**< IA-32/AMD64 cvtsi2ss opcode. */
    /* 309 */ kOPcvtpi2pd,  /**< IA-32/AMD64 cvtpi2pd opcode. */
    /* 310 */ kOPcvtsi2sd,  /**< IA-32/AMD64 cvtsi2sd opcode. */
    /* 311 */ kOPcvttps2pi, /**< IA-32/AMD64 cvttps2pi opcode. */
    /* 312 */ kOPcvttss2si, /**< IA-32/AMD64 cvttss2si opcode. */
    /* 313 */ kOPcvttpd2pi, /**< IA-32/AMD64 cvttpd2pi opcode. */
    /* 314 */ kOPcvttsd2si, /**< IA-32/AMD64 cvttsd2si opcode. */
    /* 315 */ kOPcvtps2pi,  /**< IA-32/AMD64 cvtps2pi opcode. */
    /* 316 */ kOPcvtss2si,  /**< IA-32/AMD64 cvtss2si opcode. */
    /* 317 */ kOPcvtpd2pi,  /**< IA-32/AMD64 cvtpd2pi opcode. */
    /* 318 */ kOPcvtsd2si,  /**< IA-32/AMD64 cvtsd2si opcode. */
    /* 319 */ kOPucomiss,   /**< IA-32/AMD64 ucomiss opcode. */
    /* 320 */ kOPucomisd,   /**< IA-32/AMD64 ucomisd opcode. */
    /* 321 */ kOPcomiss,    /**< IA-32/AMD64 comiss opcode. */
    /* 322 */ kOPcomisd,    /**< IA-32/AMD64 comisd opcode. */
    /* 323 */ kOPmovmskps,  /**< IA-32/AMD64 movmskps opcode. */
    /* 324 */ kOPmovmskpd,  /**< IA-32/AMD64 movmskpd opcode. */
    /* 325 */ kOPsqrtps,    /**< IA-32/AMD64 sqrtps opcode. */
    /* 326 */ kOPsqrtss,    /**< IA-32/AMD64 sqrtss opcode. */
    /* 327 */ kOPsqrtpd,    /**< IA-32/AMD64 sqrtpd opcode. */
    /* 328 */ kOPsqrtsd,    /**< IA-32/AMD64 sqrtsd opcode. */
    /* 329 */ kOPrsqrtps,   /**< IA-32/AMD64 rsqrtps opcode. */
    /* 330 */ kOPrsqrtss,   /**< IA-32/AMD64 rsqrtss opcode. */
    /* 331 */ kOPrcpps,     /**< IA-32/AMD64 rcpps opcode. */
    /* 332 */ kOPrcpss,     /**< IA-32/AMD64 rcpss opcode. */
    /* 333 */ kOPandps,     /**< IA-32/AMD64 andps opcode. */
    /* 334 */ kOPandpd,     /**< IA-32/AMD64 andpd opcode. */
    /* 335 */ kOPandnps,    /**< IA-32/AMD64 andnps opcode. */
    /* 336 */ kOPandnpd,    /**< IA-32/AMD64 andnpd opcode. */
    /* 337 */ kOPorps,      /**< IA-32/AMD64 orps opcode. */
    /* 338 */ kOPorpd,      /**< IA-32/AMD64 orpd opcode. */
    /* 339 */ kOPxorps,     /**< IA-32/AMD64 xorps opcode. */
    /* 340 */ kOPxorpd,     /**< IA-32/AMD64 xorpd opcode. */
    /* 341 */ kOPaddps,     /**< IA-32/AMD64 addps opcode. */
    /* 342 */ kOPaddss,     /**< IA-32/AMD64 addss opcode. */
    /* 343 */ kOPaddpd,     /**< IA-32/AMD64 addpd opcode. */
    /* 344 */ kOPaddsd,     /**< IA-32/AMD64 addsd opcode. */
    /* 345 */ kOPmulps,     /**< IA-32/AMD64 mulps opcode. */
    /* 346 */ kOPmulss,     /**< IA-32/AMD64 mulss opcode. */
    /* 347 */ kOPmulpd,     /**< IA-32/AMD64 mulpd opcode. */
    /* 348 */ kOPmulsd,     /**< IA-32/AMD64 mulsd opcode. */
    /* 349 */ kOPcvtps2pd,  /**< IA-32/AMD64 cvtps2pd opcode. */
    /* 350 */ kOPcvtss2sd,  /**< IA-32/AMD64 cvtss2sd opcode. */
    /* 351 */ kOPcvtpd2ps,  /**< IA-32/AMD64 cvtpd2ps opcode. */
    /* 352 */ kOPcvtsd2ss,  /**< IA-32/AMD64 cvtsd2ss opcode. */
    /* 353 */ kOPcvtdq2ps,  /**< IA-32/AMD64 cvtdq2ps opcode. */
    /* 354 */ kOPcvttps2dq, /**< IA-32/AMD64 cvttps2dq opcode. */
    /* 355 */ kOPcvtps2dq,  /**< IA-32/AMD64 cvtps2dq opcode. */
    /* 356 */ kOPsubps,     /**< IA-32/AMD64 subps opcode. */
    /* 357 */ kOPsubss,     /**< IA-32/AMD64 subss opcode. */
    /* 358 */ kOPsubpd,     /**< IA-32/AMD64 subpd opcode. */
    /* 359 */ kOPsubsd,     /**< IA-32/AMD64 subsd opcode. */
    /* 360 */ kOPminps,     /**< IA-32/AMD64 minps opcode. */
    /* 361 */ kOPminss,     /**< IA-32/AMD64 minss opcode. */
    /* 362 */ kOPminpd,     /**< IA-32/AMD64 minpd opcode. */
    /* 363 */ kOPminsd,     /**< IA-32/AMD64 minsd opcode. */
    /* 364 */ kOPdivps,     /**< IA-32/AMD64 divps opcode. */
    /* 365 */ kOPdivss,     /**< IA-32/AMD64 divss opcode. */
    /* 366 */ kOPdivpd,     /**< IA-32/AMD64 divpd opcode. */
    /* 367 */ kOPdivsd,     /**< IA-32/AMD64 divsd opcode. */
    /* 368 */ kOPmaxps,     /**< IA-32/AMD64 maxps opcode. */
    /* 369 */ kOPmaxss,     /**< IA-32/AMD64 maxss opcode. */
    /* 370 */ kOPmaxpd,     /**< IA-32/AMD64 maxpd opcode. */
    /* 371 */ kOPmaxsd,     /**< IA-32/AMD64 maxsd opcode. */
    /* 372 */ kOPcmpps,     /**< IA-32/AMD64 cmpps opcode. */
    /* 373 */ kOPcmpss,     /**< IA-32/AMD64 cmpss opcode. */
    /* 374 */ kOPcmppd,     /**< IA-32/AMD64 cmppd opcode. */
    /* 375 */ kOPcmpsd,     /**< IA-32/AMD64 cmpsd opcode. */
    /* 376 */ kOPshufps,    /**< IA-32/AMD64 shufps opcode. */
    /* 377 */ kOPshufpd,    /**< IA-32/AMD64 shufpd opcode. */
    /* 378 */ kOPcvtdq2pd,  /**< IA-32/AMD64 cvtdq2pd opcode. */
    /* 379 */ kOPcvttpd2dq, /**< IA-32/AMD64 cvttpd2dq opcode. */
    /* 380 */ kOPcvtpd2dq,  /**< IA-32/AMD64 cvtpd2dq opcode. */
    /* 381 */ kOPnop,       /**< IA-32/AMD64 nop opcode. */
    /* 382 */ kOPpause,     /**< IA-32/AMD64 pause opcode. */

    /* 383 */ kOPins,        /**< IA-32/AMD64 ins opcode. */
    /* 384 */ kOPrep_ins,    /**< IA-32/AMD64 rep_ins opcode. */
    /* 385 */ kOPouts,       /**< IA-32/AMD64 outs opcode. */
    /* 386 */ kOPrep_outs,   /**< IA-32/AMD64 rep_outs opcode. */
    /* 387 */ kOPmovs,       /**< IA-32/AMD64 movs opcode. */
    /* 388 */ kOPrep_movs,   /**< IA-32/AMD64 rep_movs opcode. */
    /* 389 */ kOPstos,       /**< IA-32/AMD64 stos opcode. */
    /* 390 */ kOPrep_stos,   /**< IA-32/AMD64 rep_stos opcode. */
    /* 391 */ kOPlods,       /**< IA-32/AMD64 lods opcode. */
    /* 392 */ kOPrep_lods,   /**< IA-32/AMD64 rep_lods opcode. */
    /* 393 */ kOPcmps,       /**< IA-32/AMD64 cmps opcode. */
    /* 394 */ kOPrep_cmps,   /**< IA-32/AMD64 rep_cmps opcode. */
    /* 395 */ kOPrepne_cmps, /**< IA-32/AMD64 repne_cmps opcode. */
    /* 396 */ kOPscas,       /**< IA-32/AMD64 scas opcode. */
    /* 397 */ kOPrep_scas,   /**< IA-32/AMD64 rep_scas opcode. */
    /* 398 */ kOPrepne_scas, /**< IA-32/AMD64 repne_scas opcode. */

    /* 399 */ kOPfadd,    /**< IA-32/AMD64 fadd opcode. */
    /* 400 */ kOPfmul,    /**< IA-32/AMD64 fmul opcode. */
    /* 401 */ kOPfcom,    /**< IA-32/AMD64 fcom opcode. */
    /* 402 */ kOPfcomp,   /**< IA-32/AMD64 fcomp opcode. */
    /* 403 */ kOPfsub,    /**< IA-32/AMD64 fsub opcode. */
    /* 404 */ kOPfsubr,   /**< IA-32/AMD64 fsubr opcode. */
    /* 405 */ kOPfdiv,    /**< IA-32/AMD64 fdiv opcode. */
    /* 406 */ kOPfdivr,   /**< IA-32/AMD64 fdivr opcode. */
    /* 407 */ kOPfld,     /**< IA-32/AMD64 fld opcode. */
    /* 408 */ kOPfst,     /**< IA-32/AMD64 fst opcode. */
    /* 409 */ kOPfstp,    /**< IA-32/AMD64 fstp opcode. */
    /* 410 */ kOPfldenv,  /**< IA-32/AMD64 fldenv opcode. */
    /* 411 */ kOPfldcw,   /**< IA-32/AMD64 fldcw opcode. */
    /* 412 */ kOPfnstenv, /**< IA-32/AMD64 fnstenv opcode. */
    /* 413 */ kOPfnstcw,  /**< IA-32/AMD64 fnstcw opcode. */
    /* 414 */ kOPfiadd,   /**< IA-32/AMD64 fiadd opcode. */
    /* 415 */ kOPfimul,   /**< IA-32/AMD64 fimul opcode. */
    /* 416 */ kOPficom,   /**< IA-32/AMD64 ficom opcode. */
    /* 417 */ kOPficomp,  /**< IA-32/AMD64 ficomp opcode. */
    /* 418 */ kOPfisub,   /**< IA-32/AMD64 fisub opcode. */
    /* 419 */ kOPfisubr,  /**< IA-32/AMD64 fisubr opcode. */
    /* 420 */ kOPfidiv,   /**< IA-32/AMD64 fidiv opcode. */
    /* 421 */ kOPfidivr,  /**< IA-32/AMD64 fidivr opcode. */
    /* 422 */ kOPfild,    /**< IA-32/AMD64 fild opcode. */
    /* 423 */ kOPfist,    /**< IA-32/AMD64 fist opcode. */
    /* 424 */ kOPfistp,   /**< IA-32/AMD64 fistp opcode. */
    /* 425 */ kOPfrstor,  /**< IA-32/AMD64 frstor opcode. */
    /* 426 */ kOPfnsave,  /**< IA-32/AMD64 fnsave opcode. */
    /* 427 */ kOPfnstsw,  /**< IA-32/AMD64 fnstsw opcode. */

    /* 428 */ kOPfbld,  /**< IA-32/AMD64 fbld opcode. */
    /* 429 */ kOPfbstp, /**< IA-32/AMD64 fbstp opcode. */

    /* 430 */ kOPfxch,     /**< IA-32/AMD64 fxch opcode. */
    /* 431 */ kOPfnop,     /**< IA-32/AMD64 fnop opcode. */
    /* 432 */ kOPfchs,     /**< IA-32/AMD64 fchs opcode. */
    /* 433 */ kOPfabs,     /**< IA-32/AMD64 fabs opcode. */
    /* 434 */ kOPftst,     /**< IA-32/AMD64 ftst opcode. */
    /* 435 */ kOPfxam,     /**< IA-32/AMD64 fxam opcode. */
    /* 436 */ kOPfld1,     /**< IA-32/AMD64 fld1 opcode. */
    /* 437 */ kOPfldl2t,   /**< IA-32/AMD64 fldl2t opcode. */
    /* 438 */ kOPfldl2e,   /**< IA-32/AMD64 fldl2e opcode. */
    /* 439 */ kOPfldpi,    /**< IA-32/AMD64 fldpi opcode. */
    /* 440 */ kOPfldlg2,   /**< IA-32/AMD64 fldlg2 opcode. */
    /* 441 */ kOPfldln2,   /**< IA-32/AMD64 fldln2 opcode. */
    /* 442 */ kOPfldz,     /**< IA-32/AMD64 fldz opcode. */
    /* 443 */ kOPf2xm1,    /**< IA-32/AMD64 f2xm1 opcode. */
    /* 444 */ kOPfyl2x,    /**< IA-32/AMD64 fyl2x opcode. */
    /* 445 */ kOPfptan,    /**< IA-32/AMD64 fptan opcode. */
    /* 446 */ kOPfpatan,   /**< IA-32/AMD64 fpatan opcode. */
    /* 447 */ kOPfxtract,  /**< IA-32/AMD64 fxtract opcode. */
    /* 448 */ kOPfprem1,   /**< IA-32/AMD64 fprem1 opcode. */
    /* 449 */ kOPfdecstp,  /**< IA-32/AMD64 fdecstp opcode. */
    /* 450 */ kOPfincstp,  /**< IA-32/AMD64 fincstp opcode. */
    /* 451 */ kOPfprem,    /**< IA-32/AMD64 fprem opcode. */
    /* 452 */ kOPfyl2xp1,  /**< IA-32/AMD64 fyl2xp1 opcode. */
    /* 453 */ kOPfsqrt,    /**< IA-32/AMD64 fsqrt opcode. */
    /* 454 */ kOPfsincos,  /**< IA-32/AMD64 fsincos opcode. */
    /* 455 */ kOPfrndint,  /**< IA-32/AMD64 frndint opcode. */
    /* 456 */ kOPfscale,   /**< IA-32/AMD64 fscale opcode. */
    /* 457 */ kOPfsin,     /**< IA-32/AMD64 fsin opcode. */
    /* 458 */ kOPfcos,     /**< IA-32/AMD64 fcos opcode. */
    /* 459 */ kOPfcmovb,   /**< IA-32/AMD64 fcmovb opcode. */
    /* 460 */ kOPfcmove,   /**< IA-32/AMD64 fcmove opcode. */
    /* 461 */ kOPfcmovbe,  /**< IA-32/AMD64 fcmovbe opcode. */
    /* 462 */ kOPfcmovu,   /**< IA-32/AMD64 fcmovu opcode. */
    /* 463 */ kOPfucompp,  /**< IA-32/AMD64 fucompp opcode. */
    /* 464 */ kOPfcmovnb,  /**< IA-32/AMD64 fcmovnb opcode. */
    /* 465 */ kOPfcmovne,  /**< IA-32/AMD64 fcmovne opcode. */
    /* 466 */ kOPfcmovnbe, /**< IA-32/AMD64 fcmovnbe opcode. */
    /* 467 */ kOPfcmovnu,  /**< IA-32/AMD64 fcmovnu opcode. */
    /* 468 */ kOPfnclex,   /**< IA-32/AMD64 fnclex opcode. */
    /* 469 */ kOPfninit,   /**< IA-32/AMD64 fninit opcode. */
    /* 470 */ kOPfucomi,   /**< IA-32/AMD64 fucomi opcode. */
    /* 471 */ kOPfcomi,    /**< IA-32/AMD64 fcomi opcode. */
    /* 472 */ kOPffree,    /**< IA-32/AMD64 ffree opcode. */
    /* 473 */ kOPfucom,    /**< IA-32/AMD64 fucom opcode. */
    /* 474 */ kOPfucomp,   /**< IA-32/AMD64 fucomp opcode. */
    /* 475 */ kOPfaddp,    /**< IA-32/AMD64 faddp opcode. */
    /* 476 */ kOPfmulp,    /**< IA-32/AMD64 fmulp opcode. */
    /* 477 */ kOPfcompp,   /**< IA-32/AMD64 fcompp opcode. */
    /* 478 */ kOPfsubrp,   /**< IA-32/AMD64 fsubrp opcode. */
    /* 479 */ kOPfsubp,    /**< IA-32/AMD64 fsubp opcode. */
    /* 480 */ kOPfdivrp,   /**< IA-32/AMD64 fdivrp opcode. */
    /* 481 */ kOPfdivp,    /**< IA-32/AMD64 fdivp opcode. */
    /* 482 */ kOPfucomip,  /**< IA-32/AMD64 fucomip opcode. */
    /* 483 */ kOPfcomip,   /**< IA-32/AMD64 fcomip opcode. */

    /* SSE3 instructions */
    /* 484 */ kOPfisttp,   /**< IA-32/AMD64 fisttp opcode. */
    /* 485 */ kOPhaddpd,   /**< IA-32/AMD64 haddpd opcode. */
    /* 486 */ kOPhaddps,   /**< IA-32/AMD64 haddps opcode. */
    /* 487 */ kOPhsubpd,   /**< IA-32/AMD64 hsubpd opcode. */
    /* 488 */ kOPhsubps,   /**< IA-32/AMD64 hsubps opcode. */
    /* 489 */ kOPaddsubpd, /**< IA-32/AMD64 addsubpd opcode. */
    /* 490 */ kOPaddsubps, /**< IA-32/AMD64 addsubps opcode. */
    /* 491 */ kOPlddqu,    /**< IA-32/AMD64 lddqu opcode. */
    /* 492 */ kOPmonitor,  /**< IA-32/AMD64 monitor opcode. */
    /* 493 */ kOPmwait,    /**< IA-32/AMD64 mwait opcode. */
    /* 494 */ kOPmovsldup, /**< IA-32/AMD64 movsldup opcode. */
    /* 495 */ kOPmovshdup, /**< IA-32/AMD64 movshdup opcode. */
    /* 496 */ kOPmovddup,  /**< IA-32/AMD64 movddup opcode. */

    /* 3D-Now! instructions */
    /* 497 */ kOPfemms,         /**< IA-32/AMD64 femms opcode. */
    /* 498 */ kOPunknown_3dnow, /**< IA-32/AMD64 unknown_3dnow opcode. */
    /* 499 */ kOPpavgusb,       /**< IA-32/AMD64 pavgusb opcode. */
    /* 500 */ kOPpfadd,         /**< IA-32/AMD64 pfadd opcode. */
    /* 501 */ kOPpfacc,         /**< IA-32/AMD64 pfacc opcode. */
    /* 502 */ kOPpfcmpge,       /**< IA-32/AMD64 pfcmpge opcode. */
    /* 503 */ kOPpfcmpgt,       /**< IA-32/AMD64 pfcmpgt opcode. */
    /* 504 */ kOPpfcmpeq,       /**< IA-32/AMD64 pfcmpeq opcode. */
    /* 505 */ kOPpfmin,         /**< IA-32/AMD64 pfmin opcode. */
    /* 506 */ kOPpfmax,         /**< IA-32/AMD64 pfmax opcode. */
    /* 507 */ kOPpfmul,         /**< IA-32/AMD64 pfmul opcode. */
    /* 508 */ kOPpfrcp,         /**< IA-32/AMD64 pfrcp opcode. */
    /* 509 */ kOPpfrcpit1,      /**< IA-32/AMD64 pfrcpit1 opcode. */
    /* 510 */ kOPpfrcpit2,      /**< IA-32/AMD64 pfrcpit2 opcode. */
    /* 511 */ kOPpfrsqrt,       /**< IA-32/AMD64 pfrsqrt opcode. */
    /* 512 */ kOPpfrsqit1,      /**< IA-32/AMD64 pfrsqit1 opcode. */
    /* 513 */ kOPpmulhrw,       /**< IA-32/AMD64 pmulhrw opcode. */
    /* 514 */ kOPpfsub,         /**< IA-32/AMD64 pfsub opcode. */
    /* 515 */ kOPpfsubr,        /**< IA-32/AMD64 pfsubr opcode. */
    /* 516 */ kOPpi2fd,         /**< IA-32/AMD64 pi2fd opcode. */
    /* 517 */ kOPpf2id,         /**< IA-32/AMD64 pf2id opcode. */
    /* 518 */ kOPpi2fw,         /**< IA-32/AMD64 pi2fw opcode. */
    /* 519 */ kOPpf2iw,         /**< IA-32/AMD64 pf2iw opcode. */
    /* 520 */ kOPpfnacc,        /**< IA-32/AMD64 pfnacc opcode. */
    /* 521 */ kOPpfpnacc,       /**< IA-32/AMD64 pfpnacc opcode. */
    /* 522 */ kOPpswapd,        /**< IA-32/AMD64 pswapd opcode. */

    /* SSSE3 */
    /* 523 */ kOPpshufb,    /**< IA-32/AMD64 pshufb opcode. */
    /* 524 */ kOPphaddw,    /**< IA-32/AMD64 phaddw opcode. */
    /* 525 */ kOPphaddd,    /**< IA-32/AMD64 phaddd opcode. */
    /* 526 */ kOPphaddsw,   /**< IA-32/AMD64 phaddsw opcode. */
    /* 527 */ kOPpmaddubsw, /**< IA-32/AMD64 pmaddubsw opcode. */
    /* 528 */ kOPphsubw,    /**< IA-32/AMD64 phsubw opcode. */
    /* 529 */ kOPphsubd,    /**< IA-32/AMD64 phsubd opcode. */
    /* 530 */ kOPphsubsw,   /**< IA-32/AMD64 phsubsw opcode. */
    /* 531 */ kOPpsignb,    /**< IA-32/AMD64 psignb opcode. */
    /* 532 */ kOPpsignw,    /**< IA-32/AMD64 psignw opcode. */
    /* 533 */ kOPpsignd,    /**< IA-32/AMD64 psignd opcode. */
    /* 534 */ kOPpmulhrsw,  /**< IA-32/AMD64 pmulhrsw opcode. */
    /* 535 */ kOPpabsb,     /**< IA-32/AMD64 pabsb opcode. */
    /* 536 */ kOPpabsw,     /**< IA-32/AMD64 pabsw opcode. */
    /* 537 */ kOPpabsd,     /**< IA-32/AMD64 pabsd opcode. */
    /* 538 */ kOPpalignr,   /**< IA-32/AMD64 palignr opcode. */

    /* SSE4 (incl AMD (SSE4A) and Intel-specific (SSE4.1, SSE4.2) extensions */
    /* 539 */ kOPpopcnt,     /**< IA-32/AMD64 popcnt opcode. */
    /* 540 */ kOPmovntss,    /**< IA-32/AMD64 movntss opcode. */
    /* 541 */ kOPmovntsd,    /**< IA-32/AMD64 movntsd opcode. */
    /* 542 */ kOPextrq,      /**< IA-32/AMD64 extrq opcode. */
    /* 543 */ kOPinsertq,    /**< IA-32/AMD64 insertq opcode. */
    /* 544 */ kOPlzcnt,      /**< IA-32/AMD64 lzcnt opcode. */
    /* 545 */ kOPpblendvb,   /**< IA-32/AMD64 pblendvb opcode. */
    /* 546 */ kOPblendvps,   /**< IA-32/AMD64 blendvps opcode. */
    /* 547 */ kOPblendvpd,   /**< IA-32/AMD64 blendvpd opcode. */
    /* 548 */ kOPptest,      /**< IA-32/AMD64 ptest opcode. */
    /* 549 */ kOPpmovsxbw,   /**< IA-32/AMD64 pmovsxbw opcode. */
    /* 550 */ kOPpmovsxbd,   /**< IA-32/AMD64 pmovsxbd opcode. */
    /* 551 */ kOPpmovsxbq,   /**< IA-32/AMD64 pmovsxbq opcode. */
    /* 552 */ kOPpmovsxwd,   /**< IA-32/AMD64 pmovsxwd opcode. */
    /* 553 */ kOPpmovsxwq,   /**< IA-32/AMD64 pmovsxwq opcode. */
    /* 554 */ kOPpmovsxdq,   /**< IA-32/AMD64 pmovsxdq opcode. */
    /* 555 */ kOPpmuldq,     /**< IA-32/AMD64 pmuldq opcode. */
    /* 556 */ kOPpcmpeqq,    /**< IA-32/AMD64 pcmpeqq opcode. */
    /* 557 */ kOPmovntdqa,   /**< IA-32/AMD64 movntdqa opcode. */
    /* 558 */ kOPpackusdw,   /**< IA-32/AMD64 packusdw opcode. */
    /* 559 */ kOPpmovzxbw,   /**< IA-32/AMD64 pmovzxbw opcode. */
    /* 560 */ kOPpmovzxbd,   /**< IA-32/AMD64 pmovzxbd opcode. */
    /* 561 */ kOPpmovzxbq,   /**< IA-32/AMD64 pmovzxbq opcode. */
    /* 562 */ kOPpmovzxwd,   /**< IA-32/AMD64 pmovzxwd opcode. */
    /* 563 */ kOPpmovzxwq,   /**< IA-32/AMD64 pmovzxwq opcode. */
    /* 564 */ kOPpmovzxdq,   /**< IA-32/AMD64 pmovzxdq opcode. */
    /* 565 */ kOPpcmpgtq,    /**< IA-32/AMD64 pcmpgtq opcode. */
    /* 566 */ kOPpminsb,     /**< IA-32/AMD64 pminsb opcode. */
    /* 567 */ kOPpminsd,     /**< IA-32/AMD64 pminsd opcode. */
    /* 568 */ kOPpminuw,     /**< IA-32/AMD64 pminuw opcode. */
    /* 569 */ kOPpminud,     /**< IA-32/AMD64 pminud opcode. */
    /* 570 */ kOPpmaxsb,     /**< IA-32/AMD64 pmaxsb opcode. */
    /* 571 */ kOPpmaxsd,     /**< IA-32/AMD64 pmaxsd opcode. */
    /* 572 */ kOPpmaxuw,     /**< IA-32/AMD64 pmaxuw opcode. */
    /* 573 */ kOPpmaxud,     /**< IA-32/AMD64 pmaxud opcode. */
    /* 574 */ kOPpmulld,     /**< IA-32/AMD64 pmulld opcode. */
    /* 575 */ kOPphminposuw, /**< IA-32/AMD64 phminposuw opcode. */
    /* 576 */ kOPcrc32,      /**< IA-32/AMD64 crc32 opcode. */
    /* 577 */ kOPpextrb,     /**< IA-32/AMD64 pextrb opcode. */
    /* 578 */ kOPpextrd,     /**< IA-32/AMD64 pextrd opcode. */
    /* 579 */ kOPextractps,  /**< IA-32/AMD64 extractps opcode. */
    /* 580 */ kOProundps,    /**< IA-32/AMD64 roundps opcode. */
    /* 581 */ kOProundpd,    /**< IA-32/AMD64 roundpd opcode. */
    /* 582 */ kOProundss,    /**< IA-32/AMD64 roundss opcode. */
    /* 583 */ kOProundsd,    /**< IA-32/AMD64 roundsd opcode. */
    /* 584 */ kOPblendps,    /**< IA-32/AMD64 blendps opcode. */
    /* 585 */ kOPblendpd,    /**< IA-32/AMD64 blendpd opcode. */
    /* 586 */ kOPpblendw,    /**< IA-32/AMD64 pblendw opcode. */
    /* 587 */ kOPpinsrb,     /**< IA-32/AMD64 pinsrb opcode. */
    /* 588 */ kOPinsertps,   /**< IA-32/AMD64 insertps opcode. */
    /* 589 */ kOPpinsrd,     /**< IA-32/AMD64 pinsrd opcode. */
    /* 590 */ kOPdpps,       /**< IA-32/AMD64 dpps opcode. */
    /* 591 */ kOPdppd,       /**< IA-32/AMD64 dppd opcode. */
    /* 592 */ kOPmpsadbw,    /**< IA-32/AMD64 mpsadbw opcode. */
    /* 593 */ kOPpcmpestrm,  /**< IA-32/AMD64 pcmpestrm opcode. */
    /* 594 */ kOPpcmpestri,  /**< IA-32/AMD64 pcmpestri opcode. */
    /* 595 */ kOPpcmpistrm,  /**< IA-32/AMD64 pcmpistrm opcode. */
    /* 596 */ kOPpcmpistri,  /**< IA-32/AMD64 pcmpistri opcode. */

    /* x64 */
    /* 597 */ kOPmovsxd, /**< IA-32/AMD64 movsxd opcode. */
    /* 598 */ kOPswapgs, /**< IA-32/AMD64 swapgs opcode. */

    /* VMX */
    /* 599 */ kOPvmcall,   /**< IA-32/AMD64 vmcall opcode. */
    /* 600 */ kOPvmlaunch, /**< IA-32/AMD64 vmlaunch opcode. */
    /* 601 */ kOPvmresume, /**< IA-32/AMD64 vmresume opcode. */
    /* 602 */ kOPvmxoff,   /**< IA-32/AMD64 vmxoff opcode. */
    /* 603 */ kOPvmptrst,  /**< IA-32/AMD64 vmptrst opcode. */
    /* 604 */ kOPvmptrld,  /**< IA-32/AMD64 vmptrld opcode. */
    /* 605 */ kOPvmxon,    /**< IA-32/AMD64 vmxon opcode. */
    /* 606 */ kOPvmclear,  /**< IA-32/AMD64 vmclear opcode. */
    /* 607 */ kOPvmread,   /**< IA-32/AMD64 vmread opcode. */
    /* 608 */ kOPvmwrite,  /**< IA-32/AMD64 vmwrite opcode. */

    /* undocumented */
    /* 609 */ kOPint1,   /**< IA-32/AMD64 int1 opcode. */
    /* 610 */ kOPsalc,   /**< IA-32/AMD64 salc opcode. */
    /* 611 */ kOPffreep, /**< IA-32/AMD64 ffreep opcode. */

    /* AMD SVM */
    /* 612 */ kOPvmrun,   /**< IA-32/AMD64 vmrun opcode. */
    /* 613 */ kOPvmmcall, /**< IA-32/AMD64 vmmcall opcode. */
    /* 614 */ kOPvmload,  /**< IA-32/AMD64 vmload opcode. */
    /* 615 */ kOPvmsave,  /**< IA-32/AMD64 vmsave opcode. */
    /* 616 */ kOPstgi,    /**< IA-32/AMD64 stgi opcode. */
    /* 617 */ kOPclgi,    /**< IA-32/AMD64 clgi opcode. */
    /* 618 */ kOPskinit,  /**< IA-32/AMD64 skinit opcode. */
    /* 619 */ kOPinvlpga, /**< IA-32/AMD64 invlpga opcode. */
                          /* AMD though not part of SVM */
    /* 620 */ kOPrdtscp,  /**< IA-32/AMD64 rdtscp opcode. */

    /* Intel VMX additions */
    /* 621 */ kOPinvept,  /**< IA-32/AMD64 invept opcode. */
    /* 622 */ kOPinvvpid, /**< IA-32/AMD64 invvpid opcode. */

    /* added in Intel Westmere */
    /* 623 */ kOPpclmulqdq,       /**< IA-32/AMD64 pclmulqdq opcode. */
    /* 624 */ kOPaesimc,          /**< IA-32/AMD64 aesimc opcode. */
    /* 625 */ kOPaesenc,          /**< IA-32/AMD64 aesenc opcode. */
    /* 626 */ kOPaesenclast,      /**< IA-32/AMD64 aesenclast opcode. */
    /* 627 */ kOPaesdec,          /**< IA-32/AMD64 aesdec opcode. */
    /* 628 */ kOPaesdeclast,      /**< IA-32/AMD64 aesdeclast opcode. */
    /* 629 */ kOPaeskeygenassist, /**< IA-32/AMD64 aeskeygenassist opcode. */

    /* added in Intel Atom */
    /* 630 */ kOPmovbe, /**< IA-32/AMD64 movbe opcode. */

    /* added in Intel Sandy Bridge */
    /* 631 */ kOPxgetbv,     /**< IA-32/AMD64 xgetbv opcode. */
    /* 632 */ kOPxsetbv,     /**< IA-32/AMD64 xsetbv opcode. */
    /* 633 */ kOPxsave32,    /**< IA-32/AMD64 xsave opcode. */
    /* 634 */ kOPxrstor32,   /**< IA-32/AMD64 xrstor opcode. */
    /* 635 */ kOPxsaveopt32, /**< IA-32/AMD64 xsaveopt opcode. */

    /* AVX */
    /* 636 */ kOPvmovss,           /**< IA-32/AMD64 vmovss opcode. */
    /* 637 */ kOPvmovsd,           /**< IA-32/AMD64 vmovsd opcode. */
    /* 638 */ kOPvmovups,          /**< IA-32/AMD64 vmovups opcode. */
    /* 639 */ kOPvmovupd,          /**< IA-32/AMD64 vmovupd opcode. */
    /* 640 */ kOPvmovlps,          /**< IA-32/AMD64 vmovlps opcode. */
    /* 641 */ kOPvmovsldup,        /**< IA-32/AMD64 vmovsldup opcode. */
    /* 642 */ kOPvmovlpd,          /**< IA-32/AMD64 vmovlpd opcode. */
    /* 643 */ kOPvmovddup,         /**< IA-32/AMD64 vmovddup opcode. */
    /* 644 */ kOPvunpcklps,        /**< IA-32/AMD64 vunpcklps opcode. */
    /* 645 */ kOPvunpcklpd,        /**< IA-32/AMD64 vunpcklpd opcode. */
    /* 646 */ kOPvunpckhps,        /**< IA-32/AMD64 vunpckhps opcode. */
    /* 647 */ kOPvunpckhpd,        /**< IA-32/AMD64 vunpckhpd opcode. */
    /* 648 */ kOPvmovhps,          /**< IA-32/AMD64 vmovhps opcode. */
    /* 649 */ kOPvmovshdup,        /**< IA-32/AMD64 vmovshdup opcode. */
    /* 650 */ kOPvmovhpd,          /**< IA-32/AMD64 vmovhpd opcode. */
    /* 651 */ kOPvmovaps,          /**< IA-32/AMD64 vmovaps opcode. */
    /* 652 */ kOPvmovapd,          /**< IA-32/AMD64 vmovapd opcode. */
    /* 653 */ kOPvcvtsi2ss,        /**< IA-32/AMD64 vcvtsi2ss opcode. */
    /* 654 */ kOPvcvtsi2sd,        /**< IA-32/AMD64 vcvtsi2sd opcode. */
    /* 655 */ kOPvmovntps,         /**< IA-32/AMD64 vmovntps opcode. */
    /* 656 */ kOPvmovntpd,         /**< IA-32/AMD64 vmovntpd opcode. */
    /* 657 */ kOPvcvttss2si,       /**< IA-32/AMD64 vcvttss2si opcode. */
    /* 658 */ kOPvcvttsd2si,       /**< IA-32/AMD64 vcvttsd2si opcode. */
    /* 659 */ kOPvcvtss2si,        /**< IA-32/AMD64 vcvtss2si opcode. */
    /* 660 */ kOPvcvtsd2si,        /**< IA-32/AMD64 vcvtsd2si opcode. */
    /* 661 */ kOPvucomiss,         /**< IA-32/AMD64 vucomiss opcode. */
    /* 662 */ kOPvucomisd,         /**< IA-32/AMD64 vucomisd opcode. */
    /* 663 */ kOPvcomiss,          /**< IA-32/AMD64 vcomiss opcode. */
    /* 664 */ kOPvcomisd,          /**< IA-32/AMD64 vcomisd opcode. */
    /* 665 */ kOPvmovmskps,        /**< IA-32/AMD64 vmovmskps opcode. */
    /* 666 */ kOPvmovmskpd,        /**< IA-32/AMD64 vmovmskpd opcode. */
    /* 667 */ kOPvsqrtps,          /**< IA-32/AMD64 vsqrtps opcode. */
    /* 668 */ kOPvsqrtss,          /**< IA-32/AMD64 vsqrtss opcode. */
    /* 669 */ kOPvsqrtpd,          /**< IA-32/AMD64 vsqrtpd opcode. */
    /* 670 */ kOPvsqrtsd,          /**< IA-32/AMD64 vsqrtsd opcode. */
    /* 671 */ kOPvrsqrtps,         /**< IA-32/AMD64 vrsqrtps opcode. */
    /* 672 */ kOPvrsqrtss,         /**< IA-32/AMD64 vrsqrtss opcode. */
    /* 673 */ kOPvrcpps,           /**< IA-32/AMD64 vrcpps opcode. */
    /* 674 */ kOPvrcpss,           /**< IA-32/AMD64 vrcpss opcode. */
    /* 675 */ kOPvandps,           /**< IA-32/AMD64 vandps opcode. */
    /* 676 */ kOPvandpd,           /**< IA-32/AMD64 vandpd opcode. */
    /* 677 */ kOPvandnps,          /**< IA-32/AMD64 vandnps opcode. */
    /* 678 */ kOPvandnpd,          /**< IA-32/AMD64 vandnpd opcode. */
    /* 679 */ kOPvorps,            /**< IA-32/AMD64 vorps opcode. */
    /* 680 */ kOPvorpd,            /**< IA-32/AMD64 vorpd opcode. */
    /* 681 */ kOPvxorps,           /**< IA-32/AMD64 vxorps opcode. */
    /* 682 */ kOPvxorpd,           /**< IA-32/AMD64 vxorpd opcode. */
    /* 683 */ kOPvaddps,           /**< IA-32/AMD64 vaddps opcode. */
    /* 684 */ kOPvaddss,           /**< IA-32/AMD64 vaddss opcode. */
    /* 685 */ kOPvaddpd,           /**< IA-32/AMD64 vaddpd opcode. */
    /* 686 */ kOPvaddsd,           /**< IA-32/AMD64 vaddsd opcode. */
    /* 687 */ kOPvmulps,           /**< IA-32/AMD64 vmulps opcode. */
    /* 688 */ kOPvmulss,           /**< IA-32/AMD64 vmulss opcode. */
    /* 689 */ kOPvmulpd,           /**< IA-32/AMD64 vmulpd opcode. */
    /* 690 */ kOPvmulsd,           /**< IA-32/AMD64 vmulsd opcode. */
    /* 691 */ kOPvcvtps2pd,        /**< IA-32/AMD64 vcvtps2pd opcode. */
    /* 692 */ kOPvcvtss2sd,        /**< IA-32/AMD64 vcvtss2sd opcode. */
    /* 693 */ kOPvcvtpd2ps,        /**< IA-32/AMD64 vcvtpd2ps opcode. */
    /* 694 */ kOPvcvtsd2ss,        /**< IA-32/AMD64 vcvtsd2ss opcode. */
    /* 695 */ kOPvcvtdq2ps,        /**< IA-32/AMD64 vcvtdq2ps opcode. */
    /* 696 */ kOPvcvttps2dq,       /**< IA-32/AMD64 vcvttps2dq opcode. */
    /* 697 */ kOPvcvtps2dq,        /**< IA-32/AMD64 vcvtps2dq opcode. */
    /* 698 */ kOPvsubps,           /**< IA-32/AMD64 vsubps opcode. */
    /* 699 */ kOPvsubss,           /**< IA-32/AMD64 vsubss opcode. */
    /* 700 */ kOPvsubpd,           /**< IA-32/AMD64 vsubpd opcode. */
    /* 701 */ kOPvsubsd,           /**< IA-32/AMD64 vsubsd opcode. */
    /* 702 */ kOPvminps,           /**< IA-32/AMD64 vminps opcode. */
    /* 703 */ kOPvminss,           /**< IA-32/AMD64 vminss opcode. */
    /* 704 */ kOPvminpd,           /**< IA-32/AMD64 vminpd opcode. */
    /* 705 */ kOPvminsd,           /**< IA-32/AMD64 vminsd opcode. */
    /* 706 */ kOPvdivps,           /**< IA-32/AMD64 vdivps opcode. */
    /* 707 */ kOPvdivss,           /**< IA-32/AMD64 vdivss opcode. */
    /* 708 */ kOPvdivpd,           /**< IA-32/AMD64 vdivpd opcode. */
    /* 709 */ kOPvdivsd,           /**< IA-32/AMD64 vdivsd opcode. */
    /* 710 */ kOPvmaxps,           /**< IA-32/AMD64 vmaxps opcode. */
    /* 711 */ kOPvmaxss,           /**< IA-32/AMD64 vmaxss opcode. */
    /* 712 */ kOPvmaxpd,           /**< IA-32/AMD64 vmaxpd opcode. */
    /* 713 */ kOPvmaxsd,           /**< IA-32/AMD64 vmaxsd opcode. */
    /* 714 */ kOPvpunpcklbw,       /**< IA-32/AMD64 vpunpcklbw opcode. */
    /* 715 */ kOPvpunpcklwd,       /**< IA-32/AMD64 vpunpcklwd opcode. */
    /* 716 */ kOPvpunpckldq,       /**< IA-32/AMD64 vpunpckldq opcode. */
    /* 717 */ kOPvpacksswb,        /**< IA-32/AMD64 vpacksswb opcode. */
    /* 718 */ kOPvpcmpgtb,         /**< IA-32/AMD64 vpcmpgtb opcode. */
    /* 719 */ kOPvpcmpgtw,         /**< IA-32/AMD64 vpcmpgtw opcode. */
    /* 720 */ kOPvpcmpgtd,         /**< IA-32/AMD64 vpcmpgtd opcode. */
    /* 721 */ kOPvpackuswb,        /**< IA-32/AMD64 vpackuswb opcode. */
    /* 722 */ kOPvpunpckhbw,       /**< IA-32/AMD64 vpunpckhbw opcode. */
    /* 723 */ kOPvpunpckhwd,       /**< IA-32/AMD64 vpunpckhwd opcode. */
    /* 724 */ kOPvpunpckhdq,       /**< IA-32/AMD64 vpunpckhdq opcode. */
    /* 725 */ kOPvpackssdw,        /**< IA-32/AMD64 vpackssdw opcode. */
    /* 726 */ kOPvpunpcklqdq,      /**< IA-32/AMD64 vpunpcklqdq opcode. */
    /* 727 */ kOPvpunpckhqdq,      /**< IA-32/AMD64 vpunpckhqdq opcode. */
    /* 728 */ kOPvmovd,            /**< IA-32/AMD64 vmovd opcode. */
    /* 729 */ kOPvpshufhw,         /**< IA-32/AMD64 vpshufhw opcode. */
    /* 730 */ kOPvpshufd,          /**< IA-32/AMD64 vpshufd opcode. */
    /* 731 */ kOPvpshuflw,         /**< IA-32/AMD64 vpshuflw opcode. */
    /* 732 */ kOPvpcmpeqb,         /**< IA-32/AMD64 vpcmpeqb opcode. */
    /* 733 */ kOPvpcmpeqw,         /**< IA-32/AMD64 vpcmpeqw opcode. */
    /* 734 */ kOPvpcmpeqd,         /**< IA-32/AMD64 vpcmpeqd opcode. */
    /* 735 */ kOPvmovq,            /**< IA-32/AMD64 vmovq opcode. */
    /* 736 */ kOPvcmpps,           /**< IA-32/AMD64 vcmpps opcode. */
    /* 737 */ kOPvcmpss,           /**< IA-32/AMD64 vcmpss opcode. */
    /* 738 */ kOPvcmppd,           /**< IA-32/AMD64 vcmppd opcode. */
    /* 739 */ kOPvcmpsd,           /**< IA-32/AMD64 vcmpsd opcode. */
    /* 740 */ kOPvpinsrw,          /**< IA-32/AMD64 vpinsrw opcode. */
    /* 741 */ kOPvpextrw,          /**< IA-32/AMD64 vpextrw opcode. */
    /* 742 */ kOPvshufps,          /**< IA-32/AMD64 vshufps opcode. */
    /* 743 */ kOPvshufpd,          /**< IA-32/AMD64 vshufpd opcode. */
    /* 744 */ kOPvpsrlw,           /**< IA-32/AMD64 vpsrlw opcode. */
    /* 745 */ kOPvpsrld,           /**< IA-32/AMD64 vpsrld opcode. */
    /* 746 */ kOPvpsrlq,           /**< IA-32/AMD64 vpsrlq opcode. */
    /* 747 */ kOPvpaddq,           /**< IA-32/AMD64 vpaddq opcode. */
    /* 748 */ kOPvpmullw,          /**< IA-32/AMD64 vpmullw opcode. */
    /* 749 */ kOPvpmovmskb,        /**< IA-32/AMD64 vpmovmskb opcode. */
    /* 750 */ kOPvpsubusb,         /**< IA-32/AMD64 vpsubusb opcode. */
    /* 751 */ kOPvpsubusw,         /**< IA-32/AMD64 vpsubusw opcode. */
    /* 752 */ kOPvpminub,          /**< IA-32/AMD64 vpminub opcode. */
    /* 753 */ kOPvpand,            /**< IA-32/AMD64 vpand opcode. */
    /* 754 */ kOPvpaddusb,         /**< IA-32/AMD64 vpaddusb opcode. */
    /* 755 */ kOPvpaddusw,         /**< IA-32/AMD64 vpaddusw opcode. */
    /* 756 */ kOPvpmaxub,          /**< IA-32/AMD64 vpmaxub opcode. */
    /* 757 */ kOPvpandn,           /**< IA-32/AMD64 vpandn opcode. */
    /* 758 */ kOPvpavgb,           /**< IA-32/AMD64 vpavgb opcode. */
    /* 759 */ kOPvpsraw,           /**< IA-32/AMD64 vpsraw opcode. */
    /* 760 */ kOPvpsrad,           /**< IA-32/AMD64 vpsrad opcode. */
    /* 761 */ kOPvpavgw,           /**< IA-32/AMD64 vpavgw opcode. */
    /* 762 */ kOPvpmulhuw,         /**< IA-32/AMD64 vpmulhuw opcode. */
    /* 763 */ kOPvpmulhw,          /**< IA-32/AMD64 vpmulhw opcode. */
    /* 764 */ kOPvcvtdq2pd,        /**< IA-32/AMD64 vcvtdq2pd opcode. */
    /* 765 */ kOPvcvttpd2dq,       /**< IA-32/AMD64 vcvttpd2dq opcode. */
    /* 766 */ kOPvcvtpd2dq,        /**< IA-32/AMD64 vcvtpd2dq opcode. */
    /* 767 */ kOPvmovntdq,         /**< IA-32/AMD64 vmovntdq opcode. */
    /* 768 */ kOPvpsubsb,          /**< IA-32/AMD64 vpsubsb opcode. */
    /* 769 */ kOPvpsubsw,          /**< IA-32/AMD64 vpsubsw opcode. */
    /* 770 */ kOPvpminsw,          /**< IA-32/AMD64 vpminsw opcode. */
    /* 771 */ kOPvpor,             /**< IA-32/AMD64 vpor opcode. */
    /* 772 */ kOPvpaddsb,          /**< IA-32/AMD64 vpaddsb opcode. */
    /* 773 */ kOPvpaddsw,          /**< IA-32/AMD64 vpaddsw opcode. */
    /* 774 */ kOPvpmaxsw,          /**< IA-32/AMD64 vpmaxsw opcode. */
    /* 775 */ kOPvpxor,            /**< IA-32/AMD64 vpxor opcode. */
    /* 776 */ kOPvpsllw,           /**< IA-32/AMD64 vpsllw opcode. */
    /* 777 */ kOPvpslld,           /**< IA-32/AMD64 vpslld opcode. */
    /* 778 */ kOPvpsllq,           /**< IA-32/AMD64 vpsllq opcode. */
    /* 779 */ kOPvpmuludq,         /**< IA-32/AMD64 vpmuludq opcode. */
    /* 780 */ kOPvpmaddwd,         /**< IA-32/AMD64 vpmaddwd opcode. */
    /* 781 */ kOPvpsadbw,          /**< IA-32/AMD64 vpsadbw opcode. */
    /* 782 */ kOPvmaskmovdqu,      /**< IA-32/AMD64 vmaskmovdqu opcode. */
    /* 783 */ kOPvpsubb,           /**< IA-32/AMD64 vpsubb opcode. */
    /* 784 */ kOPvpsubw,           /**< IA-32/AMD64 vpsubw opcode. */
    /* 785 */ kOPvpsubd,           /**< IA-32/AMD64 vpsubd opcode. */
    /* 786 */ kOPvpsubq,           /**< IA-32/AMD64 vpsubq opcode. */
    /* 787 */ kOPvpaddb,           /**< IA-32/AMD64 vpaddb opcode. */
    /* 788 */ kOPvpaddw,           /**< IA-32/AMD64 vpaddw opcode. */
    /* 789 */ kOPvpaddd,           /**< IA-32/AMD64 vpaddd opcode. */
    /* 790 */ kOPvpsrldq,          /**< IA-32/AMD64 vpsrldq opcode. */
    /* 791 */ kOPvpslldq,          /**< IA-32/AMD64 vpslldq opcode. */
    /* 792 */ kOPvmovdqu,          /**< IA-32/AMD64 vmovdqu opcode. */
    /* 793 */ kOPvmovdqa,          /**< IA-32/AMD64 vmovdqa opcode. */
    /* 794 */ kOPvhaddpd,          /**< IA-32/AMD64 vhaddpd opcode. */
    /* 795 */ kOPvhaddps,          /**< IA-32/AMD64 vhaddps opcode. */
    /* 796 */ kOPvhsubpd,          /**< IA-32/AMD64 vhsubpd opcode. */
    /* 797 */ kOPvhsubps,          /**< IA-32/AMD64 vhsubps opcode. */
    /* 798 */ kOPvaddsubpd,        /**< IA-32/AMD64 vaddsubpd opcode. */
    /* 799 */ kOPvaddsubps,        /**< IA-32/AMD64 vaddsubps opcode. */
    /* 800 */ kOPvlddqu,           /**< IA-32/AMD64 vlddqu opcode. */
    /* 801 */ kOPvpshufb,          /**< IA-32/AMD64 vpshufb opcode. */
    /* 802 */ kOPvphaddw,          /**< IA-32/AMD64 vphaddw opcode. */
    /* 803 */ kOPvphaddd,          /**< IA-32/AMD64 vphaddd opcode. */
    /* 804 */ kOPvphaddsw,         /**< IA-32/AMD64 vphaddsw opcode. */
    /* 805 */ kOPvpmaddubsw,       /**< IA-32/AMD64 vpmaddubsw opcode. */
    /* 806 */ kOPvphsubw,          /**< IA-32/AMD64 vphsubw opcode. */
    /* 807 */ kOPvphsubd,          /**< IA-32/AMD64 vphsubd opcode. */
    /* 808 */ kOPvphsubsw,         /**< IA-32/AMD64 vphsubsw opcode. */
    /* 809 */ kOPvpsignb,          /**< IA-32/AMD64 vpsignb opcode. */
    /* 810 */ kOPvpsignw,          /**< IA-32/AMD64 vpsignw opcode. */
    /* 811 */ kOPvpsignd,          /**< IA-32/AMD64 vpsignd opcode. */
    /* 812 */ kOPvpmulhrsw,        /**< IA-32/AMD64 vpmulhrsw opcode. */
    /* 813 */ kOPvpabsb,           /**< IA-32/AMD64 vpabsb opcode. */
    /* 814 */ kOPvpabsw,           /**< IA-32/AMD64 vpabsw opcode. */
    /* 815 */ kOPvpabsd,           /**< IA-32/AMD64 vpabsd opcode. */
    /* 816 */ kOPvpalignr,         /**< IA-32/AMD64 vpalignr opcode. */
    /* 817 */ kOPvpblendvb,        /**< IA-32/AMD64 vpblendvb opcode. */
    /* 818 */ kOPvblendvps,        /**< IA-32/AMD64 vblendvps opcode. */
    /* 819 */ kOPvblendvpd,        /**< IA-32/AMD64 vblendvpd opcode. */
    /* 820 */ kOPvptest,           /**< IA-32/AMD64 vptest opcode. */
    /* 821 */ kOPvpmovsxbw,        /**< IA-32/AMD64 vpmovsxbw opcode. */
    /* 822 */ kOPvpmovsxbd,        /**< IA-32/AMD64 vpmovsxbd opcode. */
    /* 823 */ kOPvpmovsxbq,        /**< IA-32/AMD64 vpmovsxbq opcode. */
    /* 824 */ kOPvpmovsxwd,        /**< IA-32/AMD64 vpmovsxwd opcode. */
    /* 825 */ kOPvpmovsxwq,        /**< IA-32/AMD64 vpmovsxwq opcode. */
    /* 826 */ kOPvpmovsxdq,        /**< IA-32/AMD64 vpmovsxdq opcode. */
    /* 827 */ kOPvpmuldq,          /**< IA-32/AMD64 vpmuldq opcode. */
    /* 828 */ kOPvpcmpeqq,         /**< IA-32/AMD64 vpcmpeqq opcode. */
    /* 829 */ kOPvmovntdqa,        /**< IA-32/AMD64 vmovntdqa opcode. */
    /* 830 */ kOPvpackusdw,        /**< IA-32/AMD64 vpackusdw opcode. */
    /* 831 */ kOPvpmovzxbw,        /**< IA-32/AMD64 vpmovzxbw opcode. */
    /* 832 */ kOPvpmovzxbd,        /**< IA-32/AMD64 vpmovzxbd opcode. */
    /* 833 */ kOPvpmovzxbq,        /**< IA-32/AMD64 vpmovzxbq opcode. */
    /* 834 */ kOPvpmovzxwd,        /**< IA-32/AMD64 vpmovzxwd opcode. */
    /* 835 */ kOPvpmovzxwq,        /**< IA-32/AMD64 vpmovzxwq opcode. */
    /* 836 */ kOPvpmovzxdq,        /**< IA-32/AMD64 vpmovzxdq opcode. */
    /* 837 */ kOPvpcmpgtq,         /**< IA-32/AMD64 vpcmpgtq opcode. */
    /* 838 */ kOPvpminsb,          /**< IA-32/AMD64 vpminsb opcode. */
    /* 839 */ kOPvpminsd,          /**< IA-32/AMD64 vpminsd opcode. */
    /* 840 */ kOPvpminuw,          /**< IA-32/AMD64 vpminuw opcode. */
    /* 841 */ kOPvpminud,          /**< IA-32/AMD64 vpminud opcode. */
    /* 842 */ kOPvpmaxsb,          /**< IA-32/AMD64 vpmaxsb opcode. */
    /* 843 */ kOPvpmaxsd,          /**< IA-32/AMD64 vpmaxsd opcode. */
    /* 844 */ kOPvpmaxuw,          /**< IA-32/AMD64 vpmaxuw opcode. */
    /* 845 */ kOPvpmaxud,          /**< IA-32/AMD64 vpmaxud opcode. */
    /* 846 */ kOPvpmulld,          /**< IA-32/AMD64 vpmulld opcode. */
    /* 847 */ kOPvphminposuw,      /**< IA-32/AMD64 vphminposuw opcode. */
    /* 848 */ kOPvaesimc,          /**< IA-32/AMD64 vaesimc opcode. */
    /* 849 */ kOPvaesenc,          /**< IA-32/AMD64 vaesenc opcode. */
    /* 850 */ kOPvaesenclast,      /**< IA-32/AMD64 vaesenclast opcode. */
    /* 851 */ kOPvaesdec,          /**< IA-32/AMD64 vaesdec opcode. */
    /* 852 */ kOPvaesdeclast,      /**< IA-32/AMD64 vaesdeclast opcode. */
    /* 853 */ kOPvpextrb,          /**< IA-32/AMD64 vpextrb opcode. */
    /* 854 */ kOPvpextrd,          /**< IA-32/AMD64 vpextrd opcode. */
    /* 855 */ kOPvextractps,       /**< IA-32/AMD64 vextractps opcode. */
    /* 856 */ kOPvroundps,         /**< IA-32/AMD64 vroundps opcode. */
    /* 857 */ kOPvroundpd,         /**< IA-32/AMD64 vroundpd opcode. */
    /* 858 */ kOPvroundss,         /**< IA-32/AMD64 vroundss opcode. */
    /* 859 */ kOPvroundsd,         /**< IA-32/AMD64 vroundsd opcode. */
    /* 860 */ kOPvblendps,         /**< IA-32/AMD64 vblendps opcode. */
    /* 861 */ kOPvblendpd,         /**< IA-32/AMD64 vblendpd opcode. */
    /* 862 */ kOPvpblendw,         /**< IA-32/AMD64 vpblendw opcode. */
    /* 863 */ kOPvpinsrb,          /**< IA-32/AMD64 vpinsrb opcode. */
    /* 864 */ kOPvinsertps,        /**< IA-32/AMD64 vinsertps opcode. */
    /* 865 */ kOPvpinsrd,          /**< IA-32/AMD64 vpinsrd opcode. */
    /* 866 */ kOPvdpps,            /**< IA-32/AMD64 vdpps opcode. */
    /* 867 */ kOPvdppd,            /**< IA-32/AMD64 vdppd opcode. */
    /* 868 */ kOPvmpsadbw,         /**< IA-32/AMD64 vmpsadbw opcode. */
    /* 869 */ kOPvpcmpestrm,       /**< IA-32/AMD64 vpcmpestrm opcode. */
    /* 870 */ kOPvpcmpestri,       /**< IA-32/AMD64 vpcmpestri opcode. */
    /* 871 */ kOPvpcmpistrm,       /**< IA-32/AMD64 vpcmpistrm opcode. */
    /* 872 */ kOPvpcmpistri,       /**< IA-32/AMD64 vpcmpistri opcode. */
    /* 873 */ kOPvpclmulqdq,       /**< IA-32/AMD64 vpclmulqdq opcode. */
    /* 874 */ kOPvaeskeygenassist, /**< IA-32/AMD64 vaeskeygenassist opcode. */
    /* 875 */ kOPvtestps,          /**< IA-32/AMD64 vtestps opcode. */
    /* 876 */ kOPvtestpd,          /**< IA-32/AMD64 vtestpd opcode. */
    /* 877 */ kOPvzeroupper,       /**< IA-32/AMD64 vzeroupper opcode. */
    /* 878 */ kOPvzeroall,         /**< IA-32/AMD64 vzeroall opcode. */
    /* 879 */ kOPvldmxcsr,         /**< IA-32/AMD64 vldmxcsr opcode. */
    /* 880 */ kOPvstmxcsr,         /**< IA-32/AMD64 vstmxcsr opcode. */
    /* 881 */ kOPvbroadcastss,     /**< IA-32/AMD64 vbroadcastss opcode. */
    /* 882 */ kOPvbroadcastsd,     /**< IA-32/AMD64 vbroadcastsd opcode. */
    /* 883 */ kOPvbroadcastf128,   /**< IA-32/AMD64 vbroadcastf128 opcode. */
    /* 884 */ kOPvmaskmovps,       /**< IA-32/AMD64 vmaskmovps opcode. */
    /* 885 */ kOPvmaskmovpd,       /**< IA-32/AMD64 vmaskmovpd opcode. */
    /* 886 */ kOPvpermilps,        /**< IA-32/AMD64 vpermilps opcode. */
    /* 887 */ kOPvpermilpd,        /**< IA-32/AMD64 vpermilpd opcode. */
    /* 888 */ kOPvperm2f128,       /**< IA-32/AMD64 vperm2f128 opcode. */
    /* 889 */ kOPvinsertf128,      /**< IA-32/AMD64 vinsertf128 opcode. */
    /* 890 */ kOPvextractf128,     /**< IA-32/AMD64 vextractf128 opcode. */

    /* added in Ivy Bridge I believe, and covered by F16C cpuid flag */
    /* 891 */ kOPvcvtph2ps, /**< IA-32/AMD64 vcvtph2ps opcode. */
    /* 892 */ kOPvcvtps2ph, /**< IA-32/AMD64 vcvtps2ph opcode. */

    /* FMA */
    /* 893 */ kOPvfmadd132ps,    /**< IA-32/AMD64 vfmadd132ps opcode. */
    /* 894 */ kOPvfmadd132pd,    /**< IA-32/AMD64 vfmadd132pd opcode. */
    /* 895 */ kOPvfmadd213ps,    /**< IA-32/AMD64 vfmadd213ps opcode. */
    /* 896 */ kOPvfmadd213pd,    /**< IA-32/AMD64 vfmadd213pd opcode. */
    /* 897 */ kOPvfmadd231ps,    /**< IA-32/AMD64 vfmadd231ps opcode. */
    /* 898 */ kOPvfmadd231pd,    /**< IA-32/AMD64 vfmadd231pd opcode. */
    /* 899 */ kOPvfmadd132ss,    /**< IA-32/AMD64 vfmadd132ss opcode. */
    /* 900 */ kOPvfmadd132sd,    /**< IA-32/AMD64 vfmadd132sd opcode. */
    /* 901 */ kOPvfmadd213ss,    /**< IA-32/AMD64 vfmadd213ss opcode. */
    /* 902 */ kOPvfmadd213sd,    /**< IA-32/AMD64 vfmadd213sd opcode. */
    /* 903 */ kOPvfmadd231ss,    /**< IA-32/AMD64 vfmadd231ss opcode. */
    /* 904 */ kOPvfmadd231sd,    /**< IA-32/AMD64 vfmadd231sd opcode. */
    /* 905 */ kOPvfmaddsub132ps, /**< IA-32/AMD64 vfmaddsub132ps opcode. */
    /* 906 */ kOPvfmaddsub132pd, /**< IA-32/AMD64 vfmaddsub132pd opcode. */
    /* 907 */ kOPvfmaddsub213ps, /**< IA-32/AMD64 vfmaddsub213ps opcode. */
    /* 908 */ kOPvfmaddsub213pd, /**< IA-32/AMD64 vfmaddsub213pd opcode. */
    /* 909 */ kOPvfmaddsub231ps, /**< IA-32/AMD64 vfmaddsub231ps opcode. */
    /* 910 */ kOPvfmaddsub231pd, /**< IA-32/AMD64 vfmaddsub231pd opcode. */
    /* 911 */ kOPvfmsubadd132ps, /**< IA-32/AMD64 vfmsubadd132ps opcode. */
    /* 912 */ kOPvfmsubadd132pd, /**< IA-32/AMD64 vfmsubadd132pd opcode. */
    /* 913 */ kOPvfmsubadd213ps, /**< IA-32/AMD64 vfmsubadd213ps opcode. */
    /* 914 */ kOPvfmsubadd213pd, /**< IA-32/AMD64 vfmsubadd213pd opcode. */
    /* 915 */ kOPvfmsubadd231ps, /**< IA-32/AMD64 vfmsubadd231ps opcode. */
    /* 916 */ kOPvfmsubadd231pd, /**< IA-32/AMD64 vfmsubadd231pd opcode. */
    /* 917 */ kOPvfmsub132ps,    /**< IA-32/AMD64 vfmsub132ps opcode. */
    /* 918 */ kOPvfmsub132pd,    /**< IA-32/AMD64 vfmsub132pd opcode. */
    /* 919 */ kOPvfmsub213ps,    /**< IA-32/AMD64 vfmsub213ps opcode. */
    /* 920 */ kOPvfmsub213pd,    /**< IA-32/AMD64 vfmsub213pd opcode. */
    /* 921 */ kOPvfmsub231ps,    /**< IA-32/AMD64 vfmsub231ps opcode. */
    /* 922 */ kOPvfmsub231pd,    /**< IA-32/AMD64 vfmsub231pd opcode. */
    /* 923 */ kOPvfmsub132ss,    /**< IA-32/AMD64 vfmsub132ss opcode. */
    /* 924 */ kOPvfmsub132sd,    /**< IA-32/AMD64 vfmsub132sd opcode. */
    /* 925 */ kOPvfmsub213ss,    /**< IA-32/AMD64 vfmsub213ss opcode. */
    /* 926 */ kOPvfmsub213sd,    /**< IA-32/AMD64 vfmsub213sd opcode. */
    /* 927 */ kOPvfmsub231ss,    /**< IA-32/AMD64 vfmsub231ss opcode. */
    /* 928 */ kOPvfmsub231sd,    /**< IA-32/AMD64 vfmsub231sd opcode. */
    /* 929 */ kOPvfnmadd132ps,   /**< IA-32/AMD64 vfnmadd132ps opcode. */
    /* 930 */ kOPvfnmadd132pd,   /**< IA-32/AMD64 vfnmadd132pd opcode. */
    /* 931 */ kOPvfnmadd213ps,   /**< IA-32/AMD64 vfnmadd213ps opcode. */
    /* 932 */ kOPvfnmadd213pd,   /**< IA-32/AMD64 vfnmadd213pd opcode. */
    /* 933 */ kOPvfnmadd231ps,   /**< IA-32/AMD64 vfnmadd231ps opcode. */
    /* 934 */ kOPvfnmadd231pd,   /**< IA-32/AMD64 vfnmadd231pd opcode. */
    /* 935 */ kOPvfnmadd132ss,   /**< IA-32/AMD64 vfnmadd132ss opcode. */
    /* 936 */ kOPvfnmadd132sd,   /**< IA-32/AMD64 vfnmadd132sd opcode. */
    /* 937 */ kOPvfnmadd213ss,   /**< IA-32/AMD64 vfnmadd213ss opcode. */
    /* 938 */ kOPvfnmadd213sd,   /**< IA-32/AMD64 vfnmadd213sd opcode. */
    /* 939 */ kOPvfnmadd231ss,   /**< IA-32/AMD64 vfnmadd231ss opcode. */
    /* 940 */ kOPvfnmadd231sd,   /**< IA-32/AMD64 vfnmadd231sd opcode. */
    /* 941 */ kOPvfnmsub132ps,   /**< IA-32/AMD64 vfnmsub132ps opcode. */
    /* 942 */ kOPvfnmsub132pd,   /**< IA-32/AMD64 vfnmsub132pd opcode. */
    /* 943 */ kOPvfnmsub213ps,   /**< IA-32/AMD64 vfnmsub213ps opcode. */
    /* 944 */ kOPvfnmsub213pd,   /**< IA-32/AMD64 vfnmsub213pd opcode. */
    /* 945 */ kOPvfnmsub231ps,   /**< IA-32/AMD64 vfnmsub231ps opcode. */
    /* 946 */ kOPvfnmsub231pd,   /**< IA-32/AMD64 vfnmsub231pd opcode. */
    /* 947 */ kOPvfnmsub132ss,   /**< IA-32/AMD64 vfnmsub132ss opcode. */
    /* 948 */ kOPvfnmsub132sd,   /**< IA-32/AMD64 vfnmsub132sd opcode. */
    /* 949 */ kOPvfnmsub213ss,   /**< IA-32/AMD64 vfnmsub213ss opcode. */
    /* 950 */ kOPvfnmsub213sd,   /**< IA-32/AMD64 vfnmsub213sd opcode. */
    /* 951 */ kOPvfnmsub231ss,   /**< IA-32/AMD64 vfnmsub231ss opcode. */
    /* 952 */ kOPvfnmsub231sd,   /**< IA-32/AMD64 vfnmsub231sd opcode. */

    /* 953 */ kOPmovq2dq, /**< IA-32/AMD64 movq2dq opcode. */
    /* 954 */ kOPmovdq2q, /**< IA-32/AMD64 movdq2q opcode. */

    /* 955 */ kOPfxsave64,   /**< IA-32/AMD64 fxsave64 opcode. */
    /* 956 */ kOPfxrstor64,  /**< IA-32/AMD64 fxrstor64 opcode. */
    /* 957 */ kOPxsave64,    /**< IA-32/AMD64 xsave64 opcode. */
    /* 958 */ kOPxrstor64,   /**< IA-32/AMD64 xrstor64 opcode. */
    /* 959 */ kOPxsaveopt64, /**< IA-32/AMD64 xsaveopt64 opcode. */

    /* added in Intel Ivy Bridge: RDRAND and FSGSBASE cpuid flags */
    /* 960 */ kOPrdrand,   /**< IA-32/AMD64 rdrand opcode. */
    /* 961 */ kOPrdfsbase, /**< IA-32/AMD64 rdfsbase opcode. */
    /* 962 */ kOPrdgsbase, /**< IA-32/AMD64 rdgsbase opcode. */
    /* 963 */ kOPwrfsbase, /**< IA-32/AMD64 wrfsbase opcode. */
    /* 964 */ kOPwrgsbase, /**< IA-32/AMD64 wrgsbase opcode. */

    /* coming in the future but adding now since enough details are known */
    /* 965 */ kOPrdseed, /**< IA-32/AMD64 rdseed opcode. */

    /* AMD FMA4 */
    /* 966 */ kOPvfmaddsubps, /**< IA-32/AMD64 vfmaddsubps opcode. */
    /* 967 */ kOPvfmaddsubpd, /**< IA-32/AMD64 vfmaddsubpd opcode. */
    /* 968 */ kOPvfmsubaddps, /**< IA-32/AMD64 vfmsubaddps opcode. */
    /* 969 */ kOPvfmsubaddpd, /**< IA-32/AMD64 vfmsubaddpd opcode. */
    /* 970 */ kOPvfmaddps,    /**< IA-32/AMD64 vfmaddps opcode. */
    /* 971 */ kOPvfmaddpd,    /**< IA-32/AMD64 vfmaddpd opcode. */
    /* 972 */ kOPvfmaddss,    /**< IA-32/AMD64 vfmaddss opcode. */
    /* 973 */ kOPvfmaddsd,    /**< IA-32/AMD64 vfmaddsd opcode. */
    /* 974 */ kOPvfmsubps,    /**< IA-32/AMD64 vfmsubps opcode. */
    /* 975 */ kOPvfmsubpd,    /**< IA-32/AMD64 vfmsubpd opcode. */
    /* 976 */ kOPvfmsubss,    /**< IA-32/AMD64 vfmsubss opcode. */
    /* 977 */ kOPvfmsubsd,    /**< IA-32/AMD64 vfmsubsd opcode. */
    /* 978 */ kOPvfnmaddps,   /**< IA-32/AMD64 vfnmaddps opcode. */
    /* 979 */ kOPvfnmaddpd,   /**< IA-32/AMD64 vfnmaddpd opcode. */
    /* 980 */ kOPvfnmaddss,   /**< IA-32/AMD64 vfnmaddss opcode. */
    /* 981 */ kOPvfnmaddsd,   /**< IA-32/AMD64 vfnmaddsd opcode. */
    /* 982 */ kOPvfnmsubps,   /**< IA-32/AMD64 vfnmsubps opcode. */
    /* 983 */ kOPvfnmsubpd,   /**< IA-32/AMD64 vfnmsubpd opcode. */
    /* 984 */ kOPvfnmsubss,   /**< IA-32/AMD64 vfnmsubss opcode. */
    /* 985 */ kOPvfnmsubsd,   /**< IA-32/AMD64 vfnmsubsd opcode. */

    /* AMD XOP */
    /* 986 */ kOPvfrczps,     /**< IA-32/AMD64 vfrczps opcode. */
    /* 987 */ kOPvfrczpd,     /**< IA-32/AMD64 vfrczpd opcode. */
    /* 988 */ kOPvfrczss,     /**< IA-32/AMD64 vfrczss opcode. */
    /* 989 */ kOPvfrczsd,     /**< IA-32/AMD64 vfrczsd opcode. */
    /* 990 */ kOPvpcmov,      /**< IA-32/AMD64 vpcmov opcode. */
    /* 991 */ kOPvpcomb,      /**< IA-32/AMD64 vpcomb opcode. */
    /* 992 */ kOPvpcomw,      /**< IA-32/AMD64 vpcomw opcode. */
    /* 993 */ kOPvpcomd,      /**< IA-32/AMD64 vpcomd opcode. */
    /* 994 */ kOPvpcomq,      /**< IA-32/AMD64 vpcomq opcode. */
    /* 995 */ kOPvpcomub,     /**< IA-32/AMD64 vpcomub opcode. */
    /* 996 */ kOPvpcomuw,     /**< IA-32/AMD64 vpcomuw opcode. */
    /* 997 */ kOPvpcomud,     /**< IA-32/AMD64 vpcomud opcode. */
    /* 998 */ kOPvpcomuq,     /**< IA-32/AMD64 vpcomuq opcode. */
    /* 999 */ kOPvpermil2pd,  /**< IA-32/AMD64 vpermil2pd opcode. */
    /* 1000 */ kOPvpermil2ps, /**< IA-32/AMD64 vpermil2ps opcode. */
    /* 1001 */ kOPvphaddbw,   /**< IA-32/AMD64 vphaddbw opcode. */
    /* 1002 */ kOPvphaddbd,   /**< IA-32/AMD64 vphaddbd opcode. */
    /* 1003 */ kOPvphaddbq,   /**< IA-32/AMD64 vphaddbq opcode. */
    /* 1004 */ kOPvphaddwd,   /**< IA-32/AMD64 vphaddwd opcode. */
    /* 1005 */ kOPvphaddwq,   /**< IA-32/AMD64 vphaddwq opcode. */
    /* 1006 */ kOPvphadddq,   /**< IA-32/AMD64 vphadddq opcode. */
    /* 1007 */ kOPvphaddubw,  /**< IA-32/AMD64 vphaddubw opcode. */
    /* 1008 */ kOPvphaddubd,  /**< IA-32/AMD64 vphaddubd opcode. */
    /* 1009 */ kOPvphaddubq,  /**< IA-32/AMD64 vphaddubq opcode. */
    /* 1010 */ kOPvphadduwd,  /**< IA-32/AMD64 vphadduwd opcode. */
    /* 1011 */ kOPvphadduwq,  /**< IA-32/AMD64 vphadduwq opcode. */
    /* 1012 */ kOPvphaddudq,  /**< IA-32/AMD64 vphaddudq opcode. */
    /* 1013 */ kOPvphsubbw,   /**< IA-32/AMD64 vphsubbw opcode. */
    /* 1014 */ kOPvphsubwd,   /**< IA-32/AMD64 vphsubwd opcode. */
    /* 1015 */ kOPvphsubdq,   /**< IA-32/AMD64 vphsubdq opcode. */
    /* 1016 */ kOPvpmacssww,  /**< IA-32/AMD64 vpmacssww opcode. */
    /* 1017 */ kOPvpmacsswd,  /**< IA-32/AMD64 vpmacsswd opcode. */
    /* 1018 */ kOPvpmacssdql, /**< IA-32/AMD64 vpmacssdql opcode. */
    /* 1019 */ kOPvpmacssdd,  /**< IA-32/AMD64 vpmacssdd opcode. */
    /* 1020 */ kOPvpmacssdqh, /**< IA-32/AMD64 vpmacssdqh opcode. */
    /* 1021 */ kOPvpmacsww,   /**< IA-32/AMD64 vpmacsww opcode. */
    /* 1022 */ kOPvpmacswd,   /**< IA-32/AMD64 vpmacswd opcode. */
    /* 1023 */ kOPvpmacsdql,  /**< IA-32/AMD64 vpmacsdql opcode. */
    /* 1024 */ kOPvpmacsdd,   /**< IA-32/AMD64 vpmacsdd opcode. */
    /* 1025 */ kOPvpmacsdqh,  /**< IA-32/AMD64 vpmacsdqh opcode. */
    /* 1026 */ kOPvpmadcsswd, /**< IA-32/AMD64 vpmadcsswd opcode. */
    /* 1027 */ kOPvpmadcswd,  /**< IA-32/AMD64 vpmadcswd opcode. */
    /* 1028 */ kOPvpperm,     /**< IA-32/AMD64 vpperm opcode. */
    /* 1029 */ kOPvprotb,     /**< IA-32/AMD64 vprotb opcode. */
    /* 1030 */ kOPvprotw,     /**< IA-32/AMD64 vprotw opcode. */
    /* 1031 */ kOPvprotd,     /**< IA-32/AMD64 vprotd opcode. */
    /* 1032 */ kOPvprotq,     /**< IA-32/AMD64 vprotq opcode. */
    /* 1033 */ kOPvpshlb,     /**< IA-32/AMD64 vpshlb opcode. */
    /* 1034 */ kOPvpshlw,     /**< IA-32/AMD64 vpshlw opcode. */
    /* 1035 */ kOPvpshld,     /**< IA-32/AMD64 vpshld opcode. */
    /* 1036 */ kOPvpshlq,     /**< IA-32/AMD64 vpshlq opcode. */
    /* 1037 */ kOPvpshab,     /**< IA-32/AMD64 vpshab opcode. */
    /* 1038 */ kOPvpshaw,     /**< IA-32/AMD64 vpshaw opcode. */
    /* 1039 */ kOPvpshad,     /**< IA-32/AMD64 vpshad opcode. */
    /* 1040 */ kOPvpshaq,     /**< IA-32/AMD64 vpshaq opcode. */

    /* AMD TBM */
    /* 1041 */ kOPbextr,   /**< IA-32/AMD64 bextr opcode. */
    /* 1042 */ kOPblcfill, /**< IA-32/AMD64 blcfill opcode. */
    /* 1043 */ kOPblci,    /**< IA-32/AMD64 blci opcode. */
    /* 1044 */ kOPblcic,   /**< IA-32/AMD64 blcic opcode. */
    /* 1045 */ kOPblcmsk,  /**< IA-32/AMD64 blcmsk opcode. */
    /* 1046 */ kOPblcs,    /**< IA-32/AMD64 blcs opcode. */
    /* 1047 */ kOPblsfill, /**< IA-32/AMD64 blsfill opcode. */
    /* 1048 */ kOPblsic,   /**< IA-32/AMD64 blsic opcode. */
    /* 1049 */ kOPt1mskc,  /**< IA-32/AMD64 t1mskc opcode. */
    /* 1050 */ kOPtzmsk,   /**< IA-32/AMD64 tzmsk opcode. */

    /* AMD LWP */
    /* 1051 */ kOPllwpcb, /**< IA-32/AMD64 llwpcb opcode. */
    /* 1052 */ kOPslwpcb, /**< IA-32/AMD64 slwpcb opcode. */
    /* 1053 */ kOPlwpins, /**< IA-32/AMD64 lwpins opcode. */
    /* 1054 */ kOPlwpval, /**< IA-32/AMD64 lwpval opcode. */

    /* Intel BMI1 */
    /* (includes non-immed form of kOPbextr) */
    /* 1055 */ kOPandn,   /**< IA-32/AMD64 andn opcode. */
    /* 1056 */ kOPblsr,   /**< IA-32/AMD64 blsr opcode. */
    /* 1057 */ kOPblsmsk, /**< IA-32/AMD64 blsmsk opcode. */
    /* 1058 */ kOPblsi,   /**< IA-32/AMD64 blsi opcode. */
    /* 1059 */ kOPtzcnt,  /**< IA-32/AMD64 tzcnt opcode. */

    /* Intel BMI2 */
    /* 1060 */ kOPbzhi, /**< IA-32/AMD64 bzhi opcode. */
    /* 1061 */ kOPpext, /**< IA-32/AMD64 pext opcode. */
    /* 1062 */ kOPpdep, /**< IA-32/AMD64 pdep opcode. */
    /* 1063 */ kOPsarx, /**< IA-32/AMD64 sarx opcode. */
    /* 1064 */ kOPshlx, /**< IA-32/AMD64 shlx opcode. */
    /* 1065 */ kOPshrx, /**< IA-32/AMD64 shrx opcode. */
    /* 1066 */ kOProrx, /**< IA-32/AMD64 rorx opcode. */
    /* 1067 */ kOPmulx, /**< IA-32/AMD64 mulx opcode. */

    /* Intel Safer Mode Extensions */
    /* 1068 */ kOPgetsec, /**< IA-32/AMD64 getsec opcode. */

    /* Misc Intel additions */
    /* 1069 */ kOPvmfunc,  /**< IA-32/AMD64 vmfunc opcode. */
    /* 1070 */ kOPinvpcid, /**< IA-32/AMD64 invpcid opcode. */

    /* Intel TSX */
    /* 1071 */ kOPxabort, /**< IA-32/AMD64 xabort opcode. */
    /* 1072 */ kOPxbegin, /**< IA-32/AMD64 xbegin opcode. */
    /* 1073 */ kOPxend,   /**< IA-32/AMD64 xend opcode. */
    /* 1074 */ kOPxtest,  /**< IA-32/AMD64 xtest opcode. */

    /* AVX2 */
    /* 1075 */ kOPvpgatherdd,     /**< IA-32/AMD64 vpgatherdd opcode. */
    /* 1076 */ kOPvpgatherdq,     /**< IA-32/AMD64 vpgatherdq opcode. */
    /* 1077 */ kOPvpgatherqd,     /**< IA-32/AMD64 vpgatherqd opcode. */
    /* 1078 */ kOPvpgatherqq,     /**< IA-32/AMD64 vpgatherqq opcode. */
    /* 1079 */ kOPvgatherdps,     /**< IA-32/AMD64 vgatherdps opcode. */
    /* 1080 */ kOPvgatherdpd,     /**< IA-32/AMD64 vgatherdpd opcode. */
    /* 1081 */ kOPvgatherqps,     /**< IA-32/AMD64 vgatherqps opcode. */
    /* 1082 */ kOPvgatherqpd,     /**< IA-32/AMD64 vgatherqpd opcode. */
    /* 1083 */ kOPvbroadcasti128, /**< IA-32/AMD64 vbroadcasti128 opcode. */
    /* 1084 */ kOPvinserti128,    /**< IA-32/AMD64 vinserti128 opcode. */
    /* 1085 */ kOPvextracti128,   /**< IA-32/AMD64 vextracti128 opcode. */
    /* 1086 */ kOPvpmaskmovd,     /**< IA-32/AMD64 vpmaskmovd opcode. */
    /* 1087 */ kOPvpmaskmovq,     /**< IA-32/AMD64 vpmaskmovq opcode. */
    /* 1088 */ kOPvperm2i128,     /**< IA-32/AMD64 vperm2i128 opcode. */
    /* 1089 */ kOPvpermd,         /**< IA-32/AMD64 vpermd opcode. */
    /* 1090 */ kOPvpermps,        /**< IA-32/AMD64 vpermps opcode. */
    /* 1091 */ kOPvpermq,         /**< IA-32/AMD64 vpermq opcode. */
    /* 1092 */ kOPvpermpd,        /**< IA-32/AMD64 vpermpd opcode. */
    /* 1093 */ kOPvpblendd,       /**< IA-32/AMD64 vpblendd opcode. */
    /* 1094 */ kOPvpsllvd,        /**< IA-32/AMD64 vpsllvd opcode. */
    /* 1095 */ kOPvpsllvq,        /**< IA-32/AMD64 vpsllvq opcode. */
    /* 1096 */ kOPvpsravd,        /**< IA-32/AMD64 vpsravd opcode. */
    /* 1097 */ kOPvpsrlvd,        /**< IA-32/AMD64 vpsrlvd opcode. */
    /* 1098 */ kOPvpsrlvq,        /**< IA-32/AMD64 vpsrlvq opcode. */
    /* 1099 */ kOPvpbroadcastb,   /**< IA-32/AMD64 vpbroadcastb opcode. */
    /* 1100 */ kOPvpbroadcastw,   /**< IA-32/AMD64 vpbroadcastw opcode. */
    /* 1101 */ kOPvpbroadcastd,   /**< IA-32/AMD64 vpbroadcastd opcode. */
    /* 1102 */ kOPvpbroadcastq,   /**< IA-32/AMD64 vpbroadcastq opcode. */

    /* added in Intel Skylake */
    /* 1103 */ kOPxsavec32, /**< IA-32/AMD64 xsavec opcode. */
    /* 1104 */ kOPxsavec64, /**< IA-32/AMD64 xsavec64 opcode. */

    /* Intel ADX */
    /* 1105 */ kOPadox, /**< IA-32/AMD64 adox opcode. */
    /* 1106 */ kOPadcx, /**< IA-32/AMD64 adox opcode. */

    /* Intel AVX-512 VEX */
    /* 1107 */ kOPkmovw,    /**< IA-32/AMD64 AVX-512 kmovw opcode. */
    /* 1108 */ kOPkmovb,    /**< IA-32/AMD64 AVX-512 kmovb opcode. */
    /* 1109 */ kOPkmovq,    /**< IA-32/AMD64 AVX-512 kmovq opcode. */
    /* 1110 */ kOPkmovd,    /**< IA-32/AMD64 AVX-512 kmovd opcode. */
    /* 1111 */ kOPkandw,    /**< IA-32/AMD64 AVX-512 kandw opcode. */
    /* 1112 */ kOPkandb,    /**< IA-32/AMD64 AVX-512 kandb opcode. */
    /* 1113 */ kOPkandq,    /**< IA-32/AMD64 AVX-512 kandq opcode. */
    /* 1114 */ kOPkandd,    /**< IA-32/AMD64 AVX-512 kandd opcode. */
    /* 1115 */ kOPkandnw,   /**< IA-32/AMD64 AVX-512 kandnw opcode. */
    /* 1116 */ kOPkandnb,   /**< IA-32/AMD64 AVX-512 kandnb opcode. */
    /* 1117 */ kOPkandnq,   /**< IA-32/AMD64 AVX-512 kandnq opcode. */
    /* 1118 */ kOPkandnd,   /**< IA-32/AMD64 AVX-512 kandnd opcode. */
    /* 1119 */ kOPkunpckbw, /**< IA-32/AMD64 AVX-512 kunpckbw opcode. */
    /* 1120 */ kOPkunpckwd, /**< IA-32/AMD64 AVX-512 kunpckwd opcode. */
    /* 1121 */ kOPkunpckdq, /**< IA-32/AMD64 AVX-512 kunpckdq opcode. */
    /* 1122 */ kOPknotw,    /**< IA-32/AMD64 AVX-512 knotw opcode. */
    /* 1123 */ kOPknotb,    /**< IA-32/AMD64 AVX-512 knotb opcode. */
    /* 1124 */ kOPknotq,    /**< IA-32/AMD64 AVX-512 knotq opcode. */
    /* 1125 */ kOPknotd,    /**< IA-32/AMD64 AVX-512 knotd opcode. */
    /* 1126 */ kOPkorw,     /**< IA-32/AMD64 AVX-512 korw opcode. */
    /* 1127 */ kOPkorb,     /**< IA-32/AMD64 AVX-512 korb opcode. */
    /* 1128 */ kOPkorq,     /**< IA-32/AMD64 AVX-512 korq opcode. */
    /* 1129 */ kOPkord,     /**< IA-32/AMD64 AVX-512 kord opcode. */
    /* 1130 */ kOPkxnorw,   /**< IA-32/AMD64 AVX-512 kxnorw opcode. */
    /* 1131 */ kOPkxnorb,   /**< IA-32/AMD64 AVX-512 kxnorb opcode. */
    /* 1132 */ kOPkxnorq,   /**< IA-32/AMD64 AVX-512 kxnorq opcode. */
    /* 1133 */ kOPkxnord,   /**< IA-32/AMD64 AVX-512 kxnord opcode. */
    /* 1134 */ kOPkxorw,    /**< IA-32/AMD64 AVX-512 kxorw opcode. */
    /* 1135 */ kOPkxorb,    /**< IA-32/AMD64 AVX-512 kxorb opcode. */
    /* 1136 */ kOPkxorq,    /**< IA-32/AMD64 AVX-512 kxorq opcode. */
    /* 1137 */ kOPkxord,    /**< IA-32/AMD64 AVX-512 kxord opcode. */
    /* 1138 */ kOPkaddw,    /**< IA-32/AMD64 AVX-512 kaddw opcode. */
    /* 1139 */ kOPkaddb,    /**< IA-32/AMD64 AVX-512 kaddb opcode. */
    /* 1140 */ kOPkaddq,    /**< IA-32/AMD64 AVX-512 kaddq opcode. */
    /* 1141 */ kOPkaddd,    /**< IA-32/AMD64 AVX-512 kaddd opcode. */
    /* 1142 */ kOPkortestw, /**< IA-32/AMD64 AVX-512 kortestw opcode. */
    /* 1143 */ kOPkortestb, /**< IA-32/AMD64 AVX-512 kortestb opcode. */
    /* 1144 */ kOPkortestq, /**< IA-32/AMD64 AVX-512 kortestq opcode. */
    /* 1145 */ kOPkortestd, /**< IA-32/AMD64 AVX-512 kortestd opcode. */
    /* 1146 */ kOPkshiftlw, /**< IA-32/AMD64 AVX-512 kshiftlw opcode. */
    /* 1147 */ kOPkshiftlb, /**< IA-32/AMD64 AVX-512 kshiftlb opcode. */
    /* 1148 */ kOPkshiftlq, /**< IA-32/AMD64 AVX-512 kshiftlq opcode. */
    /* 1149 */ kOPkshiftld, /**< IA-32/AMD64 AVX-512 kshiftld opcode. */
    /* 1150 */ kOPkshiftrw, /**< IA-32/AMD64 AVX-512 kshiftrw opcode. */
    /* 1151 */ kOPkshiftrb, /**< IA-32/AMD64 AVX-512 kshiftrb opcode. */
    /* 1152 */ kOPkshiftrq, /**< IA-32/AMD64 AVX-512 kshiftrq opcode. */
    /* 1153 */ kOPkshiftrd, /**< IA-32/AMD64 AVX-512 kshiftrd opcode. */
    /* 1154 */ kOPktestw,   /**< IA-32/AMD64 AVX-512 ktestd opcode. */
    /* 1155 */ kOPktestb,   /**< IA-32/AMD64 AVX-512 ktestd opcode. */
    /* 1156 */ kOPktestq,   /**< IA-32/AMD64 AVX-512 ktestd opcode. */
    /* 1157 */ kOPktestd,   /**< IA-32/AMD64 AVX-512 ktestd opcode. */

    /* Intel AVX-512 EVEX */
    /* XXX i#1312: The opcode enum numbers here are changing as long as
     * AVX-512 instructions are being added to DynamoRIO. Users are advised
     * not to rely on the numerical value until i#1312 has been completed.
     */
    /* 1158 */ kOPvalignd,         /**< IA-32/AMD64 AVX-512 kOPvalignd opcode. */
    /* 1159 */ kOPvalignq,         /**< IA-32/AMD64 AVX-512 kOPvalignq opcode. */
    /* 1160 */ kOPvblendmpd,       /**< IA-32/AMD64 AVX-512 kOPvblendmpd opcode. */
    /* 1161 */ kOPvblendmps,       /**< IA-32/AMD64 AVX-512 kOPvblendmps opcode. */
    /* 1162 */ kOPvbroadcastf32x2, /**< IA-32/AMD64 AVX-512 kOPvbroadcastf32x2 opcode. */
    /* 1163 */ kOPvbroadcastf32x4, /**< IA-32/AMD64 AVX-512 kOPvbroadcastf32x4 opcode. */
    /* 1164 */ kOPvbroadcastf32x8, /**< IA-32/AMD64 AVX-512 kOPvbroadcastf32x8 opcode. */
    /* 1165 */ kOPvbroadcastf64x2, /**< IA-32/AMD64 AVX-512 kOPvbroadcastf64x2 opcode. */
    /* 1166 */ kOPvbroadcastf64x4, /**< IA-32/AMD64 AVX-512 kOPvbroadcastf64x4 opcode. */
    /* 1167 */ kOPvbroadcasti32x2, /**< IA-32/AMD64 AVX-512 kOPvbroadcasti32x2 opcode. */
    /* 1168 */ kOPvbroadcasti32x4, /**< IA-32/AMD64 AVX-512 kOPvbroadcasti32x4 opcode. */
    /* 1169 */ kOPvbroadcasti32x8, /**< IA-32/AMD64 AVX-512 kOPvbroadcasti32x8 opcode. */
    /* 1170 */ kOPvbroadcasti64x2, /**< IA-32/AMD64 AVX-512 kOPvbroadcasti64x2 opcode. */
    /* 1171 */ kOPvbroadcasti64x4, /**< IA-32/AMD64 AVX-512 kOPvbroadcasti64x4 opcode. */
    /* 1172 */ kOPvcompresspd,     /**< IA-32/AMD64 AVX-512 kOPvcompresspd opcode. */
    /* 1173 */ kOPvcompressps,     /**< IA-32/AMD64 AVX-512 kOPvcompressps opcode. */
    /* 1174 */ kOPvcvtpd2qq,       /**< IA-32/AMD64 AVX-512 kOPvcvtpd2qq opcode. */
    /* 1175 */ kOPvcvtpd2udq,      /**< IA-32/AMD64 AVX-512 kOPvcvtpd2udq opcode. */
    /* 1176 */ kOPvcvtpd2uqq,      /**< IA-32/AMD64 AVX-512 kOPvcvtpd2uqq opcode. */
    /* 1177 */ kOPvcvtps2qq,       /**< IA-32/AMD64 AVX-512 kOPvcvtps2qq opcode. */
    /* 1178 */ kOPvcvtps2udq,      /**< IA-32/AMD64 AVX-512 kOPvcvtps2udq opcode. */
    /* 1179 */ kOPvcvtps2uqq,      /**< IA-32/AMD64 AVX-512 kOPvcvtps2uqq opcode. */
    /* 1180 */ kOPvcvtqq2pd,       /**< IA-32/AMD64 AVX-512 kOPvcvtqq2pd opcode. */
    /* 1181 */ kOPvcvtqq2ps,       /**< IA-32/AMD64 AVX-512 kOPvcvtqq2ps opcode. */
    /* 1182 */ kOPvcvtsd2usi,      /**< IA-32/AMD64 AVX-512 kOPvcvtsd2usi opcode. */
    /* 1183 */ kOPvcvtss2usi,      /**< IA-32/AMD64 AVX-512 kOPvcvtss2usi opcode. */
    /* 1184 */ kOPvcvttpd2qq,      /**< IA-32/AMD64 AVX-512 kOPvcvttpd2qq opcode. */
    /* 1185 */ kOPvcvttpd2udq,     /**< IA-32/AMD64 AVX-512 kOPvcvttpd2udq opcode. */
    /* 1186 */ kOPvcvttpd2uqq,     /**< IA-32/AMD64 AVX-512 kOPvcvttpd2uqq opcode. */
    /* 1187 */ kOPvcvttps2qq,      /**< IA-32/AMD64 AVX-512 kOPvcvttps2qq opcode. */
    /* 1188 */ kOPvcvttps2udq,     /**< IA-32/AMD64 AVX-512 kOPvcvttps2udq opcode. */
    /* 1189 */ kOPvcvttps2uqq,     /**< IA-32/AMD64 AVX-512 kOPvcvttps2uqq opcode. */
    /* 1190 */ kOPvcvttsd2usi,     /**< IA-32/AMD64 AVX-512 kOPvcvttsd2usi opcode. */
    /* 1191 */ kOPvcvttss2usi,     /**< IA-32/AMD64 AVX-512 kOPvcvttss2usi opcode. */
    /* 1192 */ kOPvcvtudq2pd,      /**< IA-32/AMD64 AVX-512 kOPvcvtudq2pd opcode. */
    /* 1193 */ kOPvcvtudq2ps,      /**< IA-32/AMD64 AVX-512 kOPvcvtudq2ps opcode. */
    /* 1194 */ kOPvcvtuqq2pd,      /**< IA-32/AMD64 AVX-512 kOPvcvtuqq2pd opcode. */
    /* 1195 */ kOPvcvtuqq2ps,      /**< IA-32/AMD64 AVX-512 kOPvcvtuqq2ps opcode. */
    /* 1196 */ kOPvcvtusi2sd,      /**< IA-32/AMD64 AVX-512 kOPvcvtusi2sd opcode. */
    /* 1197 */ kOPvcvtusi2ss,      /**< IA-32/AMD64 AVX-512 kOPvcvtusi2ss opcode. */
    /* 1198 */ kOPvdbpsadbw,       /**< IA-32/AMD64 AVX-512 kOPvdbpsadbw opcode. */
    /* 1199 */ kOPvexp2pd,         /**< IA-32/AMD64 AVX-512 kOPvexp2pd opcode. */
    /* 1200 */ kOPvexp2ps,         /**< IA-32/AMD64 AVX-512 kOPvexp2ps opcode. */
    /* 1201 */ kOPvexpandpd,       /**< IA-32/AMD64 AVX-512 kOPvexpandpd opcode. */
    /* 1202 */ kOPvexpandps,       /**< IA-32/AMD64 AVX-512 kOPvexpandps opcode. */
    /* 1203 */ kOPvextractf32x4,   /**< IA-32/AMD64 AVX-512 kOPvextractf32x4 opcode. */
    /* 1204 */ kOPvextractf32x8,   /**< IA-32/AMD64 AVX-512 kOPvextractf32x8 opcode. */
    /* 1205 */ kOPvextractf64x2,   /**< IA-32/AMD64 AVX-512 kOPvextractf64x2 opcode. */
    /* 1206 */ kOPvextractf64x4,   /**< IA-32/AMD64 AVX-512 kOPvextractf64x4 opcode. */
    /* 1207 */ kOPvextracti32x4,   /**< IA-32/AMD64 AVX-512 kOPvextracti32x4 opcode. */
    /* 1208 */ kOPvextracti32x8,   /**< IA-32/AMD64 AVX-512 kOPvextracti32x8 opcode. */
    /* 1209 */ kOPvextracti64x2,   /**< IA-32/AMD64 AVX-512 kOPvextracti64x2 opcode. */
    /* 1210 */ kOPvextracti64x4,   /**< IA-32/AMD64 AVX-512 kOPvextracti64x4 opcode. */
    /* 1211 */ kOPvfixupimmpd,     /**< IA-32/AMD64 AVX-512 kOPvfixupimmpd opcode. */
    /* 1212 */ kOPvfixupimmps,     /**< IA-32/AMD64 AVX-512 kOPvfixupimmps opcode. */
    /* 1213 */ kOPvfixupimmsd,     /**< IA-32/AMD64 AVX-512 kOPvfixupimmsd opcode. */
    /* 1214 */ kOPvfixupimmss,     /**< IA-32/AMD64 AVX-512 kOPvfixupimmss opcode. */
    /* 1215 */ kOPvfpclasspd,      /**< IA-32/AMD64 AVX-512 kOPvfpclasspd opcode. */
    /* 1216 */ kOPvfpclassps,      /**< IA-32/AMD64 AVX-512 kOPvfpclassps opcode. */
    /* 1217 */ kOPvfpclasssd,      /**< IA-32/AMD64 AVX-512 kOPvfpclasssd opcode. */
    /* 1218 */ kOPvfpclassss,      /**< IA-32/AMD64 AVX-512 kOPvfpclassss opcode. */
    /* 1219 */ kOPvgatherpf0dpd,   /**< IA-32/AMD64 AVX-512 kOPvgatherpf0dps opcode. */
    /* 1220 */ kOPvgatherpf0dps,   /**< IA-32/AMD64 AVX-512 kOPvgatherpf0dps opcode. */
    /* 1221 */ kOPvgatherpf0qpd,   /**< IA-32/AMD64 AVX-512 kOPvgatherpf0qpd opcode. */
    /* 1222 */ kOPvgatherpf0qps,   /**< IA-32/AMD64 AVX-512 kOPvgatherpf0qps opcode. */
    /* 1223 */ kOPvgatherpf1dpd,   /**< IA-32/AMD64 AVX-512 kOPvgatherpf1dpd opcode. */
    /* 1224 */ kOPvgatherpf1dps,   /**< IA-32/AMD64 AVX-512 kOPvgatherpf1dps opcode. */
    /* 1225 */ kOPvgatherpf1qpd,   /**< IA-32/AMD64 AVX-512 kOPvgatherpf1qpd opcode. */
    /* 1226 */ kOPvgatherpf1qps,   /**< IA-32/AMD64 AVX-512 kOPvgatherpf1qps opcode. */
    /* 1227 */ kOPvgetexppd,       /**< IA-32/AMD64 AVX-512 kOPvgetexppd opcode. */
    /* 1228 */ kOPvgetexpps,       /**< IA-32/AMD64 AVX-512 kOPvgetexpps opcode. */
    /* 1229 */ kOPvgetexpsd,       /**< IA-32/AMD64 AVX-512 kOPvgetexpsd opcode. */
    /* 1230 */ kOPvgetexpss,       /**< IA-32/AMD64 AVX-512 kOPvgetexpss opcode. */
    /* 1231 */ kOPvgetmantpd,      /**< IA-32/AMD64 AVX-512 kOPvgetmantpd opcode. */
    /* 1232 */ kOPvgetmantps,      /**< IA-32/AMD64 AVX-512 kOPvgetmantps opcode. */
    /* 1233 */ kOPvgetmantsd,      /**< IA-32/AMD64 AVX-512 kOPvgetmantsd opcode. */
    /* 1234 */ kOPvgetmantss,      /**< IA-32/AMD64 AVX-512 kOPvgetmantss opcode. */
    /* 1235 */ kOPvinsertf32x4,    /**< IA-32/AMD64 AVX-512 kOPvinsertf32x4 opcode. */
    /* 1236 */ kOPvinsertf32x8,    /**< IA-32/AMD64 AVX-512 kOPvinsertf32x8 opcode. */
    /* 1237 */ kOPvinsertf64x2,    /**< IA-32/AMD64 AVX-512 kOPvinsertf64x2 opcode. */
    /* 1238 */ kOPvinsertf64x4,    /**< IA-32/AMD64 AVX-512 kOPvinsertf64x4 opcode. */
    /* 1239 */ kOPvinserti32x4,    /**< IA-32/AMD64 AVX-512 kOPvinserti32x4 opcode. */
    /* 1240 */ kOPvinserti32x8,    /**< IA-32/AMD64 AVX-512 kOPvinserti32x8 opcode. */
    /* 1241 */ kOPvinserti64x2,    /**< IA-32/AMD64 AVX-512 kOPvinserti64x2 opcode. */
    /* 1242 */ kOPvinserti64x4,    /**< IA-32/AMD64 AVX-512 kOPvinserti64x4 opcode. */
    /* 1243 */ kOPvmovdqa32,       /**< IA-32/AMD64 AVX-512 kOPvmovdqa32 opcode. */
    /* 1244 */ kOPvmovdqa64,       /**< IA-32/AMD64 AVX-512 kOPvmovdqa64 opcode. */
    /* 1245 */ kOPvmovdqu16,       /**< IA-32/AMD64 AVX-512 kOPvmovdqu16 opcode. */
    /* 1246 */ kOPvmovdqu32,       /**< IA-32/AMD64 AVX-512 kOPvmovdqu32 opcode. */
    /* 1247 */ kOPvmovdqu64,       /**< IA-32/AMD64 AVX-512 kOPvmovdqu64 opcode. */
    /* 1248 */ kOPvmovdqu8,        /**< IA-32/AMD64 AVX-512 kOPvmovdqu8 opcode. */
    /* 1249 */ kOPvpabsq,          /**< IA-32/AMD64 AVX-512 kOPvpabsq opcode. */
    /* 1250 */ kOPvpandd,          /**< IA-32/AMD64 AVX-512 kOPvpandd opcode. */
    /* 1251 */ kOPvpandnd,         /**< IA-32/AMD64 AVX-512 kOPvpandnd opcode. */
    /* 1252 */ kOPvpandnq,         /**< IA-32/AMD64 AVX-512 kOPvpandnq opcode. */
    /* 1253 */ kOPvpandq,          /**< IA-32/AMD64 AVX-512 kOPvpandq opcode. */
    /* 1254 */ kOPvpblendmb,       /**< IA-32/AMD64 AVX-512 kOPvpblendmb opcode. */
    /* 1255 */ kOPvpblendmd,       /**< IA-32/AMD64 AVX-512 kOPvpblendmd opcode. */
    /* 1256 */ kOPvpblendmq,       /**< IA-32/AMD64 AVX-512 kOPvpblendmq opcode. */
    /* 1257 */ kOPvpblendmw,       /**< IA-32/AMD64 AVX-512 kOPvpblendmw opcode. */
    /* 1258 */ kOPvpbroadcastmb2q, /**< IA-32/AMD64 AVX-512 kOPvpbroadcastmb2q opcode. */
    /* 1259 */ kOPvpbroadcastmw2d, /**< IA-32/AMD64 AVX-512 kOPvpbroadcastmw2d opcode. */
    /* 1260 */ kOPvpcmpb,          /**< IA-32/AMD64 AVX-512 kOPvpcmpb opcode. */
    /* 1261 */ kOPvpcmpd,          /**< IA-32/AMD64 AVX-512 kOPvpcmpd opcode. */
    /* 1262 */ kOPvpcmpq,          /**< IA-32/AMD64 AVX-512 kOPvpcmpq opcode. */
    /* 1263 */ kOPvpcmpub,         /**< IA-32/AMD64 AVX-512 kOPvpcmpub opcode. */
    /* 1264 */ kOPvpcmpud,         /**< IA-32/AMD64 AVX-512 kOPvpcmpud opcode. */
    /* 1265 */ kOPvpcmpuq,         /**< IA-32/AMD64 AVX-512 kOPvpcmpuq opcode. */
    /* 1266 */ kOPvpcmpuw,         /**< IA-32/AMD64 AVX-512 kOPvpcmpuw opcode. */
    /* 1267 */ kOPvpcmpw,          /**< IA-32/AMD64 AVX-512 kOPvpcmpw opcode. */
    /* 1268 */ kOPvpcompressd,     /**< IA-32/AMD64 AVX-512 kOPvpcompressd opcode. */
    /* 1269 */ kOPvpcompressq,     /**< IA-32/AMD64 AVX-512 kOPvpcompressq opcode. */
    /* 1270 */ kOPvpconflictd,     /**< IA-32/AMD64 AVX-512 kOPvpconflictd opcode. */
    /* 1271 */ kOPvpconflictq,     /**< IA-32/AMD64 AVX-512 kOPvpconflictq opcode. */
    /* 1272 */ kOPvpermb,          /**< IA-32/AMD64 AVX-512 kOPvpermb opcode. */
    /* 1273 */ kOPvpermi2b,        /**< IA-32/AMD64 AVX-512 kOPvpermi2b opcode. */
    /* 1274 */ kOPvpermi2d,        /**< IA-32/AMD64 AVX-512 kOPvpermi2d opcode. */
    /* 1275 */ kOPvpermi2pd,       /**< IA-32/AMD64 AVX-512 kOPvpermi2pd opcode. */
    /* 1276 */ kOPvpermi2ps,       /**< IA-32/AMD64 AVX-512 kOPvpermi2ps opcode. */
    /* 1277 */ kOPvpermi2q,        /**< IA-32/AMD64 AVX-512 kOPvpermi2q opcode. */
    /* 1278 */ kOPvpermi2w,        /**< IA-32/AMD64 AVX-512 kOPvpermi2w opcode. */
    /* 1279 */ kOPvpermt2b,        /**< IA-32/AMD64 AVX-512 kOPvpermt2b opcode. */
    /* 1280 */ kOPvpermt2d,        /**< IA-32/AMD64 AVX-512 kOPvpermt2d opcode. */
    /* 1281 */ kOPvpermt2pd,       /**< IA-32/AMD64 AVX-512 kOPvpermt2pd opcode. */
    /* 1282 */ kOPvpermt2ps,       /**< IA-32/AMD64 AVX-512 kOPvpermt2ps opcode. */
    /* 1283 */ kOPvpermt2q,        /**< IA-32/AMD64 AVX-512 kOPvpermt2q opcode. */
    /* 1284 */ kOPvpermt2w,        /**< IA-32/AMD64 AVX-512 kOPvpermt2w opcode. */
    /* 1285 */ kOPvpermw,          /**< IA-32/AMD64 AVX-512 kOPvpermw opcode. */
    /* 1286 */ kOPvpexpandd,       /**< IA-32/AMD64 AVX-512 kOPvpexpandd opcode. */
    /* 1287 */ kOPvpexpandq,       /**< IA-32/AMD64 AVX-512 kOPvpexpandq opcode. */
    /* 1288 */ kOPvpextrq,         /**< IA-32/AMD64 AVX-512 kOPvpextrq opcode. */
    /* 1289 */ kOPvpinsrq,         /**< IA-32/AMD64 AVX-512 kOPvpinsrq opcode. */
    /* 1290 */ kOPvplzcntd,        /**< IA-32/AMD64 AVX-512 kOPvplzcntd opcode. */
    /* 1291 */ kOPvplzcntq,        /**< IA-32/AMD64 AVX-512 kOPvplzcntq opcode. */
    /* 1292 */ kOPvpmadd52huq,     /**< IA-32/AMD64 AVX-512 kOPvpmadd52huq opcode. */
    /* 1293 */ kOPvpmadd52luq,     /**< IA-32/AMD64 AVX-512 kOPvpmadd52luq opcode. */
    /* 1294 */ kOPvpmaxsq,         /**< IA-32/AMD64 AVX-512 kOPvpmaxsq opcode. */
    /* 1295 */ kOPvpmaxuq,         /**< IA-32/AMD64 AVX-512 kOPvpmaxuq opcode. */
    /* 1296 */ kOPvpminsq,         /**< IA-32/AMD64 AVX-512 kOPvpminsq opcode. */
    /* 1297 */ kOPvpminuq,         /**< IA-32/AMD64 AVX-512 kOPvpminuq opcode. */
    /* 1298 */ kOPvpmovb2m,        /**< IA-32/AMD64 AVX-512 kOPvpmovb2m opcode. */
    /* 1299 */ kOPvpmovd2m,        /**< IA-32/AMD64 AVX-512 kOPvpmovd2m opcode. */
    /* 1300 */ kOPvpmovdb,         /**< IA-32/AMD64 AVX-512 kOPvpmovdb opcode. */
    /* 1301 */ kOPvpmovdw,         /**< IA-32/AMD64 AVX-512 kOPvpmovdw opcode. */
    /* 1302 */ kOPvpmovm2b,        /**< IA-32/AMD64 AVX-512 kOPvpmovm2b opcode. */
    /* 1303 */ kOPvpmovm2d,        /**< IA-32/AMD64 AVX-512 kOPvpmovm2d opcode. */
    /* 1304 */ kOPvpmovm2q,        /**< IA-32/AMD64 AVX-512 kOPvpmovm2q opcode. */
    /* 1305 */ kOPvpmovm2w,        /**< IA-32/AMD64 AVX-512 kOPvpmovm2w opcode. */
    /* 1306 */ kOPvpmovq2m,        /**< IA-32/AMD64 AVX-512 kOPvpmovq2m opcode. */
    /* 1307 */ kOPvpmovqb,         /**< IA-32/AMD64 AVX-512 kOPvpmovqb opcode. */
    /* 1308 */ kOPvpmovqd,         /**< IA-32/AMD64 AVX-512 kOPvpmovqd opcode. */
    /* 1309 */ kOPvpmovqw,         /**< IA-32/AMD64 AVX-512 kOPvpmovqw opcode. */
    /* 1310 */ kOPvpmovsdb,        /**< IA-32/AMD64 AVX-512 kOPvpmovsdb opcode. */
    /* 1311 */ kOPvpmovsdw,        /**< IA-32/AMD64 AVX-512 kOPvpmovsdw opcode. */
    /* 1312 */ kOPvpmovsqb,        /**< IA-32/AMD64 AVX-512 kOPvpmovsqb opcode. */
    /* 1313 */ kOPvpmovsqd,        /**< IA-32/AMD64 AVX-512 kOPvpmovsqd opcode. */
    /* 1314 */ kOPvpmovsqw,        /**< IA-32/AMD64 AVX-512 kOPvpmovsqw opcode. */
    /* 1315 */ kOPvpmovswb,        /**< IA-32/AMD64 AVX-512 kOPvpmovswb opcode. */
    /* 1316 */ kOPvpmovusdb,       /**< IA-32/AMD64 AVX-512 kOPvpmovusdb opcode. */
    /* 1317 */ kOPvpmovusdw,       /**< IA-32/AMD64 AVX-512 kOPvpmovusdw opcode. */
    /* 1318 */ kOPvpmovusqb,       /**< IA-32/AMD64 AVX-512 kOPvpmovusqb opcode. */
    /* 1319 */ kOPvpmovusqd,       /**< IA-32/AMD64 AVX-512 kOPvpmovusqd opcode. */
    /* 1320 */ kOPvpmovusqw,       /**< IA-32/AMD64 AVX-512 kOPvpmovusqw opcode. */
    /* 1321 */ kOPvpmovuswb,       /**< IA-32/AMD64 AVX-512 kOPvpmovuswb opcode. */
    /* 1322 */ kOPvpmovw2m,        /**< IA-32/AMD64 AVX-512 kOPvpmovw2m opcode. */
    /* 1323 */ kOPvpmovwb,         /**< IA-32/AMD64 AVX-512 kOPvpmovwb opcode. */
    /* 1324 */ kOPvpmullq,         /**< IA-32/AMD64 AVX-512 kOPvpmullq opcode. */
    /* 1325 */ kOPvpord,           /**< IA-32/AMD64 AVX-512 kOPvpord opcode. */
    /* 1326 */ kOPvporq,           /**< IA-32/AMD64 AVX-512 kOPvporq opcode. */
    /* 1327 */ kOPvprold,          /**< IA-32/AMD64 AVX-512 kOPvprold opcode. */
    /* 1328 */ kOPvprolq,          /**< IA-32/AMD64 AVX-512 kOPvprolq opcode. */
    /* 1329 */ kOPvprolvd,         /**< IA-32/AMD64 AVX-512 kOPvprolvd opcode. */
    /* 1330 */ kOPvprolvq,         /**< IA-32/AMD64 AVX-512 kOPvprolvq opcode. */
    /* 1331 */ kOPvprord,          /**< IA-32/AMD64 AVX-512 kOPvprord opcode. */
    /* 1332 */ kOPvprorq,          /**< IA-32/AMD64 AVX-512 kOPvprorq opcode. */
    /* 1333 */ kOPvprorvd,         /**< IA-32/AMD64 AVX-512 kOPvprorvd opcode. */
    /* 1334 */ kOPvprorvq,         /**< IA-32/AMD64 AVX-512 kOPvprorvq opcode. */
    /* 1335 */ kOPvpscatterdd,     /**< IA-32/AMD64 AVX-512 kOPvpscatterdd opcode. */
    /* 1336 */ kOPvpscatterdq,     /**< IA-32/AMD64 AVX-512 kOPvpscatterdq opcode. */
    /* 1337 */ kOPvpscatterqd,     /**< IA-32/AMD64 AVX-512 kOPvpscatterqd opcode. */
    /* 1338 */ kOPvpscatterqq,     /**< IA-32/AMD64 AVX-512 kOPvpscatterqq opcode. */
    /* 1339 */ kOPvpsllvw,         /**< IA-32/AMD64 AVX-512 kOPvpsllvw opcode. */
    /* 1340 */ kOPvpsraq,          /**< IA-32/AMD64 AVX-512 kOPvpsraq opcode. */
    /* 1341 */ kOPvpsravq,         /**< IA-32/AMD64 AVX-512 kOPvpsravq opcode. */
    /* 1342 */ kOPvpsravw,         /**< IA-32/AMD64 AVX-512 kOPvpsravw opcode. */
    /* 1343 */ kOPvpsrlvw,         /**< IA-32/AMD64 AVX-512 kOPvpsrlvw opcode. */
    /* 1344 */ kOPvpternlogd,      /**< IA-32/AMD64 AVX-512 kOPvpternlogd opcode. */
    /* 1345 */ kOPvpternlogq,      /**< IA-32/AMD64 AVX-512 kOPvpternlogd opcode. */
    /* 1346 */ kOPvptestmb,        /**< IA-32/AMD64 AVX-512 kOPvptestmb opcode. */
    /* 1347 */ kOPvptestmd,        /**< IA-32/AMD64 AVX-512 kOPvptestmd opcode. */
    /* 1348 */ kOPvptestmq,        /**< IA-32/AMD64 AVX-512 kOPvptestmq opcode. */
    /* 1349 */ kOPvptestmw,        /**< IA-32/AMD64 AVX-512 kOPvptestmw opcode. */
    /* 1350 */ kOPvptestnmb,       /**< IA-32/AMD64 AVX-512 kOPvptestnmb opcode. */
    /* 1351 */ kOPvptestnmd,       /**< IA-32/AMD64 AVX-512 kOPvptestnmd opcode. */
    /* 1352 */ kOPvptestnmq,       /**< IA-32/AMD64 AVX-512 kOPvptestnmq opcode. */
    /* 1353 */ kOPvptestnmw,       /**< IA-32/AMD64 AVX-512 kOPvptestnmw opcode. */
    /* 1354 */ kOPvpxord,          /**< IA-32/AMD64 AVX-512 kOPvpxordvpxord opcode. */
    /* 1355 */ kOPvpxorq,          /**< IA-32/AMD64 AVX-512 kOPvpxorq opcode. */
    /* 1356 */ kOPvrangepd,        /**< IA-32/AMD64 AVX-512 kOPvrangepd opcode. */
    /* 1357 */ kOPvrangeps,        /**< IA-32/AMD64 AVX-512 kOPvrangeps opcode. */
    /* 1358 */ kOPvrangesd,        /**< IA-32/AMD64 AVX-512 kOPvrangesd opcode. */
    /* 1359 */ kOPvrangess,        /**< IA-32/AMD64 AVX-512 kOPvrangess opcode. */
    /* 1360 */ kOPvrcp14pd,        /**< IA-32/AMD64 AVX-512 kOPvrcp14pd opcode. */
    /* 1361 */ kOPvrcp14ps,        /**< IA-32/AMD64 AVX-512 kOPvrcp14ps opcode. */
    /* 1362 */ kOPvrcp14sd,        /**< IA-32/AMD64 AVX-512 kOPvrcp14sd opcode. */
    /* 1363 */ kOPvrcp14ss,        /**< IA-32/AMD64 AVX-512 kOPvrcp14ss opcode. */
    /* 1364 */ kOPvrcp28pd,        /**< IA-32/AMD64 AVX-512 kOPvrcp28pd opcode. */
    /* 1365 */ kOPvrcp28ps,        /**< IA-32/AMD64 AVX-512 kOPvrcp28ps opcode. */
    /* 1366 */ kOPvrcp28sd,        /**< IA-32/AMD64 AVX-512 kOPvrcp28sd opcode. */
    /* 1367 */ kOPvrcp28ss,        /**< IA-32/AMD64 AVX-512 kOPvrcp28ss opcode. */
    /* 1368 */ kOPvreducepd,       /**< IA-32/AMD64 AVX-512 kOPvreducepd opcode. */
    /* 1369 */ kOPvreduceps,       /**< IA-32/AMD64 AVX-512 kOPvreduceps opcode. */
    /* 1370 */ kOPvreducesd,       /**< IA-32/AMD64 AVX-512 kOPvreducesd opcode. */
    /* 1371 */ kOPvreducess,       /**< IA-32/AMD64 AVX-512 kOPvreducess opcode. */
    /* 1372 */ kOPvrndscalepd,     /**< IA-32/AMD64 AVX-512 kOPvrndscalepd opcode. */
    /* 1373 */ kOPvrndscaleps,     /**< IA-32/AMD64 AVX-512 kOPvrndscaleps opcode. */
    /* 1374 */ kOPvrndscalesd,     /**< IA-32/AMD64 AVX-512 kOPvrndscalesd opcode. */
    /* 1375 */ kOPvrndscaless,     /**< IA-32/AMD64 AVX-512 kOPvrndscaless opcode. */
    /* 1376 */ kOPvrsqrt14pd,      /**< IA-32/AMD64 AVX-512 kOPvrsqrt14pd opcode. */
    /* 1377 */ kOPvrsqrt14ps,      /**< IA-32/AMD64 AVX-512 kOPvrsqrt14ps opcode. */
    /* 1378 */ kOPvrsqrt14sd,      /**< IA-32/AMD64 AVX-512 kOPvrsqrt14sd opcode. */
    /* 1379 */ kOPvrsqrt14ss,      /**< IA-32/AMD64 AVX-512 kOPvrsqrt14ss opcode. */
    /* 1380 */ kOPvrsqrt28pd,      /**< IA-32/AMD64 AVX-512 kOPvrsqrt28pd opcode. */
    /* 1381 */ kOPvrsqrt28ps,      /**< IA-32/AMD64 AVX-512 kOPvrsqrt28ps opcode. */
    /* 1382 */ kOPvrsqrt28sd,      /**< IA-32/AMD64 AVX-512 kOPvrsqrt28sd opcode. */
    /* 1383 */ kOPvrsqrt28ss,      /**< IA-32/AMD64 AVX-512 kOPvrsqrt28ss opcode. */
    /* 1384 */ kOPvscalefpd,       /**< IA-32/AMD64 AVX-512 kOPvscalepd opcode. */
    /* 1385 */ kOPvscalefps,       /**< IA-32/AMD64 AVX-512 kOPvscaleps opcode. */
    /* 1386 */ kOPvscalefsd,       /**< IA-32/AMD64 AVX-512 kOPvscalesd opcode. */
    /* 1387 */ kOPvscalefss,       /**< IA-32/AMD64 AVX-512 kOPvscalesss opcode. */
    /* 1388 */ kOPvscatterdpd,     /**< IA-32/AMD64 AVX-512 kOPvscatterdpd opcode. */
    /* 1389 */ kOPvscatterdps,     /**< IA-32/AMD64 AVX-512 kOPvscatterdps opcode. */
    /* 1390 */ kOPvscatterqpd,     /**< IA-32/AMD64 AVX-512 kOPvscatterqpd opcode. */
    /* 1391 */ kOPvscatterqps,     /**< IA-32/AMD64 AVX-512 kOPvscatterqps opcode. */
    /* 1392 */ kOPvscatterpf0dpd,  /**< IA-32/AMD64 AVX-512 kOPvscatterpf0dpd opcode. */
    /* 1393 */ kOPvscatterpf0dps,  /**< IA-32/AMD64 AVX-512 kOPvscatterpf0dps opcode. */
    /* 1394 */ kOPvscatterpf0qpd,  /**< IA-32/AMD64 AVX-512 kOPvscatterpf0qpd opcode. */
    /* 1395 */ kOPvscatterpf0qps,  /**< IA-32/AMD64 AVX-512 kOPvscatterpf0qps opcode. */
    /* 1396 */ kOPvscatterpf1dpd,  /**< IA-32/AMD64 AVX-512 kOPvscatterpf1dpd opcode. */
    /* 1397 */ kOPvscatterpf1dps,  /**< IA-32/AMD64 AVX-512 kOPvscatterpf1dps opcode. */
    /* 1398 */ kOPvscatterpf1qpd,  /**< IA-32/AMD64 AVX-512 kOPvscatterpf1qpd opcode. */
    /* 1399 */ kOPvscatterpf1qps,  /**< IA-32/AMD64 AVX-512 kOPvscatterpf1qps opcode. */
    /* 1400 */ kOPvshuff32x4,      /**< IA-32/AMD64 AVX-512 kOPvshuff32x4 opcode. */
    /* 1401 */ kOPvshuff64x2,      /**< IA-32/AMD64 AVX-512 kOPvshuff64x2 opcode. */
    /* 1402 */ kOPvshufi32x4,      /**< IA-32/AMD64 AVX-512 kOPvshufi32x4 opcode. */
    /* 1403 */ kOPvshufi64x2,      /**< IA-32/AMD64 AVX-512 kOPvshufi64x2 opcode. */

    /* Intel SHA extensions */
    /* 1404 */ kOPsha1msg1,    /**< IA-32/AMD64 SHA kOPsha1msg1 opcode. */
    /* 1405 */ kOPsha1msg2,    /**< IA-32/AMD64 SHA kOPsha1msg2 opcode. */
    /* 1406 */ kOPsha1nexte,   /**< IA-32/AMD64 SHA kOPsha1nexte opcode. */
    /* 1407 */ kOPsha1rnds4,   /**< IA-32/AMD64 SHA kOPsha1rnds4 opcode. */
    /* 1408 */ kOPsha256msg1,  /**< IA-32/AMD64 SHA kOPsha2msg1 opcode. */
    /* 1409 */ kOPsha256msg2,  /**< IA-32/AMD64 SHA kOPsha2msg2 opcode. */
    /* 1410 */ kOPsha256rnds2, /**< IA-32/AMD64 SHA kOPsha2rnds2 opcode. */

    /* Intel MPX extensions */
    /* 1411 */ kOPbndcl,  /**< IA-32/AMD64 MPX kOPbndcl opcode. */
    /* 1412 */ kOPbndcn,  /**< IA-32/AMD64 MPX kOPbndcn opcode. */
    /* 1413 */ kOPbndcu,  /**< IA-32/AMD64 MPX kOPbndcu opcode. */
    /* 1414 */ kOPbndldx, /**< IA-32/AMD64 MPX kOPbndldx opcode. */
    /* 1415 */ kOPbndmk,  /**< IA-32/AMD64 MPX kOPbndmk opcode. */
    /* 1416 */ kOPbndmov, /**< IA-32/AMD64 MPX kOPbndmov opcode. */
    /* 1417 */ kOPbndstx, /**< IA-32/AMD64 MPX kOPbndstx opcode. */

    /* Intel PT extensions */
    /* 1418 */ kOPptwrite, /**< IA-32/AMD64 PT kOPptwrite opcode. */

    /* AMD monitor extensions */
    /* 1419 */ kOPmonitorx, /**< AMD64 monitorx opcode. */
    /* 1420 */ kOPmwaitx,   /**< AMD64 mwaitx opcode. */

    kOPAFTER_LAST,
    kOPFIRST = kOPadd,           /**< First real opcode. */
    kOPLAST = kOPAFTER_LAST - 1, /**< Last real opcode. */
} OperatorType;

} 

#endif //_OPERATOR_H_
