
/**
 * @file ed.h
 *
 * @brief the bit-parallel dynamic programming algorithm
 *
 * @detail
 * References:
 *      Myers, G. (1999). A fast bit-vector algorithm for approximate string
 *          matching based on dynamic programming. Journal of the ACM 46(3),
 *          May 1999, pp 395-415. 
 */
#include <stdint.h>
#include <stdlib.h>
#include <x86intrin.h>

/**
 * popcnt
 */
#ifdef __POPCNT__
	#define popcnt(x)		( (uint64_t)_mm_popcnt_u64(x) )
#else
	// #warning "popcnt instruction is not enabled."
	static inline
	uint64_t popcnt(uint64_t n)
	{
		uint64_t c = 0;
		c = (n & 0x5555555555555555) + ((n>>1) & 0x5555555555555555);
		c = (c & 0x3333333333333333) + ((c>>2) & 0x3333333333333333);
		c = (c & 0x0f0f0f0f0f0f0f0f) + ((c>>4) & 0x0f0f0f0f0f0f0f0f);
		c = (c & 0x00ff00ff00ff00ff) + ((c>>8) & 0x00ff00ff00ff00ff);
		c = (c & 0x0000ffff0000ffff) + ((c>>16) & 0x0000ffff0000ffff);
		c = (c & 0x00000000ffffffff) + ((c>>32) & 0x00000000ffffffff);
		return(c);
	}
#endif

/**
 * 8bit 32cell vector
 */
