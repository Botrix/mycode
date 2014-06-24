#ifndef RSS_PKG_REASM_H
#define RSS_PKG_REASM_H

struct pkg_hdr_op
{
    uint32_t (*hdr_parser)(const char* buf); /* return total pkg size, including header. */
    uint32_t header_size;	/* Once the size of data in pkg_reasm >= header_size, we call hdr_parser. */
	uint32_t max_pkg_size;
	uint32_t pkg_size;	/* pkg size is constant */
};

struct pkg_reasm* 
pkg_reasm_create(struct pkg_hdr_op* hdr_op);

inline  __attribute__((always_inline)) uint32_t 
pkg_reasm_size(struct pkg_reasm* reasm);

inline  __attribute__((always_inline)) int 
pkg_reasm_add(struct pkg_reasm* reasm, const char* buf, uint32_t size);

inline  __attribute__((always_inline)) char* 
pkg_reasm_get(struct pkg_reasm* reasm);

inline  __attribute__((always_inline)) uint32_t
pkg_reasm_pkg_size(struct pkg_reasm* reasm);

inline  __attribute__((always_inline)) void
pkg_reasm_cutoff(struct pkg_reasm* reasm);

void 
pkg_reasm_release(struct pkg_reasm* reasm);

#endif