#if defined(__AVX2__)
	/* 8bit 32cell */
	typedef struct v32i8_s {
		__m256i v1;
	} v32i8_t;

	/**
	 * @struct v32_mask_s
	 *
	 * @brief common 32cell-wide mask type
	 */
	typedef struct v32_mask_s {
		uint32_t m1;
	} v32_mask_t;
	typedef struct v32_mask_s v32i8_mask_t;

	/**
	 * @union v32_mask_u
	 */
	typedef union v32_mask_u {
		v32_mask_t mask;
		uint32_t all;
	} v32_masku_t;
	typedef union v32_mask_u v32i8_masku_t;

	/* expanders (without argument) */
	#define _e_x_v32i8_1(u)

	/* expanders (without immediate) */
	#define _e_v_v32i8_1(a)				(a).v1
	#define _e_vv_v32i8_1(a, b)			(a).v1, (b).v1
	#define _e_vvv_v32i8_1(a, b, c)		(a).v1, (b).v1, (c).v1

	/* expanders with immediate */
	#define _e_i_v32i8_1(imm)			(imm)
	#define _e_vi_v32i8_1(a, imm)		(a).v1, (imm)
	#define _e_vvi_v32i8_1(a, b, imm)	(a).v1, (b).v1, (imm)

	/* address calculation macros */
	#define _addr_v32i8_1(imm)			( (__m256i *)(imm) )
	#define _pv_v32i8(ptr)				( _addr_v32i8_1(ptr) )
	/* expanders with pointers */
	#define _e_p_v32i8_1(ptr)			_addr_v32i8_1(ptr)
	#define _e_pv_v32i8_1(ptr, a)		_addr_v32i8_1(ptr), (a).v1

	/* expand intrinsic name */
	#define _i_v32i8(intrin) 			_mm256_##intrin##_epi8
	#define _i_v32i8x(intrin)			_mm256_##intrin##_si256

	/* apply */
	#define _a_v32i8(intrin, expander, ...) ( \
		(v32i8_t) { \
			_i_v32i8(intrin)(expander##_v32i8_1(__VA_ARGS__)) \
		} \
	)
	#define _a_v32i8x(intrin, expander, ...) ( \
		(v32i8_t) { \
			_i_v32i8x(intrin)(expander##_v32i8_1(__VA_ARGS__)) \
		} \
	)
	#define _a_v32i8xv(intrin, expander, ...) { \
		_i_v32i8x(intrin)(expander##_v32i8_1(__VA_ARGS__)); \
	}

	/* load and store */
	#define _load_v32i8(...)	_a_v32i8x(load, _e_p, __VA_ARGS__)
	#define _loadu_v32i8(...)	_a_v32i8x(loadu, _e_p, __VA_ARGS__)
	#define _store_v32i8(...)	_a_v32i8xv(store, _e_pv, __VA_ARGS__)
	#define _storeu_v32i8(...)	_a_v32i8xv(storeu, _e_pv, __VA_ARGS__)

	/* broadcast */
	#define _set_v32i8(...)		_a_v32i8(set1, _e_i, __VA_ARGS__)
	#define _zero_v32i8()		_a_v32i8x(setzero, _e_x, _unused)

	/* swap (reverse) */
	#define _swap_idx_v32i8() ( \
		_mm256_broadcastsi128_si256(_mm_set_epi8( \
			0, 1, 2, 3, 4, 5, 6, 7, \
			8, 9, 10, 11, 12, 13, 14, 15)) \
	)
	#define _swap_v32i8(a) ( \
		(v32i8_t) { \
			_mm256_permute2x128_si256( \
				_mm256_shuffle_epi8((a).v1, _swap_idx_v32i8()), \
				_mm256_shuffle_epi8((a).v1, _swap_idx_v32i8()), \
				0x01) \
		} \
	)

	/* logics */
	#define _not_v32i8(...)		_a_v32i8x(not, _e_v, __VA_ARGS__)
	#define _and_v32i8(...)		_a_v32i8x(and, _e_vv, __VA_ARGS__)
	#define _or_v32i8(...)		_a_v32i8x(or, _e_vv, __VA_ARGS__)
	#define _xor_v32i8(...)		_a_v32i8x(xor, _e_vv, __VA_ARGS__)
	#define _andn_v32i8(...)	_a_v32i8x(andnot, _e_vv, __VA_ARGS__)

	/* arithmetics */
	#define _add_v32i8(...)		_a_v32i8(add, _e_vv, __VA_ARGS__)
	#define _sub_v32i8(...)		_a_v32i8(sub, _e_vv, __VA_ARGS__)
	#define _adds_v32i8(...)	_a_v32i8(adds, _e_vv, __VA_ARGS__)
	#define _subs_v32i8(...)	_a_v32i8(subs, _e_vv, __VA_ARGS__)
	#define _max_v32i8(...)		_a_v32i8(max, _e_vv, __VA_ARGS__)
	#define _min_v32i8(...)		_a_v32i8(min, _e_vv, __VA_ARGS__)

	/* shuffle */
	#define _shuf_v32i8(...)	_a_v32i8(shuffle, _e_vv, __VA_ARGS__)

	/* blend */
	// #define _sel_v32i8(...)		_a_v32i8(blendv, _e_vvv, __VA_ARGS__)

	/* compare */
	#define _eq_v32i8(...)		_a_v32i8(cmpeq, _e_vv, __VA_ARGS__)
	#define _lt_v32i8(...)		_a_v32i8(cmplt, _e_vv, __VA_ARGS__)
	#define _gt_v32i8(...)		_a_v32i8(cmpgt, _e_vv, __VA_ARGS__)

	/* insert and extract */
	#define _ins_v32i8(a, val, imm) { \
		(a).v1 = _i_v32i8(insert)((a).v1, (val), (imm)); \
	}
	#define _ext_v32i8(a, imm) ( \
		(int8_t)_i_v32i8(extract)((a).v1, (imm)) \
	)

	/* shift */
	#define _bsl_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm256_alignr_epi8( \
				(a).v1, \
				_mm256_permute2x128_si256((a).v1, (a).v1, 0x08), \
				15) \
		} \
	)
	#define _bsr_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm256_alignr_epi8( \
				_mm256_castsi128_si256( \
					_mm256_extracti128_si256((a).v1, 1)), \
				(a).v1, \
				1) \
		} \
	)
	#define _shl_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm256_slli_epi32((a).v1, (imm)) \
		} \
	)
	#define _shr_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm256_srli_epi32((a).v1, (imm)) \
		} \
	)
	#define _sal_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm256_slai_epi32((a).v1, (imm)) \
		} \
	)
	#define _sar_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm256_srai_epi32((a).v1, (imm)) \
		} \
	)

	/* mask */
	#define _mask_v32i8(a) ( \
		(v32_mask_t) { \
			.m1 = _i_v32i8(movemask)((a).v1) \
		} \
	)

#elif defined(__SSE4_1__)
	/* 8bit 32cell */
	typedef struct v32i8_s {
		__m128i v1;
		__m128i v2;
	} v32i8_t;

	/**
	 * @struct v32_mask_s
	 *
	 * @brief common 32cell-wide mask type
	 */
	typedef struct v32_mask_s {
		uint16_t m1;
		uint16_t m2;
	} v32_mask_t;
	typedef struct v32_mask_s v32i8_mask_t;

	/**
	 * @union v32_mask_u
	 */
	typedef union v32_mask_u {
		v32_mask_t mask;
		uint32_t all;
	} v32_masku_t;
	typedef union v32_mask_u v32i8_masku_t;

	/* expanders (without argument) */
	#define _e_x_v32i8_1(u)
	#define _e_x_v32i8_2(u)

	/* expanders (without immediate) */
	#define _e_v_v32i8_1(a)				(a).v1
	#define _e_v_v32i8_2(a)				(a).v2
	#define _e_vv_v32i8_1(a, b)			(a).v1, (b).v1
	#define _e_vv_v32i8_2(a, b)			(a).v2, (b).v2
	#define _e_vvv_v32i8_1(a, b, c)		(a).v1, (b).v1, (c).v1
	#define _e_vvv_v32i8_2(a, b, c)		(a).v2, (b).v2, (c).v2

	/* expanders with immediate */
	#define _e_i_v32i8_1(imm)			(imm)
	#define _e_i_v32i8_2(imm)			(imm)
	#define _e_vi_v32i8_1(a, imm)		(a).v1, (imm)
	#define _e_vi_v32i8_2(a, imm)		(a).v2, (imm)
	#define _e_vvi_v32i8_1(a, b, imm)	(a).v1, (b).v1, (imm)
	#define _e_vvi_v32i8_2(a, b, imm)	(a).v2, (b).v2, (imm)

	/* address calculation macros */
	#define _addr_v32i8_1(imm)			( (__m128i *)(imm) )
	#define _addr_v32i8_2(imm)			( (__m128i *)(imm) + 1 )
	#define _pv_v32i8(ptr)				( _addr_v32i8_1(ptr) )
	/* expanders with pointers */
	#define _e_p_v32i8_1(ptr)			_addr_v32i8_1(ptr)
	#define _e_p_v32i8_2(ptr)			_addr_v32i8_2(ptr)
	#define _e_pv_v32i8_1(ptr, a)		_addr_v32i8_1(ptr), (a).v1
	#define _e_pv_v32i8_2(ptr, a)		_addr_v32i8_2(ptr), (a).v2

	/* expand intrinsic name */
	#define _i_v32i8(intrin) 			_mm_##intrin##_epi8
	#define _i_v32i8x(intrin)			_mm_##intrin##_si128

	/* apply */
	#define _a_v32i8(intrin, expander, ...) ( \
		(v32i8_t) { \
			_i_v32i8(intrin)(expander##_v32i8_1(__VA_ARGS__)), \
			_i_v32i8(intrin)(expander##_v32i8_2(__VA_ARGS__)) \
		} \
	)
	#define _a_v32i8x(intrin, expander, ...) ( \
		(v32i8_t) { \
			_i_v32i8x(intrin)(expander##_v32i8_1(__VA_ARGS__)), \
			_i_v32i8x(intrin)(expander##_v32i8_2(__VA_ARGS__)) \
		} \
	)
	#define _a_v32i8xv(intrin, expander, ...) { \
		_i_v32i8x(intrin)(expander##_v32i8_1(__VA_ARGS__)); \
		_i_v32i8x(intrin)(expander##_v32i8_2(__VA_ARGS__)); \
	}

	/* load and store */
	#define _load_v32i8(...)	_a_v32i8x(load, _e_p, __VA_ARGS__)
	#define _loadu_v32i8(...)	_a_v32i8x(loadu, _e_p, __VA_ARGS__)
	#define _store_v32i8(...)	_a_v32i8xv(store, _e_pv, __VA_ARGS__)
	#define _storeu_v32i8(...)	_a_v32i8xv(storeu, _e_pv, __VA_ARGS__)

	/* broadcast */
	#define _set_v32i8(...)		_a_v32i8(set1, _e_i, __VA_ARGS__)
	#define _zero_v32i8()		_a_v32i8x(setzero, _e_x, _unused)

	/* swap (reverse) */
	#define _swap_idx_v32i8() ( \
		_mm_set_epi8( \
			0, 1, 2, 3, 4, 5, 6, 7, \
			8, 9, 10, 11, 12, 13, 14, 15) \
	)
	#define _swap_v32i8(a) ( \
		(v32i8_t) { \
			_mm_shuffle_epi8((a).v2, _swap_idx_v32i8()), \
			_mm_shuffle_epi8((a).v1, _swap_idx_v32i8()) \
		} \
	)

	/* logics */
	#define _not_v32i8(...)		_a_v32i8x(not, _e_v, __VA_ARGS__)
	#define _and_v32i8(...)		_a_v32i8x(and, _e_vv, __VA_ARGS__)
	#define _or_v32i8(...)		_a_v32i8x(or, _e_vv, __VA_ARGS__)
	#define _xor_v32i8(...)		_a_v32i8x(xor, _e_vv, __VA_ARGS__)
	#define _andn_v32i8(...)	_a_v32i8x(andnot, _e_vv, __VA_ARGS__)

	/* arithmetics */
	#define _add_v32i8(...)		_a_v32i8(add, _e_vv, __VA_ARGS__)
	#define _sub_v32i8(...)		_a_v32i8(sub, _e_vv, __VA_ARGS__)
	#define _adds_v32i8(...)	_a_v32i8(adds, _e_vv, __VA_ARGS__)
	#define _subs_v32i8(...)	_a_v32i8(subs, _e_vv, __VA_ARGS__)
	#define _max_v32i8(...)		_a_v32i8(max, _e_vv, __VA_ARGS__)
	#define _min_v32i8(...)		_a_v32i8(min, _e_vv, __VA_ARGS__)

	/* shuffle */
	#define _shuf_v32i8(...)	_a_v32i8(shuffle, _e_vv, __VA_ARGS__)

	/* blend */
	// #define _sel_v32i8(...)		_a_v32i8(blendv, _e_vvv, __VA_ARGS__)

	/* compare */
	#define _eq_v32i8(...)		_a_v32i8(cmpeq, _e_vv, __VA_ARGS__)
	#define _lt_v32i8(...)		_a_v32i8(cmplt, _e_vv, __VA_ARGS__)
	#define _gt_v32i8(...)		_a_v32i8(cmpgt, _e_vv, __VA_ARGS__)

	/* insert and extract */
	#define _ins_v32i8(a, val, imm) { \
		if((imm) < sizeof(__m128i)) { \
			(a).v1 = _i_v32i8(insert)((a).v1, (val), (imm)); \
		} else { \
			(a).v2 = _i_v32i8(insert)((a).v2, (val), (imm) - sizeof(__m128i)); \
		} \
	}
	#define _ext_v32i8(a, imm) ( \
		(int8_t)(((imm) < sizeof(__m128i)) \
			? _i_v32i8(extract)((a).v1, (imm)) \
			: _i_v32i8(extract)((a).v2, (imm) - sizeof(__m128i))) \
	)

	/* shift */
	#define _bsl_v32i8(a, imm) ( \
		(v32i8_t) { \
			_i_v32i8x(slli)((a).v1, (imm)), \
			_i_v32i8(alignr)((a).v2, (a).v1, sizeof(__m128i) - (imm)) \
		} \
	)
	#define _bsr_v32i8(a, imm) ( \
		(v32i8_t) { \
			_i_v32i8(alignr)((a).v2, (a).v1, (imm)), \
			_i_v32i8x(srli)((a).v2, (imm)) \
		} \
	)
	#define _shl_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm_slli_epi32((a).v1, (imm)), \
			_mm_slli_epi32((a).v2, (imm)) \
		} \
	)
	#define _shr_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm_srli_epi32((a).v1, (imm)), \
			_mm_srli_epi32((a).v2, (imm)) \
		} \
	)
	#define _sal_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm_slai_epi32((a).v1, (imm)), \
			_mm_slai_epi32((a).v2, (imm)) \
		} \
	)
	#define _sar_v32i8(a, imm) ( \
		(v32i8_t) { \
			_mm_srai_epi32((a).v1, (imm)), \
			_mm_srai_epi32((a).v2, (imm)) \
		} \
	)

	/* mask */
	#define _mask_v32i8(a) ( \
		(v32_mask_t) { \
			.m1 = _i_v32i8(movemask)((a).v1), \
			.m2 = _i_v32i8(movemask)((a).v2) \
		} \
	)

#else
	#error "No SIMD instruction set enabled. Check if SSE4.1 or AVX2 instructions are available and add `-msse4.1' or `-mavx2' to CFLAGS."
#endif

static inline
uint64_t editdist(
	char const *a,
	uint64_t alen,
	char const *b,
	uint64_t blen)
{
	#define _m(x)	( (uint64_t)((v32_masku_t){ .mask = (_mask_v32i8(x)) }).all )

	v32i8_t a1 = _loadu_v32i8(a), a2 = _loadu_v32i8(a + 32);
	uint64_t ph = 0xffffffffffffffff, mh = 0;
	for(char const *p = b; p < b + blen; p++) {
		v32i8_t b = _set_v32i8(*p);
		uint64_t eq = (_m(_eq_v32i8(b, a2))<<32) | _m(_eq_v32i8(b, a1));
		uint64_t xh = eq | mh, xv = (((eq & ph) + ph) ^ ph) | eq;
		uint64_t pv = mh | ~(xv | ph), mv = ph & xv;
		pv<<=1; mv<<=1; pv |= 0x01ULL;
		ph = mv | ~(xh | pv); mh = pv & xh;
	}
	uint64_t mask = (0x01ULL<<alen) - 1;
	return(blen + popcnt(ph & mask) - popcnt(mh & mask));
}

/**
 * end of ed.h
 */
